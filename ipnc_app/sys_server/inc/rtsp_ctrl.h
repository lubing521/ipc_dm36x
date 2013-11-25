#ifndef __RTSP_CTRL_H__
#define __RTSP_CTRL_H__

#define RTSP_RUN		0x01
#define RTSP_MUTE		0x02
#define RTSP_AAC		0x04
#define RTSP_MULTICAST	0x08
#define RTSP_SVC		0x10

#define RTSP_FUNC_MASK	(RTSP_MUTE | RTSP_AAC | RTSP_MULTICAST | RTSP_SVC)

int RTSP_ControlInit(void);
int GetRTSPStreamOption(void);
int SetRTSPStreamingON(int nExtFunc);
int SetRTSPStreamingOFF(void);
int RTSP_ControlExit(void);
int RTSP_AudioRecvON(void);

#endif /* __RTSP_CTRL_H__ */
