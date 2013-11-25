#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include <web_translate.h>
#include <sys_env_type.h>
#include <file_msg_drv.h>
#include <net_config.h>
#include <sysctrl.h>
#include <file_list.h>
#include <para_list.h>
#include <system_default.h>

char html_paralist_header[] = "<HTML><META HTTP-EQUIV=\"CACHE-CONTROL\" CONTENT=\"NO-CACHE\"><TITLE>Current Parameter List </TITLE><style>"
"html {scrollbar-face-color: #182439;scrollbar-shadow-color: #182439;scrollbar-highlight-color: #182439;scrollbar-3dlight-color: #182439;scrollbar-darkshadow-color: #182439;scrollbar-track-color: 182439;scrollbar-arrow-color: #182439;}"
".scrClr{scrollbar-face-color: #1E558C;scrollbar-shadow-color: #182439;scrollbar-highlight-color: #182439;scrollbar-3dlight-color: #182439;scrollbar-darkshadow-color: #182439;scrollbar-track-color: 123456;scrollbar-arrow-color: #182439;}"
"BODY {	PADDING-RIGHT: 0px; MARGIN-TOP: 10px; PADDING-LEFT: 10px; MARGIN-LEFT: auto; WIDTH: auto; MARGIN-RIGHT: auto; background-color:#182439; FONT-FAMILY: Tahoma, Geneva, Arial, \"Arial Narrow\", Verdana, sans-serif; font-size:18px; color:#FFFFFF}A.linktxt:link {PADDING-RIGHT: 0px; PADDING-LEFT: 0px; FONT-WEIGHT: normal; FONT-SIZE: 18px; PADDING-BOTTOM: 0px; MARGIN: 0px; COLOR: #FFFFFF; PADDING-TOP: 0px; TEXT-DECORATION: underline}.linktxt:visited {PADDING-RIGHT: 0px; PADDING-LEFT: 0px; FONT-WEIGHT: normal; FONT-SIZE: 18px; PADDING-BOTTOM: 0px; MARGIN: 0px; COLOR: #FFFFFF; PADDING-TOP: 0px; TEXT-DECORATION: underline} A.linktxt:active { PADDING-RIGHT: 0px; PADDING-LEFT: 0px; FONT-WEIGHT: normal; FONT-SIZE: 18px; PADDING-BOTTOM: 0px; MARGIN: 0px; COLOR: #FFFFFF; PADDING-TOP: 0px; TEXT-DECORATION: underline} A.linktxt:hover { FONT-WEIGHT: normal; FONT-SIZE: 18px; COLOR: #FFFFFF; TEXT-DECORATION: underline}.heading{ FONT-FAMILY: Tahoma, Geneva, Arial, \"Arial Narrow\", Verdana, sans-serif; font-size:18pt; color:#FFFFFF } .normaltxt{ 	FONT-FAMILY: Courier New, Tahoma, Geneva, Arial, \"Arial Narrow\", Verdana, sans-serif; font-size:18px; color:#FFFFFF }.tableheadtxt{ 	FONT-FAMILY: Courier New, Tahoma, Geneva, Arial, \"Arial Narrow\", Verdana, sans-serif; font-size:18px; color:#000000} </style><BODY><PRE style='font-family:\"Courier New\"; font-size:18px'><PRE style='font-family:\"Courier New\"; font-size:18px'><span class=heading>Current Parameter List</span><br><br>"
"<div class='scrClr' STYLE=\"position:relative;width:620;height:80%;overflow-y:scroll;z-index:10;\"><table border=0 cellspacing=1 cellpadding=1 class=normaltxt width=600><tr align=center bgcolor=#EEEEEE class=tableheadtxt><td>Parameter Name </td><td>Current Value </td><td>Default Value</td></tr><script>var z=\"sddel.htm\";function F(b){var o=\"\";var i;for(i=0;i<b;i++){o+=' ';}return o;}"
"function A(a,b,c){var o=\'';o+='<tr valign=middle><td>'+a+'</td><td>';o+=F(15-a.length)+b+' </td><td> '+c+' </td></tr><tr><td colspan=5><hr size=1 color=#3030A0></td></tr>';document.write(o);}";

extern char html_content[1024];

void GetStreamType(char *sType)
{
   SysInfo* pSysInfo = GetSysInfo();
   if(pSysInfo == NULL)
		strcpy(sType, "NULL");

    if(pSysInfo->lan_config.nVideocodecmode >= 0 && pSysInfo->lan_config.nVideocodecmode <=2)
   	{
            strcpy(sType, "Single");
	}
	else  if(pSysInfo->lan_config.nVideocodecmode >= 3 && pSysInfo->lan_config.nVideocodecmode <=7)
	{
            strcpy(sType, "Dual");
	}
	else
	{
            strcpy(sType, "Tri-Stream");
	}

}

void GetCodecCombo(char *cCombo)
{
       SysInfo* pSysInfo = GetSysInfo();
   	   if(pSysInfo == NULL)
		  strcpy(cCombo, "NULL");

       switch(pSysInfo->lan_config.nVideocodecmode)
       {
		 default:
		 case 0:
		   	strcpy(cCombo, "H.264" );
			break;
	     case 1:
		   	strcpy(cCombo, "MPEG4" );
			break;
	     case 2:
		   	strcpy(cCombo, "MegaPixel");
			break;
	     case 3:
		   	strcpy(cCombo, "H.264 + JPEG");
			break;
	     case 4:
		   	strcpy(cCombo, "MPEG4 + JPEG");
			break;
	     case 5:
		   	strcpy(cCombo, "Dual H.264");
			break;
	     case 6:
		   	strcpy(cCombo, "Dual MPEG4");
			break;
	     case 7:
		   	strcpy(cCombo, "H264 + MPEG4");
			break;
	     case 8:
		   	strcpy(cCombo, "Dual H.264 + JPEG");
			break;
	     case 9:
		   	strcpy(cCombo, "Dual MPEG4 + JPEG");
			break;
	 }

}

void GetResolution(char *resl)
{
       SysInfo* pSysInfo = GetSysInfo();
   	   if(pSysInfo == NULL)
		  strcpy(resl, "NULL");

       switch(pSysInfo->lan_config.nVideocodecmode)
       {
         default:
         case 0:
		   	if(pSysInfo->lan_config.nVideocodecres == 0)
		   	     strcpy(resl, "H264:720" );
			else if(pSysInfo->lan_config.nVideocodecres == 1)
			     strcpy(resl, "H264:D1" );
			else if(pSysInfo->lan_config.nVideocodecres == 2)
			     strcpy(resl, "H264:SXVGA" );
			else if(pSysInfo->lan_config.nVideocodecres == 3)
			     strcpy(resl, "H264:1080" );
			else if(pSysInfo->lan_config.nVideocodecres == 4)
			     strcpy(resl, "H264:720P60" );

			break;
	     case 1:
		   	if(pSysInfo->lan_config.nVideocodecres == 0)
		   	     strcpy(resl, "MPEG4:720" );
			else if(pSysInfo->lan_config.nVideocodecres == 1)
		   	     strcpy(resl, "MPEG4:D1" );
			else if(pSysInfo->lan_config.nVideocodecres == 2)
		   	     strcpy(resl, "MPEG4:SXVGA" );
			else if(pSysInfo->lan_config.nVideocodecres == 3)
		   	     strcpy(resl, "MPEG4:1080" );
			else if(pSysInfo->lan_config.nVideocodecres == 4)
		   	     strcpy(resl, "MPEG4:720P60" );

			break;
	     case 2:
		   	if(pSysInfo->lan_config.nVideocodecres == 0)
		   	     strcpy(resl, "H264:1600x1200" );
		   	else if(pSysInfo->lan_config.nVideocodecres == 1)
		   	     strcpy(resl, "JPEG:1600x1200" );
			else if(pSysInfo->lan_config.nVideocodecres == 2)
			     strcpy(resl, "H264:2048x1536" );
			else if(pSysInfo->lan_config.nVideocodecres == 3)
			     strcpy(resl, "JPEG:2048x1536" );
			else if(pSysInfo->lan_config.nVideocodecres == 4)
			     strcpy(resl, "H264:2592x1920" );
			else if(pSysInfo->lan_config.nVideocodecres == 5)
			     strcpy(resl, "JPEG:2592x1920" );
			break;
	     case 3:
		   	if(pSysInfo->lan_config.nVideocodecres == 0)
		   	     strcpy(resl, "H264:720,JPEG:VGA" );
		    else if(pSysInfo->lan_config.nVideocodecres == 1)
		   	     strcpy(resl, "H264:D1,JPEG:D1" );
			else if(pSysInfo->lan_config.nVideocodecres == 2)
			     strcpy(resl, "H264:720,JPEG:720" );

			break;
	     case 4:
		   	if(pSysInfo->lan_config.nVideocodecres == 0)
		   	     strcpy(resl, "MPEG4:720,JPEG:VGA" );
		    else if(pSysInfo->lan_config.nVideocodecres == 1)
		   	     strcpy(resl, "MPEG4:D1,JPEG:D1" );
		    else if(pSysInfo->lan_config.nVideocodecres == 2)
		   	     strcpy(resl, "MPEG4:720,JPEG:720" );

			break;
	     case 5:
		   	if(pSysInfo->lan_config.nVideocodecres == 0)
		   	     strcpy(resl, "H264:720,H264:QVGA" );
		    else if(pSysInfo->lan_config.nVideocodecres == 1)
		   	     strcpy(resl, "H264:D1,H264:D1" );
		    else if(pSysInfo->lan_config.nVideocodecres == 2)
		   	     strcpy(resl, "H264:D1,H264:QVGA" );
		    else if(pSysInfo->lan_config.nVideocodecres == 3)
		   	     strcpy(resl, "H264:1080,H264:QVGA" );

			break;
	     case 6:
		   	if(pSysInfo->lan_config.nVideocodecres == 0)
		   	     strcpy(resl, "MPEG4:720,MPEG4:QVGA" );
		 	else if(pSysInfo->lan_config.nVideocodecres == 1)
		   	     strcpy(resl, "MPEG4:D1,MPEG4:D1" );
		 	else if(pSysInfo->lan_config.nVideocodecres == 2)
		   	     strcpy(resl, "MPEG4:D1,MPEG4:QVGA" );
			else if(pSysInfo->lan_config.nVideocodecres == 3)
		   	     strcpy(resl, "MPEG4:1080,MPEG4:QVGA" );

			break;
	     case 7:
		   	strcpy(resl, "H264:D1,MPEG4:D1" );
			break;
	     case 8:
		   	strcpy(resl, "H264:720,JPEG:VGA,H264:QVGA");
			break;
	     case 9:
		   	strcpy(resl, "MPEG4:720,JPEG:VGA,MPEG4:QVGA");
			break;
	 }
}

void GetFaceDetect(char *faceDetect)
{
     SysInfo* pSysInfo = GetSysInfo();

     if(pSysInfo->face_config.fdetect)
     {
          strcpy(faceDetect, "Enable");
     }
     else
     {
          strcpy(faceDetect, "Disable");
     }

}

void GetPrivacyMask(char *privacyMask)
{
     SysInfo* pSysInfo = GetSysInfo();

     if(pSysInfo->face_config.pmask)
     {
           strcpy(privacyMask, "Enable");
     }
     else
     {
          strcpy(privacyMask, "Disable");
     }

}

void GetFaceRecognition(char *fRecognition)
{
     SysInfo* pSysInfo = GetSysInfo();

     if(pSysInfo->face_config.frecog == 0)
     {
          strcpy(fRecognition, "OFF");
     }
     else if(pSysInfo->face_config.frecog == 1)
     {
          strcpy(fRecognition, "RECOGNIZE USER");
     }
	 else if(pSysInfo->face_config.frecog == 2)
     {
          strcpy(fRecognition, "REGISTER USER");
     }
	 else if(pSysInfo->face_config.frecog == 3)
     {
          strcpy(fRecognition, "CLEAR ALL USERS");
     }

}

void GetBLC(char *blc)
{
     SysInfo* pSysInfo = GetSysInfo();

     if(pSysInfo->lan_config.nBacklightControl)
     {
           strcpy(blc, "Enable");
     }
     else
     {
          strcpy(blc, "Disable");
     }

}

void GetBackLight(char *backLight)
{
     SysInfo* pSysInfo = GetSysInfo();

     if(pSysInfo->lan_config.nBackLight == 0)
     {
          strcpy(backLight, "Min");
     }
     else if(pSysInfo->lan_config.nBackLight== 1)
     {
          strcpy(backLight, "Mid");
     }
     else
     {
          strcpy(backLight, "Max");
     }

}

void GetDynRange(char *dynRange)
{
	SysInfo* pSysInfo = GetSysInfo();

	if(pSysInfo->lan_config.dynRange == 2)
	{
		strcpy(dynRange, "Local Enhancement");
	}
	else if(pSysInfo->lan_config.dynRange == 1)
	{
		strcpy(dynRange, "Global Enhancement");
	}
	else
	{
		strcpy(dynRange, "Disable");
	}
}

void GetWhiteBalance(char *wbalance)
{
     SysInfo* pSysInfo = GetSysInfo();

     if(pSysInfo->lan_config.nWhiteBalance == 0)
     {
          strcpy(wbalance, "Auto");
     }
     else if(pSysInfo->lan_config.nWhiteBalance== 1)
     {
          strcpy(wbalance, "Indoor");
     }
     else
     {
          strcpy(wbalance, "Outdoor");
     }
}

void GetDayNightMode(char *mode)
{
     SysInfo* pSysInfo = GetSysInfo();

     if(pSysInfo->lan_config.nDayNight == 1)
     {
           strcpy(mode, "Day");
     }
     else
     {
          strcpy(mode, "Night");
     }

}

void GetHistogram(char *histogram)
{
     SysInfo* pSysInfo = GetSysInfo();

     if(pSysInfo->lan_config.histogram)
     {
          strcpy(histogram, "Enable");
     }
     else
     {
          strcpy(histogram, "Disable");
     }

}

void GetVStab(char *vStab)
{
     SysInfo* pSysInfo = GetSysInfo();

     if(pSysInfo->lan_config.AdvanceMode&FFLAG_VS)
     {
          strcpy(vStab, "Enable");
     }
     else
     {
          strcpy(vStab, "Disable");
     }

}

void GetLDC(char *ldc)
{
     SysInfo* pSysInfo = GetSysInfo();

     if(pSysInfo->lan_config.AdvanceMode&FFLAG_LDC)
     {
          strcpy(ldc, "Enable");
     }
     else
     {
          strcpy(ldc, "Disable");
     }
}

void GetImgSensorMode(char *imgsenormode)
{
     SysInfo* pSysInfo = GetSysInfo();

     if(pSysInfo->lan_config.nBinning == 0)
     {
          strcpy(imgsenormode, "Binning");
     }
     else if(pSysInfo->lan_config.nBinning == 1)
     {
          strcpy(imgsenormode, "Skipping");
     }
     else
     {
          strcpy(imgsenormode, "Window");
     }
}

void Get2AESwitch(char *AEWSwitch)
{
     SysInfo* pSysInfo = GetSysInfo();

     if(pSysInfo->lan_config.nAEWswitch== 0)
     {
           strcpy(AEWSwitch, "NONE");
     }
     else if(pSysInfo->lan_config.nAEWswitch== 1)
     {
          strcpy(AEWSwitch, "APPRO");
     }
     else
     {
           strcpy(AEWSwitch, "TI");
     }
}

void Get2AEType(char *AEWType)
{
     SysInfo* pSysInfo = GetSysInfo();

     if(pSysInfo->lan_config.nAEWtype== 0)
     {
           strcpy(AEWType, "OFF");
     }
     else if(pSysInfo->lan_config.nAEWtype== 1)
     {
          strcpy(AEWType, "Auto Exposure");
     }
     else if(pSysInfo->lan_config.nAEWtype== 2)
     {
           strcpy(AEWType, "Auto White Balance");
     }
      else
     {
           strcpy(AEWType, "Auto Exposure + Auto White Balance");
     }
}

void GetSNF(char *snf)
{
     SysInfo* pSysInfo = GetSysInfo();

     if(pSysInfo->lan_config.AdvanceMode&FFLAG_SNF)
     {
           strcpy(snf, "ON");
     }
     else
     {
           strcpy(snf, "OFF");
     }
}

void GetTnf(char *tnflt)
{
     SysInfo* pSysInfo = GetSysInfo();

     if(pSysInfo->lan_config.AdvanceMode&FFLAG_TNF)
     {
          strcpy(tnflt, "Enable");
     }
     else
     {
          strcpy(tnflt, "Disable");
     }
}

void GetAudio(char *audio)
{
     SysInfo* pSysInfo = GetSysInfo();

     if(pSysInfo->audio_config.audioenable)
     {
          strcpy(audio, "Enable");
     }
     else
     {
          strcpy(audio, "Disable");
     }

}

void GetAlarm(char *alarm)
{
     SysInfo* pSysInfo = GetSysInfo();

     if(pSysInfo->lan_config.nAlarmEnable)
     {
           strcpy(alarm, "Enable");
     }
     else
     {
          strcpy(alarm, "Disable");
     }

}

void GetMotionDetectionAlarm(char *motionAlarm)
{
     SysInfo* pSysInfo = GetSysInfo();

     if(pSysInfo->motion_config.motionenable)
     {
          strcpy(motionAlarm, "Enable");
     }
     else
     {
          strcpy(motionAlarm, "Disable");
     }

}

void GetEthernetLostAlarm(char *ethLostAlarm)
{
     SysInfo* pSysInfo = GetSysInfo();

     if(pSysInfo->lan_config.lostalarm)
     {
          strcpy(ethLostAlarm, "Enable");
     }
     else
     {
          strcpy(ethLostAlarm, "Disable");
     }

}

void GetAudioAlarm(char *audioAlarm)
{
     SysInfo* pSysInfo = GetSysInfo();

     if(pSysInfo->audio_config.alarmON)
     {
          strcpy(audioAlarm, "Enable");
     }
     else
     {
          strcpy(audioAlarm, "Disable");
     }

}

void GetExternTriggers(char *extTriggersAlarm)
{
     SysInfo* pSysInfo = GetSysInfo();

     if(pSysInfo->lan_config.nExtAlarm)
     {
          strcpy(extTriggersAlarm, "Enable");
     }
     else
     {
          strcpy(extTriggersAlarm, "Disable");
     }

}

void PutFrameRate(void *pOutMem, int MaxSize)
{
     char String[32];
     SysInfo* pSysInfo = GetSysInfo();

    if(pSysInfo->lan_config.nVideocodecmode >= 0 && pSysInfo->lan_config.nVideocodecmode <=2)
   	{
        sprintf(String, "%d fps", pSysInfo->lan_config.frameRateVal1/1000);
		sprintf(html_content,"A(\"%s\",\"%s\",\"%s\");","Frame Rate", String, "30 fps");
		if( MaxSize > strlen(pOutMem) )
			strncat(pOutMem, html_content, MaxSize-strlen(pOutMem));
	}
	else  if(pSysInfo->lan_config.nVideocodecmode >= 3 && pSysInfo->lan_config.nVideocodecmode <=7)
	{
        sprintf(String, "%d fps", pSysInfo->lan_config.frameRateVal1/1000);
		sprintf(html_content,"A(\"%s\",\"%s\",\"%s\");","Frame Rate(Stream1)", String, "30 fps");
		if( MaxSize > strlen(pOutMem) )
			strncat(pOutMem, html_content, MaxSize-strlen(pOutMem));

        sprintf(String, "%d fps", pSysInfo->lan_config.frameRateVal2/1000);
		sprintf(html_content,"A(\"%s\",\"%s\",\"%s\");","Frame Rate(Stream2)", String, "30 fps");
    	if( MaxSize > strlen(pOutMem) )
	  		strncat(pOutMem, html_content, MaxSize-strlen(pOutMem));
	}
	else
	{
        sprintf(String, "%d fps", pSysInfo->lan_config.frameRateVal1/1000);
		sprintf(html_content,"A(\"%s\",\"%s\",\"%s\");","Frame Rate(Stream1)", String, "30 fps");
		if( MaxSize > strlen(pOutMem) )
			strncat(pOutMem, html_content, MaxSize-strlen(pOutMem));

        sprintf(String, "%d fps", pSysInfo->lan_config.frameRateVal2/1000);
		sprintf(html_content,"A(\"%s\",\"%s\",\"%s\");","Frame Rate(Stream2)", String, "30 fps");
		if( MaxSize > strlen(pOutMem) )
			strncat(pOutMem, html_content, MaxSize-strlen(pOutMem));

        sprintf(String, "%d fps", pSysInfo->lan_config.frameRateVal3/1000);
		sprintf(html_content,"A(\"%s\",\"%s\",\"%s\");","Frame Rate(Stream3)", String, "30 fps");
		if( MaxSize > strlen(pOutMem) )
			strncat(pOutMem, html_content, MaxSize-strlen(pOutMem));
	}
}

void PutBitRate(void *pOutMem, int MaxSize)
{
     char String1[32],String2[32];
     SysInfo* pSysInfo = GetSysInfo();

     if(pSysInfo->lan_config.nVideocodecmode == 0 ||  pSysInfo->lan_config.nVideocodecmode == 1)
     {
          sprintf(String1, "%d kbps", pSysInfo->lan_config.nMpeg41bitrate/1000);
          sprintf(String2, "%d kbps", MPEG41BITRATE_DEFAULT/1000);
          sprintf(html_content,"A(\"%s\",\"%s\",\"%s\");","Bit Rate", String1, String2);
			   if( MaxSize > strlen(pOutMem) )
		         strncat(pOutMem, html_content, MaxSize-strlen(pOutMem));
     }
     else  if(pSysInfo->lan_config.nVideocodecmode == 2)
     {
		  if(pSysInfo->lan_config.nVideocodecres&1) {
			  sprintf(String1, "%d", pSysInfo->lan_config.njpegquality);
			  sprintf(String2, "%d", JPEGQUALITY_DEF);
			  sprintf(html_content,"A(\"%s\",\"%s\",\"%s\");","Quality Factor", String1, String2);
				   if( MaxSize > strlen(pOutMem) )
					 strncat(pOutMem, html_content, MaxSize-strlen(pOutMem));
          }
          else {
			  sprintf(String1, "%d kbps", pSysInfo->lan_config.nMpeg41bitrate/1000);
			  sprintf(String2, "%d kbps", MPEG41BITRATE_DEFAULT/1000);
			  sprintf(html_content,"A(\"%s\",\"%s\",\"%s\");","Bit Rate", String1, String2);
				   if( MaxSize > strlen(pOutMem) )
					 strncat(pOutMem, html_content, MaxSize-strlen(pOutMem));
	  	  }
     }
     else if(pSysInfo->lan_config.nVideocodecmode == 3 || pSysInfo->lan_config.nVideocodecmode == 4)
     {
          sprintf(String1, "%d kbps", pSysInfo->lan_config.nMpeg41bitrate/1000);
          sprintf(String2, "%d kbps", MPEG41BITRATE_DEFAULT/1000);
          sprintf(html_content,"A(\"%s\",\"%s\",\"%s\");","Bit Rate(Stream1)", String1, String2);
			   if( MaxSize > strlen(pOutMem) )
		         strncat(pOutMem, html_content, MaxSize-strlen(pOutMem));

	   	  sprintf(String1, "%d", pSysInfo->lan_config.njpegquality);
          sprintf(String2, "%d", JPEGQUALITY_DEF);
          sprintf(html_content,"A(\"%s\",\"%s\",\"%s\");","Quality Factor(Stream2)", String1, String2);
			   if( MaxSize > strlen(pOutMem) )
		         strncat(pOutMem, html_content, MaxSize-strlen(pOutMem));
     }
     else if(pSysInfo->lan_config.nVideocodecmode == 5 || pSysInfo->lan_config.nVideocodecmode == 6 || pSysInfo->lan_config.nVideocodecmode == 7 )
     {
          sprintf(String1, "%d kbps", pSysInfo->lan_config.nMpeg41bitrate/1000);
          sprintf(String2, "%d kbps", MPEG41BITRATE_DEFAULT/1000);
          sprintf(html_content,"A(\"%s\",\"%s\",\"%s\");","Bit Rate(Stream1)", String1, String2);
			   if( MaxSize > strlen(pOutMem) )
		         strncat(pOutMem, html_content, MaxSize-strlen(pOutMem));

	      sprintf(String1, "%d kbps", pSysInfo->lan_config.nMpeg42bitrate/1000);
          sprintf(String2, "%d kbps", MPEG42BITRATE_DEFAULT/1000);
          sprintf(html_content,"A(\"%s\",\"%s\",\"%s\");","Bit Rate(Stream2)", String1, String2);
			   if( MaxSize > strlen(pOutMem) )
		         strncat(pOutMem, html_content, MaxSize-strlen(pOutMem));
     }
     else if(pSysInfo->lan_config.nVideocodecmode == 8 || pSysInfo->lan_config.nVideocodecmode == 9 )
     {
          sprintf(String1, "%d kbps", pSysInfo->lan_config.nMpeg41bitrate/1000);
          sprintf(String2, "%d kbps", MPEG41BITRATE_DEFAULT/1000);
          sprintf(html_content,"A(\"%s\",\"%s\",\"%s\");","Bit Rate(Stream1)", String1, String2);
			   if( MaxSize > strlen(pOutMem) )
		         strncat(pOutMem, html_content, MaxSize-strlen(pOutMem));

	   	  sprintf(String1, "%d", pSysInfo->lan_config.njpegquality);
          sprintf(String2, "%d", JPEGQUALITY_DEF);
          sprintf(html_content,"A(\"%s\",\"%s\",\"%s\");","Quality Factor(Stream2)", String1, String2);
			   if( MaxSize > strlen(pOutMem) )
		         strncat(pOutMem, html_content, MaxSize-strlen(pOutMem));

	      sprintf(String1, "%d kbps", pSysInfo->lan_config.nMpeg42bitrate/1000);
          sprintf(String2, "%d kbps", MPEG42BITRATE_DEFAULT/1000);
          sprintf(html_content,"A(\"%s\",\"%s\",\"%s\");","Bit Rate(Stream3)", String1, String2);
			   if( MaxSize > strlen(pOutMem) )
		         strncat(pOutMem, html_content, MaxSize-strlen(pOutMem));
     }
}

int Curr_Param_List_To_html(void *pOutMem, int MaxSize)
{
     char http_ip_addr[100];
	 char tempbuff[100];
	 char String[50];

	GetIP_Addr(http_ip_addr);
	fprintf(stderr,"para_netip %s \n",http_ip_addr);

	strncpy( pOutMem, html_paralist_header, MaxSize );

	GetStreamType(String);
      sprintf(html_content,"A(\"%s\",\"%s\",\"%s\");","Stream Type", String, "Single");
	if( MaxSize > strlen(pOutMem) )
		strncat(pOutMem, html_content, MaxSize-strlen(pOutMem));

	GetCodecCombo(String);
	sprintf(html_content,"A(\"%s\",\"%s\",\"%s\");","Codec Combo", String, "H.264");
	if( MaxSize > strlen(pOutMem) )
		strncat(pOutMem, html_content, MaxSize-strlen(pOutMem));

	GetResolution(String);
	sprintf(html_content,"A(\"%s\",\"%s\",\"%s\");","Resolution", String, "H264:720");
	if( MaxSize > strlen(pOutMem) )
		strncat(pOutMem, html_content, MaxSize-strlen(pOutMem));

	PutFrameRate(pOutMem, MaxSize);

	PutBitRate(pOutMem, MaxSize);

	GetFaceDetect(String);
	sprintf(html_content,"A(\"%s\",\"%s\",\"%s\");","Face Detect", String, "Disable");
	if( MaxSize > strlen(pOutMem) )
		strncat(pOutMem, html_content, MaxSize-strlen(pOutMem));

	GetPrivacyMask(String);
	sprintf(html_content,"A(\"%s\",\"%s\",\"%s\");","Privacy Mask", String, "Disable");
	if( MaxSize > strlen(pOutMem) )
		strncat(pOutMem, html_content, MaxSize-strlen(pOutMem));

	GetFaceRecognition(String);
	sprintf(html_content,"A(\"%s\",\"%s\",\"%s\");","Face Recognition", String, "OFF");
	if( MaxSize > strlen(pOutMem) )
		strncat(pOutMem, html_content, MaxSize-strlen(pOutMem));

	GetBLC(String);
	sprintf(html_content,"A(\"%s\",\"%s\",\"%s\");","Back Light Compensation", String, "Enable");
	if( MaxSize > strlen(pOutMem) )
		strncat(pOutMem, html_content, MaxSize-strlen(pOutMem));

	GetBackLight(String);
	sprintf(html_content,"A(\"%s\",\"%s\",\"%s\");","Back Light", String, "Mid");
	if( MaxSize > strlen(pOutMem) )
		strncat(pOutMem, html_content, MaxSize-strlen(pOutMem));

	GetDynRange(String);
	sprintf(html_content,"A(\"%s\",\"%s\",\"%s\");","Dynamic Range Enhancement", String, "Disable");
	if( MaxSize > strlen(pOutMem) )
		strncat(pOutMem, html_content, MaxSize-strlen(pOutMem));

	GetWhiteBalance(String);
	sprintf(html_content,"A(\"%s\",\"%s\",\"%s\");","White Balance", String, "Auto");
	if( MaxSize > strlen(pOutMem) )
		strncat(pOutMem, html_content, MaxSize-strlen(pOutMem));

	GetDayNightMode(String);
	sprintf(html_content,"A(\"%s\",\"%s\",\"%s\");","Mode", String, "Day");
	if( MaxSize > strlen(pOutMem) )
		strncat(pOutMem, html_content, MaxSize-strlen(pOutMem));

	GetHistogram(String);
	sprintf(html_content,"A(\"%s\",\"%s\",\"%s\");","Histogram", String, "Disable");
	if( MaxSize > strlen(pOutMem) )
		strncat(pOutMem, html_content, MaxSize-strlen(pOutMem));

	GetVStab(String);
	sprintf(html_content,"A(\"%s\",\"%s\",\"%s\");","Video Stablization", String, "Disable");
	if( MaxSize > strlen(pOutMem) )
		strncat(pOutMem, html_content, MaxSize-strlen(pOutMem));

	GetLDC(String);
	sprintf(html_content,"A(\"%s\",\"%s\",\"%s\");","Lens Distortion Correction", String, "Disable");
	if( MaxSize > strlen(pOutMem) )
		strncat(pOutMem, html_content, MaxSize-strlen(pOutMem));

	GetImgSensorMode(String);
	sprintf(html_content,"A(\"%s\",\"%s\",\"%s\");","Image Sensor Mode", String, "Binnning");
	if( MaxSize > strlen(pOutMem) )
		strncat(pOutMem, html_content, MaxSize-strlen(pOutMem));

    Get2AESwitch(String);
	sprintf(html_content,"A(\"%s\",\"%s\",\"%s\");","2A Engine", String, "APPRO");
	if( MaxSize > strlen(pOutMem) )
		strncat(pOutMem, html_content, MaxSize-strlen(pOutMem));

	Get2AEType(String);
	sprintf(html_content,"A(\"%s\",\"%s\",\"%s\");","2A Mode", String, "Auto Exposure + Auto White Balance");
	if( MaxSize > strlen(pOutMem) )
		strncat(pOutMem, html_content, MaxSize-strlen(pOutMem));

    GetSNF(String);
	sprintf(html_content,"A(\"%s\",\"%s\",\"%s\");","Spatial Noise Filter", String, "OFF");
	if( MaxSize > strlen(pOutMem) )
		strncat(pOutMem, html_content, MaxSize-strlen(pOutMem));

	GetTnf(String);
	sprintf(html_content,"A(\"%s\",\"%s\",\"%s\");","Temporal Noise Filter", String, "Disable");
	if( MaxSize > strlen(pOutMem) )
		strncat(pOutMem, html_content, MaxSize-strlen(pOutMem));

	GetAudio(String);
	sprintf(html_content,"A(\"%s\",\"%s\",\"%s\");","Audio", String, "Disable");
	if( MaxSize > strlen(pOutMem) )
		strncat(pOutMem, html_content, MaxSize-strlen(pOutMem));

	GetAlarm(String);
	sprintf(html_content,"A(\"%s\",\"%s\",\"%s\");","Alarm", String, "Enable");
	if( MaxSize > strlen(pOutMem) )
		strncat(pOutMem, html_content, MaxSize-strlen(pOutMem));

	GetMotionDetectionAlarm(String);
	sprintf(html_content,"A(\"%s\",\"%s\",\"%s\");","Motion Detection Alarm", String, "Disable");
	if( MaxSize > strlen(pOutMem) )
		strncat(pOutMem, html_content, MaxSize-strlen(pOutMem));

	GetEthernetLostAlarm(String);
	sprintf(html_content,"A(\"%s\",\"%s\",\"%s\");","Ethernet Lost Alarm", String, "Disable");
	if( MaxSize > strlen(pOutMem) )
		strncat(pOutMem, html_content, MaxSize-strlen(pOutMem));

	GetExternTriggers(String);
	sprintf(html_content,"A(\"%s\",\"%s\",\"%s\");","External Triggers Alarm", String, "Disable");
	if( MaxSize > strlen(pOutMem) )
		strncat(pOutMem, html_content, MaxSize-strlen(pOutMem));

	GetAudioAlarm(String);
	sprintf(html_content,"A(\"%s\",\"%s\",\"%s\");","Audio Alarm", String, "Disable");
	if( MaxSize > strlen(pOutMem) )
		strncat(pOutMem, html_content, MaxSize-strlen(pOutMem));

	if( MaxSize > strlen(pOutMem) )
	    strncat(pOutMem, "</SCRIPT>\n</table>\n</DIV>\n<HR size=1 color=#C0C0C0 width=620 align=left>\n", MaxSize-strlen(pOutMem));
	else
		fprintf(stderr,"MaxSize is not enough!\n");

}
