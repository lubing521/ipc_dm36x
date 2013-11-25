#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include <dm365mm.h>
#include <osa_cmem.h>
#include <avserver.h>

//#define OSD_CMEM_ALLOC OSA_cmemAlloc
#define OSD_CMEM_ALLOC OSA_cmemAllocCached

#ifdef FR_OSD_DRAW //ANR
static FR_USER_INFO Fr_UsrInfo[SWOSD_WINDOW_FR_MAX];
#endif

typedef struct _videoSWOSDMessage
{
      int streamId;
      OSA_BufInfo *pBufInfo;

}videoSWOSDMessage;

static  OSA_MbxHndl   swOSDMbx;

extern char TILogo_Y_160_64[];
extern char TILogo_UV_160_64[];

extern char TILogo_Y_80_32[];
extern char TILogo_UV_80_32[];

#ifdef SWOSD_USER_TRANSPARANCY_ON

extern char YData_1540_40_WFONT[];
extern char UVData_1540_40_WFONT[];

extern char YData_1232_30[];
extern char UVData_1232_30[];

extern char YData_1232_30_WFONT[];
extern char UVData_1232_30_WFONT[];

extern char YData_1078_28[];
extern char UVData_1078_28[];

extern char YData_1078_28_WFONT[];
extern char UVData_1078_28_WFONT[];

extern char YData_770_20[];
extern char UVData_770_20[];

extern char YData_770_20_WFONT[];
extern char UVData_770_20_WFONT[];

extern char YData_462_12[];
extern char UVData_462_12[];

extern char YData_462_12_WFONT[];
extern char UVData_462_12_WFONT[];

#else

extern char YData_1540_40_WFONT_PURE[];
extern char UVData_1540_40_WFONT_PURE[];

extern char YData_1232_30_WFONT_PURE[];
extern char UVData_1232_30_WFONT_PURE[];

extern char YData_1078_28_WFONT_PURE[];
extern char UVData_1078_28_WFONT_PURE[];

extern char YData_770_20_WFONT_PURE[];
extern char UVData_770_20_WFONT_PURE[];

extern char YData_462_12_WFONT_PURE[];
extern char UVData_462_12_WFONT_PURE[];

#endif


SWOSD_Hndl *osdHndl = NULL;
int idxHndl[AVSERVER_MAX_STREAMS];

static FILE  *fpLogo160, *fpLogo80;
char *iconBuff160_Y = NULL, *iconBuff160_UV = NULL;
char *iconBuff80_Y = NULL, *iconBuff80_UV = NULL;

char  * fontBuff770_Y = NULL, *fontBuff770_UV = NULL;
int fontDB770_Y[256], fontDB770_UV[256];

#ifdef SWOSD_USER_TRANSPARANCY_ON
char  * fontBuff1540_Y1 = NULL, * fontBuff1540_UV1 = NULL;
char  * fontBuff1232_Y = NULL, *fontBuff1232_UV = NULL, * fontBuff1232_Y1 = NULL, *fontBuff1232_UV1 = NULL;
char  * fontBuff1078_Y = NULL, *fontBuff1078_UV = NULL, * fontBuff1078_Y1 = NULL, *fontBuff1078_UV1 = NULL;
char  * fontBuff770_Y1 = NULL, *fontBuff770_UV1 = NULL;
char  * fontBuff462_Y = NULL, *fontBuff462_UV = NULL, * fontBuff462_Y1 = NULL, *fontBuff462_UV1 = NULL;

int fontDB1540_Y1[256], fontDB1540_UV1[256];
int fontDB1232_Y[256], fontDB1232_UV[256],fontDB1232_Y1[256], fontDB1232_UV1[256];
int fontDB1078_Y[256], fontDB1078_UV[256], fontDB1078_Y1[256], fontDB1078_UV1[256];
int fontDB770_Y1[256], fontDB770_UV1[256];
int fontDB462_Y[256], fontDB462_UV[256],fontDB462_Y1[256], fontDB462_UV1[256];
#else
char  * fontBuff1540_Y1 = NULL, * fontBuff1540_UV1 = NULL;
char  * fontBuff1232_Y1 = NULL, *fontBuff1232_UV1 = NULL;
char  * fontBuff1078_Y1 = NULL, *fontBuff1078_UV1 = NULL;
char  * fontBuff770_Y1 = NULL, *fontBuff770_UV1 = NULL;
char  * fontBuff462_Y1 = NULL, *fontBuff462_UV1 = NULL;

int fontDB1540_Y1[256], fontDB1540_UV1[256];
int fontDB1232_Y1[256], fontDB1232_UV1[256];
int fontDB1078_Y1[256], fontDB1078_UV1[256];
int fontDB770_Y1[256], fontDB770_UV1[256];
int fontDB462_Y1[256], fontDB462_UV1[256];
#endif

#define MAX_STR_BUFF	(10)
char *strBuff1540_Y[AVSERVER_MAX_STREAMS], *strBuff1540_UV[AVSERVER_MAX_STREAMS];
char *strBuff1232_Y[AVSERVER_MAX_STREAMS][MAX_STR_BUFF], *strBuff1232_UV[AVSERVER_MAX_STREAMS][MAX_STR_BUFF];
char *strBuff1078_Y[AVSERVER_MAX_STREAMS][MAX_STR_BUFF], *strBuff1078_UV[AVSERVER_MAX_STREAMS][MAX_STR_BUFF];
char *strBuff770_Y[AVSERVER_MAX_STREAMS][MAX_STR_BUFF], *strBuff770_UV[AVSERVER_MAX_STREAMS][MAX_STR_BUFF];
char *strBuff462_Y[AVSERVER_MAX_STREAMS][MAX_STR_BUFF], *strBuff462_UV[AVSERVER_MAX_STREAMS][MAX_STR_BUFF];

#ifdef FR_OSD_DRAW  //ANR
char  *strBuff1232_Y_FR[SWOSD_WINDOW_FR_MAX], *strBuff1232_UV_FR[SWOSD_WINDOW_FR_MAX];
#endif

static inline void swosdSetLogoPrm(void)
{
	fpLogo80 =  fopen ( SWOSD_TIICON_80 , "rb" );

	if(fpLogo80 == NULL )
	{
#ifdef AVSERVER_DEBUG_VIDEO_SWOSD_THR
		OSA_printf("\r\n Default Icon TI Logo\n");
#endif
		memcpy(iconBuff80_Y, TILogo_Y_80_32, SWOSD_CHARWIDTHTILOGO_80_32*SWOSD_CHARHEIGHTTILOGO_80_32);
		memcpy(iconBuff80_UV, TILogo_UV_80_32, (SWOSD_CHARWIDTHTILOGO_80_32*SWOSD_CHARHEIGHTTILOGO_80_32)/2);
	}
	else
	{
		fread(iconBuff80_Y, 1, SWOSD_CHARWIDTHTILOGO_80_32*SWOSD_CHARHEIGHTTILOGO_80_32, fpLogo80);
		fread(iconBuff80_UV, 1, (SWOSD_CHARWIDTHTILOGO_80_32*SWOSD_CHARHEIGHTTILOGO_80_32)/2, fpLogo80);
		fclose(fpLogo80);
	}


	fpLogo160 =  fopen ( SWOSD_TIICON_160 , "rb" );

	if(fpLogo160 == NULL)
	{
#ifdef AVSERVER_DEBUG_VIDEO_SWOSD_THR
		OSA_printf("\r\n Default Icon TI Logo\n");
#endif
		memcpy(iconBuff160_Y, TILogo_Y_160_64, SWOSD_CHARWIDTHTILOGO_160_64*SWOSD_CHARHEIGHTTILOGO_160_64);
		memcpy(iconBuff160_UV, TILogo_UV_160_64, (SWOSD_CHARWIDTHTILOGO_160_64*SWOSD_CHARHEIGHTTILOGO_160_64)/2);
	}
	else
	{
		fread(iconBuff160_Y, 1, SWOSD_CHARWIDTHTILOGO_160_64*SWOSD_CHARHEIGHTTILOGO_160_64, fpLogo160);
		fread(iconBuff160_UV, 1, (SWOSD_CHARWIDTHTILOGO_160_64*SWOSD_CHARHEIGHTTILOGO_160_64)/2, fpLogo160);
		fclose(fpLogo160);
	}

}

static inline void swosdCreateFontDB()
{

	SWOSD_Fontdata fontInfo;

	fontInfo.format 		= SWOSD_FORMATYUV420p;
	fontInfo.charWidth		= SWOSD_CHARWIDTH_462_12;
	fontInfo.charHeight 	= SWOSD_CHARHEIGHT_462_12;
	fontInfo.totFontWidth	= SWOSD_STRINGWIDTH_462_12;
	fontInfo.totFontHeight	= SWOSD_CHARHEIGHT_462_12;
	fontInfo.totFontLineoffset	= SWOSD_STRINGWIDTH_462_12;
	fontInfo.fontAddr		= fontBuff462_Y1;
	fontInfo.fontAddrUV 	= fontBuff462_UV1;

	SWOSD_createFontDatabase(fontDB462_Y1,fontDB462_UV1,(char *)SWOSD_STRINGPATTERN,&fontInfo);

#ifdef SWOSD_USER_TRANSPARANCY_ON
	fontInfo.fontAddr		= fontBuff462_Y;
	fontInfo.fontAddrUV 	= fontBuff462_UV;

	SWOSD_createFontDatabase(fontDB462_Y,fontDB462_UV,(char *)SWOSD_STRINGPATTERN,&fontInfo);
#endif

	fontInfo.format			= SWOSD_FORMATYUV420p;
	fontInfo.charWidth		= SWOSD_CHARWIDTH_770_20;
	fontInfo.charHeight		= SWOSD_CHARHEIGHT_770_20;
	fontInfo.totFontWidth	= SWOSD_STRINGWIDTH_770_20;
	fontInfo.totFontHeight	= SWOSD_CHARHEIGHT_770_20;
	fontInfo.totFontLineoffset	= SWOSD_STRINGWIDTH_770_20;
	fontInfo.fontAddr		= fontBuff770_Y1;
	fontInfo.fontAddrUV     = fontBuff770_UV1;

	SWOSD_createFontDatabase(fontDB770_Y1,fontDB770_UV1,(char *)SWOSD_STRINGPATTERN,&fontInfo);

#ifdef SWOSD_USER_TRANSPARANCY_ON

	fontInfo.fontAddr		= fontBuff770_Y;
	fontInfo.fontAddrUV     = fontBuff770_UV;

	SWOSD_createFontDatabase(fontDB770_Y,fontDB770_UV,(char *)SWOSD_STRINGPATTERN,&fontInfo);
#endif

	fontInfo.format			= SWOSD_FORMATYUV420p;
	fontInfo.charWidth		= SWOSD_CHARWIDTH_1078_28;
	fontInfo.charHeight		= SWOSD_CHARHEIGHT_1078_28;
	fontInfo.totFontWidth	= SWOSD_STRINGWIDTH_1078_28;
	fontInfo.totFontHeight	= SWOSD_CHARHEIGHT_1078_28;
	fontInfo.totFontLineoffset	= SWOSD_STRINGWIDTH_1078_28;
	fontInfo.fontAddr		= fontBuff1078_Y1;
	fontInfo.fontAddrUV     = fontBuff1078_UV1;

	SWOSD_createFontDatabase(fontDB1078_Y1,fontDB1078_UV1,(char *)SWOSD_STRINGPATTERN,&fontInfo);

#ifdef SWOSD_USER_TRANSPARANCY_ON
	fontInfo.fontAddr		= fontBuff1078_Y;
	fontInfo.fontAddrUV     = fontBuff1078_UV;

	SWOSD_createFontDatabase(fontDB1078_Y,fontDB1078_UV,(char *)SWOSD_STRINGPATTERN,&fontInfo);
#endif

	fontInfo.format			= SWOSD_FORMATYUV420p;
	fontInfo.charWidth		= SWOSD_CHARWIDTH_1232_30;
	fontInfo.charHeight		= SWOSD_CHARHEIGHT_1232_30;
	fontInfo.totFontWidth	= SWOSD_STRINGWIDTH_1232_30;
	fontInfo.totFontHeight	= SWOSD_CHARHEIGHT_1232_30;
	fontInfo.totFontLineoffset	= SWOSD_STRINGWIDTH_1232_30;
	fontInfo.fontAddr		= fontBuff1232_Y1;
	fontInfo.fontAddrUV     = fontBuff1232_UV1;

	SWOSD_createFontDatabase(fontDB1232_Y1,fontDB1232_UV1,(char *)SWOSD_STRINGPATTERN,&fontInfo);

#ifdef SWOSD_USER_TRANSPARANCY_ON
	fontInfo.fontAddr		= fontBuff1232_Y;
	fontInfo.fontAddrUV     = fontBuff1232_UV;

	SWOSD_createFontDatabase(fontDB1232_Y,fontDB1232_UV,(char *)SWOSD_STRINGPATTERN,&fontInfo);
#endif

	fontInfo.format			= SWOSD_FORMATYUV420p;
	fontInfo.charWidth		= SWOSD_CHARWIDTH_1540_40;
	fontInfo.charHeight		= SWOSD_CHARHEIGHT_1540_40;
	fontInfo.totFontWidth	= SWOSD_STRINGWIDTH_1540_40;
	fontInfo.totFontHeight	= SWOSD_CHARHEIGHT_1540_40;
	fontInfo.totFontLineoffset	= SWOSD_STRINGWIDTH_1540_40;
	fontInfo.fontAddr		= fontBuff1540_Y1;
	fontInfo.fontAddrUV     = fontBuff1540_UV1;

	SWOSD_createFontDatabase(fontDB1540_Y1,fontDB1540_UV1,(char *)SWOSD_STRINGPATTERN,&fontInfo);
}

static inline void swosdGetCurTime(DATE_TIME_INFO *pInfo)
{
	time_t timep;
	struct tm *p;

	tzset();
	time(&timep);
	p=localtime(&timep);

	pInfo->year 	= 1900+p->tm_year;
	pInfo->month	= 1+p->tm_mon;
	pInfo->mday		= p->tm_mday;
	pInfo->hours	= p->tm_hour;
	pInfo->min		= p->tm_min;
	pInfo->sec		= p->tm_sec;

}

static int swosdGetDateTimeString(char *timeStr,  char *dateStr, int streamId)
{
        DATE_TIME_INFO  DTIMEInfo;

        swosdGetCurTime(&DTIMEInfo);

        if(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdTimeFormat == SWOSD_FMT_12HR){
			if(DTIMEInfo.hours >= 13)
				sprintf(timeStr, "%02d:%02d:%02dPM", DTIMEInfo.hours-12, DTIMEInfo.min, DTIMEInfo.sec);
			else if(DTIMEInfo.hours == 12)
				sprintf(timeStr, "%02d:%02d:%02dPM", DTIMEInfo.hours, DTIMEInfo.min, DTIMEInfo.sec);
			else if(DTIMEInfo.hours == 0)
				sprintf(timeStr, "%02d:%02d:%02dAM", DTIMEInfo.hours+12, DTIMEInfo.min, DTIMEInfo.sec);
			else
				sprintf(timeStr, "%02d:%02d:%02dAM", DTIMEInfo.hours, DTIMEInfo.min, DTIMEInfo.sec);
		}
        else if(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdTimeFormat == SWOSD_FMT_24HR){
			sprintf(timeStr, "%02d:%02d:%02d  ", DTIMEInfo.hours, DTIMEInfo.min, DTIMEInfo.sec);
		}

        switch( gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdDateFormat)
        {
			case SWOSD_FMT_YMD:
				sprintf(dateStr, "%04d/%02d/%02d", DTIMEInfo.year, DTIMEInfo.month, DTIMEInfo.mday);
				break;
			case SWOSD_FMT_MDY:
        			sprintf(dateStr, "%02d/%02d/%04d", DTIMEInfo.month, DTIMEInfo.mday, DTIMEInfo.year);
				break;
       		default:
			case SWOSD_FMT_DMY:
				sprintf(dateStr, "%02d/%02d/%04d", DTIMEInfo.mday, DTIMEInfo.month, DTIMEInfo.year);
				break;

        }

		return OSA_SOK;
}

static int swosdGetCodecType(int streamId, char codecStr[])
{

	if(gAVSERVER_config.encodeConfig[streamId].codecType == ALG_VID_CODEC_MJPEG)
		strcpy(codecStr, "MJPEG");
	else if(gAVSERVER_config.encodeConfig[streamId].codecType == ALG_VID_CODEC_H264)
		strcpy(codecStr, "H.264");
	else if(gAVSERVER_config.encodeConfig[streamId].codecType == ALG_VID_CODEC_MPEG4)
		strcpy(codecStr, "MPEG4");

	return OSA_SOK;
}

static int swosdGetRateControl(int streamId, char rateStr[])
{
	if(gAVSERVER_config.encodeConfig[streamId].rateControlType == ALG_VID_ENC_RATE_CONTROL_RCOFF)
		strcpy(rateStr, "OFF");
	else if(gAVSERVER_config.encodeConfig[streamId].rateControlType == ALG_VID_ENC_RATE_CONTROL_VBR)
		strcpy(rateStr, "VBR");
	else if(gAVSERVER_config.encodeConfig[streamId].rateControlType == ALG_VID_ENC_RATE_CONTROL_CBR)
		strcpy(rateStr, "CBR");

	return OSA_SOK;
}

static unsigned int swosdGetBitRate(int streamId)
{
	return gAVSERVER_config.encodeConfig[streamId].codecBitrate;
}

static int swosdGetAutoFocus(int streamId, char autoFocusStr[])
{
	sprintf(autoFocusStr, "%7d", gAVSERVER_config.aewb_config.autoFocusVal);

	return OSA_SOK;
}

static int OSD_checkEvalVersion(int streamId)
{
	int osdEvalFlag=0;

	if(gAVSERVER_config.captureConfig[streamId].vsEnable)
		osdEvalFlag = 1;
	else if(gAVSERVER_config.captureConfig[streamId].snfEnable || gAVSERVER_config.captureConfig[streamId].tnfEnable)
		osdEvalFlag = 1;
	else if(gAVSERVER_config.faceDetectConfig.frRegUsr || gAVSERVER_config.faceDetectConfig.frIdentify)
		osdEvalFlag = 1;
	else if(gAVSERVER_config.aewb_config.saldreEnable)
		osdEvalFlag = 1;
	else
		osdEvalFlag = 0;

	return osdEvalFlag;
}

static int swosdSetWinParam(int height, int width)
{
	SWOSD_BmpWinPrm bmpWinPrm;
	int i = 0;

#ifdef SWOSD_USER_TRANSPARANCY_ON
	bmpWinPrm.userTransparancy = SWOSD_RBOOLTRUE;
#else
	bmpWinPrm.userTransparancy = SWOSD_RBOOLFALSE;
#endif

	if(width <= VIDEO_RESOL_WIDTH_SMALL) {   //For QVGA Types of Resolution
		//for window 1
		bmpWinPrm.enable				= SWOSD_RBOOLFALSE;
		bmpWinPrm.format 				= SWOSD_FORMATYUV420p;
		bmpWinPrm.startX				= 0;
		bmpWinPrm.startY				= 0;
		bmpWinPrm.width					= SWOSD_CHARWIDTHTILOGO_80_32;
		bmpWinPrm.height				= SWOSD_CHARHEIGHTTILOGO_80_32;
		bmpWinPrm.lineOffset			= SWOSD_CHARWIDTHTILOGO_80_32;
		bmpWinPrm.transperencyVal		= SWOSD_OSDBLACKTRANSVAL;
		bmpWinPrm.transperencyRange		= SWOSD_OSDBLACKTRANSRANGE;
		bmpWinPrm.transperencyEnable   	= SWOSD_BMPTRANSDISABLE;
		bmpWinPrm.resizeEnable			= 0;
		bmpWinPrm.bmpWinAddr			= iconBuff80_Y;
		bmpWinPrm.bmpWinAddrUV         	= iconBuff80_UV;
		SWOSD_setBmpWinPrm(osdHndl, SWOSD_WINDOW_ONE, &bmpWinPrm);

		//for window 2
		bmpWinPrm.enable				= SWOSD_RBOOLTRUE;
		bmpWinPrm.startX				= OSA_align(width - SWOSD_MAX_STRING_LENGTH * SWOSD_CHARWIDTH_462_12, 4) ;
		bmpWinPrm.startY				= SWOSD_CHARWIDTH_462_12/3;
		bmpWinPrm.width					= SWOSD_STRINGWIDTH_462_12;
		bmpWinPrm.height				= SWOSD_CHARHEIGHT_462_12;
		bmpWinPrm.lineOffset			= SWOSD_STRINGWIDTH_462_12;
		bmpWinPrm.transperencyVal		= SWOSD_OSDBLACKTRANSVAL;
		bmpWinPrm.transperencyRange		= SWOSD_OSDBLACKTRANSRANGE;
		bmpWinPrm.transperencyEnable   	= SWOSD_BMPTRANSDISABLE;
		bmpWinPrm.resizeEnable			= 0;
		bmpWinPrm.bmpWinAddr			= SWOSD_NULL;
		bmpWinPrm.bmpWinAddrUV         	= SWOSD_NULL;
		SWOSD_setBmpWinPrm(osdHndl, SWOSD_WINDOW_TWO, &bmpWinPrm);

		//for window 3
		bmpWinPrm.enable				= SWOSD_RBOOLFALSE;
		bmpWinPrm.startX				=  OSA_align(SWOSD_CHARWIDTH_462_12, 4);
		bmpWinPrm.startY				= height/2 - 2 * SWOSD_CHARHEIGHT_462_12;
		SWOSD_setBmpWinPrm(osdHndl, SWOSD_WINDOW_THREE, &bmpWinPrm);

		//for window 4
		bmpWinPrm.startY				= height/2 - 1 * SWOSD_CHARHEIGHT_462_12;
		SWOSD_setBmpWinPrm(osdHndl, SWOSD_WINDOW_FOUR, &bmpWinPrm);

		//for window 5
		bmpWinPrm.startY				= height/2 - 0 * SWOSD_CHARHEIGHT_462_12;
		SWOSD_setBmpWinPrm(osdHndl, SWOSD_WINDOW_FIVE, &bmpWinPrm);

		//for window 6
		bmpWinPrm.startY				= height/2 + 1 * SWOSD_CHARHEIGHT_462_12;
		SWOSD_setBmpWinPrm(osdHndl, SWOSD_WINDOW_SIX, &bmpWinPrm);

		//for window 7
		bmpWinPrm.startY				= height/2 + 2 * SWOSD_CHARHEIGHT_462_12;
		SWOSD_setBmpWinPrm(osdHndl, SWOSD_WINDOW_SEVEN, &bmpWinPrm);

		//for window 10
		bmpWinPrm.startY				= height/2 + 3 * SWOSD_CHARHEIGHT_462_12;
		SWOSD_setBmpWinPrm(osdHndl, SWOSD_WINDOW_TEN, &bmpWinPrm);

		//for window 11
		bmpWinPrm.startX				= OSA_align(((width - SWOSD_MAX_STRING_LENGTH)/2)*SWOSD_CHARWIDTH_462_12, 4);
		bmpWinPrm.startY				= 0;
		SWOSD_setBmpWinPrm(osdHndl, SWOSD_WINDOW_ELEVEN, &bmpWinPrm);

		//for window 8
		bmpWinPrm.startX				= OSA_align(width - SWOSD_MAX_STRING_LENGTH  * SWOSD_CHARWIDTH_462_12, 4);
		bmpWinPrm.startY				= height - 3 * SWOSD_CHARHEIGHT_462_12;
		bmpWinPrm.transperencyEnable   	= SWOSD_BMPTRANSDISABLE;
		SWOSD_setBmpWinPrm(osdHndl, SWOSD_WINDOW_EIGHT, &bmpWinPrm);

		//for window 9
		bmpWinPrm.startY				= height - 2 * SWOSD_CHARHEIGHT_462_12;
		bmpWinPrm.transperencyEnable   	= SWOSD_BMPTRANSDISABLE;
		SWOSD_setBmpWinPrm(osdHndl, SWOSD_WINDOW_NINE, &bmpWinPrm);

#ifdef FR_OSD_DRAW //ANR - FR
		for (i = 0; i < SWOSD_WINDOW_FR_MAX; i++){	//ANR - FR
			bmpWinPrm.startX				= 0; //width - SWOSD_MAX_STRING_LENGTH * SWOSD_CHARWIDTH_1232_30;
			bmpWinPrm.startY				= 0; //height - 3 * SWOSD_CHARHEIGHT_1232_30;
			bmpWinPrm.transperencyEnable   = SWOSD_BMPWINENABLE;
			SWOSD_setBmpWinPrm(osdHndl, SWOSD_WINDOW_FR_ST+i, &bmpWinPrm);
		}
#endif
	}
	else if (width<=VIDEO_RESOL_WIDTH_MEDIUM ) {                    //For VGA Types of Resolution
		//for window 1
		bmpWinPrm.enable				= SWOSD_RBOOLFALSE;
		bmpWinPrm.format 				= SWOSD_FORMATYUV420p;
		bmpWinPrm.startX				= 0;
		bmpWinPrm.startY				= 0;
		bmpWinPrm.width					= SWOSD_CHARWIDTHTILOGO_160_64;
		bmpWinPrm.height				= SWOSD_CHARHEIGHTTILOGO_160_64;
		bmpWinPrm.lineOffset			= SWOSD_CHARWIDTHTILOGO_160_64;
		bmpWinPrm.transperencyVal		= SWOSD_OSDWHITETRANSVAL;
		bmpWinPrm.transperencyRange		= SWOSD_OSDWHITETRANSRANGE;
		bmpWinPrm.transperencyEnable   	= SWOSD_BMPTRANSDISABLE;
		bmpWinPrm.resizeEnable			= 0;
		bmpWinPrm.bmpWinAddr			= iconBuff160_Y;
		bmpWinPrm.bmpWinAddrUV         	= iconBuff160_UV;
		SWOSD_setBmpWinPrm(osdHndl, SWOSD_WINDOW_ONE, &bmpWinPrm);


		//for window 2
		bmpWinPrm.enable				= SWOSD_RBOOLTRUE;
		bmpWinPrm.startX				= OSA_align(width - SWOSD_MAX_STRING_LENGTH * SWOSD_CHARWIDTH_1078_28, 4);
		bmpWinPrm.startY				= SWOSD_CHARWIDTH_1078_28/3;
		bmpWinPrm.width					= SWOSD_STRINGWIDTH_1078_28;
		bmpWinPrm.height				= SWOSD_CHARHEIGHT_1078_28;
		bmpWinPrm.lineOffset			= SWOSD_STRINGWIDTH_1078_28;
		bmpWinPrm.transperencyVal		= SWOSD_OSDBLACKTRANSVAL;
		bmpWinPrm.transperencyRange		= SWOSD_OSDBLACKTRANSRANGE;
		bmpWinPrm.transperencyEnable   	= SWOSD_BMPTRANSDISABLE;
		bmpWinPrm.resizeEnable			= 0;
		bmpWinPrm.bmpWinAddr			= SWOSD_NULL;
		bmpWinPrm.bmpWinAddrUV         	= SWOSD_NULL;
		SWOSD_setBmpWinPrm(osdHndl, SWOSD_WINDOW_TWO, &bmpWinPrm);

		//for window 3
		bmpWinPrm.enable				= SWOSD_RBOOLFALSE;
		bmpWinPrm.startX				=  OSA_align(SWOSD_CHARWIDTH_770_20, 4);
		bmpWinPrm.startY				= height/2 - 2 * SWOSD_CHARHEIGHT_770_20;
		bmpWinPrm.width					= SWOSD_STRINGWIDTH_770_20;
		bmpWinPrm.height				= SWOSD_CHARHEIGHT_770_20;
		bmpWinPrm.lineOffset			= SWOSD_STRINGWIDTH_770_20;
		bmpWinPrm.transperencyVal		= SWOSD_OSDBLACKTRANSVAL;
		bmpWinPrm.transperencyRange		= SWOSD_OSDBLACKTRANSRANGE;
		bmpWinPrm.transperencyEnable   	= SWOSD_BMPTRANSDISABLE;
		bmpWinPrm.resizeEnable			= 0;
		bmpWinPrm.bmpWinAddr			= SWOSD_NULL;
		bmpWinPrm.bmpWinAddrUV         	= SWOSD_NULL;
		SWOSD_setBmpWinPrm(osdHndl, SWOSD_WINDOW_THREE, &bmpWinPrm);

		//for window 4
		bmpWinPrm.startY				= height/2 - 1 * SWOSD_CHARHEIGHT_770_20;
		SWOSD_setBmpWinPrm(osdHndl, SWOSD_WINDOW_FOUR, &bmpWinPrm);

		//for window 5
		bmpWinPrm.startY				= height/2 - 0 * SWOSD_CHARHEIGHT_770_20;
		SWOSD_setBmpWinPrm(osdHndl, SWOSD_WINDOW_FIVE, &bmpWinPrm);

		//for window 6
		bmpWinPrm.startY				= height/2 + 1 * SWOSD_CHARHEIGHT_770_20;
		SWOSD_setBmpWinPrm(osdHndl, SWOSD_WINDOW_SIX, &bmpWinPrm);

		//for window 7
		bmpWinPrm.startY				= height/2 + 2 * SWOSD_CHARHEIGHT_770_20;
		SWOSD_setBmpWinPrm(osdHndl, SWOSD_WINDOW_SEVEN, &bmpWinPrm);

		//for window 10
		bmpWinPrm.startY				= height/2 + 3 * SWOSD_CHARHEIGHT_770_20;
		SWOSD_setBmpWinPrm(osdHndl, SWOSD_WINDOW_TEN, &bmpWinPrm);

		//for window 11
		bmpWinPrm.startX				= OSA_align((width - (SWOSD_MAX_STRING_LENGTH*SWOSD_CHARHEIGHT_770_20))/2, 4);
		bmpWinPrm.startY				= 0;
		SWOSD_setBmpWinPrm(osdHndl, SWOSD_WINDOW_ELEVEN, &bmpWinPrm);

		//for window 8
		bmpWinPrm.startX				= OSA_align(width - SWOSD_MAX_STRING_LENGTH * SWOSD_CHARWIDTH_770_20, 4);
		bmpWinPrm.startY				= height - 3 * SWOSD_CHARHEIGHT_770_20;
		bmpWinPrm.transperencyEnable   	= SWOSD_BMPTRANSDISABLE;
		SWOSD_setBmpWinPrm(osdHndl, SWOSD_WINDOW_EIGHT, &bmpWinPrm);


		//for window 9
		bmpWinPrm.startY				= height - 2 * SWOSD_CHARHEIGHT_770_20;
		bmpWinPrm.transperencyEnable   	= SWOSD_BMPTRANSDISABLE;
		SWOSD_setBmpWinPrm(osdHndl, SWOSD_WINDOW_NINE, &bmpWinPrm);

#ifdef FR_OSD_DRAW //ANR - FR
		for (i = 0; i < SWOSD_WINDOW_FR_MAX; i++){	//ANR - FR
			bmpWinPrm.startX				= 0; //width - SWOSD_MAX_STRING_LENGTH * SWOSD_CHARWIDTH_1232_30;
			bmpWinPrm.startY				= 0; //height - 3 * SWOSD_CHARHEIGHT_1232_30;
			bmpWinPrm.transperencyEnable   = SWOSD_BMPWINENABLE;
			SWOSD_setBmpWinPrm(osdHndl, SWOSD_WINDOW_FR_ST+i, &bmpWinPrm);
		}
#endif

	}
	else if(width <VIDEO_RESOL_WIDTH_LARGE) {     //For SVGA Types of Resolution
		//for window 1
		bmpWinPrm.enable				= SWOSD_RBOOLFALSE;
		bmpWinPrm.format 				= SWOSD_FORMATYUV420p;
		bmpWinPrm.startX				= 0;
		bmpWinPrm.startY				= 0;
		bmpWinPrm.width					= SWOSD_CHARWIDTHTILOGO_160_64;
		bmpWinPrm.height				= SWOSD_CHARHEIGHTTILOGO_160_64;
		bmpWinPrm.lineOffset			= SWOSD_CHARWIDTHTILOGO_160_64;
		bmpWinPrm.transperencyVal		= SWOSD_OSDWHITETRANSVAL;
		bmpWinPrm.transperencyRange		= SWOSD_OSDWHITETRANSRANGE;
		bmpWinPrm.transperencyEnable   	= SWOSD_BMPTRANSDISABLE;
		bmpWinPrm.resizeEnable			= 0;
		bmpWinPrm.bmpWinAddr			= iconBuff160_Y;
		bmpWinPrm.bmpWinAddrUV         	= iconBuff160_UV;
		SWOSD_setBmpWinPrm(osdHndl, SWOSD_WINDOW_ONE, &bmpWinPrm);

		//for window 2
		bmpWinPrm.enable				= SWOSD_RBOOLTRUE;
		bmpWinPrm.startX				= OSA_align(width - SWOSD_MAX_STRING_LENGTH * SWOSD_CHARWIDTH_1232_30, 4) ;
		bmpWinPrm.startY				= SWOSD_CHARWIDTH_1232_30/3;
		bmpWinPrm.width					= SWOSD_STRINGWIDTH_1232_30;
		bmpWinPrm.height				= SWOSD_CHARHEIGHT_1232_30;
		bmpWinPrm.lineOffset			= SWOSD_STRINGWIDTH_1232_30;
		bmpWinPrm.transperencyVal		= SWOSD_OSDBLACKTRANSVAL;
		bmpWinPrm.transperencyRange		= SWOSD_OSDWHITETRANSRANGE;
		bmpWinPrm.transperencyEnable   	= SWOSD_BMPTRANSDISABLE;
		bmpWinPrm.resizeEnable			= 0;
		bmpWinPrm.bmpWinAddr			= SWOSD_NULL;
		bmpWinPrm.bmpWinAddrUV         	= SWOSD_NULL;
		SWOSD_setBmpWinPrm(osdHndl, SWOSD_WINDOW_TWO, &bmpWinPrm);

		//for window 3
		bmpWinPrm.enable				= SWOSD_RBOOLFALSE;
		bmpWinPrm.startX				= OSA_align( SWOSD_CHARWIDTH_1078_28, 4);
		bmpWinPrm.startY				= height/2 - 2 * SWOSD_CHARHEIGHT_1078_28;
		bmpWinPrm.width					= SWOSD_STRINGWIDTH_1078_28;
		bmpWinPrm.height				= SWOSD_CHARHEIGHT_1078_28;
		bmpWinPrm.lineOffset			= SWOSD_STRINGWIDTH_1078_28;
		bmpWinPrm.transperencyVal		= SWOSD_OSDBLACKTRANSVAL;
		bmpWinPrm.transperencyRange		= SWOSD_OSDBLACKTRANSRANGE;
		bmpWinPrm.transperencyEnable   	= SWOSD_BMPTRANSDISABLE;
		bmpWinPrm.resizeEnable			= 0;
		bmpWinPrm.bmpWinAddr			= SWOSD_NULL;
		bmpWinPrm.bmpWinAddrUV         	= SWOSD_NULL;
		SWOSD_setBmpWinPrm(osdHndl, SWOSD_WINDOW_THREE, &bmpWinPrm);

		//for window 4
		bmpWinPrm.startY				= height/2 - 1 * SWOSD_CHARHEIGHT_1078_28;
		SWOSD_setBmpWinPrm(osdHndl, SWOSD_WINDOW_FOUR, &bmpWinPrm);

		//for window 5
		bmpWinPrm.startY				= height/2 - 0 * SWOSD_CHARHEIGHT_1078_28;
		SWOSD_setBmpWinPrm(osdHndl, SWOSD_WINDOW_FIVE, &bmpWinPrm);

		//for window 6
		bmpWinPrm.startY				= height/2 + 1 * SWOSD_CHARHEIGHT_1078_28;
		SWOSD_setBmpWinPrm(osdHndl, SWOSD_WINDOW_SIX, &bmpWinPrm);

		//for window 7
		bmpWinPrm.startY				= height/2 + 2 * SWOSD_CHARHEIGHT_1078_28;
		SWOSD_setBmpWinPrm(osdHndl, SWOSD_WINDOW_SEVEN, &bmpWinPrm);

		//for window 10
		bmpWinPrm.startY				= height/2 + 3 * SWOSD_CHARHEIGHT_1078_28;
		SWOSD_setBmpWinPrm(osdHndl, SWOSD_WINDOW_TEN, &bmpWinPrm);

		//for window 11
		bmpWinPrm.startX				= OSA_align(((width - SWOSD_MAX_STRING_LENGTH)/2)*SWOSD_CHARHEIGHT_1078_28, 4);
		bmpWinPrm.startY				= 0;
		SWOSD_setBmpWinPrm(osdHndl, SWOSD_WINDOW_ELEVEN, &bmpWinPrm);

		//for window 8
		bmpWinPrm.startX				= OSA_align(width - SWOSD_MAX_STRING_LENGTH  * SWOSD_CHARWIDTH_1078_28, 4);
		bmpWinPrm.startY				= height - 3 * SWOSD_CHARHEIGHT_1078_28;
		bmpWinPrm.transperencyEnable   	= SWOSD_BMPTRANSDISABLE;
		SWOSD_setBmpWinPrm(osdHndl, SWOSD_WINDOW_EIGHT, &bmpWinPrm);

		//for window 9
		bmpWinPrm.startY				= height - 2 * SWOSD_CHARHEIGHT_1078_28;
		bmpWinPrm.transperencyEnable   	= SWOSD_BMPTRANSDISABLE;
		SWOSD_setBmpWinPrm(osdHndl, SWOSD_WINDOW_NINE, &bmpWinPrm);

#ifdef FR_OSD_DRAW //ANR - FR
		for (i = 0; i < SWOSD_WINDOW_FR_MAX; i++){	//ANR - FR
			bmpWinPrm.startX				= 0; //width - SWOSD_MAX_STRING_LENGTH * SWOSD_CHARWIDTH_1232_30;
			bmpWinPrm.startY				= 0; //height - 3 * SWOSD_CHARHEIGHT_1232_30;
			bmpWinPrm.transperencyEnable   = SWOSD_BMPWINENABLE;
			SWOSD_setBmpWinPrm(osdHndl, SWOSD_WINDOW_FR_ST+i, &bmpWinPrm);
		}
#endif
	}
	else if(width >=VIDEO_RESOL_WIDTH_LARGE) {
		//for window 1
		bmpWinPrm.enable				= SWOSD_RBOOLFALSE;
		bmpWinPrm.format 				= SWOSD_FORMATYUV420p;
		bmpWinPrm.startX				= 0;
		bmpWinPrm.startY				= 0;
		bmpWinPrm.width					= SWOSD_CHARWIDTHTILOGO_160_64;
		bmpWinPrm.height				= SWOSD_CHARHEIGHTTILOGO_160_64;
		bmpWinPrm.lineOffset			= SWOSD_CHARWIDTHTILOGO_160_64;
		bmpWinPrm.transperencyVal		= SWOSD_OSDWHITETRANSVAL;
		bmpWinPrm.transperencyRange		= SWOSD_OSDWHITETRANSRANGE;
		bmpWinPrm.transperencyEnable   	= SWOSD_BMPTRANSDISABLE;
		bmpWinPrm.resizeEnable			= 0;
		bmpWinPrm.bmpWinAddr			= iconBuff160_Y;
		bmpWinPrm.bmpWinAddrUV         	= iconBuff160_UV;

		SWOSD_setBmpWinPrm(osdHndl, SWOSD_WINDOW_ONE, &bmpWinPrm);

		//for window 2
		bmpWinPrm.enable				= SWOSD_RBOOLTRUE;
		bmpWinPrm.startX				= OSA_align(width - SWOSD_MAX_STRING_LENGTH * SWOSD_CHARWIDTH_1540_40, 4);
		bmpWinPrm.startY				= SWOSD_CHARWIDTH_1540_40/3;
		bmpWinPrm.width					= SWOSD_STRINGWIDTH_1540_40;
		bmpWinPrm.height				= SWOSD_CHARHEIGHT_1540_40;
		bmpWinPrm.lineOffset			= SWOSD_STRINGWIDTH_1540_40;
		bmpWinPrm.transperencyVal		= SWOSD_OSDBLACKTRANSVAL;
		bmpWinPrm.transperencyRange		= SWOSD_OSDBLACKTRANSRANGE;
		bmpWinPrm.transperencyEnable   	= SWOSD_BMPTRANSDISABLE;
		bmpWinPrm.resizeEnable			= 0;
		bmpWinPrm.bmpWinAddr			= SWOSD_NULL;
		bmpWinPrm.bmpWinAddrUV         	= SWOSD_NULL;
		SWOSD_setBmpWinPrm(osdHndl, SWOSD_WINDOW_TWO, &bmpWinPrm);

		//for window 3
		bmpWinPrm.enable				= SWOSD_RBOOLFALSE;
		bmpWinPrm.startX				= OSA_align(SWOSD_CHARWIDTH_1232_30, 4);
		bmpWinPrm.startY				= height/2 - 2 * SWOSD_CHARHEIGHT_1232_30;
		bmpWinPrm.width					= SWOSD_STRINGWIDTH_1232_30;
		bmpWinPrm.height				= SWOSD_CHARHEIGHT_1232_30;
		bmpWinPrm.lineOffset			= SWOSD_STRINGWIDTH_1232_30;
		bmpWinPrm.transperencyVal		= SWOSD_OSDBLACKTRANSVAL;
		bmpWinPrm.transperencyRange		= SWOSD_OSDBLACKTRANSRANGE;
		bmpWinPrm.transperencyEnable   	= SWOSD_BMPTRANSDISABLE;
		bmpWinPrm.resizeEnable			= 0;
		bmpWinPrm.bmpWinAddr			= SWOSD_NULL;
		bmpWinPrm.bmpWinAddrUV        	= SWOSD_NULL;
		SWOSD_setBmpWinPrm(osdHndl, SWOSD_WINDOW_THREE, &bmpWinPrm);

		//for window 4
		bmpWinPrm.startY				= height/2 - 1 * SWOSD_CHARHEIGHT_1232_30;
		SWOSD_setBmpWinPrm(osdHndl, SWOSD_WINDOW_FOUR, &bmpWinPrm);

		//for window 5
		bmpWinPrm.startY				= height/2 - 0 * SWOSD_CHARHEIGHT_1232_30;
		SWOSD_setBmpWinPrm(osdHndl, SWOSD_WINDOW_FIVE, &bmpWinPrm);

		//for window 6
		bmpWinPrm.startY				= height/2 + 1 * SWOSD_CHARHEIGHT_1232_30;
		SWOSD_setBmpWinPrm(osdHndl, SWOSD_WINDOW_SIX, &bmpWinPrm);

		//for window 7
		bmpWinPrm.startY				= height/2 + 2 * SWOSD_CHARHEIGHT_1232_30;
		SWOSD_setBmpWinPrm(osdHndl, SWOSD_WINDOW_SEVEN, &bmpWinPrm);

		//for window 10
		bmpWinPrm.startY				= height/2 + 3 * SWOSD_CHARHEIGHT_1232_30;
		SWOSD_setBmpWinPrm(osdHndl, SWOSD_WINDOW_TEN, &bmpWinPrm);

		//for window 11
		bmpWinPrm.startX				= OSA_align(((width - SWOSD_MAX_STRING_LENGTH)/2)*SWOSD_CHARHEIGHT_1232_30, 4);
		bmpWinPrm.startY				= 0;
		SWOSD_setBmpWinPrm(osdHndl, SWOSD_WINDOW_ELEVEN, &bmpWinPrm);

		//for window 8
		bmpWinPrm.startX				= OSA_align(width - SWOSD_MAX_STRING_LENGTH * SWOSD_CHARWIDTH_1232_30, 4);
		bmpWinPrm.startY				= height - 3 * SWOSD_CHARHEIGHT_1232_30;
		bmpWinPrm.transperencyEnable   	= SWOSD_BMPTRANSDISABLE;
		SWOSD_setBmpWinPrm(osdHndl, SWOSD_WINDOW_EIGHT, &bmpWinPrm);

		//for window 9
		bmpWinPrm.startY				= height - 2 * SWOSD_CHARHEIGHT_1232_30;
		bmpWinPrm.transperencyEnable   	= SWOSD_BMPTRANSDISABLE;
		SWOSD_setBmpWinPrm(osdHndl, SWOSD_WINDOW_NINE, &bmpWinPrm);

#ifdef FR_OSD_DRAW //ANR - FR
		for (i = 0; i < SWOSD_WINDOW_FR_MAX; i++){	//ANR - FR
			bmpWinPrm.startX				= 0; //width - SWOSD_MAX_STRING_LENGTH * SWOSD_CHARWIDTH_1232_30;
			bmpWinPrm.startY				= 0; //height - 3 * SWOSD_CHARHEIGHT_1232_30;
			bmpWinPrm.transperencyEnable   = SWOSD_BMPWINENABLE;
			SWOSD_setBmpWinPrm(osdHndl, SWOSD_WINDOW_FR_ST+i, &bmpWinPrm);
		}
#endif

	}

	return OSA_SOK;
}

static int swosdChangeXYParmDetail(int streamId,SWOSD_Hndl *osdHndl )
{
	int width = gAVSERVER_config.encodeConfig[streamId].cropWidth/2;
	int height = gAVSERVER_config.encodeConfig[streamId].cropHeight/2;
	int osdwidth = 0, osdheight = 0;

	if(gAVSERVER_config.encodeConfig[streamId].cropWidth<=VIDEO_RESOL_WIDTH_SMALL)
	{
		osdwidth  = SWOSD_CHARWIDTH_462_12;
		osdheight = SWOSD_CHARHEIGHT_462_12;
	}
	else if(gAVSERVER_config.encodeConfig[streamId].cropWidth<=VIDEO_RESOL_WIDTH_MEDIUM )
	{
		osdwidth  = SWOSD_CHARWIDTH_770_20;
		osdheight = SWOSD_CHARHEIGHT_770_20;
	}
	else if(gAVSERVER_config.encodeConfig[streamId].cropWidth<VIDEO_RESOL_WIDTH_LARGE)
	{
		osdwidth  = SWOSD_CHARWIDTH_1078_28;
		osdheight = SWOSD_CHARHEIGHT_1078_28;
	}
	else if(gAVSERVER_config.encodeConfig[streamId].cropWidth>=VIDEO_RESOL_WIDTH_LARGE)
	{
		osdwidth  = SWOSD_CHARWIDTH_1232_30;
		osdheight = SWOSD_CHARHEIGHT_1232_30;
	}

	SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_THREE, 0, height - 2*osdheight);
	SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_FOUR, 0,  height - 1*osdheight);
	SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_FIVE, 0, height + 0*osdheight);
	SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_SIX, 0, height + 1*osdheight);
	SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_SEVEN, 0, height + 2*osdheight);
	SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_TEN, 0, height + 3*osdheight);

	SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_ELEVEN, (width-(SWOSD_EVAL_STR_LEN*osdwidth)/2), 0);

	return OSA_SOK;
}

static char autoFocus_str[AVSERVER_MAX_STREAMS][SWOSD_MAX_STRING_LENGTH] = {"","",""};
static char codecBitRate_str[AVSERVER_MAX_STREAMS][SWOSD_MAX_STRING_LENGTH] = {"","",""};
static char frameRate_str[AVSERVER_MAX_STREAMS][SWOSD_MAX_STRING_LENGTH]= {"","",""};
static char date_str[AVSERVER_MAX_STREAMS][SWOSD_MAX_STRING_LENGTH]= {"","",""};
static char time_str[AVSERVER_MAX_STREAMS][SWOSD_MAX_STRING_LENGTH]= {"","",""};
#ifdef FR_OSD_DRAW
static char frUsr_str[AVSERVER_MAX_STREAMS][SWOSD_MAX_STRING_LENGTH]= {"","",""};
#endif

static int  swosdMakeWinString(int streamId)
{
	SWOSD_Fontdata fontInfo;
	char timeStr[SWOSD_MAX_STRING_LENGTH], dateStr[SWOSD_MAX_STRING_LENGTH];
	char frameRateStr[SWOSD_MAX_STRING_LENGTH], bitRateStr[SWOSD_MAX_STRING_LENGTH], autoFocusStr[SWOSD_MAX_STRING_LENGTH];
	int codecBitRate, bitratePrefix, bitrateLen;
	float frameRate;
#ifdef FR_OSD_DRAW
	int fr = 0;
#endif

	swosdGetDateTimeString(timeStr, dateStr, streamId);

	AVSERVER_getCaptureFrameRate(streamId, &frameRate);
	sprintf(frameRateStr, "%2.2f",  frameRate);

	codecBitRate = swosdGetBitRate(streamId);

	if(gAVSERVER_config.encodeConfig[streamId].codecType == ALG_VID_CODEC_MJPEG) {
		sprintf(bitRateStr, "%3d", codecBitRate);
		bitratePrefix = SWOSD_QUALITY_PREFIX;
		bitrateLen = bitratePrefix+strlen(bitRateStr)+SWOSD_QUALITY_SUFFIX;
	}
	else {
		sprintf(bitRateStr, "%5d", (codecBitRate/1000));
		bitratePrefix = SWOSD_BITRATE_PREFIX;
		bitrateLen = bitratePrefix+strlen(bitRateStr)+SWOSD_BITRATE_SUFFIX;
	}

	swosdGetAutoFocus(streamId, autoFocusStr);

	if(gAVSERVER_config.encodeConfig[streamId].cropWidth<=VIDEO_RESOL_WIDTH_SMALL)
	{
		if(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdLogoStringUpdate == OSD_YES)
		{
			fontInfo.format		= SWOSD_FORMATYUV420p;
			fontInfo.charWidth		= SWOSD_CHARWIDTH_462_12;
			fontInfo.charHeight	= SWOSD_CHARHEIGHT_462_12;
			fontInfo.totFontWidth	= SWOSD_STRINGWIDTH_462_12;
			fontInfo.totFontHeight	= SWOSD_CHARHEIGHT_462_12;
			fontInfo.totFontLineoffset	= SWOSD_STRINGWIDTH_462_12;
			fontInfo.fontAddr		= fontBuff462_Y1;
			fontInfo.fontAddrUV     = fontBuff462_UV1;
			fontInfo.fontDB_Y         =  fontDB462_Y1;
			fontInfo.fontDB_UV       = fontDB462_UV1;

			if(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdTextPos == SWOSD_FMT_TOP_LEFT){
				if(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdLogoPos == SWOSD_FMT_TOP_LEFT){
					SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_ONE, 0, 0);
					SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_TWO,  0, SWOSD_CHARHEIGHTTILOGO_80_32);
				}
				else {
                    SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_ONE,   gAVSERVER_config.encodeConfig[streamId].cropWidth - SWOSD_CHARWIDTHTILOGO_80_32, 0);
					SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_TWO,  0, 0);
				}
			}
			else {
				if(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdLogoPos == SWOSD_FMT_TOP_LEFT) {
					SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_ONE, 0, 0);
					SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_TWO,  gAVSERVER_config.encodeConfig[streamId].cropWidth - OSA_align((strlen(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdDispString)* SWOSD_CHARWIDTH_462_12),4), 1);
				}
				else {
                    SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_ONE, gAVSERVER_config.encodeConfig[streamId].cropWidth - SWOSD_CHARWIDTHTILOGO_80_32, 0);
					SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_TWO, gAVSERVER_config.encodeConfig[streamId].cropWidth - OSA_align((strlen(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdDispString)* SWOSD_CHARWIDTH_462_12),4), SWOSD_CHARHEIGHTTILOGO_80_32);
				}
			}

			SWOSD_MakeOsdwinstring(&fontInfo,  gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdDispString , strBuff462_Y[streamId][7], &osdHndl->bmpWinPrm[SWOSD_WINDOW_TWO], strlen(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdDispString));
			strBuff462_UV[streamId][7]  = (char*)(osdHndl->bmpWinPrm[SWOSD_WINDOW_TWO].bmpWinAddrUV );
			SWOSD_setBmpWinAddr(osdHndl, strBuff462_Y[streamId][7], strBuff462_UV[streamId][7], SWOSD_WINDOW_TWO);
			gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdLogoStringUpdate = OSD_NO;
		}

		fontInfo.format		= SWOSD_FORMATYUV420p;
		fontInfo.charWidth	= SWOSD_CHARWIDTH_462_12;
		fontInfo.charHeight	= SWOSD_CHARHEIGHT_462_12;
		fontInfo.totFontWidth	= SWOSD_STRINGWIDTH_462_12;
		fontInfo.totFontHeight	= SWOSD_CHARHEIGHT_462_12;
		fontInfo.totFontLineoffset	= SWOSD_STRINGWIDTH_462_12;

		if(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdType == SWOSD_DETAIL)
		{
			if(strcmp(frameRateStr,frameRate_str[streamId])!=0) {
				fontInfo.fontAddr		= fontBuff462_Y1;
				fontInfo.fontAddrUV     = fontBuff462_UV1;
				fontInfo.fontDB_Y       = fontDB462_Y1;
				fontInfo.fontDB_UV      = fontDB462_UV1;

				SWOSD_MakeOsdwinstring(&fontInfo, frameRateStr, strBuff462_Y[streamId][2] + (SWOSD_FRAMERATE_PREFIX * SWOSD_CHARWIDTH_462_12), &osdHndl->bmpWinPrm[SWOSD_WINDOW_FIVE], SWOSD_FRAMERATE_STR_LEN);
				strBuff462_UV[streamId][2]= (char*)(strBuff462_Y[streamId][2]+(SWOSD_FRAMERATE_STR_LEN * SWOSD_CHARWIDTH_462_12* SWOSD_CHARHEIGHT_462_12));
				SWOSD_setBmpWinAddr(osdHndl, strBuff462_Y[streamId][2], strBuff462_UV[streamId][2], SWOSD_WINDOW_FIVE);
				strcpy(frameRate_str[streamId], frameRateStr);
			}

			if(strcmp(bitRateStr,codecBitRate_str[streamId])!=0) {
				fontInfo.fontAddr		= fontBuff462_Y1;
				fontInfo.fontAddrUV     = fontBuff462_UV1;
				fontInfo.fontDB_Y       = fontDB462_Y1;
				fontInfo.fontDB_UV      = fontDB462_UV1;

				SWOSD_MakeOsdwinstring(&fontInfo, bitRateStr, strBuff462_Y[streamId][3] + ( bitratePrefix * SWOSD_CHARWIDTH_462_12), &osdHndl->bmpWinPrm[SWOSD_WINDOW_SIX], bitrateLen);
				strBuff462_UV[streamId][3]= (char*)(strBuff462_Y[streamId][3]+(bitrateLen * SWOSD_CHARWIDTH_462_12*SWOSD_CHARHEIGHT_462_12));
				SWOSD_setBmpWinAddr(osdHndl,strBuff462_Y[streamId][3], strBuff462_UV[streamId][3], SWOSD_WINDOW_SIX);
				strcpy(codecBitRate_str[streamId], bitRateStr);
			}

			if(strcmp(autoFocusStr, autoFocus_str[streamId])!=0) {
				fontInfo.fontAddr		= fontBuff462_Y1;
				fontInfo.fontAddrUV     = fontBuff462_UV1;
				fontInfo.fontDB_Y       = fontDB462_Y1;
				fontInfo.fontDB_UV      = fontDB462_UV1;

				SWOSD_MakeOsdwinstring(&fontInfo, autoFocusStr, strBuff462_Y[streamId][8] + (SWOSD_AUTOFOCUS_PREFIX * SWOSD_CHARWIDTH_462_12), &osdHndl->bmpWinPrm[SWOSD_WINDOW_TEN], SWOSD_AUTOFOCUS_STR_LEN);
				strBuff462_UV[streamId][8]= (char*)(strBuff462_Y[streamId][8]+(SWOSD_AUTOFOCUS_STR_LEN * SWOSD_CHARWIDTH_462_12* SWOSD_CHARHEIGHT_462_12));
				SWOSD_setBmpWinAddr(osdHndl, strBuff462_Y[streamId][8], strBuff462_UV[streamId][8], SWOSD_WINDOW_TEN);
				strcpy(autoFocus_str[streamId], autoFocusStr);
			}
		}

#ifdef SWOSD_USER_TRANSPARANCY_ON
		fontInfo.fontAddr		= fontBuff462_Y;
		fontInfo.fontAddrUV 	= fontBuff462_UV;
		fontInfo.fontDB_Y		= fontDB462_Y;
		fontInfo.fontDB_UV		= fontDB462_UV;
#else
		fontInfo.fontAddr		= fontBuff462_Y1;
		fontInfo.fontAddrUV 	= fontBuff462_UV1;
		fontInfo.fontDB_Y		= fontDB462_Y1;
		fontInfo.fontDB_UV		= fontDB462_UV1;
#endif

		if(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdDateTimeUpdate == OSD_YES)
		{
			if(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdDatePos==SWOSD_FMT_BOTTOM_LEFT) {
				if(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdTimePos==SWOSD_FMT_BOTTOM_LEFT) {
					SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_EIGHT, 0, gAVSERVER_config.encodeConfig[streamId].cropHeight - 3 * SWOSD_CHARHEIGHT_462_12);
					SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_NINE, 0, gAVSERVER_config.encodeConfig[streamId].cropHeight - 2 * SWOSD_CHARHEIGHT_462_12);
				}
				else {
					SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_EIGHT, 0, gAVSERVER_config.encodeConfig[streamId].cropHeight - 2 * SWOSD_CHARHEIGHT_462_12);
					SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_NINE, gAVSERVER_config.encodeConfig[streamId].cropWidth -OSA_align((SWOSD_MAX_DATE_TIME_LENGHT * SWOSD_CHARWIDTH_462_12),4), gAVSERVER_config.encodeConfig[streamId].cropHeight - 2 * SWOSD_CHARHEIGHT_462_12);
				}
			}
			else {
				if(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdTimePos==SWOSD_FMT_BOTTOM_LEFT) {
					SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_EIGHT, gAVSERVER_config.encodeConfig[streamId].cropWidth -OSA_align((SWOSD_MAX_DATE_TIME_LENGHT * SWOSD_CHARWIDTH_462_12),4) , gAVSERVER_config.encodeConfig[streamId].cropHeight - 2 * SWOSD_CHARHEIGHT_462_12);
					SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_NINE, 0, gAVSERVER_config.encodeConfig[streamId].cropHeight - 2 * SWOSD_CHARHEIGHT_462_12);
				}
				else {
					SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_EIGHT, gAVSERVER_config.encodeConfig[streamId].cropWidth - OSA_align((SWOSD_MAX_DATE_TIME_LENGHT  * SWOSD_CHARWIDTH_462_12),4), gAVSERVER_config.encodeConfig[streamId].cropHeight - 3 * SWOSD_CHARHEIGHT_462_12);
					SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_NINE, gAVSERVER_config.encodeConfig[streamId].cropWidth - OSA_align((SWOSD_MAX_DATE_TIME_LENGHT * SWOSD_CHARWIDTH_462_12),4) , gAVSERVER_config.encodeConfig[streamId].cropHeight - 2 * SWOSD_CHARHEIGHT_462_12);
				}
			}

			gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdDateTimeUpdate = OSD_NO;
		}

		if(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdDateEnable) {
			if(strcmp(dateStr, date_str[streamId])!=0) {
				SWOSD_MakeOsdwinstring(&fontInfo, dateStr, strBuff462_Y[streamId][5] + (SWOSD_DATE_PREFIX * SWOSD_CHARWIDTH_462_12) , &osdHndl->bmpWinPrm[SWOSD_WINDOW_EIGHT], SWOSD_DATE_STR_LEN);
				strBuff462_UV[streamId][5]= (char*)(strBuff462_Y[streamId][5]+(SWOSD_DATE_STR_LEN * SWOSD_CHARWIDTH_462_12 * SWOSD_CHARHEIGHT_462_12));
				SWOSD_setBmpWinAddr(osdHndl, strBuff462_Y[streamId][5], strBuff462_UV[streamId][5], SWOSD_WINDOW_EIGHT);
				strcpy(date_str[streamId], dateStr);
			}
		}

		if(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdTimeEnable) {
			if(strcmp(timeStr, time_str[streamId])!=0) {
				SWOSD_MakeOsdwinstring(&fontInfo, timeStr, strBuff462_Y[streamId][6] + (SWOSD_TIME_PREFIX * SWOSD_CHARWIDTH_462_12), &osdHndl->bmpWinPrm[SWOSD_WINDOW_NINE], SWOSD_TIME_STR_LEN);
				strBuff462_UV[streamId][6]= (char*)(strBuff462_Y[streamId][6]+(SWOSD_TIME_STR_LEN * SWOSD_CHARWIDTH_462_12 * SWOSD_CHARHEIGHT_462_12));
				SWOSD_setBmpWinAddr(osdHndl, strBuff462_Y[streamId][6], strBuff462_UV[streamId][6], SWOSD_WINDOW_NINE);
				strcpy(time_str[streamId], timeStr);
			}
		}

#ifdef FR_OSD_DRAW   //ANR - FR

		fontInfo.format 	= SWOSD_FORMATYUV420p;
		fontInfo.charWidth	= SWOSD_CHARWIDTH_770_20;
		fontInfo.charHeight = SWOSD_CHARHEIGHT_770_20;
		fontInfo.totFontWidth	= SWOSD_STRINGWIDTH_770_20;
		fontInfo.totFontHeight	= SWOSD_CHARHEIGHT_770_20;
		fontInfo.totFontLineoffset	= SWOSD_STRINGWIDTH_770_20;
#ifdef SWOSD_USER_TRANSPARANCY_ON
		fontInfo.fontAddr		= fontBuff770_Y;
		fontInfo.fontAddrUV 	= fontBuff770_UV;
		fontInfo.fontDB_Y		= fontDB770_Y;
		fontInfo.fontDB_UV		= fontDB770_UV;
#else
		fontInfo.fontAddr		= fontBuff770_Y1;
		fontInfo.fontAddrUV 	= fontBuff770_UV1;
		fontInfo.fontDB_Y		= fontDB770_Y1;
		fontInfo.fontDB_UV		= fontDB770_UV1;
#endif
		for(fr = 0; fr < SWOSD_WINDOW_FR_MAX; fr++){
			if((Fr_UsrInfo[fr].usrID != 0 ) || (strcmp(Fr_UsrInfo[fr].usrStr, frUsr_str[streamId]) != 0) || (osdHndl->bmpWinPrm[SWOSD_WINDOW_FR_ST+fr].bmpWinAddr == NULL))
			{
				SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_FR_ST+fr, Fr_UsrInfo[fr].stX, Fr_UsrInfo[fr].stY);
				SWOSD_MakeOsdwinstring(&fontInfo, Fr_UsrInfo[fr].usrStr, strBuff1232_Y_FR[fr], &osdHndl->bmpWinPrm[SWOSD_WINDOW_FR_ST+fr], strlen (Fr_UsrInfo[fr].usrStr));	//SWOSD_FR_USRSTR_LEN
				strBuff1232_UV_FR[fr]= (char*)(strBuff1232_Y_FR[fr]+(strlen (Fr_UsrInfo[fr].usrStr) * fontInfo.charWidth* fontInfo.charHeight));
				SWOSD_setBmpWinAddr(osdHndl, strBuff1232_Y_FR[fr], strBuff1232_UV_FR[fr], SWOSD_WINDOW_FR_ST+fr);
				strcpy(frUsr_str[streamId], Fr_UsrInfo[fr].usrStr);
			}
		}
#endif
	}
	else if (gAVSERVER_config.encodeConfig[streamId].cropWidth<=VIDEO_RESOL_WIDTH_MEDIUM  )
	{
		if(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdLogoStringUpdate == OSD_YES)
		{
			fontInfo.format		= SWOSD_FORMATYUV420p;
			fontInfo.charWidth		= SWOSD_CHARWIDTH_1078_28;
			fontInfo.charHeight	= SWOSD_CHARHEIGHT_1078_28;
			fontInfo.totFontWidth	= SWOSD_STRINGWIDTH_1078_28;
			fontInfo.totFontHeight	= SWOSD_CHARHEIGHT_1078_28;
			fontInfo.totFontLineoffset	= SWOSD_STRINGWIDTH_1078_28;

			fontInfo.fontAddr		= fontBuff1078_Y1;
			fontInfo.fontAddrUV     = fontBuff1078_UV1;
			fontInfo.fontDB_Y         =  fontDB1078_Y1;
			fontInfo.fontDB_UV       = fontDB1078_UV1;

			if(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdTextPos == SWOSD_FMT_TOP_LEFT){
				if(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdLogoPos == SWOSD_FMT_TOP_LEFT){
					SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_ONE, 0, 0);
					SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_TWO,  0, SWOSD_CHARHEIGHTTILOGO_160_64);
				}
				else {
                    SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_ONE,   gAVSERVER_config.encodeConfig[streamId].cropWidth - SWOSD_CHARWIDTHTILOGO_160_64, 0);
					SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_TWO,  0, 0);
				}
			}
			else {
				if(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdLogoPos == SWOSD_FMT_TOP_LEFT) {
					SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_ONE, 0, 0);
					SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_TWO,  gAVSERVER_config.encodeConfig[streamId].cropWidth - OSA_align((strlen(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdDispString) * SWOSD_CHARWIDTH_1078_28),4), 0);
				}
				else {
                    SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_ONE,   gAVSERVER_config.encodeConfig[streamId].cropWidth - SWOSD_CHARWIDTHTILOGO_160_64, 0);
					SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_TWO,  gAVSERVER_config.encodeConfig[streamId].cropWidth - OSA_align((strlen(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdDispString) * SWOSD_CHARWIDTH_1078_28),4), SWOSD_CHARHEIGHTTILOGO_160_64);
				}
			}

			SWOSD_MakeOsdwinstring(&fontInfo,  gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdDispString , strBuff1078_Y[streamId][7], &osdHndl->bmpWinPrm[SWOSD_WINDOW_TWO], strlen(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdDispString));
			strBuff1078_UV[streamId][7]  = (char*)(osdHndl->bmpWinPrm[SWOSD_WINDOW_TWO].bmpWinAddrUV );
			SWOSD_setBmpWinAddr(osdHndl, strBuff1078_Y[streamId][7], strBuff1078_UV[streamId][7], SWOSD_WINDOW_TWO);
			gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdLogoStringUpdate = OSD_NO;
		}

		fontInfo.format		= SWOSD_FORMATYUV420p;
		fontInfo.charWidth		= SWOSD_CHARWIDTH_770_20;
		fontInfo.charHeight	= SWOSD_CHARHEIGHT_770_20;
		fontInfo.totFontWidth	= SWOSD_STRINGWIDTH_770_20;
		fontInfo.totFontHeight	= SWOSD_CHARHEIGHT_770_20;
		fontInfo.totFontLineoffset	= SWOSD_STRINGWIDTH_770_20;

		if(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdType == SWOSD_DETAIL)
		{
			if(strcmp(frameRateStr, frameRate_str[streamId]) != 0) {
				fontInfo.fontAddr		= fontBuff770_Y1;
				fontInfo.fontAddrUV     = fontBuff770_UV1;
				fontInfo.fontDB_Y         =  fontDB770_Y1;
				fontInfo.fontDB_UV       = fontDB770_UV1;

				SWOSD_MakeOsdwinstring(&fontInfo, frameRateStr, strBuff770_Y[streamId][2] + (SWOSD_FRAMERATE_PREFIX * SWOSD_CHARWIDTH_770_20), &osdHndl->bmpWinPrm[SWOSD_WINDOW_FIVE], SWOSD_FRAMERATE_STR_LEN);
				strBuff770_UV[streamId][2] =  (char*)(strBuff770_Y[streamId][2]+(SWOSD_FRAMERATE_STR_LEN * SWOSD_CHARWIDTH_770_20* SWOSD_CHARHEIGHT_770_20));
				SWOSD_setBmpWinAddr(osdHndl, strBuff770_Y[streamId][2], strBuff770_UV[streamId][2], SWOSD_WINDOW_FIVE);
				strcpy(frameRate_str[streamId], frameRateStr);
			}

			if(strcmp(bitRateStr, codecBitRate_str[streamId]) != 0) {
				fontInfo.fontAddr		= fontBuff770_Y1;
				fontInfo.fontAddrUV     = fontBuff770_UV1;
				fontInfo.fontDB_Y         =  fontDB770_Y1;
				fontInfo.fontDB_UV       = fontDB770_UV1;

				SWOSD_MakeOsdwinstring(&fontInfo, bitRateStr, strBuff770_Y[streamId][3] + ( bitratePrefix * SWOSD_CHARWIDTH_770_20), &osdHndl->bmpWinPrm[SWOSD_WINDOW_SIX], bitrateLen);
				strBuff770_UV[streamId][3]= (char*)(strBuff770_Y[streamId][3]+(bitrateLen * SWOSD_CHARWIDTH_770_20*SWOSD_CHARHEIGHT_770_20));
				SWOSD_setBmpWinAddr(osdHndl,strBuff770_Y[streamId][3], strBuff770_UV[streamId][3], SWOSD_WINDOW_SIX);
				strcpy(codecBitRate_str[streamId], bitRateStr);
			}

			if(strcmp(autoFocusStr, autoFocus_str[streamId])!=0) {
				fontInfo.fontAddr		= fontBuff770_Y1;
				fontInfo.fontAddrUV     = fontBuff770_UV1;
				fontInfo.fontDB_Y         =  fontDB770_Y1;
				fontInfo.fontDB_UV       = fontDB770_UV1;

				SWOSD_MakeOsdwinstring(&fontInfo, autoFocusStr, strBuff770_Y[streamId][8] + (SWOSD_AUTOFOCUS_PREFIX * SWOSD_CHARWIDTH_770_20), &osdHndl->bmpWinPrm[SWOSD_WINDOW_TEN], SWOSD_AUTOFOCUS_STR_LEN);
				strBuff770_UV[streamId][8] =  (char*)(strBuff770_Y[streamId][8]+(SWOSD_AUTOFOCUS_STR_LEN * SWOSD_CHARWIDTH_770_20* SWOSD_CHARHEIGHT_770_20));
				SWOSD_setBmpWinAddr(osdHndl, strBuff770_Y[streamId][8], strBuff770_UV[streamId][8], SWOSD_WINDOW_TEN);
				strcpy(autoFocus_str[streamId], autoFocusStr);
			}
		}

#ifdef SWOSD_USER_TRANSPARANCY_ON
		fontInfo.fontAddr		= fontBuff770_Y;
		fontInfo.fontAddrUV     = fontBuff770_UV;
		fontInfo.fontDB_Y       =  fontDB770_Y;
		fontInfo.fontDB_UV      = fontDB770_UV;
#else
		fontInfo.fontAddr		= fontBuff770_Y1;
		fontInfo.fontAddrUV     = fontBuff770_UV1;
		fontInfo.fontDB_Y       = fontDB770_Y1;
		fontInfo.fontDB_UV      = fontDB770_UV1;
#endif

		if(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdDateTimeUpdate == OSD_YES)
		{
			if(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdDatePos==SWOSD_FMT_BOTTOM_LEFT) {
				if(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdTimePos==SWOSD_FMT_BOTTOM_LEFT) {
					SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_EIGHT, 0, gAVSERVER_config.encodeConfig[streamId].cropHeight - 3 * SWOSD_CHARHEIGHT_770_20);
					SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_NINE, 0, gAVSERVER_config.encodeConfig[streamId].cropHeight - 2 * SWOSD_CHARHEIGHT_770_20);
				}
				else {
					SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_EIGHT, 0, gAVSERVER_config.encodeConfig[streamId].cropHeight - 2 * SWOSD_CHARHEIGHT_770_20);
					SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_NINE, gAVSERVER_config.encodeConfig[streamId].cropWidth -OSA_align((SWOSD_MAX_DATE_TIME_LENGHT * SWOSD_CHARWIDTH_770_20),4), gAVSERVER_config.encodeConfig[streamId].cropHeight - 2 * SWOSD_CHARHEIGHT_770_20);
				}
			}
			else {
				if(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdTimePos==SWOSD_FMT_BOTTOM_LEFT) {
					SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_EIGHT, gAVSERVER_config.encodeConfig[streamId].cropWidth -OSA_align((SWOSD_MAX_DATE_TIME_LENGHT * SWOSD_CHARWIDTH_770_20),4), gAVSERVER_config.encodeConfig[streamId].cropHeight - 2 * SWOSD_CHARHEIGHT_770_20);
					SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_NINE, 0, gAVSERVER_config.encodeConfig[streamId].cropHeight - 2 * SWOSD_CHARHEIGHT_770_20);
				}
				else {
					SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_EIGHT, gAVSERVER_config.encodeConfig[streamId].cropWidth -OSA_align((SWOSD_MAX_DATE_TIME_LENGHT * SWOSD_CHARWIDTH_770_20),4), gAVSERVER_config.encodeConfig[streamId].cropHeight - 3 * SWOSD_CHARHEIGHT_770_20);
					SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_NINE, gAVSERVER_config.encodeConfig[streamId].cropWidth -OSA_align((SWOSD_MAX_DATE_TIME_LENGHT * SWOSD_CHARWIDTH_770_20),4), gAVSERVER_config.encodeConfig[streamId].cropHeight - 2 * SWOSD_CHARHEIGHT_770_20);
				}
			}

			gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdDateTimeUpdate = OSD_NO;
		}

		if(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdDateEnable) {
			if(strcmp(dateStr, date_str[streamId])!=0) {
				SWOSD_MakeOsdwinstring(&fontInfo, dateStr, strBuff770_Y[streamId][5] + ( SWOSD_DATE_PREFIX * SWOSD_CHARWIDTH_770_20), &osdHndl->bmpWinPrm[SWOSD_WINDOW_EIGHT], SWOSD_DATE_STR_LEN);
				strBuff770_UV[streamId][5] = (char*)(strBuff770_Y[streamId][5]+(SWOSD_DATE_STR_LEN * SWOSD_CHARWIDTH_770_20*SWOSD_CHARHEIGHT_770_20));
				SWOSD_setBmpWinAddr(osdHndl, strBuff770_Y[streamId][5], strBuff770_UV[streamId][5], SWOSD_WINDOW_EIGHT);
				strcpy(date_str[streamId], dateStr);
			}
		}

		if(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdTimeEnable) {
			if(strcmp(timeStr, time_str[streamId])!=0) {
				SWOSD_MakeOsdwinstring(&fontInfo, timeStr, strBuff770_Y[streamId][6] + (SWOSD_TIME_PREFIX * SWOSD_CHARWIDTH_770_20), &osdHndl->bmpWinPrm[SWOSD_WINDOW_NINE], SWOSD_TIME_STR_LEN);
				strBuff770_UV[streamId][6] = (char*)(strBuff770_Y[streamId][6]+(SWOSD_TIME_STR_LEN * SWOSD_CHARWIDTH_770_20*SWOSD_CHARHEIGHT_770_20));
				SWOSD_setBmpWinAddr(osdHndl, strBuff770_Y[streamId][6], strBuff770_UV[streamId][6], SWOSD_WINDOW_NINE);
				strcpy(time_str[streamId], timeStr);
			}
		}

#ifdef FR_OSD_DRAW   //ANR - FR
		fontInfo.format		= SWOSD_FORMATYUV420p;
		fontInfo.charWidth	= SWOSD_CHARWIDTH_770_20;
		fontInfo.charHeight	= SWOSD_CHARHEIGHT_770_20;
		fontInfo.totFontWidth	= SWOSD_STRINGWIDTH_770_20;
		fontInfo.totFontHeight	= SWOSD_CHARHEIGHT_770_20;
		fontInfo.totFontLineoffset	= SWOSD_STRINGWIDTH_770_20;
#ifdef SWOSD_USER_TRANSPARANCY_ON
		fontInfo.fontAddr		= fontBuff770_Y;
		fontInfo.fontAddrUV 	= fontBuff770_UV;
		fontInfo.fontDB_Y		= fontDB770_Y;
		fontInfo.fontDB_UV		= fontDB770_UV;
#else
		fontInfo.fontAddr		= fontBuff770_Y1;
		fontInfo.fontAddrUV 	= fontBuff770_UV1;
		fontInfo.fontDB_Y		= fontDB770_Y1;
		fontInfo.fontDB_UV		= fontDB770_UV1;
#endif

		for(fr = 0; fr < SWOSD_WINDOW_FR_MAX; fr++){
			if((Fr_UsrInfo[fr].usrID != 0 ) || (strcmp(Fr_UsrInfo[fr].usrStr, frUsr_str[streamId]) != 0) || (osdHndl->bmpWinPrm[SWOSD_WINDOW_FR_ST+fr].bmpWinAddr == NULL))
			{
				SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_FR_ST+fr, Fr_UsrInfo[fr].stX, Fr_UsrInfo[fr].stY);
				SWOSD_MakeOsdwinstring(&fontInfo, Fr_UsrInfo[fr].usrStr, strBuff1232_Y_FR[fr], &osdHndl->bmpWinPrm[SWOSD_WINDOW_FR_ST+fr], strlen (Fr_UsrInfo[fr].usrStr));	//SWOSD_FR_USRSTR_LEN
				strBuff1232_UV_FR[fr]= (char*)(strBuff1232_Y_FR[fr]+(strlen (Fr_UsrInfo[fr].usrStr) * fontInfo.charWidth* fontInfo.charHeight));
				SWOSD_setBmpWinAddr(osdHndl, strBuff1232_Y_FR[fr], strBuff1232_UV_FR[fr], SWOSD_WINDOW_FR_ST+fr);
				strcpy(frUsr_str[streamId], Fr_UsrInfo[fr].usrStr);
			}
		}
#endif
	}
	else if (gAVSERVER_config.encodeConfig[streamId].cropWidth<=VIDEO_RESOL_WIDTH_LARGE)
	{
		if(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdLogoStringUpdate == OSD_YES)
		{
			fontInfo.format		= SWOSD_FORMATYUV420p;
			fontInfo.charWidth		= SWOSD_CHARWIDTH_1232_30;
			fontInfo.charHeight	= SWOSD_CHARHEIGHT_1232_30;
			fontInfo.totFontWidth	= SWOSD_STRINGWIDTH_1232_30;
			fontInfo.totFontHeight	= SWOSD_CHARHEIGHT_1232_30;
			fontInfo.totFontLineoffset	= SWOSD_STRINGWIDTH_1232_30;
			fontInfo.fontAddr		= fontBuff1232_Y1;
			fontInfo.fontAddrUV     = fontBuff1232_UV1;
			fontInfo.fontDB_Y         =  fontDB1232_Y1;
			fontInfo.fontDB_UV       = fontDB1232_UV1;

			if(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdTextPos == SWOSD_FMT_TOP_LEFT){
				if(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdLogoPos == SWOSD_FMT_TOP_LEFT){
					SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_ONE, 0, 0);
					SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_TWO,  0, SWOSD_CHARHEIGHTTILOGO_160_64);
				}
				else {
                    SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_ONE,   gAVSERVER_config.encodeConfig[streamId].cropWidth - SWOSD_CHARWIDTHTILOGO_160_64, 0);
					SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_TWO,  0, 0);
				}
			}
			else {
				if(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdLogoPos == SWOSD_FMT_TOP_LEFT) {
					SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_ONE, 0, 0);
					SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_TWO,  gAVSERVER_config.encodeConfig[streamId].cropWidth - OSA_align((strlen(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdDispString) * SWOSD_CHARWIDTH_1232_30),4), 0);
				}
				else {
                    SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_ONE,   gAVSERVER_config.encodeConfig[streamId].cropWidth - SWOSD_CHARWIDTHTILOGO_160_64, 0);
					SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_TWO,  gAVSERVER_config.encodeConfig[streamId].cropWidth - OSA_align((strlen(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdDispString) * SWOSD_CHARWIDTH_1232_30),4), SWOSD_CHARHEIGHTTILOGO_160_64);
				}
			}

			SWOSD_MakeOsdwinstring(&fontInfo,  gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdDispString , strBuff1232_Y[streamId][7], &osdHndl->bmpWinPrm[SWOSD_WINDOW_TWO], strlen(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdDispString));
			strBuff1232_UV[streamId][7]  = (char*)(osdHndl->bmpWinPrm[SWOSD_WINDOW_TWO].bmpWinAddrUV );
			SWOSD_setBmpWinAddr(osdHndl, strBuff1232_Y[streamId][7], strBuff1232_UV[streamId][7], SWOSD_WINDOW_TWO);
			gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdLogoStringUpdate = OSD_NO;
		}

		fontInfo.format		= SWOSD_FORMATYUV420p;
		fontInfo.charWidth		= SWOSD_CHARWIDTH_1078_28;
		fontInfo.charHeight	= SWOSD_CHARHEIGHT_1078_28;
		fontInfo.totFontWidth	= SWOSD_STRINGWIDTH_1078_28;
		fontInfo.totFontHeight	= SWOSD_CHARHEIGHT_1078_28;
		fontInfo.totFontLineoffset	= SWOSD_STRINGWIDTH_1078_28;

		if(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdType == SWOSD_DETAIL)
		{
			if(strcmp(frameRateStr, frameRate_str[streamId])!=0) {
				fontInfo.fontAddr		= fontBuff1078_Y1;
				fontInfo.fontAddrUV     = fontBuff1078_UV1;
				fontInfo.fontDB_Y         =  fontDB1078_Y1;
				fontInfo.fontDB_UV       = fontDB1078_UV1;

				SWOSD_MakeOsdwinstring(&fontInfo, frameRateStr, strBuff1078_Y[streamId][2] + (SWOSD_FRAMERATE_PREFIX *  SWOSD_CHARWIDTH_1078_28), &osdHndl->bmpWinPrm[SWOSD_WINDOW_FIVE], SWOSD_FRAMERATE_STR_LEN);
				strBuff1078_UV[streamId][2]= (char*)(strBuff1078_Y[streamId][2]+(SWOSD_FRAMERATE_STR_LEN * SWOSD_CHARWIDTH_1078_28* SWOSD_CHARHEIGHT_1078_28));
				SWOSD_setBmpWinAddr(osdHndl, strBuff1078_Y[streamId][2], strBuff1078_UV[streamId][2], SWOSD_WINDOW_FIVE);
				strcpy(frameRate_str[streamId], frameRateStr);
			}

			if(strcmp(bitRateStr, codecBitRate_str[streamId]) != 0) {
				fontInfo.fontAddr		= fontBuff1078_Y1;
				fontInfo.fontAddrUV     = fontBuff1078_UV1;
				fontInfo.fontDB_Y         =  fontDB1078_Y1;
				fontInfo.fontDB_UV       = fontDB1078_UV1;

				SWOSD_MakeOsdwinstring(&fontInfo, bitRateStr, strBuff1078_Y[streamId][3] + ( bitratePrefix * SWOSD_CHARWIDTH_1078_28), &osdHndl->bmpWinPrm[SWOSD_WINDOW_SIX], bitrateLen);
				strBuff1078_UV[streamId][3]= (char*)(strBuff1078_Y[streamId][3] +( bitrateLen * SWOSD_CHARWIDTH_1078_28*SWOSD_CHARHEIGHT_1078_28));
				SWOSD_setBmpWinAddr(osdHndl,strBuff1078_Y[streamId][3], strBuff1078_UV[streamId][3], SWOSD_WINDOW_SIX);
				strcpy(codecBitRate_str[streamId], bitRateStr);
			}

			if(strcmp(autoFocusStr, autoFocus_str[streamId])!=0) {
				fontInfo.fontAddr		= fontBuff1078_Y1;
				fontInfo.fontAddrUV     = fontBuff1078_UV1;
				fontInfo.fontDB_Y         =  fontDB1078_Y1;
				fontInfo.fontDB_UV       = fontDB1078_UV1;

				SWOSD_MakeOsdwinstring(&fontInfo, autoFocusStr, strBuff1078_Y[streamId][8] + (SWOSD_AUTOFOCUS_PREFIX *  SWOSD_CHARWIDTH_1078_28), &osdHndl->bmpWinPrm[SWOSD_WINDOW_TEN], SWOSD_AUTOFOCUS_STR_LEN);
				strBuff1078_UV[streamId][8]= (char*)(strBuff1078_Y[streamId][8]+(SWOSD_AUTOFOCUS_STR_LEN * SWOSD_CHARWIDTH_1078_28* SWOSD_CHARHEIGHT_1078_28));
				SWOSD_setBmpWinAddr(osdHndl, strBuff1078_Y[streamId][8], strBuff1078_UV[streamId][8], SWOSD_WINDOW_TEN);
				strcpy(autoFocus_str[streamId], autoFocusStr);
			}
		}

#ifdef SWOSD_USER_TRANSPARANCY_ON
		fontInfo.fontAddr		= fontBuff1078_Y;
		fontInfo.fontAddrUV     = fontBuff1078_UV;
		fontInfo.fontDB_Y       = fontDB1078_Y;
		fontInfo.fontDB_UV      = fontDB1078_UV;
#else
		fontInfo.fontAddr		= fontBuff1078_Y1;
		fontInfo.fontAddrUV     = fontBuff1078_UV1;
		fontInfo.fontDB_Y       = fontDB1078_Y1;
		fontInfo.fontDB_UV      = fontDB1078_UV1;
#endif

		if(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdDateTimeUpdate == OSD_YES)
		{
			if(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdDatePos==SWOSD_FMT_BOTTOM_LEFT) {
				if(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdTimePos==SWOSD_FMT_BOTTOM_LEFT) {
					SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_EIGHT, 0, gAVSERVER_config.encodeConfig[streamId].cropHeight - 3 * SWOSD_CHARHEIGHT_1078_28);
					SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_NINE, 0, gAVSERVER_config.encodeConfig[streamId].cropHeight - 2 * SWOSD_CHARHEIGHT_1078_28);
				}
				else {
					SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_EIGHT, 0, gAVSERVER_config.encodeConfig[streamId].cropHeight - 2 * SWOSD_CHARHEIGHT_1078_28);
					SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_NINE, gAVSERVER_config.encodeConfig[streamId].cropWidth -OSA_align((SWOSD_MAX_DATE_TIME_LENGHT  * SWOSD_CHARWIDTH_1078_28),4), gAVSERVER_config.encodeConfig[streamId].cropHeight - 2 * SWOSD_CHARHEIGHT_1078_28);
				}
			}
			else {
				if(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdTimePos==SWOSD_FMT_BOTTOM_LEFT) {
					SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_EIGHT, gAVSERVER_config.encodeConfig[streamId].cropWidth -OSA_align((SWOSD_MAX_DATE_TIME_LENGHT  * SWOSD_CHARWIDTH_1078_28),4), gAVSERVER_config.encodeConfig[streamId].cropHeight - 2 * SWOSD_CHARHEIGHT_1078_28);
					SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_NINE, 0, gAVSERVER_config.encodeConfig[streamId].cropHeight - 2 * SWOSD_CHARHEIGHT_1078_28);
				}
				else {
					SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_EIGHT, gAVSERVER_config.encodeConfig[streamId].cropWidth -OSA_align((SWOSD_MAX_DATE_TIME_LENGHT  * SWOSD_CHARWIDTH_1078_28),4), gAVSERVER_config.encodeConfig[streamId].cropHeight - 3 * SWOSD_CHARHEIGHT_1078_28);
					SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_NINE, gAVSERVER_config.encodeConfig[streamId].cropWidth -OSA_align((SWOSD_MAX_DATE_TIME_LENGHT  * SWOSD_CHARWIDTH_1078_28),4), gAVSERVER_config.encodeConfig[streamId].cropHeight - 2 * SWOSD_CHARHEIGHT_1078_28);
				}
			}

			gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdDateTimeUpdate = OSD_NO;
		}

		if(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdDateEnable) {
			if(strcmp(dateStr, date_str[streamId])!=0) {
				SWOSD_MakeOsdwinstring(&fontInfo, dateStr, strBuff1078_Y[streamId][5] +  ( SWOSD_DATE_PREFIX * SWOSD_CHARWIDTH_1078_28), &osdHndl->bmpWinPrm[SWOSD_WINDOW_EIGHT], SWOSD_DATE_STR_LEN);
				strBuff1078_UV[streamId][5]= (char*)(strBuff1078_Y[streamId][5]+(SWOSD_DATE_STR_LEN* SWOSD_CHARWIDTH_1078_28*SWOSD_CHARHEIGHT_1078_28));
				SWOSD_setBmpWinAddr(osdHndl, strBuff1078_Y[streamId][5], strBuff1078_UV[streamId][5], SWOSD_WINDOW_EIGHT);
				strcpy(date_str[streamId], dateStr);
			}
		}

		if(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdTimeEnable) {
			if(strcmp(timeStr, time_str[streamId])!=0) {
				SWOSD_MakeOsdwinstring(&fontInfo, timeStr, strBuff1078_Y[streamId][6] + (SWOSD_TIME_PREFIX * SWOSD_CHARWIDTH_1078_28), &osdHndl->bmpWinPrm[SWOSD_WINDOW_NINE], SWOSD_TIME_STR_LEN);
				strBuff1078_UV[streamId][6]= (char*)(strBuff1078_Y[streamId][6]+(SWOSD_TIME_STR_LEN * SWOSD_CHARWIDTH_1078_28 * SWOSD_CHARHEIGHT_1078_28));
				SWOSD_setBmpWinAddr(osdHndl, strBuff1078_Y[streamId][6], strBuff1078_UV[streamId][6], SWOSD_WINDOW_NINE);
				strcpy(time_str[streamId], timeStr);
			}
		}

#ifdef FR_OSD_DRAW   //ANR - FR
		fontInfo.format 	= SWOSD_FORMATYUV420p;
		fontInfo.charWidth	= SWOSD_CHARWIDTH_1078_28;
		fontInfo.charHeight = SWOSD_CHARHEIGHT_1078_28;
		fontInfo.totFontWidth	= SWOSD_STRINGWIDTH_1078_28;
		fontInfo.totFontHeight	= SWOSD_CHARHEIGHT_1078_28;
		fontInfo.totFontLineoffset	= SWOSD_STRINGWIDTH_1078_28;
#ifdef SWOSD_USER_TRANSPARANCY_ON
		fontInfo.fontAddr		= fontBuff1078_Y;
		fontInfo.fontAddrUV 	= fontBuff1078_UV;
		fontInfo.fontDB_Y		= fontDB1078_Y;
		fontInfo.fontDB_UV		= fontDB1078_UV;
#else
		fontInfo.fontAddr		= fontBuff1078_Y1;
		fontInfo.fontAddrUV 	= fontBuff1078_UV1;
		fontInfo.fontDB_Y		= fontDB1078_Y1;
		fontInfo.fontDB_UV		= fontDB1078_UV1;
#endif
		for(fr = 0; fr < SWOSD_WINDOW_FR_MAX; fr++){
			if((Fr_UsrInfo[fr].usrID != 0 ) || (strcmp(Fr_UsrInfo[fr].usrStr, frUsr_str[streamId]) != 0) || (osdHndl->bmpWinPrm[SWOSD_WINDOW_FR_ST+fr].bmpWinAddr == NULL))
			{
				SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_FR_ST+fr, Fr_UsrInfo[fr].stX, Fr_UsrInfo[fr].stY);
				SWOSD_MakeOsdwinstring(&fontInfo, Fr_UsrInfo[fr].usrStr, strBuff1232_Y_FR[fr], &osdHndl->bmpWinPrm[SWOSD_WINDOW_FR_ST+fr], strlen (Fr_UsrInfo[fr].usrStr));	//SWOSD_FR_USRSTR_LEN
				strBuff1232_UV_FR[fr]= (char*)(strBuff1232_Y_FR[fr]+(strlen (Fr_UsrInfo[fr].usrStr) * fontInfo.charWidth* fontInfo.charHeight));
				SWOSD_setBmpWinAddr(osdHndl, strBuff1232_Y_FR[fr], strBuff1232_UV_FR[fr], SWOSD_WINDOW_FR_ST+fr);
				strcpy(frUsr_str[streamId], Fr_UsrInfo[fr].usrStr);
			}
		}
#endif

	}
	else if(gAVSERVER_config.encodeConfig[streamId].cropWidth>= VIDEO_RESOL_WIDTH_LARGE)
	{
		if(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdLogoStringUpdate == OSD_YES)
		{
			fontInfo.format		= SWOSD_FORMATYUV420p;
			fontInfo.charWidth		= SWOSD_CHARWIDTH_1540_40;
			fontInfo.charHeight	= SWOSD_CHARHEIGHT_1540_40;
			fontInfo.totFontWidth	= SWOSD_STRINGWIDTH_1540_40;
			fontInfo.totFontHeight	= SWOSD_CHARHEIGHT_1540_40;
			fontInfo.totFontLineoffset	= SWOSD_STRINGWIDTH_1540_40;
			fontInfo.fontAddr		= fontBuff1540_Y1;
			fontInfo.fontAddrUV     = fontBuff1540_UV1;
			fontInfo.fontDB_Y         =  fontDB1540_Y1;
			fontInfo.fontDB_UV       = fontDB1540_UV1;

			if(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdTextPos == SWOSD_FMT_TOP_LEFT){
				if(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdLogoPos == SWOSD_FMT_TOP_LEFT){
					SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_ONE, 0, 0);
					SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_TWO,  0, SWOSD_CHARHEIGHTTILOGO_160_64);
				}
				else {
                    SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_ONE,   gAVSERVER_config.encodeConfig[streamId].cropWidth - SWOSD_CHARWIDTHTILOGO_160_64, 0);
					SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_TWO,  0, 0);
				}
			}
			else {
				if(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdLogoPos == SWOSD_FMT_TOP_LEFT) {
					SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_ONE, 0, 0);
					SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_TWO,  gAVSERVER_config.encodeConfig[streamId].cropWidth - OSA_align((strlen(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdDispString) * SWOSD_CHARWIDTH_1540_40),4), 0);
				}
				else {
                    SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_ONE,   gAVSERVER_config.encodeConfig[streamId].cropWidth - SWOSD_CHARWIDTHTILOGO_160_64, 0);
					SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_TWO,  gAVSERVER_config.encodeConfig[streamId].cropWidth - OSA_align((strlen(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdDispString) * SWOSD_CHARWIDTH_1540_40),4), SWOSD_CHARHEIGHTTILOGO_160_64);
				}
			}

			SWOSD_MakeOsdwinstring(&fontInfo,  gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdDispString , strBuff1540_Y[streamId], &osdHndl->bmpWinPrm[SWOSD_WINDOW_TWO], strlen(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdDispString));
			strBuff1540_UV[streamId]= (char*)(osdHndl->bmpWinPrm[SWOSD_WINDOW_TWO].bmpWinAddrUV );
			SWOSD_setBmpWinAddr(osdHndl, strBuff1540_Y[streamId], strBuff1540_UV[streamId], SWOSD_WINDOW_TWO);
			gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdLogoStringUpdate = OSD_NO;

		}

		fontInfo.format		= SWOSD_FORMATYUV420p;
		fontInfo.charWidth		= SWOSD_CHARWIDTH_1232_30;
		fontInfo.charHeight	= SWOSD_CHARHEIGHT_1232_30;
		fontInfo.totFontWidth	= SWOSD_STRINGWIDTH_1232_30;
		fontInfo.totFontHeight	= SWOSD_CHARHEIGHT_1232_30;
		fontInfo.totFontLineoffset	= SWOSD_STRINGWIDTH_1232_30;

		if(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdType == SWOSD_DETAIL)
		{
			if(strcmp(frameRateStr, frameRate_str[streamId])!=0) {
				fontInfo.fontAddr		 = fontBuff1232_Y1;
				fontInfo.fontAddrUV      = fontBuff1232_UV1;
				fontInfo.fontDB_Y        = fontDB1232_Y1;
				fontInfo.fontDB_UV       = fontDB1232_UV1;

				SWOSD_MakeOsdwinstring(&fontInfo, frameRateStr, strBuff1232_Y[streamId][2] + ( SWOSD_FRAMERATE_PREFIX * SWOSD_CHARWIDTH_1232_30), &osdHndl->bmpWinPrm[SWOSD_WINDOW_FIVE], SWOSD_FRAMERATE_STR_LEN);
				strBuff1232_UV[streamId][2]= (char*)(strBuff1232_Y[streamId][2]+(SWOSD_FRAMERATE_STR_LEN * SWOSD_CHARWIDTH_1232_30 * SWOSD_CHARHEIGHT_1232_30));
				SWOSD_setBmpWinAddr(osdHndl, strBuff1232_Y[streamId][2], strBuff1232_UV[streamId][2], SWOSD_WINDOW_FIVE);
				strcpy(frameRate_str[streamId], frameRateStr);
			}

			if(strcmp(bitRateStr, codecBitRate_str[streamId]) != 0) {
				fontInfo.fontAddr		= fontBuff1232_Y1;
				fontInfo.fontAddrUV     = fontBuff1232_UV1;
				fontInfo.fontDB_Y       = fontDB1232_Y1;
				fontInfo.fontDB_UV      = fontDB1232_UV1;

				SWOSD_MakeOsdwinstring(&fontInfo, bitRateStr, strBuff1232_Y[streamId][3] + ( bitratePrefix * SWOSD_CHARWIDTH_1232_30), &osdHndl->bmpWinPrm[SWOSD_WINDOW_SIX], bitrateLen);
				strBuff1232_UV[streamId][3]= (char*)(strBuff1232_Y[streamId][3]+(bitrateLen * SWOSD_CHARWIDTH_1232_30*SWOSD_CHARHEIGHT_1232_30));
				SWOSD_setBmpWinAddr(osdHndl,strBuff1232_Y[streamId][3], strBuff1232_UV[streamId][3], SWOSD_WINDOW_SIX);
				strcpy(codecBitRate_str[streamId], bitRateStr);
			}

			if(strcmp(autoFocusStr, autoFocus_str[streamId])!=0) {
				fontInfo.fontAddr		 = fontBuff1232_Y1;
				fontInfo.fontAddrUV      = fontBuff1232_UV1;
				fontInfo.fontDB_Y        = fontDB1232_Y1;
				fontInfo.fontDB_UV       = fontDB1232_UV1;

				SWOSD_MakeOsdwinstring(&fontInfo, autoFocusStr, strBuff1232_Y[streamId][8] + ( SWOSD_AUTOFOCUS_PREFIX * SWOSD_CHARWIDTH_1232_30), &osdHndl->bmpWinPrm[SWOSD_WINDOW_TEN], SWOSD_AUTOFOCUS_STR_LEN);
				strBuff1232_UV[streamId][8]= (char*)(strBuff1232_Y[streamId][8]+(SWOSD_AUTOFOCUS_STR_LEN * SWOSD_CHARWIDTH_1232_30 * SWOSD_CHARHEIGHT_1232_30));
				SWOSD_setBmpWinAddr(osdHndl, strBuff1232_Y[streamId][8], strBuff1232_UV[streamId][8], SWOSD_WINDOW_TEN);
				strcpy(autoFocus_str[streamId], autoFocusStr);
			}
		}

#ifdef SWOSD_USER_TRANSPARANCY_ON
		fontInfo.fontAddr		= fontBuff1232_Y;
		fontInfo.fontAddrUV     = fontBuff1232_UV;
		fontInfo.fontDB_Y       = fontDB1232_Y;
		fontInfo.fontDB_UV      = fontDB1232_UV;
#else
		fontInfo.fontAddr		= fontBuff1232_Y1;
		fontInfo.fontAddrUV     = fontBuff1232_UV1;
		fontInfo.fontDB_Y       = fontDB1232_Y1;
		fontInfo.fontDB_UV      = fontDB1232_UV1;
#endif

		if(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdDateTimeUpdate == OSD_YES)
		{
			if(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdDatePos==SWOSD_FMT_BOTTOM_LEFT) {
				if(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdTimePos==SWOSD_FMT_BOTTOM_LEFT) {
					SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_EIGHT, 0, gAVSERVER_config.encodeConfig[streamId].cropHeight - 3 * SWOSD_CHARHEIGHT_1232_30);
					SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_NINE, 0, gAVSERVER_config.encodeConfig[streamId].cropHeight - 2 * SWOSD_CHARHEIGHT_1232_30);
				}
				else {
					SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_EIGHT, 0, gAVSERVER_config.encodeConfig[streamId].cropHeight - 2 * SWOSD_CHARHEIGHT_1232_30);
					SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_NINE, gAVSERVER_config.encodeConfig[streamId].cropWidth -OSA_align((SWOSD_MAX_DATE_TIME_LENGHT * SWOSD_CHARWIDTH_1232_30),4), gAVSERVER_config.encodeConfig[streamId].cropHeight - 2 * SWOSD_CHARHEIGHT_1232_30);
				}
			}
			else {
				if(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdTimePos==SWOSD_FMT_BOTTOM_LEFT) {
					SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_EIGHT, gAVSERVER_config.encodeConfig[streamId].cropWidth -OSA_align((SWOSD_MAX_DATE_TIME_LENGHT * SWOSD_CHARWIDTH_1232_30),4), gAVSERVER_config.encodeConfig[streamId].cropHeight - 2 * SWOSD_CHARHEIGHT_1232_30);
					SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_NINE, 0, gAVSERVER_config.encodeConfig[streamId].cropHeight - 2 * SWOSD_CHARHEIGHT_1232_30);
				}
				else {
					SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_EIGHT, gAVSERVER_config.encodeConfig[streamId].cropWidth -OSA_align((SWOSD_MAX_DATE_TIME_LENGHT * SWOSD_CHARWIDTH_1232_30),4), gAVSERVER_config.encodeConfig[streamId].cropHeight - 3 * SWOSD_CHARHEIGHT_1232_30);
					SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_NINE, gAVSERVER_config.encodeConfig[streamId].cropWidth -OSA_align((SWOSD_MAX_DATE_TIME_LENGHT * SWOSD_CHARWIDTH_1232_30),4), gAVSERVER_config.encodeConfig[streamId].cropHeight - 2 * SWOSD_CHARHEIGHT_1232_30);
				}
			}

			gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdDateTimeUpdate = OSD_NO;
		}

		if(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdDateEnable) {
			if(strcmp(dateStr, date_str[streamId])!=0) {
				SWOSD_MakeOsdwinstring(&fontInfo, dateStr, strBuff1232_Y[streamId][5] + ( SWOSD_DATE_PREFIX * SWOSD_CHARWIDTH_1232_30) , &osdHndl->bmpWinPrm[SWOSD_WINDOW_EIGHT], SWOSD_DATE_STR_LEN);
				strBuff1232_UV[streamId][5]= (char*)(strBuff1232_Y[streamId][5]+(SWOSD_DATE_STR_LEN * SWOSD_CHARWIDTH_1232_30 * SWOSD_CHARHEIGHT_1232_30));
				SWOSD_setBmpWinAddr(osdHndl, strBuff1232_Y[streamId][5], strBuff1232_UV[streamId][5], SWOSD_WINDOW_EIGHT);
				strcpy(date_str[streamId], dateStr);
			}
		}

		if(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdTimeEnable) {
			if(strcmp(timeStr, time_str[streamId])!=0) {
				SWOSD_MakeOsdwinstring(&fontInfo, timeStr, strBuff1232_Y[streamId][6] + ( SWOSD_TIME_PREFIX * SWOSD_CHARWIDTH_1232_30), &osdHndl->bmpWinPrm[SWOSD_WINDOW_NINE], SWOSD_TIME_STR_LEN);
				strBuff1232_UV[streamId][6]= (char*)(strBuff1232_Y[streamId][6]+(SWOSD_TIME_STR_LEN* SWOSD_CHARWIDTH_1232_30 * SWOSD_CHARHEIGHT_1232_30));
				SWOSD_setBmpWinAddr(osdHndl, strBuff1232_Y[streamId][6], strBuff1232_UV[streamId][6], SWOSD_WINDOW_NINE);
				strcpy(time_str[streamId], timeStr);
			}
		}

#ifdef FR_OSD_DRAW   //ANR - FR
		fontInfo.format 	= SWOSD_FORMATYUV420p;
		fontInfo.charWidth	= SWOSD_CHARWIDTH_1232_30;
		fontInfo.charHeight = SWOSD_CHARHEIGHT_1232_30;
		fontInfo.totFontWidth	= SWOSD_STRINGWIDTH_1232_30;
		fontInfo.totFontHeight	= SWOSD_CHARHEIGHT_1232_30;
		fontInfo.totFontLineoffset	= SWOSD_STRINGWIDTH_1232_30;
#ifdef SWOSD_USER_TRANSPARANCY_ON
		fontInfo.fontAddr		= fontBuff1232_Y;
		fontInfo.fontAddrUV 	= fontBuff1232_UV;
		fontInfo.fontDB_Y		= fontDB1232_Y;
		fontInfo.fontDB_UV		= fontDB1232_UV;
#else
		fontInfo.fontAddr		= fontBuff1232_Y1;
		fontInfo.fontAddrUV 	= fontBuff1232_UV1;
		fontInfo.fontDB_Y		= fontDB1232_Y1;
		fontInfo.fontDB_UV		= fontDB1232_UV1;
#endif
		for(fr = 0; fr < SWOSD_WINDOW_FR_MAX; fr++){
			if((Fr_UsrInfo[fr].usrID != 0 ) || (strcmp(Fr_UsrInfo[fr].usrStr, frUsr_str[streamId]) != 0) || (osdHndl->bmpWinPrm[SWOSD_WINDOW_FR_ST+fr].bmpWinAddr == NULL))
			{
				SWOSD_setBmpchangeWinXYPrm(osdHndl, SWOSD_WINDOW_FR_ST+fr, Fr_UsrInfo[fr].stX, Fr_UsrInfo[fr].stY);
				SWOSD_MakeOsdwinstring(&fontInfo, Fr_UsrInfo[fr].usrStr, strBuff1232_Y_FR[fr], &osdHndl->bmpWinPrm[SWOSD_WINDOW_FR_ST+fr], strlen (Fr_UsrInfo[fr].usrStr));	//SWOSD_FR_USRSTR_LEN
				strBuff1232_UV_FR[fr]= (char*)(strBuff1232_Y_FR[fr]+(strlen (Fr_UsrInfo[fr].usrStr) * fontInfo.charWidth* fontInfo.charHeight));
				SWOSD_setBmpWinAddr(osdHndl, strBuff1232_Y_FR[fr], strBuff1232_UV_FR[fr], SWOSD_WINDOW_FR_ST+fr);
				strcpy(frUsr_str[streamId], Fr_UsrInfo[fr].usrStr);
			}
		}
#endif
	}

	return OSA_SOK;
}

static int  swosdMakeFixWinString(int streamId)
{
	char resolStr[SWOSD_MAX_STRING_LENGTH], bitRateStr[SWOSD_MAX_STRING_LENGTH];
	char codecStr[SWOSD_MAX_STRING_LENGTH]="Codec Name: ";
	char rateStr[SWOSD_MAX_STRING_LENGTH]="Rate Control: ";
	char framerateStr[SWOSD_MAX_STRING_LENGTH]="Frame Rate: 00.00";
	char tempStr[SWOSD_MAX_STRING_LENGTH];
	char autoFocusStr[SWOSD_MAX_STRING_LENGTH]="Auto Focus: ";
	char evalVerStr[SWOSD_MAX_STRING_LENGTH]="EVAL VERSION";
	Uint32 bitRate;
	SWOSD_Fontdata fontInfo;

	strcpy(tempStr,"\0");
	swosdGetCodecType(streamId, tempStr);
	strcat(codecStr, tempStr);

	strcpy(tempStr,"\0");
	swosdGetRateControl(streamId, tempStr);
	strcat(rateStr, tempStr);

	strcpy(tempStr,"\0");
	swosdGetAutoFocus(streamId, tempStr);
	strcat(autoFocusStr, tempStr);

	bitRate = swosdGetBitRate(streamId);
	if(gAVSERVER_config.encodeConfig[streamId].codecType == ALG_VID_CODEC_MJPEG)
		sprintf(bitRateStr, "QP Value:%3d", bitRate);
	else
		sprintf(bitRateStr, "Bit Rate:%5dKbps", (bitRate/1000));

	sprintf(resolStr, "Resolution: %dX%d", gAVSERVER_config.encodeConfig[streamId].cropWidth, gAVSERVER_config.encodeConfig[streamId].cropHeight);

	if (gAVSERVER_config.encodeConfig[streamId].cropWidth<=VIDEO_RESOL_WIDTH_SMALL )
	{
		fontInfo.format		= SWOSD_FORMATYUV420p;
		fontInfo.charWidth		= SWOSD_CHARWIDTH_462_12;
		fontInfo.charHeight	= SWOSD_CHARHEIGHT_462_12;
		fontInfo.totFontWidth	= SWOSD_STRINGWIDTH_462_12;
		fontInfo.totFontHeight	= SWOSD_CHARHEIGHT_462_12;
		fontInfo.totFontLineoffset	= SWOSD_STRINGWIDTH_462_12;
		fontInfo.fontAddr		= fontBuff462_Y1;
		fontInfo.fontAddrUV     = fontBuff462_UV1;
		fontInfo.fontDB_Y         =  fontDB462_Y1;
		fontInfo.fontDB_UV       = fontDB462_UV1;

		SWOSD_MakeOsdwinstring(&fontInfo, resolStr, strBuff462_Y[streamId][0], &osdHndl->bmpWinPrm[SWOSD_WINDOW_THREE], strlen(resolStr));
		strBuff462_UV[streamId][0]= (char*)(osdHndl->bmpWinPrm[SWOSD_WINDOW_THREE].bmpWinAddrUV );
		SWOSD_setBmpWinAddr(osdHndl, strBuff462_Y[streamId][0], strBuff462_UV[streamId][0], SWOSD_WINDOW_THREE);

		SWOSD_MakeOsdwinstring(&fontInfo, codecStr, strBuff462_Y[streamId][1], &osdHndl->bmpWinPrm[SWOSD_WINDOW_FOUR], strlen(codecStr));
		strBuff462_UV[streamId][1]= (char*)(osdHndl->bmpWinPrm[SWOSD_WINDOW_FOUR].bmpWinAddrUV );
		SWOSD_setBmpWinAddr(osdHndl, strBuff462_Y[streamId][1], strBuff462_UV[streamId][1], SWOSD_WINDOW_FOUR);

		SWOSD_MakeOsdwinstring(&fontInfo, framerateStr, strBuff462_Y[streamId][2], &osdHndl->bmpWinPrm[SWOSD_WINDOW_FIVE], strlen(framerateStr));
		strBuff462_UV[streamId][2]= (char*)(osdHndl->bmpWinPrm[SWOSD_WINDOW_FIVE].bmpWinAddrUV );
		SWOSD_setBmpWinAddr(osdHndl, strBuff462_Y[streamId][2], strBuff462_UV[streamId][2], SWOSD_WINDOW_FIVE);

		SWOSD_MakeOsdwinstring(&fontInfo, bitRateStr, strBuff462_Y[streamId][3], &osdHndl->bmpWinPrm[SWOSD_WINDOW_SIX], strlen(bitRateStr));
		strBuff462_UV[streamId][3]= (char*)(osdHndl->bmpWinPrm[SWOSD_WINDOW_SIX].bmpWinAddrUV );
		SWOSD_setBmpWinAddr(osdHndl, strBuff462_Y[streamId][3], strBuff462_UV[streamId][3], SWOSD_WINDOW_SIX);

		SWOSD_MakeOsdwinstring(&fontInfo, rateStr, strBuff462_Y[streamId][4], &osdHndl->bmpWinPrm[SWOSD_WINDOW_SEVEN], strlen(rateStr));
		strBuff462_UV[streamId][4]= (char*)(osdHndl->bmpWinPrm[SWOSD_WINDOW_SEVEN].bmpWinAddrUV );
		SWOSD_setBmpWinAddr(osdHndl, strBuff462_Y[streamId][4], strBuff462_UV[streamId][4], SWOSD_WINDOW_SEVEN);

		SWOSD_MakeOsdwinstring(&fontInfo, autoFocusStr, strBuff462_Y[streamId][8], &osdHndl->bmpWinPrm[SWOSD_WINDOW_TEN], strlen(autoFocusStr));
		strBuff462_UV[streamId][8]= (char*)(osdHndl->bmpWinPrm[SWOSD_WINDOW_TEN].bmpWinAddrUV );
		SWOSD_setBmpWinAddr(osdHndl, strBuff462_Y[streamId][8], strBuff462_UV[streamId][8], SWOSD_WINDOW_TEN);

		SWOSD_MakeOsdwinstring(&fontInfo, evalVerStr, strBuff462_Y[streamId][9], &osdHndl->bmpWinPrm[SWOSD_WINDOW_ELEVEN], strlen(evalVerStr));
		strBuff462_UV[streamId][9]= (char*)(osdHndl->bmpWinPrm[SWOSD_WINDOW_ELEVEN].bmpWinAddrUV );
		SWOSD_setBmpWinAddr(osdHndl, strBuff462_Y[streamId][9], strBuff462_UV[streamId][9], SWOSD_WINDOW_ELEVEN);

		fontInfo.format 	    = SWOSD_FORMATYUV420p;
		fontInfo.charWidth		= SWOSD_CHARWIDTH_462_12;
		fontInfo.charHeight     = SWOSD_CHARHEIGHT_462_12;
		fontInfo.totFontWidth	= SWOSD_STRINGWIDTH_462_12;
		fontInfo.totFontHeight	= SWOSD_CHARHEIGHT_462_12;
		fontInfo.totFontLineoffset	= SWOSD_STRINGWIDTH_462_12;


#ifdef SWOSD_USER_TRANSPARANCY_ON
		fontInfo.fontAddr		= fontBuff462_Y;
		fontInfo.fontAddrUV 	= fontBuff462_UV;
		fontInfo.fontDB_Y		= fontDB462_Y;
		fontInfo.fontDB_UV		= fontDB462_UV;
#else
		fontInfo.fontAddr		= fontBuff462_Y1;
		fontInfo.fontAddrUV 	= fontBuff462_UV1;
		fontInfo.fontDB_Y		= fontDB462_Y1;
		fontInfo.fontDB_UV		= fontDB462_UV1;
#endif

		SWOSD_MakeOsdwinstring(&fontInfo, "DATE:0000/00/00", strBuff462_Y[streamId][5], &osdHndl->bmpWinPrm[SWOSD_WINDOW_EIGHT], strlen("DATE:0000/00/00"));
		strBuff462_UV[streamId][5]= (char*)(osdHndl->bmpWinPrm[SWOSD_WINDOW_EIGHT].bmpWinAddrUV );
		SWOSD_setBmpWinAddr(osdHndl, strBuff462_Y[streamId][5], strBuff462_UV[streamId][5], SWOSD_WINDOW_EIGHT);

		SWOSD_MakeOsdwinstring(&fontInfo, "TIME:00:00:00  ", strBuff462_Y[streamId][6], &osdHndl->bmpWinPrm[SWOSD_WINDOW_NINE], strlen("TIME:00:00:00  "));
		strBuff462_UV[streamId][6]= (char*)(osdHndl->bmpWinPrm[SWOSD_WINDOW_NINE].bmpWinAddrUV );
		SWOSD_setBmpWinAddr(osdHndl, strBuff462_Y[streamId][6], strBuff462_UV[streamId][6], SWOSD_WINDOW_NINE);
	}
	else if (gAVSERVER_config.encodeConfig[streamId].cropWidth<=VIDEO_RESOL_WIDTH_MEDIUM  )
	{
		fontInfo.format		= SWOSD_FORMATYUV420p;
		fontInfo.charWidth		= SWOSD_CHARWIDTH_770_20;
		fontInfo.charHeight	= SWOSD_CHARHEIGHT_770_20;
		fontInfo.totFontWidth	= SWOSD_STRINGWIDTH_770_20;
		fontInfo.totFontHeight	= SWOSD_CHARHEIGHT_770_20;
		fontInfo.totFontLineoffset	= SWOSD_STRINGWIDTH_770_20;
		fontInfo.fontAddr		= fontBuff770_Y1;
		fontInfo.fontAddrUV     = fontBuff770_UV1;
		fontInfo.fontDB_Y         =  fontDB770_Y1;
		fontInfo.fontDB_UV       = fontDB770_UV1;

		SWOSD_MakeOsdwinstring(&fontInfo, resolStr, strBuff770_Y[streamId][0], &osdHndl->bmpWinPrm[SWOSD_WINDOW_THREE], strlen(resolStr));
		strBuff770_UV[streamId][0]= (char*)(osdHndl->bmpWinPrm[SWOSD_WINDOW_THREE].bmpWinAddrUV );
		SWOSD_setBmpWinAddr(osdHndl, strBuff770_Y[streamId][0], strBuff770_UV[streamId][0], SWOSD_WINDOW_THREE);

		SWOSD_MakeOsdwinstring(&fontInfo, codecStr, strBuff770_Y[streamId][1], &osdHndl->bmpWinPrm[SWOSD_WINDOW_FOUR], strlen(codecStr));
		strBuff770_UV[streamId][1]= (char*)(osdHndl->bmpWinPrm[SWOSD_WINDOW_FOUR].bmpWinAddrUV );
		SWOSD_setBmpWinAddr(osdHndl, strBuff770_Y[streamId][1], strBuff770_UV[streamId][1], SWOSD_WINDOW_FOUR);

		SWOSD_MakeOsdwinstring(&fontInfo, framerateStr, strBuff770_Y[streamId][2], &osdHndl->bmpWinPrm[SWOSD_WINDOW_FIVE], strlen(framerateStr));
		strBuff770_UV[streamId][2]= (char*)(osdHndl->bmpWinPrm[SWOSD_WINDOW_FIVE].bmpWinAddrUV );
		SWOSD_setBmpWinAddr(osdHndl, strBuff770_Y[streamId][2], strBuff770_UV[streamId][2], SWOSD_WINDOW_FIVE);

		SWOSD_MakeOsdwinstring(&fontInfo, bitRateStr, strBuff770_Y[streamId][3], &osdHndl->bmpWinPrm[SWOSD_WINDOW_SIX], strlen(bitRateStr));
		strBuff770_UV[streamId][3]= (char*)(osdHndl->bmpWinPrm[SWOSD_WINDOW_SIX].bmpWinAddrUV );
		SWOSD_setBmpWinAddr(osdHndl, strBuff770_Y[streamId][3], strBuff770_UV[streamId][3], SWOSD_WINDOW_SIX);

		SWOSD_MakeOsdwinstring(&fontInfo, rateStr, strBuff770_Y[streamId][4], &osdHndl->bmpWinPrm[SWOSD_WINDOW_SEVEN], strlen(rateStr));
		strBuff770_UV[streamId][4]= (char*)(osdHndl->bmpWinPrm[SWOSD_WINDOW_SEVEN].bmpWinAddrUV );
		SWOSD_setBmpWinAddr(osdHndl, strBuff770_Y[streamId][4], strBuff770_UV[streamId][4], SWOSD_WINDOW_SEVEN);

		SWOSD_MakeOsdwinstring(&fontInfo, autoFocusStr, strBuff770_Y[streamId][8], &osdHndl->bmpWinPrm[SWOSD_WINDOW_TEN], strlen(autoFocusStr));
		strBuff770_UV[streamId][8]= (char*)(osdHndl->bmpWinPrm[SWOSD_WINDOW_TEN].bmpWinAddrUV );
		SWOSD_setBmpWinAddr(osdHndl, strBuff770_Y[streamId][8], strBuff770_UV[streamId][8], SWOSD_WINDOW_TEN);

		SWOSD_MakeOsdwinstring(&fontInfo, evalVerStr, strBuff770_Y[streamId][9], &osdHndl->bmpWinPrm[SWOSD_WINDOW_ELEVEN], strlen(evalVerStr));
		strBuff770_UV[streamId][9]= (char*)(osdHndl->bmpWinPrm[SWOSD_WINDOW_ELEVEN].bmpWinAddrUV );
		SWOSD_setBmpWinAddr(osdHndl, strBuff770_Y[streamId][9], strBuff770_UV[streamId][9], SWOSD_WINDOW_ELEVEN);

#ifdef SWOSD_USER_TRANSPARANCY_ON
		fontInfo.fontAddr		= fontBuff770_Y;
		fontInfo.fontAddrUV     = fontBuff770_UV;
		fontInfo.fontDB_Y         =  fontDB770_Y;
		fontInfo.fontDB_UV       = fontDB770_UV;
#else
		fontInfo.fontAddr		= fontBuff770_Y1;
		fontInfo.fontAddrUV     = fontBuff770_UV1;
		fontInfo.fontDB_Y         =  fontDB770_Y1;
		fontInfo.fontDB_UV       = fontDB770_UV1;
#endif
		SWOSD_MakeOsdwinstring(&fontInfo, "DATE:0000/00/00", strBuff770_Y[streamId][5], &osdHndl->bmpWinPrm[SWOSD_WINDOW_EIGHT], strlen("DATE:0000/00/00"));
		strBuff770_UV[streamId][5]= (char*)(osdHndl->bmpWinPrm[SWOSD_WINDOW_EIGHT].bmpWinAddrUV );
		SWOSD_setBmpWinAddr(osdHndl, strBuff770_Y[streamId][5], strBuff770_UV[streamId][5], SWOSD_WINDOW_EIGHT);

		SWOSD_MakeOsdwinstring(&fontInfo, "TIME:00:00:00  ", strBuff770_Y[streamId][6], &osdHndl->bmpWinPrm[SWOSD_WINDOW_NINE], strlen("TIME:00:00:00  "));
		strBuff770_UV[streamId][6]= (char*)(osdHndl->bmpWinPrm[SWOSD_WINDOW_NINE].bmpWinAddrUV );
		SWOSD_setBmpWinAddr(osdHndl, strBuff770_Y[streamId][6], strBuff770_UV[streamId][6], SWOSD_WINDOW_NINE);
	}
	else if (gAVSERVER_config.encodeConfig[streamId].cropWidth<VIDEO_RESOL_WIDTH_LARGE)
	{
		fontInfo.format		= SWOSD_FORMATYUV420p;
		fontInfo.charWidth		= SWOSD_CHARWIDTH_1078_28;
		fontInfo.charHeight	= SWOSD_CHARHEIGHT_1078_28;
		fontInfo.totFontWidth	= SWOSD_STRINGWIDTH_1078_28;
		fontInfo.totFontHeight	= SWOSD_CHARHEIGHT_1078_28;
		fontInfo.totFontLineoffset	= SWOSD_STRINGWIDTH_1078_28;
		fontInfo.fontAddr		= fontBuff1078_Y1;
		fontInfo.fontAddrUV     = fontBuff1078_UV1;
		fontInfo.fontDB_Y         =  fontDB1078_Y1;
		fontInfo.fontDB_UV       = fontDB1078_UV1;

		SWOSD_MakeOsdwinstring(&fontInfo, resolStr, strBuff1078_Y[streamId][0], &osdHndl->bmpWinPrm[SWOSD_WINDOW_THREE], strlen(resolStr));
		strBuff1078_UV[streamId][0]= (char*)(osdHndl->bmpWinPrm[SWOSD_WINDOW_THREE].bmpWinAddrUV );
		SWOSD_setBmpWinAddr(osdHndl, strBuff1078_Y[streamId][0], strBuff1078_UV[streamId][0], SWOSD_WINDOW_THREE);

		SWOSD_MakeOsdwinstring(&fontInfo, codecStr, strBuff1078_Y[streamId][1], &osdHndl->bmpWinPrm[SWOSD_WINDOW_FOUR], strlen(codecStr));
		strBuff1078_UV[streamId][1]= (char*)(osdHndl->bmpWinPrm[SWOSD_WINDOW_FOUR].bmpWinAddrUV );
		SWOSD_setBmpWinAddr(osdHndl, strBuff1078_Y[streamId][1], strBuff1078_UV[streamId][1], SWOSD_WINDOW_FOUR);

		SWOSD_MakeOsdwinstring(&fontInfo, framerateStr, strBuff1078_Y[streamId][2], &osdHndl->bmpWinPrm[SWOSD_WINDOW_FIVE], strlen(framerateStr));
		strBuff1078_UV[streamId][2]= (char*)(osdHndl->bmpWinPrm[SWOSD_WINDOW_FIVE].bmpWinAddrUV );
		SWOSD_setBmpWinAddr(osdHndl, strBuff1078_Y[streamId][2], strBuff1078_UV[streamId][2], SWOSD_WINDOW_FIVE);

		SWOSD_MakeOsdwinstring(&fontInfo, bitRateStr, strBuff1078_Y[streamId][3], &osdHndl->bmpWinPrm[SWOSD_WINDOW_SIX], strlen(bitRateStr));
		strBuff1078_UV[streamId][3]= (char*)(osdHndl->bmpWinPrm[SWOSD_WINDOW_SIX].bmpWinAddrUV );
		SWOSD_setBmpWinAddr(osdHndl, strBuff1078_Y[streamId][3], strBuff1078_UV[streamId][3], SWOSD_WINDOW_SIX);

		SWOSD_MakeOsdwinstring(&fontInfo, rateStr, strBuff1078_Y[streamId][4], &osdHndl->bmpWinPrm[SWOSD_WINDOW_SEVEN], strlen(rateStr));
		strBuff1078_UV[streamId][4]= (char*)(osdHndl->bmpWinPrm[SWOSD_WINDOW_SEVEN].bmpWinAddrUV );
		SWOSD_setBmpWinAddr(osdHndl, strBuff1078_Y[streamId][4], strBuff1078_UV[streamId][4], SWOSD_WINDOW_SEVEN);

		SWOSD_MakeOsdwinstring(&fontInfo, autoFocusStr, strBuff1078_Y[streamId][8], &osdHndl->bmpWinPrm[SWOSD_WINDOW_TEN], strlen(autoFocusStr));
		strBuff1078_UV[streamId][8]= (char*)(osdHndl->bmpWinPrm[SWOSD_WINDOW_TEN].bmpWinAddrUV );
		SWOSD_setBmpWinAddr(osdHndl, strBuff1078_Y[streamId][8], strBuff1078_UV[streamId][8], SWOSD_WINDOW_TEN);

		SWOSD_MakeOsdwinstring(&fontInfo, evalVerStr, strBuff1078_Y[streamId][9], &osdHndl->bmpWinPrm[SWOSD_WINDOW_ELEVEN], strlen(evalVerStr));
		strBuff1078_UV[streamId][9]= (char*)(osdHndl->bmpWinPrm[SWOSD_WINDOW_ELEVEN].bmpWinAddrUV );
		SWOSD_setBmpWinAddr(osdHndl, strBuff1078_Y[streamId][9], strBuff1078_UV[streamId][9], SWOSD_WINDOW_ELEVEN);

#ifdef SWOSD_USER_TRANSPARANCY_ON
		fontInfo.fontAddr		= fontBuff1078_Y;
		fontInfo.fontAddrUV     = fontBuff1078_UV;
		fontInfo.fontDB_Y         =  fontDB1078_Y;
		fontInfo.fontDB_UV       = fontDB1078_UV;
#else
		fontInfo.fontAddr		= fontBuff1078_Y1;
		fontInfo.fontAddrUV     = fontBuff1078_UV1;
		fontInfo.fontDB_Y         =  fontDB1078_Y1;
		fontInfo.fontDB_UV       = fontDB1078_UV1;
#endif
		SWOSD_MakeOsdwinstring(&fontInfo, "DATE:0000/00/00", strBuff1078_Y[streamId][5], &osdHndl->bmpWinPrm[SWOSD_WINDOW_EIGHT], strlen("DATE:0000/00/00"));
		strBuff1078_UV[streamId][5]= (char*)(osdHndl->bmpWinPrm[SWOSD_WINDOW_EIGHT].bmpWinAddrUV );
		SWOSD_setBmpWinAddr(osdHndl, strBuff1078_Y[streamId][5], strBuff1078_UV[streamId][5], SWOSD_WINDOW_EIGHT);

		SWOSD_MakeOsdwinstring(&fontInfo, "TIME:00:00:00  ", strBuff1078_Y[streamId][6], &osdHndl->bmpWinPrm[SWOSD_WINDOW_NINE], strlen("TIME:00:00:00  "));
		strBuff1078_UV[streamId][6]= (char*)(osdHndl->bmpWinPrm[SWOSD_WINDOW_NINE].bmpWinAddrUV );
		SWOSD_setBmpWinAddr(osdHndl, strBuff1078_Y[streamId][6], strBuff1078_UV[streamId][6], SWOSD_WINDOW_NINE);
	}
	else if (gAVSERVER_config.encodeConfig[streamId].cropWidth>=VIDEO_RESOL_WIDTH_LARGE)
	{
		fontInfo.format		= SWOSD_FORMATYUV420p;
		fontInfo.charWidth		= SWOSD_CHARWIDTH_1232_30;
		fontInfo.charHeight	= SWOSD_CHARHEIGHT_1232_30;
		fontInfo.totFontWidth	= SWOSD_STRINGWIDTH_1232_30;
		fontInfo.totFontHeight	= SWOSD_CHARHEIGHT_1232_30;
		fontInfo.totFontLineoffset	= SWOSD_STRINGWIDTH_1232_30;
		fontInfo.fontAddr		= fontBuff1232_Y1;
		fontInfo.fontAddrUV     = fontBuff1232_UV1;
		fontInfo.fontDB_Y         =  fontDB1232_Y1;
		fontInfo.fontDB_UV       = fontDB1232_UV1;

		SWOSD_MakeOsdwinstring(&fontInfo, resolStr, strBuff1232_Y[streamId][0], &osdHndl->bmpWinPrm[SWOSD_WINDOW_THREE], strlen(resolStr));
		strBuff1232_UV[streamId][0]= (char*)(osdHndl->bmpWinPrm[SWOSD_WINDOW_THREE].bmpWinAddrUV );
		SWOSD_setBmpWinAddr(osdHndl, strBuff1232_Y[streamId][0], strBuff1232_UV[streamId][0], SWOSD_WINDOW_THREE);

		SWOSD_MakeOsdwinstring(&fontInfo, codecStr, strBuff1232_Y[streamId][1], &osdHndl->bmpWinPrm[SWOSD_WINDOW_FOUR], strlen(codecStr));
		strBuff1232_UV[streamId][1]= (char*)(osdHndl->bmpWinPrm[SWOSD_WINDOW_FOUR].bmpWinAddrUV );
		SWOSD_setBmpWinAddr(osdHndl, strBuff1232_Y[streamId][1], strBuff1232_UV[streamId][1], SWOSD_WINDOW_FOUR);

		SWOSD_MakeOsdwinstring(&fontInfo, framerateStr, strBuff1232_Y[streamId][2], &osdHndl->bmpWinPrm[SWOSD_WINDOW_FIVE], strlen(framerateStr));
		strBuff1232_UV[streamId][2]= (char*)(osdHndl->bmpWinPrm[SWOSD_WINDOW_FIVE].bmpWinAddrUV );
		SWOSD_setBmpWinAddr(osdHndl, strBuff1232_Y[streamId][2], strBuff1232_UV[streamId][2], SWOSD_WINDOW_FIVE);

		SWOSD_MakeOsdwinstring(&fontInfo, bitRateStr, strBuff1232_Y[streamId][3], &osdHndl->bmpWinPrm[SWOSD_WINDOW_SIX], strlen(bitRateStr));
		strBuff1232_UV[streamId][3]= (char*)(osdHndl->bmpWinPrm[SWOSD_WINDOW_SIX].bmpWinAddrUV );
		SWOSD_setBmpWinAddr(osdHndl, strBuff1232_Y[streamId][3], strBuff1232_UV[streamId][3], SWOSD_WINDOW_SIX);

		SWOSD_MakeOsdwinstring(&fontInfo, rateStr, strBuff1232_Y[streamId][4], &osdHndl->bmpWinPrm[SWOSD_WINDOW_SEVEN], strlen(rateStr));
		strBuff1232_UV[streamId][4]= (char*)(osdHndl->bmpWinPrm[SWOSD_WINDOW_SEVEN].bmpWinAddrUV );
		SWOSD_setBmpWinAddr(osdHndl, strBuff1232_Y[streamId][4], strBuff1232_UV[streamId][4], SWOSD_WINDOW_SEVEN);

		SWOSD_MakeOsdwinstring(&fontInfo, autoFocusStr, strBuff1232_Y[streamId][8], &osdHndl->bmpWinPrm[SWOSD_WINDOW_TEN], strlen(autoFocusStr));
		strBuff1232_UV[streamId][8]= (char*)(osdHndl->bmpWinPrm[SWOSD_WINDOW_TEN].bmpWinAddrUV );
		SWOSD_setBmpWinAddr(osdHndl, strBuff1232_Y[streamId][8], strBuff1232_UV[streamId][8], SWOSD_WINDOW_TEN);

		SWOSD_MakeOsdwinstring(&fontInfo, evalVerStr, strBuff1232_Y[streamId][9], &osdHndl->bmpWinPrm[SWOSD_WINDOW_ELEVEN], strlen(evalVerStr));
		strBuff1232_UV[streamId][9]= (char*)(osdHndl->bmpWinPrm[SWOSD_WINDOW_ELEVEN].bmpWinAddrUV );
		SWOSD_setBmpWinAddr(osdHndl, strBuff1232_Y[streamId][9], strBuff1232_UV[streamId][9], SWOSD_WINDOW_ELEVEN);

#ifdef SWOSD_USER_TRANSPARANCY_ON
		fontInfo.fontAddr		= fontBuff1232_Y;
		fontInfo.fontAddrUV     = fontBuff1232_UV;
		fontInfo.fontDB_Y         =  fontDB1232_Y;
		fontInfo.fontDB_UV       = fontDB1232_UV;
#else
		fontInfo.fontAddr		= fontBuff1232_Y1;
		fontInfo.fontAddrUV     = fontBuff1232_UV1;
		fontInfo.fontDB_Y         =  fontDB1232_Y1;
		fontInfo.fontDB_UV       = fontDB1232_UV1;
#endif
		SWOSD_MakeOsdwinstring(&fontInfo, "DATE:0000/00/00", strBuff1232_Y[streamId][5], &osdHndl->bmpWinPrm[SWOSD_WINDOW_EIGHT], strlen("DATE:0000/00/00"));
		strBuff1232_UV[streamId][5]= (char*)(osdHndl->bmpWinPrm[SWOSD_WINDOW_EIGHT].bmpWinAddrUV );
		SWOSD_setBmpWinAddr(osdHndl, strBuff1232_Y[streamId][5], strBuff1232_UV[streamId][5], SWOSD_WINDOW_EIGHT);

		SWOSD_MakeOsdwinstring(&fontInfo, "TIME:00:00:00  ", strBuff1232_Y[streamId][6], &osdHndl->bmpWinPrm[SWOSD_WINDOW_NINE], strlen("TIME:00:00:00  "));
		strBuff1232_UV[streamId][6]= (char*)(osdHndl->bmpWinPrm[SWOSD_WINDOW_NINE].bmpWinAddrUV );
		SWOSD_setBmpWinAddr(osdHndl, strBuff1232_Y[streamId][6], strBuff1232_UV[streamId][6], SWOSD_WINDOW_NINE);
	}

	return OSA_SOK;
}

static int swosdDisplay(int streamId, SWOSD_Hndl *osdHndl )
{
	int i = 0;

	swosdMakeWinString(streamId);
	swosdChangeXYParmDetail(streamId, osdHndl);

	if(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdLogoEnable)
		SWOSD_setBmpWinEnable(osdHndl, SWOSD_WINDOW_ONE, SWOSD_BMPWINENABLE);//LOGO
	else
		SWOSD_setBmpWinEnable(osdHndl, SWOSD_WINDOW_ONE, SWOSD_BMPWINDISABLE);//LOGO

	if(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdTextEnable)
		SWOSD_setBmpWinEnable(osdHndl, SWOSD_WINDOW_TWO, SWOSD_BMPWINENABLE);//TEXT
	else
		SWOSD_setBmpWinEnable(osdHndl, SWOSD_WINDOW_TWO, SWOSD_BMPWINDISABLE);//TEXT

	if(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdType == SWOSD_DETAIL) {
		SWOSD_setBmpWinEnable(osdHndl, SWOSD_WINDOW_THREE, SWOSD_BMPWINENABLE);
		SWOSD_setBmpWinEnable(osdHndl, SWOSD_WINDOW_FOUR, SWOSD_BMPWINENABLE);
		SWOSD_setBmpWinEnable(osdHndl, SWOSD_WINDOW_FIVE, SWOSD_BMPWINENABLE);
		SWOSD_setBmpWinEnable(osdHndl, SWOSD_WINDOW_SIX, SWOSD_BMPWINENABLE);
		SWOSD_setBmpWinEnable(osdHndl, SWOSD_WINDOW_SEVEN, SWOSD_BMPWINENABLE);
		SWOSD_setBmpWinEnable(osdHndl, SWOSD_WINDOW_TEN, SWOSD_BMPWINENABLE);
	} else {
		SWOSD_setBmpWinEnable(osdHndl, SWOSD_WINDOW_THREE, SWOSD_BMPWINDISABLE);
		SWOSD_setBmpWinEnable(osdHndl, SWOSD_WINDOW_FOUR, SWOSD_BMPWINDISABLE);
		SWOSD_setBmpWinEnable(osdHndl, SWOSD_WINDOW_FIVE, SWOSD_BMPWINDISABLE);
		SWOSD_setBmpWinEnable(osdHndl, SWOSD_WINDOW_SIX, SWOSD_BMPWINDISABLE);
		SWOSD_setBmpWinEnable(osdHndl, SWOSD_WINDOW_SEVEN, SWOSD_BMPWINDISABLE);
		SWOSD_setBmpWinEnable(osdHndl, SWOSD_WINDOW_TEN, SWOSD_BMPWINDISABLE);
	}

	if(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdDateEnable){
		SWOSD_setBmpWinEnable(osdHndl, SWOSD_WINDOW_EIGHT, SWOSD_BMPWINENABLE);//DATE
	} else {
		SWOSD_setBmpWinEnable(osdHndl, SWOSD_WINDOW_EIGHT, SWOSD_BMPWINDISABLE);//DATE
	}

	if(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdTimeEnable){
		SWOSD_setBmpWinEnable(osdHndl, SWOSD_WINDOW_NINE, SWOSD_BMPWINENABLE);//TIME
	}
	else{
		SWOSD_setBmpWinEnable(osdHndl, SWOSD_WINDOW_NINE, SWOSD_BMPWINDISABLE);//TIME
	}

	if(OSD_checkEvalVersion(streamId)){
		SWOSD_setBmpWinEnable(osdHndl, SWOSD_WINDOW_ELEVEN, SWOSD_BMPWINENABLE);
	}
	else{
		SWOSD_setBmpWinEnable(osdHndl, SWOSD_WINDOW_ELEVEN, SWOSD_BMPWINDISABLE);
	}

#ifdef FR_OSD_DRAW
	for(i = 0;  i < SWOSD_WINDOW_FR_MAX; i++){  //ANR - FR
		if(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdFREnable[i]){
			SWOSD_setBmpWinEnable(osdHndl, SWOSD_WINDOW_FR_ST+i, SWOSD_BMPWINENABLE);
		} else {
			SWOSD_setBmpWinEnable(osdHndl, SWOSD_WINDOW_FR_ST+i, SWOSD_BMPWINDISABLE);
		}
	}
#endif

	return OSA_SOK;
}

#ifdef FR_OSD_DRAW
static int swosdSetFrUserString(ALG_SwosdRunPrm *prm, int streamId)
{   /*ANR */

	int status =0;
	int fr = 0, count = 0;
	int centerX, centerY, size;

	if(prm->faceInfo.frFaces){
		for(count=0; count<prm->faceInfo.numFaces; count++) {

			if(prm->faceInfo.frInfo[count].userID != 0 && fr < SWOSD_WINDOW_FR_MAX){

				centerX = ((prm->faceInfo.info[count].centerX) * prm->videoWidth ) / (prm->faceInfoInWidth);
				centerY = ((prm->faceInfo.info[count].centerY) * prm->videoHeight ) / (prm->faceInfoInHeight);
				size    = (CSL_FACE_DETECT_GET_SIZE(prm->faceInfo.info[count].sizeConfidence)*prm->videoWidth) / prm->faceInfoInWidth;

				Fr_UsrInfo[count].stX = (centerX - size/2);
				Fr_UsrInfo[count].stY = (centerY + size/2);

				if((Fr_UsrInfo[fr].stY + SWOSD_FR_USRSTR_HEIGHT) >= prm->videoHeight ||(Fr_UsrInfo[fr].stX + SWOSD_FR_USRSTR_WIDTH) >= prm->videoWidth ){
					gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdFREnable[fr] = FALSE;
					continue;
				}

				strcpy(Fr_UsrInfo[fr].usrStr, prm->faceInfo.frInfo[count].usrName);
				//strncpy(Fr_UsrInfo[fr].usrStr, prm->faceInfo.info[count].usrName,SWOSD_FR_USRSTR_LEN);
				Fr_UsrInfo[fr].usrID = prm->faceInfo.frInfo[count].userID;
				gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdFREnable[fr] = TRUE;
				fr++;
				//printf("\n Fr_UsrInfo[count].usrStr %s \n", Fr_UsrInfo[count].usrStr);

			}
		}
	}

	for ( ; fr < SWOSD_WINDOW_FR_MAX; fr ++)
		gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdFREnable[fr] = FALSE;

	Fr_UsrInfo[fr].usrID = 0;

	return status;

}
#endif

int VIDEO_swosdTskRun(int streamId, OSA_BufInfo *pBufInfo)
{
	VIDEO_CaptureStream *pCaptureStream;
	VIDEO_BufHeader *pBufHeader;
	VIDEO_BufHeader bufHeader;
	ALG_SwosdRunPrm swosdRunPrm;
	SWOSD_MainWinPrm mainWinPrm;
	int startX = 0;
	int startY = 0;
	int fdStreamId;
	static int osd_count[AVSERVER_MAX_STREAMS] = {0, 0, 0, 0};

	if(streamId<0 || streamId >= AVSERVER_MAX_STREAMS) {
		OSA_ERROR("Invalid streamId %d\n", streamId);
		return OSA_EFAIL;
	}

	pCaptureStream = &gVIDEO_ctrl.captureStream[streamId];
	pBufHeader = &bufHeader;

	OSA_assert((void*)pBufInfo->virtAddr !=NULL);

	memcpy((void*)pBufHeader,(void*)pBufInfo->virtAddr,sizeof(bufHeader));

	VIDEO_fdGetFaceStatus(&swosdRunPrm.faceInfo);

#ifdef AVSERVER_DEBUG_VIDEO_SWOSD
#ifdef AVSERVER_DEBUG_RUNNING
	OSA_printf(" SWOSD: Running on stream %d, num faces %d found\n", streamId, swosdRunPrm.faceInfo.numFaces);
#endif
#endif

	fdStreamId = gAVSERVER_config.faceDetectConfig.captureStreamId;

	swosdRunPrm.faceInfoInWidth  = gVIDEO_ctrl.captureStream[fdStreamId].captureOutWidth;
	swosdRunPrm.faceInfoInHeight = gVIDEO_ctrl.captureStream[fdStreamId].captureOutHeight;
	swosdRunPrm.videoWidth       = gVIDEO_ctrl.captureStream[streamId].captureOutWidth;
	swosdRunPrm.videoHeight      = gVIDEO_ctrl.captureStream[streamId].captureOutHeight;
	swosdRunPrm.videoOffsetH     = pBufHeader->offsetH;
	swosdRunPrm.videoOffsetV     = pBufHeader->offsetV;
	swosdRunPrm.videoDataFormat  = gAVSERVER_config.captureYuvFormat;
	swosdRunPrm.videoInOutAddr   = pBufInfo->virtAddr + VIDEO_BUF_HEADER_SIZE;

	swosdRunPrm.faceInfoROIx 	= gAVSERVER_config.faceDetectConfig.startX;
	swosdRunPrm.faceInfoROIy 	= gAVSERVER_config.faceDetectConfig.startY;
	swosdRunPrm.faceInfoROIw 	= gAVSERVER_config.faceDetectConfig.width;
	swosdRunPrm.faceInfoROIh 	= gAVSERVER_config.faceDetectConfig.height;
	swosdRunPrm.privacyMask 	= gAVSERVER_config.faceDetectConfig.privacyMaskEnable;
	swosdRunPrm.histEnable 		= gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdHistEnable;
	swosdRunPrm.fxdROIflag 		= gAVSERVER_config.fxdROIEnable;

	DRV_ipipeCpyHistData(swosdRunPrm.histData, &swosdRunPrm.histStatus);  //ANR-Hist
	ALG_swosdRun(pCaptureStream->algSwosdHndl, &swosdRunPrm);

#ifdef FR_OSD_DRAW
	swosdSetFrUserString(&swosdRunPrm, streamId);
#endif

	if(gAVSERVER_config.captureConfig[streamId].swosdConfig.swosdEnable)
	{
		if((pBufHeader->width<gAVSERVER_config.encodeConfig[streamId].cropWidth) ||
			(pBufHeader->height<gAVSERVER_config.encodeConfig[streamId].cropHeight))
			return OSA_SOK; /* skip this to avoid 720+720P same buffer case */

		startX = pBufHeader->startX + (pBufHeader->width  - gAVSERVER_config.encodeConfig[streamId].cropWidth)/2;
		startY = pBufHeader->startY + (pBufHeader->height - gAVSERVER_config.encodeConfig[streamId].cropHeight)/2;

		pBufHeader->startY 	= startY;
		pBufHeader->startX 	= startX;
		pBufHeader->height 	= gAVSERVER_config.encodeConfig[streamId].cropHeight;
		pBufHeader->width 	= gAVSERVER_config.encodeConfig[streamId].cropWidth;

		osdHndl = (SWOSD_Hndl *)gVIDEO_ctrl.captureStream[streamId].algSwosdHndl;

		mainWinPrm.format			= gAVSERVER_config.captureYuvFormat==DRV_DATA_FORMAT_YUV422?SWOSD_FORMATYUV422i:SWOSD_FORMATYUV420p;

		mainWinPrm.lineOffset		= pBufHeader->offsetH;
		mainWinPrm.width			= pBufHeader->width;
		mainWinPrm.height			= pBufHeader->offsetV - OSA_floor(startY, 2 )/2;
		/*OSD does not take care of Vertical offset, therefore care is needed to read UV data*/

	    OSA_prfBegin(&gAVSERVER_ctrl.swosdPrf[streamId]);

		SWOSD_setMainWinPrm(osdHndl, &mainWinPrm);

		if(osd_count[streamId]>=(gAVSERVER_config.encodeConfig[streamId].frameRateBase/1000)) {
			swosdDisplay(streamId, osdHndl);
			osd_count[streamId] = 0;
		}
		else
			osd_count[streamId]++;

		SWOSD_MsgRecvHandler();

		SWOSD_winDrawHandle(osdHndl, (short*)(pBufInfo->virtAddr+VIDEO_BUF_HEADER_SIZE + OSA_floor(startY, 2)  * pBufHeader->offsetH + OSA_floor(startX, 2)));

		OSA_prfEnd(&gAVSERVER_ctrl.swosdPrf[streamId], 1);
	}

	return OSA_SOK;
}

int VIDEO_swosdTskCreate(void)
{
	int i, j;
	int status = 0;
	ALG_SwosdCreate createPrm;

	createPrm.maxOverlays = 100;

	if(SWOSD_init()!=0)	/* Run SW OSD*/
	{
		OSA_ERROR(" SWOSD: SWOSD_init Failure \n");
		return OSA_EFAIL;
	}

	iconBuff160_Y  = (char *)OSD_CMEM_ALLOC((SWOSD_CHARWIDTHTILOGO_160_64*SWOSD_CHARHEIGHTTILOGO_160_64), 32) ;
	iconBuff160_UV  = (char *)OSD_CMEM_ALLOC(((SWOSD_CHARWIDTHTILOGO_160_64*SWOSD_CHARHEIGHTTILOGO_160_64)/2), 32);
	iconBuff80_Y  = (char *)OSD_CMEM_ALLOC((SWOSD_CHARWIDTHTILOGO_80_32*SWOSD_CHARHEIGHTTILOGO_80_32), 32) ;
	iconBuff80_UV  = (char *)OSD_CMEM_ALLOC(((SWOSD_CHARWIDTHTILOGO_80_32*SWOSD_CHARHEIGHTTILOGO_80_32)/2), 32);

	swosdSetLogoPrm();

	for(j=0; j<gAVSERVER_config.numCaptureStream; j++) {
		strBuff1540_Y[j] = (char*)OSD_CMEM_ALLOC((SWOSD_MAX_STRING_LENGTH*SWOSD_CHARWIDTH_1540_40*SWOSD_CHARHEIGHT_1540_40*3)/2,  32);
		for(i=0; i<MAX_STR_BUFF; i++)
		{
			strBuff1232_Y[j][i] = (char*)OSD_CMEM_ALLOC((SWOSD_MAX_STRING_LENGTH*SWOSD_CHARWIDTH_1232_30*SWOSD_CHARHEIGHT_1232_30*3)/2,  32);
			strBuff1078_Y[j][i] = (char*)OSD_CMEM_ALLOC((SWOSD_MAX_STRING_LENGTH*SWOSD_CHARWIDTH_1078_28*SWOSD_CHARHEIGHT_1078_28*3)/2,  32);
			strBuff770_Y[j][i] = (char*)OSD_CMEM_ALLOC((SWOSD_MAX_STRING_LENGTH*SWOSD_CHARWIDTH_770_20*SWOSD_CHARHEIGHT_770_20*3)/2,  32);
			strBuff462_Y[j][i] = (char*)OSD_CMEM_ALLOC((SWOSD_MAX_STRING_LENGTH*SWOSD_CHARWIDTH_462_12*SWOSD_CHARHEIGHT_462_12*3)/2,  32);
		}
	}

#ifdef SWOSD_USER_TRANSPARANCY_ON

	fontBuff1540_Y1= (char *)OSD_CMEM_ALLOC((SWOSD_STRINGWIDTH_1540_40*SWOSD_CHARHEIGHT_1540_40) , 32);
	memcpy(fontBuff1540_Y1, YData_1540_40_WFONT, SWOSD_STRINGWIDTH_1540_40*SWOSD_CHARHEIGHT_1540_40);
	fontBuff1540_UV1= (char *)OSD_CMEM_ALLOC((SWOSD_STRINGWIDTH_1540_40*(SWOSD_CHARHEIGHT_1540_40/2)) ,32);
	memcpy(fontBuff1540_UV1, UVData_1540_40_WFONT, SWOSD_STRINGWIDTH_1540_40*(SWOSD_CHARHEIGHT_1540_40/2));

	fontBuff1232_Y= (char *)OSD_CMEM_ALLOC((SWOSD_STRINGWIDTH_1232_30*SWOSD_CHARHEIGHT_1232_30) , 32);
	memcpy(fontBuff1232_Y, YData_1232_30, SWOSD_STRINGWIDTH_1232_30*SWOSD_CHARHEIGHT_1232_30);
	fontBuff1232_UV= (char *)OSD_CMEM_ALLOC((SWOSD_STRINGWIDTH_1232_30*(SWOSD_CHARHEIGHT_1232_30/2)) ,32);
	memcpy(fontBuff1232_UV, UVData_1232_30, SWOSD_STRINGWIDTH_1232_30*(SWOSD_CHARHEIGHT_1232_30/2));

	fontBuff1232_Y1= (char *)OSD_CMEM_ALLOC((SWOSD_STRINGWIDTH_1232_30*SWOSD_CHARHEIGHT_1232_30) , 32);
	memcpy(fontBuff1232_Y1, YData_1232_30_WFONT, SWOSD_STRINGWIDTH_1232_30*SWOSD_CHARHEIGHT_1232_30);
	fontBuff1232_UV1= (char *)OSD_CMEM_ALLOC((SWOSD_STRINGWIDTH_1232_30*(SWOSD_CHARHEIGHT_1232_30/2)) ,32);
	memcpy(fontBuff1232_UV1, UVData_1232_30_WFONT, SWOSD_STRINGWIDTH_1232_30*(SWOSD_CHARHEIGHT_1232_30/2));

	fontBuff1078_Y= (char *)OSD_CMEM_ALLOC((SWOSD_STRINGWIDTH_1078_28*SWOSD_CHARHEIGHT_1078_28) , 32);
	memcpy(fontBuff1078_Y, YData_1078_28, SWOSD_STRINGWIDTH_1078_28*SWOSD_CHARHEIGHT_1078_28);
	fontBuff1078_UV= (char *)OSD_CMEM_ALLOC((SWOSD_STRINGWIDTH_1078_28*(SWOSD_CHARHEIGHT_1078_28/2)) ,32);
	memcpy(fontBuff1078_UV, UVData_1078_28, SWOSD_STRINGWIDTH_1078_28*(SWOSD_CHARHEIGHT_1078_28/2));

	fontBuff1078_Y1= (char *)OSD_CMEM_ALLOC((SWOSD_STRINGWIDTH_1078_28*SWOSD_CHARHEIGHT_1078_28) , 32);
	memcpy(fontBuff1078_Y1, YData_1078_28_WFONT, SWOSD_STRINGWIDTH_1078_28*SWOSD_CHARHEIGHT_1078_28);
	fontBuff1078_UV1= (char *)OSD_CMEM_ALLOC((SWOSD_STRINGWIDTH_1078_28*(SWOSD_CHARHEIGHT_1078_28/2)) ,32);
	memcpy(fontBuff1078_UV1, UVData_1078_28_WFONT, SWOSD_STRINGWIDTH_1078_28*(SWOSD_CHARHEIGHT_1078_28/2));

	fontBuff770_Y= (char *)OSD_CMEM_ALLOC((SWOSD_STRINGWIDTH_770_20*SWOSD_CHARHEIGHT_770_20) , 32);
	memcpy(fontBuff770_Y, YData_770_20, SWOSD_STRINGWIDTH_770_20*SWOSD_CHARHEIGHT_770_20);
	fontBuff770_UV= (char *)OSD_CMEM_ALLOC((SWOSD_STRINGWIDTH_770_20*(SWOSD_CHARHEIGHT_770_20/2)) ,32);
	memcpy(fontBuff770_UV, UVData_770_20, SWOSD_STRINGWIDTH_770_20*(SWOSD_CHARHEIGHT_770_20/2));

	fontBuff770_Y1= (char *)OSD_CMEM_ALLOC((SWOSD_STRINGWIDTH_770_20*SWOSD_CHARHEIGHT_770_20) , 32);
	memcpy(fontBuff770_Y1, YData_770_20_WFONT, SWOSD_STRINGWIDTH_770_20*SWOSD_CHARHEIGHT_770_20);
	fontBuff770_UV1= (char *)OSD_CMEM_ALLOC((SWOSD_STRINGWIDTH_770_20*(SWOSD_CHARHEIGHT_770_20/2)) ,32);
	memcpy(fontBuff770_UV1, UVData_770_20_WFONT, SWOSD_STRINGWIDTH_770_20*(SWOSD_CHARHEIGHT_770_20/2));

	fontBuff462_Y= (char *)OSD_CMEM_ALLOC((SWOSD_STRINGWIDTH_462_12*SWOSD_CHARHEIGHT_462_12) , 32);
	memcpy(fontBuff462_Y, YData_462_12, SWOSD_STRINGWIDTH_462_12*SWOSD_CHARHEIGHT_462_12);
	fontBuff462_UV= (char *)OSD_CMEM_ALLOC((SWOSD_STRINGWIDTH_462_12*(SWOSD_CHARHEIGHT_462_12/2)) ,32);
	memcpy(fontBuff462_UV, UVData_462_12, SWOSD_STRINGWIDTH_462_12*(SWOSD_CHARHEIGHT_462_12/2));

	fontBuff462_Y1= (char *)OSD_CMEM_ALLOC((SWOSD_STRINGWIDTH_462_12*SWOSD_CHARHEIGHT_462_12) , 32);
	memcpy(fontBuff462_Y1, YData_462_12_WFONT, SWOSD_STRINGWIDTH_462_12*SWOSD_CHARHEIGHT_462_12);
	fontBuff462_UV1= (char *)OSD_CMEM_ALLOC((SWOSD_STRINGWIDTH_462_12*(SWOSD_CHARHEIGHT_462_12/2)) ,32);
	memcpy(fontBuff462_UV1, UVData_462_12_WFONT, SWOSD_STRINGWIDTH_462_12*(SWOSD_CHARHEIGHT_462_12/2));
#else

	fontBuff1540_Y1= (char *)OSD_CMEM_ALLOC((SWOSD_STRINGWIDTH_1540_40*SWOSD_CHARHEIGHT_1540_40) , 32);
	memcpy(fontBuff1540_Y1, YData_1540_40_WFONT_PURE, SWOSD_STRINGWIDTH_1540_40*SWOSD_CHARHEIGHT_1540_40);
	fontBuff1540_UV1= (char *)OSD_CMEM_ALLOC((SWOSD_STRINGWIDTH_1540_40*(SWOSD_CHARHEIGHT_1540_40/2)) ,32);
	memcpy(fontBuff1540_UV1, UVData_1540_40_WFONT_PURE, SWOSD_STRINGWIDTH_1540_40*(SWOSD_CHARHEIGHT_1540_40/2));

	fontBuff1232_Y1= (char *)OSD_CMEM_ALLOC((SWOSD_STRINGWIDTH_1232_30*SWOSD_CHARHEIGHT_1232_30) , 32);
	memcpy(fontBuff1232_Y1, YData_1232_30_WFONT_PURE, SWOSD_STRINGWIDTH_1232_30*SWOSD_CHARHEIGHT_1232_30);
	fontBuff1232_UV1= (char *)OSD_CMEM_ALLOC((SWOSD_STRINGWIDTH_1232_30*(SWOSD_CHARHEIGHT_1232_30/2)) ,32);
	memcpy(fontBuff1232_UV1, UVData_1232_30_WFONT_PURE, SWOSD_STRINGWIDTH_1232_30*(SWOSD_CHARHEIGHT_1232_30/2));

	fontBuff1078_Y1= (char *)OSD_CMEM_ALLOC((SWOSD_STRINGWIDTH_1078_28*SWOSD_CHARHEIGHT_1078_28) , 32);
	memcpy(fontBuff1078_Y1, YData_1078_28_WFONT_PURE, SWOSD_STRINGWIDTH_1078_28*SWOSD_CHARHEIGHT_1078_28);
	fontBuff1078_UV1= (char *)OSD_CMEM_ALLOC((SWOSD_STRINGWIDTH_1078_28*(SWOSD_CHARHEIGHT_1078_28/2)) ,32);
	memcpy(fontBuff1078_UV1, UVData_1078_28_WFONT_PURE, SWOSD_STRINGWIDTH_1078_28*(SWOSD_CHARHEIGHT_1078_28/2));

	fontBuff770_Y1= (char *)OSD_CMEM_ALLOC((SWOSD_STRINGWIDTH_770_20*SWOSD_CHARHEIGHT_770_20) , 32);
	memcpy(fontBuff770_Y1, YData_770_20_WFONT_PURE, SWOSD_STRINGWIDTH_770_20*SWOSD_CHARHEIGHT_770_20);
	fontBuff770_UV1= (char *)OSD_CMEM_ALLOC((SWOSD_STRINGWIDTH_770_20*(SWOSD_CHARHEIGHT_770_20/2)) ,32);
	memcpy(fontBuff770_UV1, UVData_770_20_WFONT_PURE, SWOSD_STRINGWIDTH_770_20*(SWOSD_CHARHEIGHT_770_20/2));

	fontBuff462_Y1= (char *)OSD_CMEM_ALLOC((SWOSD_STRINGWIDTH_462_12*SWOSD_CHARHEIGHT_462_12) , 32);
	memcpy(fontBuff462_Y1, YData_462_12_WFONT_PURE, SWOSD_STRINGWIDTH_462_12*SWOSD_CHARHEIGHT_462_12);
	fontBuff462_UV1= (char *)OSD_CMEM_ALLOC((SWOSD_STRINGWIDTH_462_12*(SWOSD_CHARHEIGHT_462_12/2)) ,32);
	memcpy(fontBuff462_UV1, UVData_462_12_WFONT_PURE, SWOSD_STRINGWIDTH_462_12*(SWOSD_CHARHEIGHT_462_12/2));
#endif

#ifdef FR_OSD_DRAW
	for(i=0; i<SWOSD_WINDOW_FR_MAX; i++)  /* ANR - FR */
	{
		strBuff1232_Y_FR[i] = (char*)OSD_CMEM_ALLOC((SWOSD_FR_USRSTR_LEN*SWOSD_CHARWIDTH_1232_30*SWOSD_CHARHEIGHT_1232_30*3)/2,  32);
	}
#endif

	swosdCreateFontDB();

	for(i=0; i<gAVSERVER_config.numCaptureStream; i++)
	{
		if(SWOSD_createHandle(&osdHndl, &idxHndl[i]) != SUCCESS)
		{
			OSA_ERROR("Handle Creation Failed in SWOSD(%d)", i);
			return OSA_EFAIL;
		}
		else
		{
			SWOSD_setNumWindows(osdHndl, SWOSD_WINMAX); //Setting for 32 BMP windows currently
			swosdSetWinParam(gAVSERVER_config.encodeConfig[i].cropHeight, gAVSERVER_config.encodeConfig[i].cropWidth);
			swosdMakeFixWinString(i);
		}

		gVIDEO_ctrl.captureStream[i].algSwosdHndl = osdHndl;

	}

	status = OSA_mbxCreate(&swOSDMbx);

	if(status != OSA_SOK) {
		OSA_ERROR("OSA_mbxCreate()\n");
		return OSA_EFAIL;
	}

	DM365MM_init();

	return status;
}

int VIDEO_swosdTskDelete(void)
{
	int i, j;
	int status = 0;

	for(i=0; i<gAVSERVER_config.numCaptureStream; i++)
	{
		ALG_swosdDelete(gVIDEO_ctrl.captureStream[i].algSwosdHndl);
	}

	if(iconBuff160_Y!=NULL)
	{
		OSA_cmemFree((Uint8*)iconBuff160_Y);
		iconBuff160_Y=NULL;
	}

	if(iconBuff160_UV != NULL)
	{
		OSA_cmemFree((Uint8*)iconBuff160_UV);
		iconBuff160_UV=NULL;
	}

	if(iconBuff80_Y!=NULL)
	{
		OSA_cmemFree((Uint8*)iconBuff80_Y);
		iconBuff80_Y=NULL;
	}

	if(iconBuff80_UV != NULL)
	{
		OSA_cmemFree((Uint8*)iconBuff80_UV);
		iconBuff80_UV=NULL;
	}

	for(j=0; j<gAVSERVER_config.numCaptureStream; j++) {
		if(strBuff1540_Y[j] != NULL)
		{
			OSA_cmemFree((Uint8*)strBuff1540_Y[j]);
			strBuff1540_Y[j] = NULL;
		}
		for(i = 0; i<MAX_STR_BUFF; i++)
		{
			if(strBuff1232_Y[j][i] != NULL)
			{
				OSA_cmemFree((Uint8*)strBuff1232_Y[j][i]);
				strBuff1232_Y[j][i] = NULL;
			}

			if(strBuff1078_Y[j][i] != NULL)
			{
				OSA_cmemFree((Uint8*)strBuff1078_Y[j][i]);
				strBuff1078_Y[j][i] = NULL;
			}

			if(strBuff770_Y[j][i] != NULL)
			{
				OSA_cmemFree((Uint8*)strBuff770_Y[j][i]);
				strBuff770_Y[j][i] = NULL;
			}

			if(strBuff462_Y[j][i] != NULL)
			{
				OSA_cmemFree((Uint8*)strBuff462_Y[j][i]);
				strBuff462_Y[j][i]= NULL;
			}
		}
	}

#ifdef SWOSD_USER_TRANSPARANCY_ON
	 if(fontBuff1232_Y!=NULL)
	 {
             OSA_cmemFree((Uint8*)fontBuff1232_Y);
	      fontBuff1232_Y= NULL;
	 }

	 if(fontBuff1232_UV!=NULL)
	 {
            OSA_cmemFree((Uint8*)fontBuff1232_UV);
	     fontBuff1232_UV= NULL;
	 }

     if(fontBuff1078_Y!=NULL)
	 {
            OSA_cmemFree((Uint8*)fontBuff1078_Y);
	     fontBuff1078_Y = NULL;
	 }

	 if(fontBuff1078_UV!=NULL)
	 {
              OSA_cmemFree((Uint8*)fontBuff1078_UV);
	       fontBuff1078_UV= NULL;
	 }

	 if(fontBuff770_Y!=NULL)
	 {
            OSA_cmemFree((Uint8*)fontBuff770_Y);
	     fontBuff770_Y = NULL;
	 }

	 if(fontBuff770_UV!=NULL)
	 {
            OSA_cmemFree((Uint8*)fontBuff770_UV);
	     fontBuff770_UV= NULL;
	 }

	 if(fontBuff462_Y!=NULL)
	 {
            OSA_cmemFree((Uint8*)fontBuff462_Y);
	     fontBuff462_Y = NULL;
	 }

	 if(fontBuff462_UV!=NULL)
	 {
            OSA_cmemFree((Uint8*)fontBuff462_UV);
	     fontBuff462_UV= NULL;
	 }

#endif

	if(fontBuff1540_Y1!=NULL)
	{
		OSA_cmemFree((Uint8*)fontBuff1540_Y1);
		fontBuff1540_Y1= NULL;
	}

	if(fontBuff1540_UV1!=NULL)
	{
		OSA_cmemFree((Uint8*)fontBuff1540_UV1);
		fontBuff1540_UV1= NULL;
	}

	if(fontBuff1232_Y1!=NULL)
	{
		OSA_cmemFree((Uint8*)fontBuff1232_Y1);
	  	fontBuff1232_Y1= NULL;
	}

	if(fontBuff1232_UV1!=NULL)
	{
		OSA_cmemFree((Uint8*)fontBuff1232_UV1);
	 	fontBuff1232_UV1= NULL;
	}

	if(fontBuff1078_Y1!=NULL)
	{
		OSA_cmemFree((Uint8*)fontBuff1078_Y1);
	 	fontBuff1078_Y1= NULL;
	}

	if(fontBuff1078_UV1!=NULL)
	{
		OSA_cmemFree((Uint8*)fontBuff1078_UV1);
	   	fontBuff1078_UV1= NULL;
	}

	if(fontBuff770_Y1!=NULL)
	{
		OSA_cmemFree((Uint8*)fontBuff770_Y1);
	 	fontBuff770_Y1= NULL;
	}

	if(fontBuff770_UV1!=NULL)
	{
		OSA_cmemFree((Uint8*)fontBuff770_UV1);
	 	fontBuff770_UV1= NULL;
	}

	if(fontBuff462_Y1!=NULL)
	{
		OSA_cmemFree((Uint8*)fontBuff462_Y1);
	 	fontBuff462_Y1= NULL;
	}

	if(fontBuff462_UV1!=NULL)
	{
		OSA_cmemFree((Uint8*)fontBuff462_UV1);
	 	fontBuff462_UV1= NULL;
	}


#ifdef FR_OSD_DRAW
	for(i = 0; i<SWOSD_WINDOW_FR_MAX; i++) //ANR FR
	{
		if(strBuff1232_Y_FR[i] != NULL)
		{
			OSA_cmemFree((Uint8*)strBuff1232_Y_FR[i]);
			strBuff1232_Y_FR[i] = NULL;
			strBuff1232_UV_FR[i] = NULL;
		}
	}
#endif

	DM365MM_exit();

	status = OSA_mbxDelete(&swOSDMbx);

	return status;
}

int VIDEO_swosdTskMain(struct OSA_TskHndl *pTsk, OSA_MsgHndl *pMsg, Uint32 curState )
{
	int status;
	videoSWOSDMessage * msg;
	Bool done=FALSE, ackMsg = FALSE;
	Uint16 cmd = OSA_msgGetCmd(pMsg);

#ifdef AVSERVER_DEBUG_VIDEO_SWOSD_THR
	OSA_printf(" SWOSD: Recevied CMD = 0x%04x\n", cmd);
#endif

	if(cmd!=AVSERVER_CMD_CREATE) {
		OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
		return OSA_SOK;
	}

	status = VIDEO_swosdTskCreate();

	OSA_tskAckOrFreeMsg(pMsg, status);

	if(status!=OSA_SOK) {
		OSA_ERROR("VIDEO_swosdTskCreate()\n");
		return status;
	}

#ifdef AVSERVER_DEBUG_VIDEO_SWOSD_THR
  	OSA_printf(" SWOSD: Create...DONE\n");
#endif

    while(!done) {

		status = OSA_tskWaitMsg(pTsk, &pMsg);
		if(status!=OSA_SOK)
			continue;

		cmd = OSA_msgGetCmd(pMsg);

		switch(cmd) {
			case AVSERVER_CMD_DELETE:
				done = TRUE;
				ackMsg = TRUE;
			break;

			case AVSERVER_CMD_NEW_DATA:
				msg = (videoSWOSDMessage *)OSA_msgGetPrm(pMsg);
				status = VIDEO_swosdTskRun(msg->streamId,msg->pBufInfo);
				OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
			break;

      		default:

				#ifdef AVSERVER_DEBUG_VIDEO_SWOSD_THR
				OSA_printf(" SWOSD: Unknown CMD = 0x%04x\n", cmd);
				#endif

				OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
        	break;
		}
	}

#ifdef AVSERVER_DEBUG_VIDEO_SWOSD_THR
  	OSA_printf(" SWOSD: Delete...\n");
#endif

  	VIDEO_swosdTskDelete();

	if(ackMsg)
		OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);

#ifdef AVSERVER_DEBUG_VIDEO_SWOSD_THR
  OSA_printf(" SWOSD: Delete...DONE\n");
#endif

	return OSA_SOK;
}

int VIDEO_swosdCreate(void)
{
  int status;

  status = OSA_tskCreate( &gVIDEO_ctrl.swosdTsk, VIDEO_swosdTskMain, VIDEO_SWOSD_THR_PRI, VIDEO_SWOSD_STACK_SIZE, 0);
  if(status!=OSA_SOK) {
    OSA_ERROR("OSA_tskCreate()\n");
    return status;
  }

  return status;
}

int VIDEO_swosdRun(int tskId, int streamId, OSA_BufInfo *pBufInfo)
{
	videoSWOSDMessage msg;

	if(gVIDEO_ctrl.captureStream[streamId].swosdRunTsk != tskId)
		return OSA_SOK;

	msg.streamId = streamId;
	msg.pBufInfo = pBufInfo;
	OSA_tskSendMsg(&gVIDEO_ctrl.swosdTsk, (OSA_TskHndl *)&swOSDMbx, AVSERVER_CMD_NEW_DATA, (void*)&msg, OSA_MBX_WAIT_ACK);

	return OSA_SOK;
}

int VIDEO_swosdDelete(void)
{
  int status;

  status = OSA_tskDelete( &gVIDEO_ctrl.swosdTsk);

  return status;
}

