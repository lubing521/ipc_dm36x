

#ifndef _SWOSD_OSD_H_
#define _SWOSD_OSD_H_

#define OSA_align(value, align)   ((( (value) + ( (align) - 1 ) ) / (align) ) * (align) )

#define SWOSD_SOK	 (0)
#define SWOSD_EFAIL	 (-1)

#define SWOSD_WINMAX     		(24)
#define SWOSD_MAXNUMHNDL 		(3)

/* OSD return Values */
#define SWOSD_NULLHANDLE		(-2)
#define SWOSD_REACHEDMAX		(-3)
#define SWOSD_NOTENABLED		(-4)
#define SWOSD_INVALIDPARAM		(-5)

#define SWOSD_FORMATYUV422i		(0)
#define SWOSD_FORMATYUV420p		(1)
#define SWOSD_RRETSUCCESS		(0)
#define SWOSD_RRETFAIL			(-1)
#define SWOSD_RBOOLFALSE		-1
#define SWOSD_RBOOLTRUE		 	0

#ifndef SWOSD_NULL
#define SWOSD_NULL		        0
#endif

#define SWOSD_SETBMPWINENABLE		0x0001
#define SWOSD_SETBMPMAINWINPRM 		0x0002
#define SWOSD_SETBMPWINCHGTRANS		0x0003
#define SWOSD_SETBMPWINADDR			0x0004
#define SWOSD_SETBMPWINXY			0x0005
#define SWOSD_SETMAINWINDOW			0x0006
#define SWOSD_EXIT					0x0007
#define SWOSD_SETBMPTRANSPARENCY	0x0008

#define SWOSD_VID_MSG_KEY			0x0F864426
#define SWOSD_VID_MSG_KEY1	        0x0F864428

#define SWOSD_BMPWINENABLE			1
#define SWOSD_BMPWINDISABLE			0
#define SWOSD_BMPTRANSENABLE		1
#define SWOSD_BMPTRANSDISABLE		0

//#define SWOSD_DEBUGPRINT
#undef  SWOSD_DEBUGPRINT

typedef struct {

   int enable;
   int format;
   int startX;
   int startY;
   int width;
   int height;
   int lineOffset;
   int transperencyVal;
   int transperencyRange;
   int transperencyEnable;
   int resizeEnable;
   int userTransparancy;
   char *bmpWinAddr;
   char *bmpWinAddrUV;

} SWOSD_BmpWinPrm;

typedef struct {

   int format;
   int width;
   int height;
   int lineOffset;
} SWOSD_MainWinPrm;

typedef struct {

   char format;
   int  charWidth;
   int  charHeight;
   int  totFontWidth;
   int  totFontHeight;
   int  totFontLineoffset;
   char *fontAddr;
   char *fontAddrUV;
   int * fontDB_Y;
   int * fontDB_UV;
} SWOSD_Fontdata;

typedef struct {

   SWOSD_MainWinPrm  mainWinPrm;
   SWOSD_BmpWinPrm  bmpWinPrm[SWOSD_WINMAX];
   SWOSD_Fontdata   *fontInfoprm;
   int numBmpWin;
   int activate;
}SWOSD_Hndl;


typedef struct {

	int msgType;
	SWOSD_Hndl *osdHandle;
	char *winAddr;
    char *winAddrUV;
	int winEnable;
	int winID;
	int format;
   	int width;
   	int height;
   	int lineOffset;
	int X;
	int Y;
	int transVal;
	int transRange;
	int transEnable;
}SWOSD_Msg;


/*
     This API Initialize the parameters required to OSD Window
*/
int SWOSD_init();

/*
      This API sets the Number of windows
*/
int SWOSD_setNumWindows(SWOSD_Hndl *Hndl, int numWindow);
/*
      This API sets the Main Window Structure
*/
int SWOSD_setMainWinPrm(SWOSD_Hndl *Hndl, SWOSD_MainWinPrm *prm);

/*
      This API sets the Main Window Structure
*/
int SWOSD_setMainWindowPrm(SWOSD_Hndl *Hndl, SWOSD_MainWinPrm *prm);

/*
     This API Initialize the bitmap window structure based on bitmap window Id.
*/
int SWOSD_setBmpWinPrm(SWOSD_Hndl *Hndl, int bmpWinId, SWOSD_BmpWinPrm *prm);
/*
      This API Eanble the window to draw
*/
int SWOSD_setBmpWinEnable(SWOSD_Hndl *Hndl, int bmpWinId, int enable);

/*
      This API changes the window parameter such as startX and startY
*/
int SWOSD_setBmpchangeWinXYPrm(SWOSD_Hndl *Hndl, int bmpWinId, int startX, int startY);

/*
      This API will change the OSD window Transperency Value and Transperancy Range
*/
int SWOSD_winChangeTransperency(SWOSD_Hndl *Hndl, int bmpWinId, int bmpTransValue, int bmpTransRange);

/*
      This API Sets the transperency
*/
int SWOSD_setBmpTransperancy(SWOSD_Hndl *Hndl, int Transparency, int WinId);

/*
     This API will update the bmp window address
*/
int SWOSD_setBmpWinAddr(SWOSD_Hndl *Hndl, char *WinAddr, char *WinAddrUV, int WinId);

/*
     This will update main window parameters
*/
int SWOSD_setPrivMainWinPrm(SWOSD_Hndl *Hndl, int format, int width, int hieght, int offset);

/*
     This API will draw the bmp window
*/
int SWOSD_winDraw(SWOSD_Hndl *Hndl, short* mainWinAddr, char *bmpWinAddr, char *bmpAddrUV, int bmpWinId);

/*
     This API converts string to YUV data
*/
int SWOSD_MakeOsdwinstring(SWOSD_Fontdata* fontInfo, char *pInputstr, void *pBuff, SWOSD_BmpWinPrm *pWinPrm, int stringWidth);

/*
     This API Recieves the OSD related message
*/
int SWOSD_MsgRecvHandler();

/*
    This API Uninitialize the parameters during Initialization
*/
int SWOSD_exit();

/* SWOSD private API's*/
int SWOSD_setPrivBmpWinEnable(SWOSD_Hndl *Hndl, int WinId, int enable);

int SWOSD_setPrivBmpchangeWinXYPrm(SWOSD_Hndl *Hndl, int bmpWinId, int startX, int startY);

int SWOSD_setPrivwinChangeTransperency(SWOSD_Hndl *Hndl, int bmpWinId, int bmpTransValue, int bmpTransRange);

int SWOSD_setPrivBmpWinAddr(SWOSD_Hndl *Hndl, char *bmpWinAddr, char *bmpWinAddrUV, int bmpWinId);

SWOSD_Hndl * SWOSD_getOSDHndls(int * numHndls);

int SWOSD_createHandle(SWOSD_Hndl ** osdHndl, int * indexHndl);

int SWOSD_deleteHandle(int indexHndl);

int SWOSD_winDrawHandle(SWOSD_Hndl *Hndl, short* mainWinAddr);

int SWOSD_createFontDatabase(int* fontDatabaseY, int* fontDatabaseUV, char * swosd_stringPattern, SWOSD_Fontdata * fontInfo);


#endif /* _SWOSD_OSD_H_ */
