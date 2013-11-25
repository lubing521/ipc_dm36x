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
// An interface to the WIS GO7007 capture device.
// C++ header

#ifndef _APPRO_INPUT_HH
#define _APPRO_INPUT_HH

#include <MediaSink.hh>
#include <Appro_interface.h>

class APPROInput: public Medium {
public:
  static APPROInput* createNew(UsageEnvironment& env, int vType);
  int videoType;
  FramedSource* videoSource();
  FramedSource* audioSource();
  virtual ~APPROInput();

private:
  APPROInput(UsageEnvironment& env, int vType); // called only by createNew()
  

//  Boolean initialize(UsageEnvironment& env);
//  Boolean openFiles(UsageEnvironment& env);
//  static Boolean initALSA(UsageEnvironment& env);
//  static Boolean initV4L(UsageEnvironment& env);
//  static unsigned long getFrame(UsageEnvironment& env, unsigned char *addr, unsigned long len);
//  static void listVideoInputDevices(UsageEnvironment& env);

private:
  friend class VideoOpenFileSource;
  friend class AudioOpenFileSource;
//  int fVideoFileNo;
  FramedSource* fOurVideoSource;
//  static int fOurAudioFileNo;
  FramedSource* fOurAudioSource;
};


enum{
	VIDEO_TYPE_MPEG4	= 0,
	VIDEO_TYPE_MPEG4_CIF,
	VIDEO_TYPE_H264		,
	VIDEO_TYPE_H264_SVC_30FPS ,
	VIDEO_TYPE_H264_SVC_15FPS ,
	VIDEO_TYPE_H264_SVC_7FPS ,
	VIDEO_TYPE_H264_SVC_3FPS ,
	VIDEO_TYPE_H264_CIF	,
	VIDEO_TYPE_MJPEG	,
};


// Functions to set the optimal buffer size for RTP sink objects.
// These should be called before each RTPSink is created.
#define AUDIO_MAX_FRAME_SIZE 20480
#define VIDEO_MAX_FRAME_SIZE 2000000
inline void setAudioRTPSinkBufferSize() { OutPacketBuffer::maxSize = AUDIO_MAX_FRAME_SIZE; }
inline void setVideoRTPSinkBufferSize() { OutPacketBuffer::maxSize = VIDEO_MAX_FRAME_SIZE; }

#endif
