#include "playCommon.hh"
#include "BasicUsageEnvironment.hh"
#include "GroupsockHelper.hh"
#include "AudioSink.hh"
#include <signal.h>

// Forward function definitions:
// ///////////////////////////////////////////////////////////////////////////////
void continueAfterOPTIONS(RTSPClient* client, int resultCode, char* resultString);
void continueAfterDESCRIBE(RTSPClient* client, int resultCode, char* resultString);
void continueAfterSETUP(RTSPClient* client, int resultCode, char* resultString);
void continueAfterPLAY(RTSPClient* client, int resultCode, char* resultString);
void continueAfterTEARDOWN(RTSPClient* client, int resultCode, char* resultString);

void setupStreams();
void closeMediaSinks();
void subsessionAfterPlaying(void* clientData);
void subsessionByeHandler(void* clientData);
void sessionAfterPlaying(void* clientData = NULL);
void sessionTimerHandler(void* clientData);
void shutdown(int exitCode = 1);
void signalHandlerShutdown(int sig);
void checkForPacketArrival(void* clientData);
void checkInterPacketGaps(void* clientData);
///////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////
char const* progName;
UsageEnvironment* env;
Medium* ourClient = NULL;
Authenticator* ourAuthenticator = NULL;
char const* streamURL = NULL;
MediaSession* session = NULL;
TaskToken sessionTimerTask = NULL;
TaskToken arrivalCheckTimerTask = NULL;
TaskToken interPacketGapCheckTimerTask = NULL;
Boolean createReceivers = True;
Boolean outputQuickTimeFile = False;
Boolean generateMP4Format = False;
QuickTimeFileSink* qtOut = NULL;
Boolean outputAVIFile = False;
AVIFileSink* aviOut = NULL;
Boolean audioOnly = False;
Boolean videoOnly = False;
char const* singleMedium = NULL;
int verbosityLevel = 0; // by default, print verbose output
double duration = 0;
double durationSlop = -1.0; // extra seconds to play at the end
double initialSeekTime = 0.0f;
float scale = 1.0f;
double endTime;
unsigned interPacketGapMaxTime = 0;
unsigned totNumPacketsReceived = ~0; // used if checking inter-packet gaps
Boolean playContinuously = False;
int simpleRTPoffsetArg = -1;
Boolean sendOptionsRequest = True;
Boolean sendOptionsRequestOnly = False;
Boolean oneFilePerFrame = False;
Boolean notifyOnPacketArrival = False;
Boolean streamUsingTCP = False;
unsigned short desiredPortNum = 0;
portNumBits tunnelOverHTTPPortNum = 0;
char* username = NULL;
char* password = NULL;
char* proxyServerName = NULL;
unsigned short proxyServerPortNum = 0;
unsigned char desiredAudioRTPPayloadFormat = 0;
char* mimeSubtype = NULL;
char const* fileNamePrefix = "";
unsigned sinkBufferSize = 1280;
unsigned socketInputBufferSize = 0;
Boolean packetLossCompensate = False;
Boolean syncStreams = False;
Boolean generateHintTracks = False;
struct timeval startTime;
Boolean allowProxyServers = False;
Boolean controlConnectionUsesTCP = True;
Boolean supportCodecSelection = False;
char const* clientProtocolName = "RTSP";
AudioSink* audioPktSink;

////////////////////////////////////////////////////////////////////


int main(int argc, char** argv) {

  TaskScheduler* scheduler = BasicTaskScheduler::createNew();
  env = BasicUsageEnvironment::createNew(*scheduler);

  progName = argv[0];

  gettimeofday(&startTime, NULL);

  signal(SIGHUP, signalHandlerShutdown);
  signal(SIGUSR1, signalHandlerShutdown);

  singleMedium = "audio";
  verbosityLevel = 0;
  streamURL = argv[1];

  // Create our client object:
  ourClient = createClient(*env, streamURL, verbosityLevel, progName);
  if (ourClient == NULL) {
    *env << "Failed to establish connection " << clientProtocolName << " client: " << env->getResultMsg() << "\n";
    shutdown();
  }

  if (sendOptionsRequest) {
	getOptions(continueAfterOPTIONS);
  } else {
    	continueAfterOPTIONS(NULL, 0, NULL);
  }

  env->taskScheduler().doEventLoop(); // does not return

  return 0; // only to prevent compiler warning
}

///////////////////////////////////
void getOptions(RTSPClient::responseHandler* afterFunc) {
  ourRTSPClient->sendOptionsCommand(afterFunc, ourAuthenticator);
}

////////////////////////////////////
void continueAfterOPTIONS(RTSPClient*, int resultCode, char* resultString) {
  if (sendOptionsRequestOnly) {
    if (resultCode != 0) {
      *env << clientProtocolName << " \"OPTIONS\" request failed: " << resultString << "\n";
    } else {
      *env << clientProtocolName << " \"OPTIONS\" request returned: " << resultString << "\n";
    }
    shutdown();
  }
  delete[] resultString;

  getSDPDescription(continueAfterDESCRIBE);
}


/////////////////////////////////////
void continueAfterDESCRIBE(RTSPClient*, int resultCode, char* resultString) {
  if (resultCode != 0) {
    *env << "Failed to get a SDP description from URL \"" << streamURL << "\": " << resultString << "\n";
    shutdown();
  }

  char* sdpDescription = resultString;
  *env << "Opened URL \"" << streamURL << "\", returning a SDP description:\n" << sdpDescription << "\n";

  // Create a media session object from this SDP description:
  session = MediaSession::createNew(*env, sdpDescription);
  delete[] sdpDescription;
  if (session == NULL) {
    *env << "Failed to create a MediaSession object from the SDP description: " << env->getResultMsg() << "\n";
    shutdown();
  } else if (!session->hasSubsessions()) {
    *env << "This session has no media subsessions (i.e., \"m=\" lines)\n";
    shutdown();
  }

  // Then, setup the "RTPSource"s for the session:
  MediaSubsessionIterator iter(*session);
  MediaSubsession *subsession;
  Boolean madeProgress = False;
  char const* singleMediumToTest = singleMedium;
  while ((subsession = iter.next()) != NULL) {
    // If we've asked to receive only a single medium, then check this now:
    if (singleMediumToTest != NULL) {
      if (strcmp(subsession->mediumName(), singleMediumToTest) != 0) {
		  *env << "Ignoring \"" << subsession->mediumName()
			  << "/" << subsession->codecName()
			  << "\" subsession, because we've asked to receive a single " << singleMedium
			  << " session only\n";
	continue;
      } else {
	// Receive this subsession only
	singleMediumToTest = "xxxxx";
	    // this hack ensures that we get only 1 subsession of this type
      }
    }

    if (desiredPortNum != 0) {
      subsession->setClientPortNum(desiredPortNum);
      desiredPortNum += 2;
    }

    if (createReceivers) {
      if (!subsession->initiate(simpleRTPoffsetArg)) {
	*env << "Unable to create receiver for \"" << subsession->mediumName() << "/" << subsession->codecName()
	     << "\" subsession: " << env->getResultMsg() << "\n";
      } else {
	*env << "Created receiver for \"" << subsession->mediumName() << "/" << subsession->codecName()
	     << "\" subsession (client ports " << subsession->clientPortNum()
	     << "-" << subsession->clientPortNum()+1 << ")\n";
	madeProgress = True;

	if (subsession->rtpSource() != NULL) {
	  // (1 second) for reordering misordered incoming packets:
	  unsigned const thresh = 1000000; // 1 second
	  subsession->rtpSource()->setPacketReorderingThresholdTime(thresh);

	  // Set the RTP source's OS socket buffer size as appropriate - either if we were explicitly asked (using -B),
	  // or if the desired FileSink buffer size happens to be larger than the current OS socket buffer size.
	  // (The latter case is a heuristic, on the assumption that if the user asked for a large FileSink buffer size,
	  // then the input data rate may be large enough to justify increasing the OS socket buffer size also.)
	  int socketNum = subsession->rtpSource()->RTPgs()->socketNum();
	  unsigned curBufferSize = getReceiveBufferSize(*env, socketNum);
	  if (socketInputBufferSize > 0 || sinkBufferSize > curBufferSize) {
	    unsigned newBufferSize = socketInputBufferSize > 0 ? socketInputBufferSize : sinkBufferSize;
	    newBufferSize = setReceiveBufferTo(*env, socketNum, newBufferSize);
	    if (socketInputBufferSize > 0) { // The user explicitly asked for the new socket buffer size; announce it:
	      *env << "Changed socket receive buffer size for the \""
		   << subsession->mediumName() << "/" << subsession->codecName() << "\" subsession from "
		   << curBufferSize << " to "  << newBufferSize << " bytes\n";
	    }
	  }
	}
      }
    } else {
      if (subsession->clientPortNum() == 0) {
	*env << "No client port was specified for the \"" << subsession->mediumName()
	     << "/" << subsession->codecName() << "\" subsession.  (Try adding the \"-p <portNum>\" option.)\n";
      } else {
		madeProgress = True;
      }
    }
  }
  if (!madeProgress) shutdown();

  // Perform additional 'setup' on each subsession, before playing them:
  setupStreams();
}

MediaSubsession *subsession;
Boolean madeProgress = False;
void continueAfterSETUP(RTSPClient*, int resultCode, char* resultString) {
  if (resultCode == 0) {
      *env << "Setup \"" << subsession->mediumName() << "/" << subsession->codecName()
	   << "\" subsession (client ports " << subsession->clientPortNum()
	   << "-" << subsession->clientPortNum()+1 << ")\n";
      madeProgress = True;
  } else {
    *env << "Failed to setup \"" << subsession->mediumName()
	 << "/" << subsession->codecName() << "\" subsession: " << env->getResultMsg() << "\n";
  }

  // Set up the next subsession, if any:
  setupStreams();
}

void setupStreams() {
  static MediaSubsessionIterator* setupIter = NULL;
  if (setupIter == NULL) setupIter = new MediaSubsessionIterator(*session);
  while ((subsession = setupIter->next()) != NULL) {
    // We have another subsession left to set up:
    if (subsession->clientPortNum() == 0) continue; // port # was not set

    setupSubsession(subsession, streamUsingTCP, continueAfterSETUP);
    return;
  }

  // We're done setting up subsessions.
  delete setupIter;
  if (!madeProgress) shutdown();

  // Create output files:
  if (createReceivers) {
      // Create and start "FileSink"s for each subsession:
      madeProgress = False;
      MediaSubsessionIterator iter(*session);
      while ((subsession = iter.next()) != NULL) {
	if (subsession->readSource() == NULL) continue; // was not initiated

	// Create an output file for each desired stream:
	char outFileName[1000];
	if (singleMedium == NULL) {
	  // Output file name is
	  //     "<filename-prefix><medium_name>-<codec_name>-<counter>"
	  static unsigned streamCounter = 0;
	  snprintf(outFileName, sizeof outFileName, "%s%s-%s-%d",
		   fileNamePrefix, subsession->mediumName(),
		   subsession->codecName(), ++streamCounter);
	} else {
	  sprintf(outFileName, "stdout");
	}

	audioPktSink = AudioSink::createNew(*env, "testfile", sinkBufferSize);

	subsession->sink = audioPktSink;
	if (subsession->sink == NULL) {
	  *env << "Failed to create FileSink for \"" << outFileName << "\": " << env->getResultMsg() << "\n";
	} else {
	  if (singleMedium == NULL) {
	    *env << "Created output file: \"" << outFileName << "\"\n";
	  } else {
	    *env << "Outputting data from the \"" << subsession->mediumName()
			<< "/" << subsession->codecName() << "\" subsession to 'stdout'\n";
	  }

	  subsession->sink->startPlaying(*(subsession->readSource()), subsessionAfterPlaying, subsession);

	  // Also set a handler to be called if a RTCP "BYE" arrives
	  // for this subsession:
	  if (subsession->rtcpInstance() != NULL) {
	    subsession->rtcpInstance()->setByeHandler(subsessionByeHandler, subsession);
	  }

	  madeProgress = True;
	}
      }
      if (!madeProgress) shutdown();
  }

  // Finally, start playing each subsession, to start the data flow:
  if (duration == 0) {
    if (scale > 0) duration = session->playEndTime() - initialSeekTime; // use SDP end time
    else if (scale < 0) duration = initialSeekTime;
  }
  if (duration < 0) duration = 0.0;

  endTime = initialSeekTime;
  if (scale > 0) {
    if (duration <= 0) endTime = -1.0f;
    else endTime = initialSeekTime + duration;
  } else {
    endTime = initialSeekTime - duration;
    if (endTime < 0) endTime = 0.0f;
  }

  startPlayingSession(session, initialSeekTime, endTime, scale, continueAfterPLAY);
}

void continueAfterPLAY(RTSPClient*, int resultCode, char* resultString) {
  if (resultCode != 0) {
    *env << "Failed to start playing session: " << resultString << "\n";
    shutdown();
  } else {
    *env << "Started playing session\n";
  }

  // Figure out how long to delay (if at all) before shutting down, or
  // repeating the playing
  Boolean timerIsBeingUsed = False;
  double secondsToDelay = duration;
  if (duration > 0) {
    timerIsBeingUsed = True;
    double absScale = scale > 0 ? scale : -scale; // ASSERT: scale != 0
    secondsToDelay = duration/absScale + durationSlop;

    int64_t uSecsToDelay = (int64_t)(secondsToDelay*1000000.0);
    sessionTimerTask = env->taskScheduler().scheduleDelayedTask(uSecsToDelay, (TaskFunc*)sessionTimerHandler, (void*)NULL);
  }

  char const* actionString
    = createReceivers? "Receiving streamed data":"Data is being streamed";
  if (timerIsBeingUsed) {
    *env << actionString
		<< " (for up to " << secondsToDelay << " seconds)...\n";
  } else {

    pid_t ourPid = getpid();
    *env << actionString
		<< " (signal with \"kill -HUP " << (int)ourPid
		<< "\" or \"kill -USR1 " << (int)ourPid << "\" to terminate)...\n";
  }

  // Watch for incoming packets (if desired):
  checkForPacketArrival(NULL);
  checkInterPacketGaps(NULL);
}

void closeMediaSinks() {
  Medium::close(qtOut);
  Medium::close(aviOut);

  if (session == NULL) return;
  MediaSubsessionIterator iter(*session);
  MediaSubsession* subsession;
  while ((subsession = iter.next()) != NULL) {
    Medium::close(subsession->sink);
    subsession->sink = NULL;
  }
}

void subsessionAfterPlaying(void* clientData) {
  // Begin by closing this media subsession's stream:
  MediaSubsession* subsession = (MediaSubsession*)clientData;
  Medium::close(subsession->sink);
  subsession->sink = NULL;

  // Next, check whether *all* subsessions' streams have now been closed:
  MediaSession& session = subsession->parentSession();
  MediaSubsessionIterator iter(session);
  while ((subsession = iter.next()) != NULL) {
    if (subsession->sink != NULL) return; // this subsession is still active
  }

  // All subsessions' streams have now been closed
  sessionAfterPlaying();
}

void subsessionByeHandler(void* clientData) {
  struct timeval timeNow;
  gettimeofday(&timeNow, NULL);
  unsigned secsDiff = timeNow.tv_sec - startTime.tv_sec;

  MediaSubsession* subsession = (MediaSubsession*)clientData;
  *env << "Received RTCP \"BYE\" on \"" << subsession->mediumName()
	<< "/" << subsession->codecName()
	<< "\" subsession (after " << secsDiff
	<< " seconds)\n";

  // Act now as if the subsession had closed:
  subsessionAfterPlaying(subsession);
}

void sessionAfterPlaying(void* /*clientData*/) {
  if (!playContinuously) {
    shutdown(0);
  } else {
    // We've been asked to play the stream(s) over again:
    startPlayingSession(session, initialSeekTime, endTime, scale, continueAfterPLAY);
  }
}

void sessionTimerHandler(void* /*clientData*/) {
  sessionTimerTask = NULL;

  sessionAfterPlaying();
}


int shutdownExitCode;
void shutdown(int exitCode) {

  shutdownExitCode = exitCode;
  if (env != NULL) {
    env->taskScheduler().unscheduleDelayedTask(sessionTimerTask);
    env->taskScheduler().unscheduleDelayedTask(arrivalCheckTimerTask);
    env->taskScheduler().unscheduleDelayedTask(interPacketGapCheckTimerTask);
  }

  // Teardown, then shutdown, any outstanding RTP/RTCP subsessions
  if (session != NULL) {
    tearDownSession(session, continueAfterTEARDOWN);
  } else {
    continueAfterTEARDOWN(NULL, 0, NULL);
  }
}

void continueAfterTEARDOWN(RTSPClient*, int /*resultCode*/, char* /*resultString*/) {
  // Now that we've stopped any more incoming data from arriving, close our output files:
  closeMediaSinks();
  Medium::close(session);

  // Finally, shut down our client:
  delete ourAuthenticator;
  Medium::close(ourClient);

  // Adios...
  exit(shutdownExitCode);
}

void signalHandlerShutdown(int /*sig*/) {
  *env << "Got shutdown signal\n";
  shutdown(0);
}

void checkForPacketArrival(void* /*clientData*/) {
  if (!notifyOnPacketArrival) return; // we're not checking

  // Check each subsession, to see whether it has received data packets:
  unsigned numSubsessionsChecked = 0;
  unsigned numSubsessionsWithReceivedData = 0;
  unsigned numSubsessionsThatHaveBeenSynced = 0;

  MediaSubsessionIterator iter(*session);
  MediaSubsession* subsession;
  while ((subsession = iter.next()) != NULL) {
    RTPSource* src = subsession->rtpSource();
    if (src == NULL) continue;
    ++numSubsessionsChecked;

    if (src->receptionStatsDB().numActiveSourcesSinceLastReset() > 0) {
      // At least one data packet has arrived
      ++numSubsessionsWithReceivedData;
    }
    if (src->hasBeenSynchronizedUsingRTCP()) {
      ++numSubsessionsThatHaveBeenSynced;
    }
  }

  unsigned numSubsessionsToCheck = numSubsessionsChecked;
  // Special case for "QuickTimeFileSink"s and "AVIFileSink"s:
  // They might not use all of the input sources:
  if (qtOut != NULL) {
    numSubsessionsToCheck = qtOut->numActiveSubsessions();
  } else if (aviOut != NULL) {
    numSubsessionsToCheck = aviOut->numActiveSubsessions();
  }

  Boolean notifyTheUser;
  if (!syncStreams) {
    notifyTheUser = numSubsessionsWithReceivedData > 0; // easy case
  } else {
    notifyTheUser = numSubsessionsWithReceivedData >= numSubsessionsToCheck
      && numSubsessionsThatHaveBeenSynced == numSubsessionsChecked;
    // Note: A subsession with no active sources is considered to be synced
  }
  if (notifyTheUser) {
    struct timeval timeNow;
    gettimeofday(&timeNow, NULL);
	char timestampStr[100];
	sprintf(timestampStr, "%ld%03ld", timeNow.tv_sec, (long)(timeNow.tv_usec/1000));
    *env << (syncStreams ? "Synchronized d" : "D")
		<< "ata packets have begun arriving [" << timestampStr << "]\007\n";
    return;
  }

  // No luck, so reschedule this check again, after a delay:
  int uSecsToDelay = 100000; // 100 ms
  arrivalCheckTimerTask
    = env->taskScheduler().scheduleDelayedTask(uSecsToDelay,
			       (TaskFunc*)checkForPacketArrival, NULL);
}

void checkInterPacketGaps(void* /*clientData*/) {
  if (interPacketGapMaxTime == 0) return; // we're not checking

  // Check each subsession, counting up how many packets have been received:
  unsigned newTotNumPacketsReceived = 0;

  MediaSubsessionIterator iter(*session);
  MediaSubsession* subsession;
  while ((subsession = iter.next()) != NULL) {
    RTPSource* src = subsession->rtpSource();
    if (src == NULL) continue;
    newTotNumPacketsReceived += src->receptionStatsDB().totNumPacketsReceived();
  }

  if (newTotNumPacketsReceived == totNumPacketsReceived) {
    // No additional packets have been received since the last time we
    // checked, so end this stream:
    *env << "Closing session, because we stopped receiving packets.\n";
    interPacketGapCheckTimerTask = NULL;
    sessionAfterPlaying();
  } else {
    totNumPacketsReceived = newTotNumPacketsReceived;
    // Check again, after the specified delay:
    interPacketGapCheckTimerTask
      = env->taskScheduler().scheduleDelayedTask(interPacketGapMaxTime*1000000,
				 (TaskFunc*)checkInterPacketGaps, NULL);
  }
}

RTSPClient* ourRTSPClient = NULL;
Medium* createClient(UsageEnvironment& env, char const* url, int verbosityLevel, char const* applicationName) {
  extern portNumBits tunnelOverHTTPPortNum;
  return ourRTSPClient = RTSPClient::createNew(env, url, verbosityLevel, applicationName, tunnelOverHTTPPortNum);
}


void getSDPDescription(RTSPClient::responseHandler* afterFunc) {
  ourRTSPClient->sendDescribeCommand(afterFunc, ourAuthenticator);
}

void setupSubsession(MediaSubsession* subsession, Boolean streamUsingTCP, RTSPClient::responseHandler* afterFunc) {
  Boolean forceMulticastOnUnspecified = False;
  ourRTSPClient->sendSetupCommand(*subsession, afterFunc, False, streamUsingTCP, forceMulticastOnUnspecified, ourAuthenticator);
}

void startPlayingSession(MediaSession* session, double start, double end, float scale, RTSPClient::responseHandler* afterFunc) {
  ourRTSPClient->sendPlayCommand(*session, afterFunc, start, end, scale, ourAuthenticator);
}

void tearDownSession(MediaSession* session, RTSPClient::responseHandler* afterFunc) {
  ourRTSPClient->sendTeardownCommand(*session, afterFunc, ourAuthenticator);
}

