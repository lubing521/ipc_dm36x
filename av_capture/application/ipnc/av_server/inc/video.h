
#ifndef _VIDEO_H_
#define _VIDEO_H_

#include <osa_tsk.h>
#include <osa_buf.h>
#include <osa_mutex.h>
#include <osa_flg.h>
#include <drv_capture.h>
#include <drv_display.h>
#include <drv_faceDetect.h>
#include <drv_frameCopy.h>
#include <drv_ldc.h>
#include <drv_resz.h>
#include <drv_imageTune.h>
#include <alg_aewb.h>
#include <alg_vstab.h>
#include <alg_vnf.h>
#include <alg_vidEnc.h>
#include <alg_jpgEnc.h>
#include <alg_motionDetect.h>
#include <alg_crypto.h>
#include <alg_swosd.h>
#include <alg_face.h>
#include <alg_ti_saldre.h>

#define VIDEO_NUM_BUF           (3)

#define VIDEO_TSK_NONE          (0x0000)
#define VIDEO_TSK_CAPTURE       (0x0001)
#define VIDEO_TSK_LDC           (0x0002)
#define VIDEO_TSK_VNF           (0x0004)
#define VIDEO_TSK_RESIZE        (0x0008)
#define VIDEO_TSK_ENCODE        (0x0010)
#define VIDEO_TSK_ENCRYPT       (0x0020)
#define VIDEO_TSK_STREAM        (0x0040)

#define VIDEO_BUF_HEADER_SIZE   (128)

#define VIDEO_ENC_FRAME_TYPE_NON_KEY_FRAME    (0)
#define VIDEO_ENC_FRAME_TYPE_KEY_FRAME        (1)

#define VIDEO_VS_FLAG_NEW_DATA     (0x0FFF)
#define VIDEO_VS_FLAG_EXIT         (0x8000)
#define VIDEO_VS_NUM_STATUS        (100)

#define VIDEO_STREAM_FILE_SAVE_STATE_IDLE   (0)
#define VIDEO_STREAM_FILE_SAVE_STATE_OPEN   (1)
#define VIDEO_STREAM_FILE_SAVE_STATE_WRITE  (2)
#define VIDEO_STREAM_FILE_SAVE_STATE_CLOSE  (3)

typedef struct {

  Uint32 timestamp;
  Uint32 temporalId;
  Uint32 frameNum;
  Uint32 startX;
  Uint32 startY;
  Uint32 width;
  Uint32 height;
  Uint32 offsetH;
  Uint32 offsetV;
  Uint32 encFrameSize;
  Uint32 encFrameType;

} VIDEO_BufHeader;

typedef struct {

  Uint32 frameNum;
  Uint16 startX;
  Uint16 startY;

} VIDEO_VsStatus;

typedef struct {

  int captureOutWidth;
  int captureOutHeight;
  int captureOutOffsetH;
  int captureOutOffsetV;

  int captureCount;

  int captureNextTsk;
  int ldcNextTsk;
  int vnfNextTsk;
  int resizeNextTsk;

  int displayCopyTsk;
  int faceDetectCopyTsk;
  int swosdRunTsk;
  int vsApplyTsk;

  OSA_BufHndl bufLdcIn;
  OSA_BufHndl bufVnfIn;
  OSA_BufHndl bufResizeIn;
  OSA_BufHndl bufEncodeIn;

  OSA_BufCreate bufLdcInCreatePrm;
  OSA_BufCreate bufVnfInCreatePrm;
  OSA_BufCreate bufResizeInCreatePrm;
  OSA_BufCreate bufEncodeInCreatePrm;

  void *algVnfHndl;
  void *algSwosdHndl;

} VIDEO_CaptureStream;

typedef struct {

  OSA_BufHndl bufDisplayIn;

  OSA_BufCreate bufDisplayInCreatePrm;

  DRV_DisplayWinHndl displayHndl;
  DRV_DisplayBufInfo displayInfo;

} VIDEO_DisplayStream;

typedef struct {

  OSA_BufHndl 	bufMotionIn;
  OSA_BufCreate bufMotionInCreatePrm;
  int maxbufsize;
  void *algMotionHndl;
	int motionBlock;
	int motionEnable;
	int motionCEnable;
	int motionLevel;
	int motionCValue;
} VIDEO_MotionStream;


typedef struct {

  int fdWidth;
  int fdHeight;
  int fdOffsetH;

  OSA_BufHndl bufFdIn;

  OSA_BufCreate bufFdInCreatePrm;

  OSA_MutexHndl statusLock;

  DRV_FaceDetectRunStatus faceStatus;

  Bool fdError;

} VIDEO_FaceDetectStream;


typedef struct {

  int encodeNextTsk;

  OSA_BufHndl bufEncryptIn;
  OSA_BufHndl bufStreamIn;

  OSA_BufCreate bufEncryptInCreatePrm;
  OSA_BufCreate bufStreamInCreatePrm;

  void *algEncHndl;
  void *algEncryptHndl;

  int fileSaveState;
  int fileSaveIndex;
  FILE *fileSaveHndl;
  FILE *fileFaceInfoHndl;

  int curFrameCount;
//next 2 line add by zeng
  int 	newRcType;
  int	newRcUpdate;

  int 	newBitrate;
  int 	newFps;
  int	newMotionUpdate;
  int	newMotionStatus;

} VIDEO_EncodeStream;

typedef struct {

  void           *algVsHndl;
  ALG_VstabCreate vsCreatePrm;
  VIDEO_VsStatus  vsStatus[VIDEO_VS_NUM_STATUS];
  OSA_MutexHndl   vsLock;
  OSA_FlgHndl     vsFlag;
  Uint32          curFrame;

} VIDEO_VsStream;

typedef struct {

  void           *algAewbHndl;
  OSA_MutexHndl  aewbLock;
  ALG_AewbStatus aewbStatus;

} VIDEO_AewbStream;

typedef struct {

  OSA_TskHndl captureTsk;
  OSA_TskHndl ldcTsk;
  OSA_TskHndl vnfTsk;
  OSA_TskHndl resizeTsk;
  OSA_TskHndl encodeTsk;
  OSA_TskHndl encryptTsk;
  OSA_TskHndl streamTsk;
  OSA_TskHndl fdTsk;
  OSA_TskHndl displayTsk;
  OSA_TskHndl vsTsk;
  OSA_TskHndl aewbTsk;
  OSA_TskHndl motionTsk;
  OSA_TskHndl swosdTsk;

  DRV_CaptureInfo captureInfo;

  int rawCaptureCount;
  Bool alawEnable;

  VIDEO_CaptureStream    captureStream[AVSERVER_MAX_STREAMS];
  VIDEO_EncodeStream     encodeStream[AVSERVER_MAX_STREAMS];
  VIDEO_DisplayStream    displayStream;
  VIDEO_FaceDetectStream faceDetectStream;
  VIDEO_VsStream         vsStream;
  VIDEO_AewbStream       aewbStream;
  VIDEO_MotionStream	 motionStream;

} VIDEO_Ctrl;

int VIDEO_aewbApplyPrm();

int VIDEO_fdGetFaceStatus(DRV_FaceDetectRunStatus *faceStatus);
int VIDEO_fdSetFaceType(DRV_FaceDetectRunType *faceType);

int VIDEO_fdCopyRun(int tskId, int streamId, OSA_BufInfo *pBufInfo);
int VIDEO_swosdRun(int tskId, int streamId, OSA_BufInfo *pBufInfo);
int VIDEO_displayCopyRun(int tskId, int streamId, OSA_BufInfo *pBufInfo);
int VIDEO_motionCopyRun( OSA_BufInfo *pBufInfo );

int VIDEO_vsGetResult(int tskId, int streamId, OSA_BufInfo *pInBufInfo);

int VIDEO_saldreSetBufInfo(int rawBufId, Uint8* inputAddr);
Uint8* VIDEO_saldreGetOutputBufAddr(void);
int VIDEO_saldreIsifPutOutBuf(void);

int VIDEO_streamShmCopy(int streamId, OSA_BufInfo *pBufInfo);

int VIDEO_streamGetJPGSerialNum(void);
int VIDEO_streamGetMediaInfo(void);

int VIDEO_captureMemFree();

int VIDEO_captureCreate();
int VIDEO_resizeCreate();
int VIDEO_ldcCreate();
int VIDEO_vnfCreate();
int VIDEO_vsCreate();
int VIDEO_aewbCreate();
int VIDEO_saldreCreate();
int VIDEO_fdCreate();
int VIDEO_displayCreate();
int VIDEO_encodeCreate();
int VIDEO_swosdCreate();
int VIDEO_motionCreate();
int VIDEO_encryptCreate();
int VIDEO_streamCreate();
int VIDEO_streamSysInit();

int VIDEO_saldreStop();

int VIDEO_captureDelete();
int VIDEO_resizeDelete();
int VIDEO_ldcDelete();
int VIDEO_vnfDelete();
int VIDEO_vsDelete();
int VIDEO_aewbDelete();
int VIDEO_saldreDelete();
int VIDEO_fdDelete();
int VIDEO_displayDelete();
int VIDEO_encodeDelete();
int VIDEO_swosdDelete();
int VIDEO_motionDelete();
int VIDEO_encryptDelete();
int VIDEO_streamDelete();
int VIDEO_streamSysExit();

#endif  /*  _VIDEO_H_ */

