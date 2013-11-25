
#ifndef _DRV_FACE_DETECT_H_
#define _DRV_FACE_DETECT_H_

#include <drv.h>
#include <drv_csl.h>

#define DRV_FACE_DETECT_CONDITION_DEFAULT 	(CSL_FACE_DETECT_DIR_UP|CSL_FACE_DETECT_FACE_SIZE_20)
#define DRV_FACE_DETECT_CONDITION_UP 		(CSL_FACE_DETECT_DIR_UP|CSL_FACE_DETECT_FACE_SIZE_20)
#define DRV_FACE_DETECT_CONDITION_LEFT 		(CSL_FACE_DETECT_DIR_LEFT|CSL_FACE_DETECT_FACE_SIZE_20)
#define DRV_FACE_DETECT_CONDITION_RIGHT 	(CSL_FACE_DETECT_DIR_RIGHT|CSL_FACE_DETECT_FACE_SIZE_20)
#define DRV_STRING_SIZE           			(16)
#define DRV_FACE_MAX_THRESHOLD           	(9)

enum{
	FACE_T_NO_DETECT = 0,
	FACE_T_DETECT ,
	FACE_T_MASK,
	FACE_T_RECOGIZE,
	FACE_T_REGUSER,
	FACE_T_DELUSER,
	FACE_T_TYPE_NUM
};


typedef struct {

  Uint8 *inPhysAddr;
  Uint8 *inVirtAddr;
  Uint16 inWidth;
  Uint16 inHeight;
  Uint32 detectThres;    // 0..15
  Uint32 detectCondition;

} DRV_FaceDetectRunPrm;

typedef struct {

  Int32 userID;
  Int8 usrName[DRV_STRING_SIZE];  //Revisit
} DRV_FaceRecogStatus; //


typedef struct {

  Uint16 numFaces;
  Uint16 type;
  Int8	 strings[DRV_STRING_SIZE];
  CSL_FaceDetectFaceStatus info[CSL_FACE_DETECT_MAX_FACES];
  Uint16 frFaces;
  DRV_FaceRecogStatus frInfo[CSL_FACE_DETECT_MAX_FACES];

} DRV_FaceDetectRunStatus;

typedef struct {
  Uint16 type;
  Int8	 strings[16];
} DRV_FaceDetectRunType;


int DRV_faceDetectOpen();
int DRV_faceDetectClose();

int DRV_faceDetectRun(DRV_FaceDetectRunPrm *prm, DRV_FaceDetectRunStatus *status);

#endif
