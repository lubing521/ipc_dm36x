/*
 * Copyright (C) 2005-2006 WIS Technologies International Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and the associated README documentation file (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
// A class that encapsulates a 'FramedSource' object that
// returns JPEG video frames.
// Implementation

#include "WISJPEGStreamSource.hh"

WISJPEGStreamSource*
WISJPEGStreamSource::createNew(FramedSource* inputSource) {
  return new WISJPEGStreamSource(inputSource);
}

WISJPEGStreamSource::WISJPEGStreamSource(FramedSource* inputSource)
  : JPEGVideoSource(inputSource->envir()),
    fLastWidth(0), fLastHeight(0), fLastQuantizationTableSize(128),fLastrestartIntervalTableSize(4) {
  fSource = inputSource;
}

WISJPEGStreamSource::~WISJPEGStreamSource() {
  Medium::close(fSource);
}

void WISJPEGStreamSource::doGetNextFrame() {
  fSource->getNextFrame(fBuffer, sizeof fBuffer,
			WISJPEGStreamSource::afterGettingFrame, this,
			FramedSource::handleClosure, this);
}

u_int8_t WISJPEGStreamSource::type() {
  return 65;
};

u_int8_t WISJPEGStreamSource::qFactor() {
  return 255; // indicates that quantization tables are dynamic
};

u_int8_t WISJPEGStreamSource::width() {
  return fLastWidth;
}

u_int8_t WISJPEGStreamSource::height() {
  return fLastHeight;
}

u_int8_t const* WISJPEGStreamSource::quantizationTables(u_int8_t& precision,
							   u_int16_t& length) {
  precision = 0;
  length = fLastQuantizationTableSize;
  return fLastQuantizationTable;
}

u_int8_t const* WISJPEGStreamSource::restartIntervalTables(u_int8_t& precision,
							   u_int16_t& length) {
  precision = 0;
  length = fLastrestartIntervalTableSize;
  return fLastrestartIntervalTable;
}

 
void WISJPEGStreamSource
::afterGettingFrame(void* clientData, unsigned frameSize,
		    unsigned numTruncatedBytes,
		    struct timeval presentationTime,
		    unsigned durationInMicroseconds) {
  WISJPEGStreamSource* source = (WISJPEGStreamSource*)clientData;
  source->afterGettingFrame1(frameSize, numTruncatedBytes,
			     presentationTime, durationInMicroseconds);
}
extern void printheader(char *pData, int size);



void WISJPEGStreamSource
::afterGettingFrame1(unsigned frameSize, unsigned numTruncatedBytes,
		     struct timeval presentationTime,
		     unsigned durationInMicroseconds) {
  fFrameSize = frameSize < fMaxSize ? frameSize : fMaxSize;
  // NOTE: Change the following if the size of the encoder's JPEG hdr changes
  unsigned int JPEGHeaderSize = 0x265;
  unsigned i = 0;
  
  
  // Look for the "SOF0" marker (0xFF 0xC0) in the header, to get the frame
  // width and height.  Also, look for the "DQT" marker(s) (0xFF 0xDB), to
  // get the quantization table(s):
  Boolean foundSOF0 = False;
  fLastQuantizationTableSize = 0;
  for (i = 0; i < JPEGHeaderSize-8; ++i) {
	if( fLastQuantizationTableSize == 0)
	{
		i = 0x14;
	}else{
		if( foundSOF0 != True )
		{
			i = 0x23E;
		}
	}
  
    if (fBuffer[i] == 0xFF) {
		if (fBuffer[i+1] == 0xDB) 
		{ // DQT
			u_int16_t length = (fBuffer[i+2]<<8) | fBuffer[i+3];
			if (i+2 + length < JPEGHeaderSize) 
			{ // sanity check
				u_int16_t tableSize = length - 3;
				if (fLastQuantizationTableSize + tableSize > 128) { // sanity check
				tableSize = 128 - fLastQuantizationTableSize;
				}
				memmove(&fLastQuantizationTable[fLastQuantizationTableSize],
				  &fBuffer[i+5], 64);
				memmove(&fLastQuantizationTable[fLastQuantizationTableSize+64],
				  &fBuffer[i+5+1+64], 64);

				
				fLastQuantizationTableSize += tableSize;
				
				if (fLastQuantizationTableSize == 128 && foundSOF0) break;
				
				  // we've found everything that we want
				i += length; // skip over table
			}
      	} 
     	else if (fBuffer[i+1] == 0xC0) 
		{ // SOF0

			fLastHeight = (fBuffer[i+5]<<5)|(fBuffer[i+6]>>3);
			fLastWidth = (fBuffer[i+7]<<5)|(fBuffer[i+8]>>3);
			foundSOF0 = True;
			if (fLastQuantizationTableSize == 128) break;


			// we've found everything that we want
			i += 8;
		}
    }
  }
  	
  for (i=0x251; i < JPEGHeaderSize-8; ++i) 
  {
    if (fBuffer[i] == 0xFF) 
    {
		if (fBuffer[i+1] == 0xDD) 
		{
			fLastrestartIntervalTable[0] = fBuffer[i+4];
			fLastrestartIntervalTable[1] = fBuffer[i+5];
			fLastrestartIntervalTable[2] = 0xFF;
			fLastrestartIntervalTable[3] = 0xFF;
			break;
		}
	}
  }

  
  
  if (fLastQuantizationTableSize == 64) {
    // Hack: We apparently saw only one quantization table.  Unfortunately,
    // media players seem to be unhappy if we don't send two (luma+chroma).
    // So, duplicate the existing table data:
    memmove(&fLastQuantizationTable[64], fLastQuantizationTable, 64);
    fLastQuantizationTableSize = 128;
  }

  if (!foundSOF0) envir() << "Failed to find SOF0 marker in JPEG header!\n";

  // Complete delivery to the client:
  fFrameSize = fFrameSize > JPEGHeaderSize ? fFrameSize - JPEGHeaderSize : 0;
  memmove(fTo, &fBuffer[JPEGHeaderSize], fFrameSize);
  fNumTruncatedBytes = numTruncatedBytes;
  fPresentationTime = presentationTime;
  fDurationInMicroseconds = fDurationInMicroseconds;
  FramedSource::afterGetting(this);

}
