#ifndef _AV_SERVER_UI_H_
#define _AV_SERVER_UI_H_

#include <avserver.h>
#include <alg.h>

#define ALIGN_ENCODE(X)	  OSA_floor((X), 16)

typedef enum{
	AVSERVER_UI_CAPTURE_MODE_D1 = 0,
	AVSERVER_UI_CAPTURE_MODE_D1_D1,
	AVSERVER_UI_CAPTURE_MODE_720P,
	
	AVSERVER_UI_CAPTURE_MODE_720P_VGA,
	AVSERVER_UI_CAPTURE_MODE_720P_QVGA,
	AVSERVER_UI_CAPTURE_MODE_720P_D1,
	AVSERVER_UI_CAPTURE_MODE_720P_CIF,	
	AVSERVER_UI_CAPTURE_MODE_720P_VGA_JPG,
	AVSERVER_UI_CAPTURE_MODE_720P_QVGA_JPG,	
	
	AVSERVER_UI_CAPTURE_MODE_960P_VGA,
	AVSERVER_UI_CAPTURE_MODE_960P_QVGA,
	AVSERVER_UI_CAPTURE_MODE_960P_D1,
	AVSERVER_UI_CAPTURE_MODE_960P_CIF,	
	AVSERVER_UI_CAPTURE_MODE_960P_VGA_JPG,
	AVSERVER_UI_CAPTURE_MODE_960P_QVGA_JPG,	
	
	AVSERVER_UI_CAPTURE_MODE_720P_720P,
	AVSERVER_UI_CAPTURE_MODE_720P_720P_30,
	AVSERVER_UI_CAPTURE_MODE_720P_MAX,
	
	AVSERVER_UI_CAPTURE_MODE_1080P_QVGA,
	AVSERVER_UI_CAPTURE_MODE_1080P_VGA,
	AVSERVER_UI_CAPTURE_MODE_1080P_D1,
	AVSERVER_UI_CAPTURE_MODE_1080P_CIF,
	
	AVSERVER_UI_CAPTURE_MODE_SXVGA,
	AVSERVER_UI_CAPTURE_MODE_2_MEGA,
	AVSERVER_UI_CAPTURE_MODE_3_MEGA,
	AVSERVER_UI_CAPTURE_MODE_5_MEGA,
	AVSERVER_UI_CAPTURE_MODE_1080P_ONLY,//add by jsk
	AVSERVER_UI_CAPTURE_MODE_720P_ONLY//add by jsk
}AVSERVER_UI_CAPTURE_MODE;

typedef struct {

  char hostName[40];
  int  mode;

  Bool quit;
  Bool fdEnable;
  Bool ldcEnable;
  Bool vsEnable;
  Bool tnfEnable;
  Bool snfEnable;
  Bool fileSaveEnable;
  Bool aewbEnable;
  Bool afEnable;
  Bool swosdEnable;            //changes by Rajiv for SWOSD
  Bool winmodeEnable;
  Bool vnfDemoEnable;
  Bool saldreEnable;

  int  fileSaveIndex;
  int  aewbType;
  int  aewbVendor;

  AVSERVER_Config avserverConfig;

} AVSERVER_UI_Ctrl;

typedef struct {

  int  mode;
  int  codec;
  int  aewbType;
  int  aewbVendor;
  int  flickerType;
  Bool vsEnable;
  Bool fdEnable;
  Bool ldcEnable;
  Bool tnfEnable;
  Bool snfEnable;
  Bool aewbEnable;
  Bool afEnable;
  Bool rtspEnable;
  Bool swosdEnable;
  Bool winmodeEnable;
  Bool vnfDemoEnable;
  Bool saldreEnable;
  Bool demomode;
  Bool flipH;
  Bool flipV;

  int numEncodes;
  int codecType[AVSERVER_MAX_STREAMS];
  int codecBitrate[AVSERVER_MAX_STREAMS];
  int codecRateControlType[AVSERVER_MAX_STREAMS];
  int codecEncodePreset[AVSERVER_MAX_STREAMS];
//////zeng add   
  int codecProfile[AVSERVER_MAX_STREAMS];

  Bool audioEnable;
  int audioSampleRate;
  int audioCodeType;
  int audioBitRate;

  Bool uartMenuEnable;
  int platform_id;

} AVSERVER_UI_Config;

extern AVSERVER_UI_Ctrl gAVSERVER_UI_ctrl;
extern AVSERVER_UI_Config gAVSERVER_UI_config;

void UI_signalHandler(int signum);

void UI_setConfig(AVSERVER_Config *config);

int  UI_start();

int  UI_menuRun();

int  UI_stop();

int UI_setDone(Bool isDone);
int UI_isDone();

#endif

