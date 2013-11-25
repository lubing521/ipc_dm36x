
#include <avserver_ui.h>

AVSERVER_UI_Config gAVSERVER_UI_config;

void showUsage()
{
  OSA_printf("\n");
  OSA_printf(" Audio Video Capture Server - (c) Texas Instruments 2009\n");
  OSA_printf("\n");
  OSA_printf(" USAGE: ./av_server.out <option 1> <option 2> ...\n");
  OSA_printf("\n");
  OSA_printf(" Following options are supported,\n");
  OSA_printf("\n");
  OSA_printf(" Resolution options - only one should be specified at a time,\n");
  OSA_printf(" D1           :720x480@30fps  + 288x192@30fps  capture mode\n");
  OSA_printf(" D1_D1        :720x480@30fps  + 720x480@30fps + 288x192@30fps capture mode\n");
  OSA_printf(" 720P         :1280x720@30fps + 320x192@30fps capture mode\n");
  OSA_printf(" 720P_720P    :1280x720@30fps + 1280x720@15fps + 320x192@15fps capture mode\n");
  OSA_printf(" 720P_720P_30 :1280x720@30fps + 1280x720@30fps + 320x192@30fps capture mode\n");
  OSA_printf(" 720P_VGA     :1280x720@30fps + 640x352@15fps + 320x192@15fps capture mode\n");
  OSA_printf(" 720P_VGA_30  :1280x720@30fps + 640x352@30fps + 320x192@30fps capture mode\n");
  OSA_printf(" SXVGA        :1280x960@30fps + 256x192@30fps capture mode\n");
  OSA_printf(" 1080P        :1920x1080@30fps + 320x192@30fps capture mode\n");
  OSA_printf(" 1080P_D1     :1920x1080@30fps + 720x480@30fps capture mode\n");
  OSA_printf(" 2MP          :1600x1200@30fps + 256x192@30fps capture mode\n");
  OSA_printf(" 3MP          :2048x1536@15fps + 256x192@15fps capture mode\n");
  OSA_printf(" 5MP          :2592x1920@10fps capture mode\n");
  OSA_printf(" 720P_MAX     :1280x720@60fps capture mode\n");
  OSA_printf("\n");
  OSA_printf(" Feature options - frame-rates may reduce depending on options enabled,\n");
  OSA_printf(" LDC          : Lens distortion correction is ENABLED\n");
  OSA_printf(" FD           : Face detection is ENABLED\n");
  OSA_printf(" VS           : Video Stabilization is ENABLED\n");
  OSA_printf(" SNF          : Spatial Noise Filter is ENABLED\n");
  OSA_printf(" TNF          : Temporal Noise Filter is ENABLED\n");
  OSA_printf(" AEWB         : AE/AWB/AEWB is ENABLED\n");
  OSA_printf(" <2A VENDOR>  : NONE2A/APPRO2A/TI2A to be selected based on the vendor for 2A algorithm\n");
  OSA_printf(" VNFDEMO      : This will put the 2A to high gain for increased noise and APPRO2A needs to be selected\n");
  OSA_printf(" DEMO         : If D1_D1 is selected this will switch off LDC, VS, SNF, TNF for second stream\n");
  OSA_printf(" SALDRE       : SALDRE is ENABLED\n");
  OSA_printf(" WIN          : Window Mode is ENABLED\n");
  OSA_printf(" RTSP         : RTSP network streaming is ENABLED\n");
  OSA_printf(" FLIPH        : Flip in H direction is ENABLED\n");
  OSA_printf(" FLIPV        : Flip in V direction is ENABLED\n");
  OSA_printf(" OSD          : Software OSD is ENABLED\n");
  OSA_printf(" NTSC / PAL / NOFLICKER : Mode for Flicker Detect\n");
  OSA_printf(" <PLATFORM>   : DM365/DM368 to be slected based on the platform chip\n");
  OSA_printf("\n");
  OSA_printf(" Codec options, \n");
  OSA_printf(" H264  <bitrate> <rcType> <preset>: H264 compression is ENABLED \n");
  OSA_printf("                 with bitrate = <bitrate>, rcType = CBR or VBR or VBR, preset = AUTO or CUSTOM\n");
  OSA_printf(" MPEG4 <bitrate> <rcType> <preset>: MPEG4 compression is ENABLED \n");
  OSA_printf("                 with bitrate = <bitrate>, rcType = CBR or VBR or VBR, preset = AUTO or CUSTOM\n");
  OSA_printf(" If SVC is needed, then 'SVC' option needs to be mentioned after <preset> \n");
  OSA_printf(" MJPEG <qvalue>                   : MJPEG compression is ENABLED \n");
  OSA_printf("                 with quality = <qvalue>\n");
  OSA_printf(" AUDIO <samplerate> <codectype> <bitrate> : AUDIO parameter details\n");
  OSA_printf("  	 <samplerate> : values are 8000 or 16000 \n");
  OSA_printf("  	 <codectype>  : type details are G711 or AAC \n");
  OSA_printf(" 		 <bitrate> 	  : bitrate value to be specified, for G711 this has no significance\n");
  OSA_printf("\n");
  OSA_printf(" Note,\n");
  OSA_printf(" - The options can be specified in any order\n");
  OSA_printf(" - Any option can be omited in which case default action is used\n");
  OSA_printf("\n");
  OSA_printf(" Example, \n");
  OSA_printf(" Example 1. # ./av_server.out DM365 AUDIO 8000 AAC 32000 D1 AEWB TI2A H264 3000000 VBR AUTO RTSP\n");
  OSA_printf(" Example 2. # ./av_server.out DM365 AUDIO 8000 AAC 48000 720P AEWB TI2A LDC TNF KNF FD OSD H264 3000000 VBR AUTO SVC RTSP\n");
  OSA_printf("\n");
}

int UI_getRateControlType(char *rateControlStr)
{
  if(strcmp(rateControlStr, "RCOFF")==0)
    return ALG_VID_ENC_RATE_CONTROL_RCOFF;

  if(strcmp(rateControlStr, "CBR")==0)
    return ALG_VID_ENC_RATE_CONTROL_CBR;

  if(strcmp(rateControlStr, "VBR")==0)
    return ALG_VID_ENC_RATE_CONTROL_VBR;

  return ALG_VID_ENC_RATE_CONTROL_VBR;
}
int UI_getEncodePreset(char *encPresetStr)
{
  if(strcmp(encPresetStr, "AUTO")==0)
    return ALG_VID_ENC_PRESET_AUTO;

  if(strcmp(encPresetStr, "CUSTOM")==0)
    return ALG_VID_ENC_PRESET_CUSTOM;

  return ALG_VID_ENC_PRESET_AUTO;
}

int main(int argc, char **argv)
{
  int status, i;

  if(argc==1) {
    showUsage();
    return 0;
  }

  memset(&gAVSERVER_UI_config, 0, sizeof(gAVSERVER_UI_config));

  #ifdef YUV_MODE_INTERLACED
  gAVSERVER_UI_config.mode 			= AVSERVER_UI_CAPTURE_MODE_D1;
  #else
  gAVSERVER_UI_config.mode 			= AVSERVER_UI_CAPTURE_MODE_720P;
  #endif
  gAVSERVER_UI_config.ldcEnable 		= FALSE;
  gAVSERVER_UI_config.saldreEnable 		= FALSE;
  gAVSERVER_UI_config.fdEnable 			= FALSE;
  gAVSERVER_UI_config.vsEnable 			= FALSE;
  gAVSERVER_UI_config.tnfEnable 		= FALSE;
  gAVSERVER_UI_config.snfEnable 		= FALSE;
  gAVSERVER_UI_config.winmodeEnable 	= FALSE;
  gAVSERVER_UI_config.vnfDemoEnable 	= FALSE;
  gAVSERVER_UI_config.codec 			= ALG_VID_CODEC_H264;
  gAVSERVER_UI_config.aewbEnable 		= TRUE;
  gAVSERVER_UI_config.afEnable 			= FALSE;
  gAVSERVER_UI_config.aewbType 			= AEWB_AEWB;
  gAVSERVER_UI_config.flickerType		= FLICKER_PAL;
  gAVSERVER_UI_config.rtspEnable 		= FALSE;
  gAVSERVER_UI_config.flipH 			= FALSE;
  gAVSERVER_UI_config.flipV 			= FALSE;
  gAVSERVER_UI_config.uartMenuEnable 	= TRUE;
  gAVSERVER_UI_config.swosdEnable		= FALSE;
  gAVSERVER_UI_config.numEncodes 		= 0;
  gAVSERVER_UI_config.aewbVendor 		= AEWB_TI;
  gAVSERVER_UI_config.audioEnable		= FALSE;//TRUE; //keep audio always on
  gAVSERVER_UI_config.demomode 			= FALSE;
  gAVSERVER_UI_config.audioSampleRate 	= AUDIO_GetSampleRate();
  gAVSERVER_UI_config.platform_id 		= PLATFORM_DM365;

  for(i=1; i< argc; i++) {

    if(strcmp(argv[i], "D1")==0)
      gAVSERVER_UI_config.mode = AVSERVER_UI_CAPTURE_MODE_D1;
    else
    if(strcmp(argv[i], "D1_D1")==0)
      gAVSERVER_UI_config.mode = AVSERVER_UI_CAPTURE_MODE_D1_D1;
    else
    if(strcmp(argv[i], "SXVGA")==0)
      gAVSERVER_UI_config.mode = AVSERVER_UI_CAPTURE_MODE_SXVGA;
    else
    if(strcmp(argv[i], "1080P_VGA")==0)
      gAVSERVER_UI_config.mode = AVSERVER_UI_CAPTURE_MODE_1080P_VGA;
    else
    if(strcmp(argv[i], "1080P_QVGA")==0)
      gAVSERVER_UI_config.mode = AVSERVER_UI_CAPTURE_MODE_1080P_QVGA;
    else
    if(strcmp(argv[i], "2MP")==0)
      gAVSERVER_UI_config.mode = AVSERVER_UI_CAPTURE_MODE_2_MEGA;
    else
    if(strcmp(argv[i], "3MP")==0)
      gAVSERVER_UI_config.mode = AVSERVER_UI_CAPTURE_MODE_3_MEGA;
    else
    if(strcmp(argv[i], "5MP")==0)
      gAVSERVER_UI_config.mode = AVSERVER_UI_CAPTURE_MODE_5_MEGA;
    else
    if(strcmp(argv[i], "720P_D1")==0)
      gAVSERVER_UI_config.mode = AVSERVER_UI_CAPTURE_MODE_720P_D1;
    else		
    if(strcmp(argv[i], "720P_VGA")==0)
      gAVSERVER_UI_config.mode = AVSERVER_UI_CAPTURE_MODE_720P_VGA;
    else
    if(strcmp(argv[i], "720P_CIF")==0)
      gAVSERVER_UI_config.mode = AVSERVER_UI_CAPTURE_MODE_720P_CIF;
    else		
    if(strcmp(argv[i], "720P_QVGA")==0)
      gAVSERVER_UI_config.mode = AVSERVER_UI_CAPTURE_MODE_720P_QVGA;
    else
    if(strcmp(argv[i], "720P_VGA_JPG")==0)
      gAVSERVER_UI_config.mode = AVSERVER_UI_CAPTURE_MODE_720P_VGA_JPG;
    else
    if(strcmp(argv[i], "720P_QVGA_JPG")==0)
      gAVSERVER_UI_config.mode = AVSERVER_UI_CAPTURE_MODE_720P_QVGA_JPG;
    else
    if(strcmp(argv[i], "960P_D1")==0)
      gAVSERVER_UI_config.mode = AVSERVER_UI_CAPTURE_MODE_960P_D1;
    else		
	if(strcmp(argv[i], "960P_VGA")==0)
      gAVSERVER_UI_config.mode = AVSERVER_UI_CAPTURE_MODE_960P_VGA;
    else
    if(strcmp(argv[i], "960P_CIF")==0)
      gAVSERVER_UI_config.mode = AVSERVER_UI_CAPTURE_MODE_960P_CIF;
    else		
    if(strcmp(argv[i], "960P_QVGA")==0)
      gAVSERVER_UI_config.mode = AVSERVER_UI_CAPTURE_MODE_960P_QVGA;
    else
    if(strcmp(argv[i], "960P_VGA_JPG")==0)
      gAVSERVER_UI_config.mode = AVSERVER_UI_CAPTURE_MODE_960P_VGA_JPG;
    else
    if(strcmp(argv[i], "960P_QVGA_JPG")==0)
      gAVSERVER_UI_config.mode = AVSERVER_UI_CAPTURE_MODE_960P_QVGA_JPG;
    else
    if(strcmp(argv[i], "720P_720P")==0)
      gAVSERVER_UI_config.mode = AVSERVER_UI_CAPTURE_MODE_720P_720P;
    else
    if(strcmp(argv[i], "720P_720P_30")==0)
      gAVSERVER_UI_config.mode = AVSERVER_UI_CAPTURE_MODE_720P_720P_30;
    else
    if(strcmp(argv[i], "720P")==0)
      gAVSERVER_UI_config.mode = AVSERVER_UI_CAPTURE_MODE_720P;
    else
	if(strcmp(argv[i], "1080P_ONLY")==0)//add by jsk
		gAVSERVER_UI_config.mode = AVSERVER_UI_CAPTURE_MODE_1080P_ONLY;
	else
	if (strcmp(argv[i], "720P_ONLY")==0)//add by jsk
		gAVSERVER_UI_config.mode = AVSERVER_UI_CAPTURE_MODE_720P_ONLY;
	else
    if(strcmp(argv[i], "720P_MAX")==0)
      gAVSERVER_UI_config.mode = AVSERVER_UI_CAPTURE_MODE_720P_MAX;
    else
    if(strcmp(argv[i], "WIN")==0)
      gAVSERVER_UI_config.winmodeEnable = TRUE;
    else
    if(strcmp(argv[i], "VNFDEMO")==0)
      gAVSERVER_UI_config.vnfDemoEnable = TRUE;
    else
    if(strcmp(argv[i], "NONE2A")==0)
      gAVSERVER_UI_config.aewbVendor = AEWB_NONE;
    else
    if(strcmp(argv[i], "APPRO2A")==0)
      gAVSERVER_UI_config.aewbVendor = AEWB_APPRO;
    else
    if(strcmp(argv[i], "TI2A")==0)
      gAVSERVER_UI_config.aewbVendor = AEWB_TI;
    else
    if(strcmp(argv[i], "NTSC")==0)
      gAVSERVER_UI_config.flickerType = FLICKER_NTSC;
    else
    if(strcmp(argv[i], "PAL")==0)
      gAVSERVER_UI_config.flickerType = FLICKER_PAL;
    else
    if(strcmp(argv[i], "NOFLICKER")==0)
      gAVSERVER_UI_config.flickerType = FLICKER_NONE;
    else
    if(strcmp(argv[i], "LDC")==0)
      gAVSERVER_UI_config.ldcEnable = TRUE;
    else
    if(strcmp(argv[i], "SALDRE")==0)
      gAVSERVER_UI_config.saldreEnable = TRUE;
    else
    if(strcmp(argv[i], "FD")==0)
      gAVSERVER_UI_config.fdEnable = TRUE;
    else
    if(strcmp(argv[i], "VS")==0)
      gAVSERVER_UI_config.vsEnable = TRUE;
    else
    if(strcmp(argv[i], "SNF")==0)
      gAVSERVER_UI_config.snfEnable = TRUE;
    else
    if(strcmp(argv[i], "KNF")==0)
      gAVSERVER_UI_config.snfEnable = TRUE;
    else
    if(strcmp(argv[i], "TNF")==0)
      gAVSERVER_UI_config.tnfEnable = TRUE;
    else
    if(strcmp(argv[i], "DM365")==0)
      gAVSERVER_UI_config.platform_id = PLATFORM_DM365;
    else
    if(strcmp(argv[i], "DM368")==0)
      gAVSERVER_UI_config.platform_id = PLATFORM_DM368;
    else
    if(strcmp(argv[i], "DEMO")==0)
      gAVSERVER_UI_config.demomode = TRUE;
    else
    if(strcmp(argv[i], "AUDIO")==0) {
	  gAVSERVER_UI_config.audioEnable		= TRUE;
      gAVSERVER_UI_config.audioSampleRate 	= atoi(argv[i+1]);
      if(strcmp(argv[i+2], "AAC")==0)
      	gAVSERVER_UI_config.audioCodeType 	= ALG_AUD_CODEC_AAC;
      else
      	gAVSERVER_UI_config.audioCodeType 	= ALG_AUD_CODEC_G711;

      gAVSERVER_UI_config.audioBitRate 		= atoi(argv[i+3]);
      i+=3;
    }
    else
    if(strcmp(argv[i], "H264")==0) {
      gAVSERVER_UI_config.codecType[gAVSERVER_UI_config.numEncodes] = ALG_VID_CODEC_H264;
      gAVSERVER_UI_config.codecBitrate[gAVSERVER_UI_config.numEncodes] = atoi(argv[i+1]);
      gAVSERVER_UI_config.codecRateControlType[gAVSERVER_UI_config.numEncodes] = UI_getRateControlType(argv[i+2]);
      gAVSERVER_UI_config.codecEncodePreset[gAVSERVER_UI_config.numEncodes] = UI_getEncodePreset(argv[i+3]);
	  gAVSERVER_UI_config.codecProfile[gAVSERVER_UI_config.numEncodes] = atoi(argv[i+4]);
	  printf("gAVSERVER_UI_config.codecProfile[%d]=%d\n" ,gAVSERVER_UI_config.numEncodes , gAVSERVER_UI_config.codecProfile[gAVSERVER_UI_config.numEncodes]);
	  
	  if(strcmp(argv[i+5], "SVC")==0) {
		gAVSERVER_UI_config.codecEncodePreset[gAVSERVER_UI_config.numEncodes] |= ALG_VID_ENC_PRESET_SVC; i++;
	  }

      gAVSERVER_UI_config.numEncodes++;
      i+=3;
    }
    else
    if(strcmp(argv[i], "MPEG4")==0) {
      gAVSERVER_UI_config.codecType[gAVSERVER_UI_config.numEncodes] = ALG_VID_CODEC_MPEG4;
      gAVSERVER_UI_config.codecBitrate[gAVSERVER_UI_config.numEncodes] = atoi(argv[i+1]);
      gAVSERVER_UI_config.codecRateControlType[gAVSERVER_UI_config.numEncodes] = UI_getRateControlType(argv[i+2]);
      gAVSERVER_UI_config.codecEncodePreset[gAVSERVER_UI_config.numEncodes] = UI_getEncodePreset(argv[i+3]);

      gAVSERVER_UI_config.numEncodes++;
      i+=3;
    }
    else
    if(strcmp(argv[i], "MJPEG")==0) {
      gAVSERVER_UI_config.codecType[gAVSERVER_UI_config.numEncodes] = ALG_VID_CODEC_MJPEG;
      gAVSERVER_UI_config.codecBitrate[gAVSERVER_UI_config.numEncodes] = atoi(argv[i+1]);
      gAVSERVER_UI_config.numEncodes++;
      i++;
    }
    else
    if(strcmp(argv[i], "AE")==0) {
      gAVSERVER_UI_config.aewbEnable = TRUE;
      gAVSERVER_UI_config.aewbType = AEWB_AE;
    }
    else
    if(strcmp(argv[i], "AWB")==0) {
      gAVSERVER_UI_config.aewbEnable = TRUE;
      gAVSERVER_UI_config.aewbType = AEWB_AWB;
    }
    else
    if(strcmp(argv[i], "AEWB")==0) {
      gAVSERVER_UI_config.aewbEnable = TRUE;
      gAVSERVER_UI_config.aewbType = AEWB_AEWB;
    }
    else
    if(strcmp(argv[i], "RTSP")==0)
      gAVSERVER_UI_config.rtspEnable = TRUE;
    else
    if(strcmp(argv[i], "FLIPH")==0)
      gAVSERVER_UI_config.flipH = TRUE;
    else
    if(strcmp(argv[i], "FLIPV")==0)
      gAVSERVER_UI_config.flipV = TRUE;
    else
    if(strcmp(argv[i], "MENUOFF")==0)
      gAVSERVER_UI_config.uartMenuEnable = FALSE;
    else
    if(strcmp(argv[i], "OSD")==0)
      gAVSERVER_UI_config.swosdEnable = TRUE;
  }


  status = UI_start();
  if(status!=OSA_SOK)
    return status;

  if(gAVSERVER_UI_config.uartMenuEnable) {
    UI_menuRun();
  } else {
    while(!UI_isDone()) {
      OSA_waitMsecs(1000);
    }
  }

  UI_stop();

  return 0;
}

