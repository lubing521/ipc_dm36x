/**********
This library is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the
Free Software Foundation; either version 2.1 of the License, or (at your
option) any later version. (See <http://www.gnu.org/copyleft/lesser.html>.)

This library is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
more details.

You should have received a copy of the GNU Lesser General Public License
along with this library; if not, write to the Free Software Foundation, Inc.,
59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
**********/
// "liveMedia"
// Copyright (c) 1996-2007 Live Networks, Inc.  All rights reserved.
// An abstract parser for H264 video streams
// Implementation

#include "H264VideoStreamFramer.hh"
#include "H264VideoStreamParser.hh"


H264VideoStreamParser
::H264VideoStreamParser(H264VideoStreamFramer* usingSource,
			FramedSource* inputSource)
  : StreamParser(inputSource, FramedSource::handleClosure, usingSource,
		 &H264VideoStreamFramer::continueReadProcessing, usingSource),
  fUsingSource(usingSource) {
}

H264VideoStreamParser::~H264VideoStreamParser() {
}

void H264VideoStreamParser::restoreSavedParserState() {
  StreamParser::restoreSavedParserState();
  fTo = fSavedTo;
  fNumTruncatedBytes = fSavedNumTruncatedBytes;
}

void H264VideoStreamParser::setParseState(H264ParseState parseState) {
//     std::cout << "H264VideoStreamPARSER: in setParseState: " << parseState << std::endl;
  fSavedTo = fTo;
  fSavedNumTruncatedBytes = fNumTruncatedBytes;
  fCurrentParseState = parseState;
  saveParserState();
}

unsigned H264VideoStreamParser::getParseState() {
    return fCurrentParseState;
}

void H264VideoStreamParser::registerReadInterest(unsigned char* to,
						 unsigned maxSize) {
//     std::cout << "Parser max size??: " << maxSize << std::endl;

  fStartOfFrame = fTo = fSavedTo = to;
  fLimit = to + maxSize;
  fNumTruncatedBytes = fSavedNumTruncatedBytes = 0;
}

unsigned H264VideoStreamParser::parse() {
    
  try {
//     std::cout << "H264VideoStreamPARSER: parse : " << fCurrentParseState << std::endl;
	fCurrentParseState = PARSING_START_SEQUENCE;
    switch (fCurrentParseState) {
    case PARSING_START_SEQUENCE: {
        return parseStartSequence();
//         return 0;
    }
    case PARSING_NAL_UNIT: {
        return parseNALUnit();
//         return 0;
    }
    default: {
      return 0; // shouldn't happen
    }
    }
  } catch (int /*e*/) {
#ifdef DEBUG
    fprintf(stderr, "H264VideoStreamParser::parse() EXCEPTION (This is normal behavior - *not* an error)\n");
#endif
    return 0;  // the parsing got interrupted
  }
}

unsigned H264VideoStreamParser::parseStartSequence()
{

    u_int32_t test = test4Bytes();

    while (test != 0x00000001)
    {
        skipBytes(1);
        test = test4Bytes();
    }
    setParseState(PARSING_START_SEQUENCE);
    skipBytes(4);
    return parseNALUnit();


}

unsigned H264VideoStreamParser::parseNALUnit()
{

    u_int32_t test = test4Bytes();
    int numBytes = 0;
    while (test != 0x00000001)
    {
        saveByte(get1Byte());
        numBytes++;
        test = test4Bytes();
    }
	//skipBytes(8);


    return curFrameSize();
}

