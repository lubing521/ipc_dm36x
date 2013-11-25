/*
    DM360 Evaluation Software

    (c)Texas Instruments 2007
*/

#ifndef _CSL_FACE_DETECT_H_
#define _CSL_FACE_DETECT_H_

#ifdef __cplusplus
extern  "C" {
#endif

#include <csl_soc.h>

/**
  \file csl_faceDetect.h
  \brief Face detect APIs
*/

/**
  \ingroup CSL_VPFE
  \defgroup  CSL_FD Face Detect (FD)

  \brief Face Detect (FD)

  @{
*/

#define CSL_FACE_DETECT_MAX_FACES     35 ///< Max Number of faces that can be detect by Face-detect

#define CSL_FACE_DETECT_GET_SIZE(sizeConfidence)  ((sizeConfidence) & CSL_FACE_DETECT_FD_CONFSIZE1_SIZE1_MASK)
#define CSL_FACE_DETECT_GET_CONFIDENCE(sizeConfidence)  (((sizeConfidence) & CSL_FACE_DETECT_FD_CONFSIZE1_CONF1_MASK) >> CSL_FACE_DETECT_FD_CONFSIZE1_CONF1_SHIFT)

#define CSL_FACE_DETECT_THRES_MAX     (0xF)
#define CSL_FACE_DETECT_THRES_MIN     (0x0)

#define CSL_FACE_DETECT_DIR_UP        (0x0)
#define CSL_FACE_DETECT_DIR_RIGHT     (0x4)
#define CSL_FACE_DETECT_DIR_LEFT      (0x8)

#define CSL_FACE_DETECT_FACE_SIZE_20  (0x0)
#define CSL_FACE_DETECT_FACE_SIZE_25  (0x1)
#define CSL_FACE_DETECT_FACE_SIZE_32  (0x2)
#define CSL_FACE_DETECT_FACE_SIZE_40  (0x3)

// data structure's

/**
  \brief face detect status
*/
  typedef struct {

    Uint32  centerX;            ///< Centre X
    Uint32  centerY;            ///< Centre Y
    Uint32  sizeConfidence;     ///< Confidence/Size of the result, confidence = 0: high confidence .. 9: low confidence
    Uint32  angle;                ///< Angle of the result

  } CSL_FaceDetectFaceStatus;

/**
  \brief  Face Detect hardware configuration structure
*/
  typedef struct {

    Uint8  *inputAddr;          ///< Picture data address in SDRAM
    Uint8  *workAreaAddr;       ///< Work area address in SDRAM

    Uint16  inStartX;           ///< Start X
    Uint16  inStartY;           ///< Start Y
    Uint16  inWidth;            ///< X size number
    Uint16  inHeight;           ///< Y size number

    Uint32  detectThres;        ///< Detect threshold, 0: low threshold .. 15: high threshold
    Uint32  detectCondition;    ///< Detect condition, CSL_FACE_DETECT_DIR_xx | CSL_FACE_DETECT_FACE_SIZE_xx

  } CSL_FaceDetectHwSetup;

/**
  \brief Module Object Structure
*/
  typedef struct {

#ifdef CSL_KERNEL_API
    CSL_FaceDetectRegsOvly regs;  ///< Register overlay
    CSL_SysFlag intFlag;
#else
    int     fd;
#endif

  } CSL_FaceDetectObj;

  typedef CSL_FaceDetectObj *CSL_FaceDetectHandle;  ///< Module Handle

// functions

/**
  \brief Open Module Handle

  \param hndl   [ O]  Module Handle

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_faceDetectOpen(CSL_FaceDetectHandle hndl);

/**
  \brief Close Module Handle

  \param hndl   [I ]  Handle

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_faceDetectClose(CSL_FaceDetectHandle hndl);

/**
  \brief Set all module registers

  \param hndl   [I ]  Handle
  \param data   [I ]  Input parameters

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_faceDetectHwSetup(CSL_FaceDetectHandle hndl, CSL_FaceDetectHwSetup * data);

/**
  \brief Enable/Disable Face Detect

  \param hndl   [I ]  Handle
  \param enable [I ]  TRUE: enable, FALSE: disable

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_faceDetectDisable(CSL_FaceDetectHandle hndl);

/**
  \brief Face detect status

  \param hndl     [I ]  Handle
  \param faceList [I ]  Face detect status
  \param numFaces [0 ]  Face detect number

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_faceDetectGetStatus(CSL_FaceDetectHandle hndl, CSL_FaceDetectFaceStatus * faceList, Uint16 * numFaces);

/**
  \brief Enable/Disable face detect interrupt

  \param hndl   [I ]  Handle
  \param enable [I ]  TRUE: enable, FALSE: disable

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_faceDetectIntEnable(CSL_FaceDetectHandle hndl, Bool32 enable);

/**
  \brief Face detect interrupt clear

  \param hndl   [I ]  Handle

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_faceDetectIntClear(CSL_FaceDetectHandle hndl);

/**
  \brief Face detect interrupt wait

  \param hndl   [I ]  Handle
  \param timeout[I ]  Timeout in msecs

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_faceDetectIntWait(CSL_FaceDetectHandle hndl, int timeout);


/**
  \brief Module lock : Mutex Lock

  \param hndl     [I ] hndl
  \param timeout  [I ] CSL_SYS_TIMEOUT_NONE or CSL_SYS_TIMEOUT_FOREVER

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_faceDetectLock(CSL_FaceDetectHandle hndl, Uint32 timeout);

/**
  \brief Module unlock : Mutex Unlock

  \param hndl     [I ] hndl

  \return CSL_SOK on success, else CSL_Exxxx
*/
  CSL_Status CSL_faceDetectUnlock(CSL_FaceDetectHandle hndl);

#ifdef __cplusplus
}
#endif
/*@}*/
#endif
