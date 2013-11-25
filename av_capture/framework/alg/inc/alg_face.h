/*
    DM365 Evaluation Software

    (c)Texas Instruments 2009

    Note:  Dont change any values /Modified values will get effect only in the
              new library (alg_fr_ti.a) built after the changes.
*/

#ifndef _ALG_FACE_H_
#define _ALG_FACE_H_

#include <alg.h>
#include <drv_faceDetect.h>

//#define FACIAL_FEATURE  /* for Facial feature */
//#define FF_HISTO_EQ         /* Histograme Eqilaisation */

#define HIST_IMG_W          320
#define HIST_IMG_H          240

#define FF_DETECTION        0x00
#define FF_FACERECOG        0x01
#define FR_REG_USER         0x02
#define FR_IDENTIFY         0x04
#define FR_VERIFY           0x08
#define FR_DEL_USER         0x10

#define FR_MAX_USER_NO      16     	/* Max no user */
#define FR_MAX_UDATA_NO     2      	/* Max no of data for a single user */
#define FR_MAX_IDENTY       2      	/* Max identified face for a user / similar face */

#define FR_ALBUM_SIZE       ((FR_MAX_USER_NO * 404)+ 32) //404-1face,32-segbyte

/* YUV420 image*/
#define USER_IMGW     		320 	//300         /* Reg. Image Width  */
#define USER_IMGH     		240 	//192         /* Reg. Image Height */

#define FR_IMAGE_SIZE       (USER_IMGW * USER_IMGH)

#define FR_CHECK_CONF       500     /* Face recognition identification confidence rate */
#define FR_REG_CONF         500     /* Face recognition registering a user confidence rate */

/* Error codes for face recognition */

#define FF_SUCCESS           0
#define FF_FAILURE          -1
#define FF_INITIALIZE       -2
#define FF_INVALIDPARAM     -3
#define FF_ALLOCMEMORY	    -4
#define FF_ERR_OTHERS       -9

/* Register user datas */

#define FR_REG_ALBUM   /* enable - Upate the existing album from FR_ALBUM_PATH */
#define FR_REG_USERS   /* enable - Register user(s) from FR_USER_FACE_PATH (YUV420 image)*/

#define FR_ALBUM_NAND_PATH      "/mnt/nand/FaceRecogAlbum.bin"     	/* Album path */
#define FR_ALBUM_MMC_SD_PATH    "/mnt/mmc/FaceRecogAlbum.bin"      	/* Album path */
#define FR_USER_FACE_PATH       "/mnt/mmc/ipnc/Face"               	/* Face path */
#define REGUSER_DISP_COUNT 		20 									//Revisit


/* for printing FR err/msg - enable */
//#define FR_PRINT_ERR   /* For Error Messages */
#define FR_PRINT_DBG   /* Infor messages - Regd user ID, Identified user ID etc */
//#define FR_PRINT_DBG2  /* Other Prints */

#define FR_MSG_PRINT   printf //OSA_printf

/* for not printing FR err/msg - enable */
//#define FR_MSG_PRINT
//#define FR_ERR_PRINT

/* Do not change or it may required to sync with drv_facedetect or csl_facedetect,
   pls check the comments before change*/

/* Needs to sync with CSL */
#define ALG_FACE_DETECT_MAX_FACES 35  //CSL_FACE_DETECT_MAX_FACES

#define ALG_FACE_STRING_SIZE      16

/* Needs to sync with face detect value */
/* ALG_FACE_DETECT_CONDITION_DEFAULT (CSL_FACE_DETECT_DIR_UP|CSL_FACE_DETECT_FACE_SIZE_20) */
#define ALG_FACE_DETECT_CONDITION_DEFAULT (0 | 0)


/* Sync with DRV_FaceDetectRunPrm */
typedef struct {

  unsigned char *inPhysAddr;
  unsigned char *inVirtAddr;
  unsigned short inWidth;
  unsigned short inHeight;
  unsigned int detectThres;    // 0..15
  unsigned int detectCondition;

}ALG_FaceDetectRunPrm;


/* Sync with DRV_FaceRecogStatus */
typedef struct {

  int userID;
  char usrName[ALG_FACE_STRING_SIZE];  //Revisit
} ALG_FaceRecogStatus;


/* Sync with CSL_FaceDetectFaceStatus */
typedef struct {

  unsigned int  centerX;			  ///< Centre X
  unsigned int  centerY;			  ///< Centre Y
  unsigned int  sizeConfidence;	  ///< Confidence/Size of the result, confidence = 0: high confidence .. 9: low confidence
  unsigned int  angle;				///< Angle of the result

}ALG_FaceDetectFaceStatus;


/* Sync with DRV_FaceDetectRunStatus */
typedef struct {

  unsigned short numFaces;
  unsigned short type;
  char	 strings[ALG_FACE_STRING_SIZE];
  ALG_FaceDetectFaceStatus info[ALG_FACE_DETECT_MAX_FACES];
  unsigned short frFaces;
  ALG_FaceRecogStatus frInfo[ALG_FACE_DETECT_MAX_FACES];

} ALG_FaceDetectRunStatus;


typedef struct {

  void *ptRecBuff;
  void *frRecBuff;
  void *ptAlgBuff[3];
  void *frAlgBuff[3];
  unsigned int *albumBuff;
  unsigned int *imageDataBuff;
  unsigned int *phy_imageDataBuff;

} ALG_FaceMem;


typedef struct {

  unsigned int nPtRec;
  unsigned int nFrRec;
  unsigned int facePTRecSize[3];
  unsigned int faceFRRecSize[3];
  unsigned int faceAlbumSize;
  unsigned int imageDataSize;

} ALG_FaceMemSize;


ALG_FaceMemSize szMemSize;
ALG_FaceMem     ffMemBuff;


typedef struct {

  void *buff;
  unsigned char size;
  char AlbumPath[40];

}ALG_FaceOpenPrm;


/* Functions */

/* Facial feature init function */
int ALG_facialFeatureOpen(ALG_FaceOpenPrm *facePrm);
/* Facial feature delete function */
int ALG_facialFeatureClose(void);
/* Facial feature Process function */
int ALG_facialFeatureRun( ALG_FaceDetectRunStatus *faceStatus, ALG_FaceDetectRunPrm *facePrm, unsigned int ffProcess);

/* Facial feature for Face Recognition -  remove a selected user from recog. album*/
int ALG_facialFeature_DeleteUser(int userID);
/* Facial feature for Face Recognition -  remove a all users from recog. album */
int ALG_facialFeature_DeleteAllUser();
/* Facial feature for Face Recognition -  Get Histograme Eq for the input y-data */
int ALG_facialFeature_GetHistoEq(ALG_FaceDetectRunPrm  *facePrm);

/* Facial feature memory allocation */
int ALG_facialFeatureGetMem();
int ALG_ffpt_GetMemRec(int *num);
int ALG_ffpt_GetMemSize(unsigned int num, void *ptMem, int *size);
int ALG_fffr_GetMemRec(int *num);
int ALG_fffr_GetMemSize(unsigned int num, void *ptMem, int *size);
int ALG_facialFeatureRelMem(void);

/* Set the confidence value */
void ALG_fffr_SetIdentifyConfig(unsigned short val);
void ALG_fffr_SetRegConfig(unsigned short val);

/* FaceTracker Functions */
#define MAX_FACETRACKER_FRAMEBUF	(320*240)

int ALG_faceTrackerOpen(void);
int ALG_faceTrackerClose(void);
int ALG_faceTrackerRun(unsigned char* bufptr, int width, int height, DRV_FaceDetectRunStatus *status);

#endif

