
#ifndef _DRV_DISPLAY_H_
#define _DRV_DISPLAY_H_

#include <drv.h>


#define DRV_DISPLAY_BUF_MAX     8

#define DRV_DISPLAY_MODE_NTSC   0
#define DRV_DISPLAY_MODE_PAL    1
#define DRV_DISPLAY_MODE_720P   2
#define DRV_DISPLAY_MODE_1080I  3

#define DRV_DISPLAY_VID_WIN_0  0

typedef struct {

  void *hndl;

} DRV_DisplayWinHndl;

typedef struct {

  Uint16 winId;
  Uint16 numBuf;
  Uint16 dataFormat;
  Uint16 startX;
  Uint16 startY;
  Uint16 width;
  Uint16 height;
  Uint16 offsetH;
  Uint16 offsetV;
  Uint16 zoomH;
  Uint16 zoomV;
  Uint16 expandH;
  Uint16 expandV;

  Uint16 cropWidth;
  Uint16 cropHeight;
  Uint16 bufFlag;

} DRV_DisplayWinConfig;

typedef struct {

  Uint16 numBuf;
  Uint32 bufSize;
  Uint16 dataFormat;
  Uint16 width;
  Uint16 height;
  Uint16 offsetH;
  Uint16 offsetV;
  Uint8  *virtAddr[DRV_DISPLAY_BUF_MAX];
  Uint8  *physAddr[DRV_DISPLAY_BUF_MAX];

} DRV_DisplayBufInfo;

int DRV_displaySetMode(int mode);

int DRV_displayOpen(DRV_DisplayWinHndl *hndl, DRV_DisplayWinConfig *config);

int DRV_displayClose(DRV_DisplayWinHndl *hndl);

int DRV_displayGetBufInfo(DRV_DisplayWinHndl *hndl, DRV_DisplayBufInfo *info);

int DRV_displayExpand(DRV_DisplayWinHndl *hndl, Uint16 expandH, Uint16 expandV);
int DRV_displayExpandEx(DRV_DisplayWinHndl *hndl,DRV_DisplayWinConfig *config);

int DRV_displayEnable(DRV_DisplayWinHndl *hndl, Bool enable);

int DRV_displaySetWinZoom(DRV_DisplayWinHndl *hndl, Uint16 zoomH, Uint16 zoomV);
int DRV_displaySetWinPos(DRV_DisplayWinHndl *hndl, Uint16 startX, Uint16 startY);
int DRV_displaySetWinSize(DRV_DisplayWinHndl *hndl, Uint16 width, Uint16 height);
int DRV_displaySwitchBuf(DRV_DisplayWinHndl *hndl, int *bufId, int bufFlag, void *bufAddr);
int DRV_displaySetFieldModeEnable(DRV_DisplayWinHndl *hndl, Bool enable);

#endif

