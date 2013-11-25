
#ifndef _ALG_VNF_H_
#define _ALG_VNF_H_

#include <alg.h>

#define ALG_VNF_PIXEL_PAD          (32)

#define ALG_VNF_MODE_NO_NF         (0)
#define ALG_VNF_MODE_ONLY_TNF      (1)
#define ALG_VNF_MODE_ONLY_KNF      (2)
#define ALG_VNF_MODE_KNF_TNF       (3)
#define ALG_VNF_MODE_KTNF          (4)

typedef enum{
	SNF_DEFAULT = 0,
	SNF_CUSTOM
} SNF_STRENGTH_MODE;

typedef struct {

  /* HWNF Parameters */
  Int32   nsf_thr00;            //color 0 level 1 offset
  Int32   nsf_thr01;            //color 0 level 1 slope
  Int32   nsf_thr02;            //color 0 level 2 offset
  Int32   nsf_thr03;            //color 0 level 2 slope
  Int32   nsf_thr04;            //color 0 level 3 offset
  Int32   nsf_thr05;            //color 0 level 3 slope
  Int32   nsf_thr10;            //color 1 level 1 offset
  Int32   nsf_thr11;            //color 1 level 1 slope
  Int32   nsf_thr12;            //color 1 level 2 offset
  Int32   nsf_thr13;            //color 1 level 2 slope
  Int32   nsf_thr14;            //color 1 level 3 offset
  Int32   nsf_thr15;            //color 1 level 3 slope
  Int32   nsf_thr20;            //color 2 level 1 offset
  Int32   nsf_thr21;            //color 2 level 1 slope
  Int32   nsf_thr22;            //color 2 level 2 offset
  Int32   nsf_thr23;            //color 2 level 2 slope
  Int32   nsf_thr24;            //color 2 level 3 offset
  Int32   nsf_thr25;            //color 2 level 3 slope
  Int32   nsf_sft_slope;        //nsf sft slope
  Int32   nsf_ee_l1_slope;      //nsf
  Int32   nsf_ee_l1_thr1;       //nsf
  Int32   nsf_ee_l1_thr2;       //nsf
  Int32   nsf_ee_l1_ofst2;      //nsf
  Int32   nsf_ee_l2_slope;      //nsf
  Int32   nsf_ee_l2_thr1;       //nsf
  Int32   nsf_ee_l2_thr2;       //nsf
  Int32   nsf_ee_l2_ofst2;      //nsf
  Int32   nsf_ee_l3_slope;      //nsf
  Int32   nsf_ee_l3_thr1;       //nsf
  Int32   nsf_ee_l3_thr2;       //nsf
  Int32   nsf_ee_l3_ofst2;      //nsf

  /* TNF Parameters */
  Uint32  TNF_A0;               /* Control Param: 0 to 255 */
  Uint32  TNF_TM;               /* Control Param: 1 to 255 */
  Uint32  TNFLuma;              /* 1: TNF applied on Luma Only, 0: TNF applied on both Luma and Chroma */

} ALG_vnfParams;

typedef struct {

  Uint16  dataFormat;   // only YUV420 supported
  Uint16  width;
  Uint16  height;
  Uint16  offsetH;      // offsetH >= width, input and output offsetH are same
  Uint16  offsetV;      // offsetV >= height+16, , input and output offsetV are same
  Uint16  mode;
  Uint16 q_num;
  Uint16 askIMCOPRes;
  Uint16 snfStrength;

  ALG_vnfParams *pVnfParams;  // if NULL, default values will be used
  Uint32  sysBaseAddr;

} ALG_VnfCreate;

typedef struct {

  Uint16  outWidth;
  Uint16  outHeight;
  Uint16  outStartX;
  Uint16  outStartY;

} ALG_VnfStatus;

typedef struct {

  Uint8  *inAddr;               //Virt Addr
  Uint8  *outAddr;              //Virt Addr

  Uint16  inStartX;
  Uint16  inStartY;

  Uint16  mode;

  ALG_vnfParams *pVnfParams; // if NULL, previously set values are applied

} ALG_VnfRunPrm;

extern ALG_vnfParams gAlg_vnfPrm;

void   *ALG_vnfCreate(ALG_VnfCreate * create);
int     ALG_vnfRun(void *hndl, ALG_VnfRunPrm *prm, ALG_VnfStatus * status);
int     ALG_vnfDelete(void *hndl);

#endif
