#ifndef _H264_VIDEO_STREAM_FRAMER_HH
#define _H264_VIDEO_STREAM_FRAMER_HH

#ifndef _FRAMED_FILTER_HH
#include "FramedFilter.hh"
#endif


class H264VideoStreamFramer: public FramedFilter {
public:

  static H264VideoStreamFramer* createNew(UsageEnvironment& env, FramedSource* inputSource);
  virtual Boolean currentNALUnitEndsAccessUnit();
  Boolean& pictureEndMarker() { return fPictureEndMarker; }    // a hack for implementing the RTP 'M' bit

protected:
  // Constructor called only by createNew(), or by subclass constructors
  H264VideoStreamFramer(UsageEnvironment& env,
                            FramedSource* inputSource,
                            Boolean createParser = True);
  virtual ~H264VideoStreamFramer();


public: 
  static void continueReadProcessing(void* clientData,
                     unsigned char* ptr, unsigned size,
                     struct timeval presentationTime);
  void continueReadProcessing();


private:
  virtual void doGetNextFrame();
  virtual Boolean isH264VideoStreamFramer() const;
  

protected:

    static void afterGettingFrame(void* clientData, unsigned frameSize,
                                unsigned numTruncatedBytes,
                                struct timeval presentationTime,
                                unsigned durationInMicroseconds);
  void afterGettingFrame1(unsigned frameSize,
                          unsigned numTruncatedBytes,
                          struct timeval presentationTime,
                          unsigned durationInMicroseconds);

  double   fFrameRate;    // Note: For MPEG-4, this is really a 'tick rate' ??
  unsigned fPictureCount; // hack used to implement doGetNextFrame() ??
  Boolean  fPictureEndMarker;

private:
  class H264VideoStreamParser* fParser;
  struct timeval fPresentationTimeBase;
};

#endif
