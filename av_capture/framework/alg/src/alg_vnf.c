#include <alg_priv.h>
#include <alg_vnf.h>
#include <osa_cmem.h>
#include <osa_file.h>
#include <asm/arch/edma.h>
#include <ti/sdo/codecs/ktnf/iktnf.h>

#define NSF_SLOPE_POINT5    0x040
#define NSF_SLOPE_POINT1    0x00C
#define NSF_SLOPE_POINT0    0x000
#define NSF_SLOPE           NSF_SLOPE_POINT0

#define NUMFRAMESLOOPCOUNT  1

/* The below two defines describe Luma and Chroma Buffer sizes and the distance between them */
/* Also, the input and output are alike in terms of these two parameters */
#define LINE_IN_OFFSET_V    752   // Distance between Luma and Chroma Buffers
#define LINE_IN_OFFSET_H    1280  // Input Offset between two rows of data

/* The below two defines describe the valid input in the input buffer on which KTNF is to be processed */
/* However, the output width and Height will be read from the runtime parameter  */
#define HEIGHT  (720)
#define WIDTH   (1280)

#define NUMFRAMES  30

typedef struct
{
    VIDENC1_Handle      hEncode;
    ALG_VnfCreate       createPrm;
    char                algName[20];
    VIDENC1_Status      encStatus;
    VIDENC1_Params      params;
    IKTNF_Params        extParams;
    IKTNF_DynamicParams extDynParams;
    Uint8 *inPastAddr;
} ALG_VnfObj;

void *ALG_vnfCreate(ALG_VnfCreate * create)
{
    ALG_VnfObj *pObj = NULL;
    if (gALG_hEngine == NULL)
    {
        return NULL;
    }

    pObj = OSA_memAlloc(sizeof(ALG_VnfObj));
    if (pObj == NULL)
    {
        return NULL;
    }

    memset(pObj, 0, sizeof(ALG_VnfObj));
    strcpy(pObj->algName, ALG_VID_ADV_FEATURE_KTNF_NAME);
    memcpy(&pObj->createPrm, create, sizeof(pObj->createPrm));

    pObj->params.size      = sizeof(IKTNF_Params);
    pObj->params.maxHeight = create->offsetV;
    pObj->params.maxWidth  = create->offsetH;
    pObj->extDynParams.videncDynamicParams.inputHeight = pObj->params.maxHeight;
    pObj->extDynParams.videncDynamicParams.inputWidth  = pObj->params.maxWidth;

    if (create->dataFormat == DRV_DATA_FORMAT_YUV420)
    {
        pObj->params.inputChromaFormat = XDM_YUV_420SP;
    } 
    else
    {
        pObj->params.inputChromaFormat = XDM_YUV_422ILE;
    }

    /* NF parameters */
    switch (create->mode)
    {
    default:
    case ALG_VNF_MODE_NO_NF:
        pObj->extParams.EnableNSF = IKTNF_NO_NF;
        break;
    case ALG_VNF_MODE_ONLY_TNF:
        pObj->extParams.EnableNSF = IKTNF_ONLY_TNF;
        break;
    case ALG_VNF_MODE_ONLY_KNF:
        pObj->extParams.EnableNSF = IKTNF_ONLY_KNF;
        break;
    case ALG_VNF_MODE_KNF_TNF:
        pObj->extParams.EnableNSF = IKTNF_KNF_TNF;
        break;
    case ALG_VNF_MODE_KTNF:
        pObj->extParams.EnableNSF = IKTNF_KTNF;
        break;

    }
    pObj->extDynParams.EnableNSF = pObj->extParams.EnableNSF;

    // Set the create time extended parameters
    pObj->extParams.videncParams    = pObj->params;
    pObj->extParams.subWindowWidth  = create->width;
    pObj->extParams.subWindowHeight = create->height;
    pObj->extParams.q_num           = EVENTQ_1;
    pObj->extParams.askIMCOPRes     = 0;
    pObj->extParams.sysBaseAddr     = create->sysBaseAddr;

    if (create->pVnfParams == NULL)
    {
        if (create->snfStrength == SNF_CUSTOM)
        {
            // light box, aperture wide open, 2x isif d-gain, 50% knob
            pObj->extDynParams.nsf_thr00 = pObj->extParams.nsf_thr00 = 26;   //color 0 level 1 offset
            pObj->extDynParams.nsf_thr01 = pObj->extParams.nsf_thr01 = -18;  //color 0 level 1 slope
            pObj->extDynParams.nsf_thr02 = pObj->extParams.nsf_thr02 = 74;   //color 0 level 2 offset
            pObj->extDynParams.nsf_thr03 = pObj->extParams.nsf_thr03 = -52;  //color 0 level 2 slope
            pObj->extDynParams.nsf_thr04 = pObj->extParams.nsf_thr04 = 37;   //color 0 level 3 offset
            pObj->extDynParams.nsf_thr05 = pObj->extParams.nsf_thr05 = -24;  //color 0 level 3 slope
            pObj->extDynParams.nsf_thr10 = pObj->extParams.nsf_thr10 = 35;   //color 1 level 1 offset
            pObj->extDynParams.nsf_thr11 = pObj->extParams.nsf_thr11 = 37;   //color 1 level 1 slope
            pObj->extDynParams.nsf_thr12 = pObj->extParams.nsf_thr12 = 30;     //color 1 level 2 offset
            pObj->extDynParams.nsf_thr13 = pObj->extParams.nsf_thr13 = 24;  //color 1 level 2 slope
            pObj->extDynParams.nsf_thr14 = pObj->extParams.nsf_thr14 = 19;      //color 1 level 3 offset
            pObj->extDynParams.nsf_thr15 = pObj->extParams.nsf_thr15 = 2;  //color 1 level 3 slope
            pObj->extDynParams.nsf_thr20 = pObj->extParams.nsf_thr20 = 25;     //color 2 level 1 offset
            pObj->extDynParams.nsf_thr21 = pObj->extParams.nsf_thr21 = 58;  //color 2 level 1 slope
            pObj->extDynParams.nsf_thr22 = pObj->extParams.nsf_thr22 = 22;     //color 2 level 2 offset
            pObj->extDynParams.nsf_thr23 = pObj->extParams.nsf_thr23 = 30;  //color 2 level 2 slope
            pObj->extDynParams.nsf_thr24 = pObj->extParams.nsf_thr24 = 15;      //color 2 level 3 offset
            pObj->extDynParams.nsf_thr25 = pObj->extParams.nsf_thr25 = 12;  //color 2 level 3 slope

            pObj->extDynParams.nsf_sft_slope = pObj->extParams.nsf_sft_slope = 2;

            /* Setting of Edge Enhancement parameters */
            pObj->extDynParams.nsf_ee_l1_slope = pObj->extParams.nsf_ee_l1_slope  = 1;
            pObj->extDynParams.nsf_ee_l1_thr1 = pObj->extParams.nsf_ee_l1_thr1    = 26;
            pObj->extDynParams.nsf_ee_l1_thr2 = pObj->extParams.nsf_ee_l1_thr2    = 36;
            pObj->extDynParams.nsf_ee_l1_ofst2 = pObj->extParams.nsf_ee_l1_ofst2  = 36;
            pObj->extDynParams.nsf_ee_l2_slope = pObj->extParams.nsf_ee_l2_slope  = 3;
            pObj->extDynParams.nsf_ee_l2_thr1 = pObj->extParams.nsf_ee_l2_thr1    = 74;
            pObj->extDynParams.nsf_ee_l2_thr2 = pObj->extParams.nsf_ee_l2_thr2    = 84;
            pObj->extDynParams.nsf_ee_l2_ofst2 = pObj->extParams.nsf_ee_l2_ofst2  = 94;
            pObj->extDynParams.nsf_ee_l3_slope = pObj->extParams.nsf_ee_l3_slope  = 3;
            pObj->extDynParams.nsf_ee_l3_thr1 = pObj->extParams.nsf_ee_l3_thr1    = 37;
            pObj->extDynParams.nsf_ee_l3_thr2 = pObj->extParams.nsf_ee_l3_thr2    = 47;
            pObj->extDynParams.nsf_ee_l3_ofst2 = pObj->extParams.nsf_ee_l3_ofst2  = 57;
        } 
        else // Set to default
        {
#if 0 //Gang: original
            pObj->extDynParams.nsf_thr00 = pObj->extParams.nsf_thr00 = 60;     //color 0 level 1 offset
            pObj->extDynParams.nsf_thr01 = pObj->extParams.nsf_thr01 = NSF_SLOPE;  //color 0 level 1 slope
            pObj->extDynParams.nsf_thr02 = pObj->extParams.nsf_thr02 = 20;     //color 0 level 2 offset
            pObj->extDynParams.nsf_thr03 = pObj->extParams.nsf_thr03 = NSF_SLOPE;  //color 0 level 2 slope
            pObj->extDynParams.nsf_thr04 = pObj->extParams.nsf_thr04 = 0;      //color 0 level 3 offset
            pObj->extDynParams.nsf_thr05 = pObj->extParams.nsf_thr05 = NSF_SLOPE;  //color 0 level 3 slope
            pObj->extDynParams.nsf_thr10 = pObj->extParams.nsf_thr10 = 40;     //color 1 level 1 offset
            pObj->extDynParams.nsf_thr11 = pObj->extParams.nsf_thr11 = NSF_SLOPE;  //color 1 level 1 slope
            pObj->extDynParams.nsf_thr12 = pObj->extParams.nsf_thr12 = 20;     //color 1 level 2 offset
            pObj->extDynParams.nsf_thr13 = pObj->extParams.nsf_thr13 = NSF_SLOPE;  //color 1 level 2 slope
            pObj->extDynParams.nsf_thr14 = pObj->extParams.nsf_thr14 = 0;      //color 1 level 3 offset
            pObj->extDynParams.nsf_thr15 = pObj->extParams.nsf_thr15 = NSF_SLOPE;  //color 1 level 3 slope
            pObj->extDynParams.nsf_thr20 = pObj->extParams.nsf_thr20 = 40;     //color 2 level 1 offset
            pObj->extDynParams.nsf_thr21 = pObj->extParams.nsf_thr21 = NSF_SLOPE;  //color 2 level 1 slope
            pObj->extDynParams.nsf_thr22 = pObj->extParams.nsf_thr22 = 20;     //color 2 level 2 offset
            pObj->extDynParams.nsf_thr23 = pObj->extParams.nsf_thr23 = NSF_SLOPE;  //color 2 level 2 slope
            pObj->extDynParams.nsf_thr24 = pObj->extParams.nsf_thr24 = 0;      //color 2 level 3 offset
            pObj->extDynParams.nsf_thr25 = pObj->extParams.nsf_thr25 = NSF_SLOPE;  //color 2 level 3 slope

            pObj->extDynParams.nsf_sft_slope = pObj->extParams.nsf_sft_slope = 1;

            /* Setting of Edge Enhancement parameters */
            pObj->extDynParams.nsf_ee_l1_slope = pObj->extParams.nsf_ee_l1_slope  = 0;
            pObj->extDynParams.nsf_ee_l1_thr1 = pObj->extParams.nsf_ee_l1_thr1    = 972;
            pObj->extDynParams.nsf_ee_l1_thr2 = pObj->extParams.nsf_ee_l1_thr2    = 746;
            pObj->extDynParams.nsf_ee_l1_ofst2 = pObj->extParams.nsf_ee_l1_ofst2  = 85;
            pObj->extDynParams.nsf_ee_l2_slope = pObj->extParams.nsf_ee_l2_slope  = 0;
            pObj->extDynParams.nsf_ee_l2_thr1 = pObj->extParams.nsf_ee_l2_thr1    = 484;
            pObj->extDynParams.nsf_ee_l2_thr2 = pObj->extParams.nsf_ee_l2_thr2    = 939;
            pObj->extDynParams.nsf_ee_l2_ofst2 = pObj->extParams.nsf_ee_l2_ofst2  = 552;
            pObj->extDynParams.nsf_ee_l3_slope = pObj->extParams.nsf_ee_l3_slope  = 1;
            pObj->extDynParams.nsf_ee_l3_thr1 = pObj->extParams.nsf_ee_l3_thr1    = 501;
            pObj->extDynParams.nsf_ee_l3_thr2 = pObj->extParams.nsf_ee_l3_thr2    = 1015;
            pObj->extDynParams.nsf_ee_l3_ofst2 = pObj->extParams.nsf_ee_l3_ofst2  = 501;
#else //Gang: test
            printf("\n==================================");
            printf("\nGang: testing KTNF parameters\n");
            printf("==================================\n");
            pObj->extDynParams.nsf_thr00 = pObj->extParams.nsf_thr00 = 80;     //color 0 level 1 offset
            pObj->extDynParams.nsf_thr01 = pObj->extParams.nsf_thr01 = 0;  //color 0 level 1 slope
            pObj->extDynParams.nsf_thr02 = pObj->extParams.nsf_thr02 = 60;     //color 0 level 2 offset
            pObj->extDynParams.nsf_thr03 = pObj->extParams.nsf_thr03 = 0;  //color 0 level 2 slope
            pObj->extDynParams.nsf_thr04 = pObj->extParams.nsf_thr04 = 40;      //color 0 level 3 offset
            pObj->extDynParams.nsf_thr05 = pObj->extParams.nsf_thr05 = 0;  //color 0 level 3 slope
            pObj->extDynParams.nsf_thr10 = pObj->extParams.nsf_thr10 = 50;     //color 1 level 1 offset
            pObj->extDynParams.nsf_thr11 = pObj->extParams.nsf_thr11 = 0;  //color 1 level 1 slope
            pObj->extDynParams.nsf_thr12 = pObj->extParams.nsf_thr12 = 50;     //color 1 level 2 offset
            pObj->extDynParams.nsf_thr13 = pObj->extParams.nsf_thr13 = 0;  //color 1 level 2 slope
            pObj->extDynParams.nsf_thr14 = pObj->extParams.nsf_thr14 = 50;      //color 1 level 3 offset
            pObj->extDynParams.nsf_thr15 = pObj->extParams.nsf_thr15 = 0;  //color 1 level 3 slope
            pObj->extDynParams.nsf_thr20 = pObj->extParams.nsf_thr20 = 80;     //color 2 level 1 offset
            pObj->extDynParams.nsf_thr21 = pObj->extParams.nsf_thr21 = 0;  //color 2 level 1 slope
            pObj->extDynParams.nsf_thr22 = pObj->extParams.nsf_thr22 = 70;     //color 2 level 2 offset
            pObj->extDynParams.nsf_thr23 = pObj->extParams.nsf_thr23 = 0;  //color 2 level 2 slope
            pObj->extDynParams.nsf_thr24 = pObj->extParams.nsf_thr24 = 60;      //color 2 level 3 offset
            pObj->extDynParams.nsf_thr25 = pObj->extParams.nsf_thr25 = 0;  //color 2 level 3 slope

            pObj->extDynParams.nsf_sft_slope = pObj->extParams.nsf_sft_slope = 0;

            /* Setting of Edge Enhancement parameters */
            pObj->extDynParams.nsf_ee_l1_slope = pObj->extParams.nsf_ee_l1_slope  = 5;
            pObj->extDynParams.nsf_ee_l1_thr1 = pObj->extParams.nsf_ee_l1_thr1    = 10;
            pObj->extDynParams.nsf_ee_l1_thr2 = pObj->extParams.nsf_ee_l1_thr2    = 30;
            pObj->extDynParams.nsf_ee_l1_ofst2 = pObj->extParams.nsf_ee_l1_ofst2  = 70;
            pObj->extDynParams.nsf_ee_l2_slope = pObj->extParams.nsf_ee_l2_slope  = 5;
            pObj->extDynParams.nsf_ee_l2_thr1 = pObj->extParams.nsf_ee_l2_thr1    = 10;
            pObj->extDynParams.nsf_ee_l2_thr2 = pObj->extParams.nsf_ee_l2_thr2    = 30;
            pObj->extDynParams.nsf_ee_l2_ofst2 = pObj->extParams.nsf_ee_l2_ofst2  = 70;
            pObj->extDynParams.nsf_ee_l3_slope = pObj->extParams.nsf_ee_l3_slope  = 5;
            pObj->extDynParams.nsf_ee_l3_thr1 = pObj->extParams.nsf_ee_l3_thr1    = 10;
            pObj->extDynParams.nsf_ee_l3_thr2 = pObj->extParams.nsf_ee_l3_thr2    = 30;
            pObj->extDynParams.nsf_ee_l3_ofst2 = pObj->extParams.nsf_ee_l3_ofst2  = 70;
#endif
        }

        /* TNF Parameters */
        pObj->extDynParams.TNF_A0 = pObj->extParams.TNF_A0      = 0;        /* Control Param: 0 to 255 */
        pObj->extDynParams.TNF_TM = pObj->extParams.TNF_TM      = 20;       /* Control Param: 1 to 255 */
        pObj->extDynParams.TNFLuma = pObj->extParams.TNFLuma    = 1;        /* 1: TNF applied on Luma Only, 0: TNF applied on both Luma and Chroma */
    } 
    else
    {
        /* KNF Parameters */
        pObj->extDynParams.nsf_thr00 = pObj->extParams.nsf_thr00 = create->pVnfParams->nsf_thr00;  //color 0 level 1 offset
        pObj->extDynParams.nsf_thr01 = pObj->extParams.nsf_thr01 = create->pVnfParams->nsf_thr01;  //color 0 level 1 slope
        pObj->extDynParams.nsf_thr02 = pObj->extParams.nsf_thr02 = create->pVnfParams->nsf_thr02;  //color 0 level 2 offset
        pObj->extDynParams.nsf_thr03 = pObj->extParams.nsf_thr03 = create->pVnfParams->nsf_thr03;  //color 0 level 2 slope
        pObj->extDynParams.nsf_thr04 = pObj->extParams.nsf_thr04 = create->pVnfParams->nsf_thr04;  //color 0 level 3 offset
        pObj->extDynParams.nsf_thr05 = pObj->extParams.nsf_thr05 = create->pVnfParams->nsf_thr05;  //color 0 level 3 slope
        pObj->extDynParams.nsf_thr10 = pObj->extParams.nsf_thr10 = create->pVnfParams->nsf_thr10;  //color 1 level 1 offset
        pObj->extDynParams.nsf_thr11 = pObj->extParams.nsf_thr11 = create->pVnfParams->nsf_thr11;  //color 1 level 1 slope
        pObj->extDynParams.nsf_thr12 = pObj->extParams.nsf_thr12 = create->pVnfParams->nsf_thr12;  //color 1 level 2 offset
        pObj->extDynParams.nsf_thr13 = pObj->extParams.nsf_thr13 = create->pVnfParams->nsf_thr13;  //color 1 level 2 slope
        pObj->extDynParams.nsf_thr14 = pObj->extParams.nsf_thr14 = create->pVnfParams->nsf_thr14;  //color 1 level 3 offset
        pObj->extDynParams.nsf_thr15 = pObj->extParams.nsf_thr15 = create->pVnfParams->nsf_thr15;  //color 1 level 3 slope
        pObj->extDynParams.nsf_thr20 = pObj->extParams.nsf_thr20 = create->pVnfParams->nsf_thr20;  //color 2 level 1 offset
        pObj->extDynParams.nsf_thr21 = pObj->extParams.nsf_thr21 = create->pVnfParams->nsf_thr21;  //color 2 level 1 slope
        pObj->extDynParams.nsf_thr22 = pObj->extParams.nsf_thr22 = create->pVnfParams->nsf_thr22;  //color 2 level 2 offset
        pObj->extDynParams.nsf_thr23 = pObj->extParams.nsf_thr23 = create->pVnfParams->nsf_thr23;  //color 2 level 2 slope
        pObj->extDynParams.nsf_thr24 = pObj->extParams.nsf_thr24 = create->pVnfParams->nsf_thr24;  //color 2 level 3 offset
        pObj->extDynParams.nsf_thr25 = pObj->extParams.nsf_thr25 = create->pVnfParams->nsf_thr25;  //color 2 level 3 slope

        pObj->extDynParams.nsf_sft_slope = pObj->extParams.nsf_sft_slope = create->pVnfParams->nsf_sft_slope;

        /* Setting of Edge Enhancement parameters */
        pObj->extDynParams.nsf_ee_l1_slope = pObj->extParams.nsf_ee_l1_slope  = create->pVnfParams->nsf_ee_l1_slope;
        pObj->extDynParams.nsf_ee_l1_thr1 = pObj->extParams.nsf_ee_l1_thr1    = create->pVnfParams->nsf_ee_l1_thr1;
        pObj->extDynParams.nsf_ee_l1_thr2 = pObj->extParams.nsf_ee_l1_thr2    = create->pVnfParams->nsf_ee_l1_thr2;
        pObj->extDynParams.nsf_ee_l1_ofst2 = pObj->extParams.nsf_ee_l1_ofst2  = create->pVnfParams->nsf_ee_l1_ofst2;
        pObj->extDynParams.nsf_ee_l2_slope = pObj->extParams.nsf_ee_l2_slope  = create->pVnfParams->nsf_ee_l2_slope;
        pObj->extDynParams.nsf_ee_l2_thr1 = pObj->extParams.nsf_ee_l2_thr1    = create->pVnfParams->nsf_ee_l2_thr1;
        pObj->extDynParams.nsf_ee_l2_thr2 = pObj->extParams.nsf_ee_l2_thr2    = create->pVnfParams->nsf_ee_l2_thr2;
        pObj->extDynParams.nsf_ee_l2_ofst2 = pObj->extParams.nsf_ee_l2_ofst2  = create->pVnfParams->nsf_ee_l2_ofst2;
        pObj->extDynParams.nsf_ee_l3_slope = pObj->extParams.nsf_ee_l3_slope  = create->pVnfParams->nsf_ee_l3_slope;
        pObj->extDynParams.nsf_ee_l3_thr1 = pObj->extParams.nsf_ee_l3_thr1    = create->pVnfParams->nsf_ee_l3_thr1;
        pObj->extDynParams.nsf_ee_l3_thr2 = pObj->extParams.nsf_ee_l3_thr2    = create->pVnfParams->nsf_ee_l3_thr2;
        pObj->extDynParams.nsf_ee_l3_ofst2 = pObj->extParams.nsf_ee_l3_ofst2  = create->pVnfParams->nsf_ee_l3_ofst2;

        /* TNF Parameters */
        pObj->extDynParams.TNF_A0     = pObj->extParams.TNF_A0    = create->pVnfParams->TNF_A0;  /* Control Param: 0 to 255 */
        pObj->extDynParams.TNF_TM     = pObj->extParams.TNF_TM    = create->pVnfParams->TNF_TM;  /* Control Param: 1 to 255 */
        pObj->extDynParams.TNFLuma    = pObj->extParams.TNFLuma   = create->pVnfParams->TNFLuma;  /* TNF applied on Luma Only */
    }

    /* Create video encoder instance */
    //OSA_printf("\n Entering Create  \n");
    IMAGE_TUNE_SetVnfParams((ALG_vnfParams*) &pObj->extParams.nsf_thr00 );

    //OSA_printf("VNF DEBUG CREATE VALUE: %d\n",pObj->extParams.nsf_thr00);

    pObj->hEncode = VIDENC1_create(gALG_hEngine, pObj->algName, (VIDENC1_Params *) & pObj->extParams);
    if (pObj->hEncode == NULL)
    {
        OSA_ERROR("Failed to open video encode algorithm: %s (0x%x)\n", pObj->algName, Engine_getLastError(gALG_hEngine));
        OSA_memFree(pObj);
        return NULL;
    }
    else
    {
        //OSA_printf("\n Exit Create successfully  \n");
    }

    return pObj;
}

int ALG_vnfRun(void *hndl, ALG_VnfRunPrm * prm, ALG_VnfStatus * runStatus)
{
    ALG_VnfObj *pObj = (ALG_VnfObj *) hndl;
    IVIDEO1_BufDescIn inBufDesc;
    XDM_BufDesc outBufDesc;
    XDAS_Int32 status;
    VIDENC1_InArgs inArgs;
    IKTNF_OutArgs outArgs;
    XDAS_Int32 inOffset, outOffset;
    XDAS_Int8 *outbufs[2];
    XDAS_Int32 outBufSizeArray[2], bufferSize;

    if (pObj == NULL)
    {
        return OSA_EFAIL;
    }

    inOffset  = pObj->createPrm.offsetH * pObj->createPrm.offsetV;
    outOffset = inOffset;

    //prm->inStartY = OSA_floor(prm->inStartY, 2);
    //prm->inStartX = OSA_floor(prm->inStartX, 2);

    /* Input buffers */
    inBufDesc.frameWidth  = OSA_align(pObj->createPrm.offsetH, 16);   // Not used
    inBufDesc.frameHeight = OSA_align(pObj->createPrm.offsetV, 16);   // Not used
    inBufDesc.framePitch  = pObj->createPrm.offsetH;

    bufferSize = (pObj->createPrm.offsetH * pObj->createPrm.height);
    inBufDesc.numBufs = 4;

    inBufDesc.bufDesc[0].bufSize = bufferSize;
    inBufDesc.bufDesc[0].buf = (XDAS_Int8 *) (prm->inAddr + prm->inStartY * pObj->createPrm.offsetH + prm->inStartX);
    inBufDesc.bufDesc[0].accessMask = 0;

    inBufDesc.bufDesc[1].bufSize = bufferSize / 2;
    inBufDesc.bufDesc[1].buf = (XDAS_Int8 *) (prm->inAddr + inOffset + (prm->inStartY / 2) * pObj->createPrm.offsetH + prm->inStartX);
    inBufDesc.bufDesc[1].accessMask = 0;

    if (pObj->inPastAddr == NULL) /* Will intialize only for first time */
    {
        pObj->inPastAddr = inBufDesc.bufDesc[0].buf;
    }

    inBufDesc.bufDesc[2].bufSize = bufferSize;
    inBufDesc.bufDesc[2].buf = (XDAS_Int8 *) pObj->inPastAddr;
    inBufDesc.bufDesc[2].accessMask = 0;

    inBufDesc.bufDesc[3].bufSize = bufferSize / 2;
    inBufDesc.bufDesc[3].buf = (XDAS_Int8 *) (pObj->inPastAddr + inOffset);
    inBufDesc.bufDesc[3].accessMask = 0;

    /* Output buffers */
    outBufDesc.numBufs = 2;
    outbufs[0] = (XDAS_Int8*)prm->outAddr;

    if ((prm->mode == ALG_VNF_MODE_ONLY_KNF) || 
        (prm->mode == ALG_VNF_MODE_KNF_TNF) || 
        (prm->mode == ALG_VNF_MODE_KTNF))
    {
        outbufs[1] = (XDAS_Int8*)(prm->outAddr + outOffset - 4 * pObj->createPrm.offsetH);
    } 
    else
    {
        outbufs[1] = (XDAS_Int8*)(prm->outAddr + outOffset);
    }

    outBufSizeArray[0] = outOffset;
    outBufSizeArray[1] = outOffset / 2;
    outBufDesc.bufSizes = outBufSizeArray;
    outBufDesc.bufs = outbufs;

    inArgs.size = sizeof(VIDENC1_InArgs);
    outArgs.videncOutArgs.size = sizeof(IKTNF_OutArgs);

    /* Set Dynamic Params */

    /* Populate the dynamic parameters */
    if (prm->pVnfParams != NULL)
    {
        /* KNF Parameters */
        pObj->extDynParams.nsf_thr00 = prm->pVnfParams->nsf_thr00;  //color 0 level 1 offset
        pObj->extDynParams.nsf_thr01 = prm->pVnfParams->nsf_thr01;  //color 0 level 1 slope
        pObj->extDynParams.nsf_thr02 = prm->pVnfParams->nsf_thr02;  //color 0 level 2 offset
        pObj->extDynParams.nsf_thr03 = prm->pVnfParams->nsf_thr03;  //color 0 level 2 slope
        pObj->extDynParams.nsf_thr04 = prm->pVnfParams->nsf_thr04;  //color 0 level 3 offset
        pObj->extDynParams.nsf_thr05 = prm->pVnfParams->nsf_thr05;  //color 0 level 3 slope
        pObj->extDynParams.nsf_thr10 = prm->pVnfParams->nsf_thr10;  //color 1 level 1 offset
        pObj->extDynParams.nsf_thr11 = prm->pVnfParams->nsf_thr11;  //color 1 level 1 slope
        pObj->extDynParams.nsf_thr12 = prm->pVnfParams->nsf_thr12;  //color 1 level 2 offset
        pObj->extDynParams.nsf_thr13 = prm->pVnfParams->nsf_thr13;  //color 1 level 2 slope
        pObj->extDynParams.nsf_thr14 = prm->pVnfParams->nsf_thr14;  //color 1 level 3 offset
        pObj->extDynParams.nsf_thr15 = prm->pVnfParams->nsf_thr15;  //color 1 level 3 slope
        pObj->extDynParams.nsf_thr20 = prm->pVnfParams->nsf_thr20;  //color 2 level 1 offset
        pObj->extDynParams.nsf_thr21 = prm->pVnfParams->nsf_thr21;  //color 2 level 1 slope
        pObj->extDynParams.nsf_thr22 = prm->pVnfParams->nsf_thr22;  //color 2 level 2 offset
        pObj->extDynParams.nsf_thr23 = prm->pVnfParams->nsf_thr23;  //color 2 level 2 slope
        pObj->extDynParams.nsf_thr24 = prm->pVnfParams->nsf_thr24;  //color 2 level 3 offset
        pObj->extDynParams.nsf_thr25 = prm->pVnfParams->nsf_thr25;  //color 2 level 3 slope

        pObj->extDynParams.nsf_sft_slope = prm->pVnfParams->nsf_sft_slope;

        /* Setting of Edge Enhancement parameters */
        pObj->extDynParams.nsf_ee_l1_slope = prm->pVnfParams->nsf_ee_l1_slope;
        pObj->extDynParams.nsf_ee_l1_thr1  = prm->pVnfParams->nsf_ee_l1_thr1;
        pObj->extDynParams.nsf_ee_l1_thr2  = prm->pVnfParams->nsf_ee_l1_thr2;
        pObj->extDynParams.nsf_ee_l1_ofst2 = prm->pVnfParams->nsf_ee_l1_ofst2;
        pObj->extDynParams.nsf_ee_l2_slope = prm->pVnfParams->nsf_ee_l2_slope;
        pObj->extDynParams.nsf_ee_l2_thr1  = prm->pVnfParams->nsf_ee_l2_thr1;
        pObj->extDynParams.nsf_ee_l2_thr2  = prm->pVnfParams->nsf_ee_l2_thr2;
        pObj->extDynParams.nsf_ee_l2_ofst2 = prm->pVnfParams->nsf_ee_l2_ofst2;
        pObj->extDynParams.nsf_ee_l3_slope = prm->pVnfParams->nsf_ee_l3_slope;
        pObj->extDynParams.nsf_ee_l3_thr1  = prm->pVnfParams->nsf_ee_l3_thr1;
        pObj->extDynParams.nsf_ee_l3_thr2  = prm->pVnfParams->nsf_ee_l3_thr2;
        pObj->extDynParams.nsf_ee_l3_ofst2 = prm->pVnfParams->nsf_ee_l3_ofst2;

        /* TNF Parameters */
        pObj->extDynParams.TNF_A0 = prm->pVnfParams->TNF_A0;  /* Control Param: 0 to 255 */
        pObj->extDynParams.TNF_TM = prm->pVnfParams->TNF_TM;  /* Control Param: 1 to 255 */
        pObj->extDynParams.TNFLuma = prm->pVnfParams->TNFLuma;  /* TNF applied on Luma Only */

    } 
    else
    {
        IMAGE_TUNE_GetVnfParams((ALG_vnfParams*) &pObj->extDynParams.nsf_thr00 );
    }

    /* Changing the Noise Filter Type or Turning on/off */
    switch (prm->mode)
    {
    default:
    case ALG_VNF_MODE_NO_NF:
        pObj->extDynParams.EnableNSF = IKTNF_NO_NF;
        break;
    case ALG_VNF_MODE_ONLY_TNF:
        pObj->extDynParams.EnableNSF = IKTNF_ONLY_TNF;
        break;
    case ALG_VNF_MODE_ONLY_KNF:
        pObj->extDynParams.EnableNSF = IKTNF_ONLY_KNF;
        break;
    case ALG_VNF_MODE_KNF_TNF:
        pObj->extDynParams.EnableNSF = IKTNF_KNF_TNF;
        break;
    case ALG_VNF_MODE_KTNF:
        pObj->extDynParams.EnableNSF = IKTNF_KTNF;
        break;
    }

    OSA_mutexLock(&vnf_imcop_lock);
    OSA_mutexLock(&vicp_imcop_lock);

    /* Call the control function */
    status = VIDENC1_control((VIDENC1_Handle)pObj->hEncode, XDM_SETPARAMS, (IVIDENC1_DynamicParams *)&pObj->extDynParams, (IVIDENC1_Status *)&status);
    if (status != VIDENC1_EOK)
    {
        OSA_ERROR(" ALG:VNF: Dynamic Parameter updation failed !!!\n");
        return OSA_EFAIL;
    }

    //OSA_printf("\n Entering VNF process  \n");
    status = VIDENC1_process(pObj->hEncode, &inBufDesc, &outBufDesc, &inArgs, (IVIDENC1_OutArgs *) & outArgs);
    //OSA_printf("\n Exiting VNF process : %d %d \n", status, VIDENC1_EOK);

    OSA_mutexUnlock(&vicp_imcop_lock);
    OSA_mutexUnlock(&vnf_imcop_lock);

    runStatus->outWidth = outArgs.outWidth;
    runStatus->outHeight = outArgs.outHeight;
    runStatus->outStartX = 0;
    runStatus->outStartY = outArgs.skipOutputLumaRows;

    pObj->inPastAddr = prm->outAddr;

    if (status != VIDENC1_EOK)
    {
        return OSA_EFAIL;
    }

    return OSA_SOK;
}

int ALG_vnfDelete(void *hndl)
{
    ALG_VnfObj *pObj = (ALG_VnfObj *) hndl;
    if (pObj == NULL)
    {
        return OSA_EFAIL;
    }

    if (pObj->hEncode)
    {
        VIDENC1_delete(pObj->hEncode);
    }

    OSA_memFree(pObj);

    return OSA_SOK;
}

Int32 ALG_vnfTestMain_ReadInputData(FILE * finFile, Uint32 frameCount, Uint32 sizeFrame, Uint8 * bufferLuma, Uint8 * bufferChroma)
{
    Uint32 bytesRead = 0;
    Uint32 offset = frameCount * ((sizeFrame * 3) >> 1);

    OSA_printf("\n frameCount = %ld, sizeFrame = %ld  ", frameCount, (sizeFrame * 3) >> 1);
    /*Read the input buffers from FILE */
    fseek(finFile, offset, SEEK_SET);
    bytesRead = fread(bufferLuma, 1, sizeFrame, finFile);
    if (bytesRead != sizeFrame)
    {
        return -1;
    }
    bytesRead = fread(bufferChroma, 1, sizeFrame / 2, finFile);
    if (bytesRead != sizeFrame / 2)
    {
        return -1;
    }

    return(bytesRead);
}


int ALG_vnfTestMain(int argc, char **argv)
{
    FILE   *finFile, *foutFile;
    int     status, i, numFrames = NUMFRAMES;
    Uint8  *inVirtAddr = NULL, *inPhysAddr, *outVirtAddr[2], *outPhysAddr[2], *curOutAddr;
    Uint8  *inBufLuma, *inBufChroma;
    OSA_PrfHndl prfVnf;
    Uint32  size = 4 * MB, sizeFrame;
    void   *algVidEncHndl;
    ALG_VnfObj *pObj;
    ALG_VnfCreate createPrm;
    ALG_VnfRunPrm runPrm;
    ALG_VnfStatus runStatus;
    XDAS_Int32 offset, bufferSize, sysRegBase;
    int curOutBuf=0;

    //status = DRV_init();
    //if(status!=OSA_SOK) {
    //  OSA_ERROR("DRV_init()\n");
    //  return status;
    //}

    status = ALG_sysInit();
    if (status != OSA_SOK)
    {
        DRV_exit();
        return status;
    }
    DM365MM_init();

    sysRegBase = DM365MM_mmap(0x01C40000,0x4000);

    inVirtAddr = OSA_cmemAlloc(size, 32);
    outVirtAddr[0] = OSA_cmemAlloc(size, 32);
    outVirtAddr[1] = OSA_cmemAlloc(size, 32);

    inPhysAddr = OSA_cmemGetPhysAddr(inVirtAddr);
    outPhysAddr[0] = OSA_cmemGetPhysAddr(outVirtAddr[0]);
    outPhysAddr[1] = OSA_cmemGetPhysAddr(outVirtAddr[1]);

    memset(outVirtAddr[0], 0x0, size);
    memset(outVirtAddr[1], 0x0, size);

    if (inVirtAddr == NULL || outVirtAddr[0] == NULL || outVirtAddr[1] == NULL
        || inPhysAddr == NULL || outPhysAddr[0] == NULL || outPhysAddr[1] == NULL)
    {
        OSA_ERROR("OSA_cmemAlloc()\n");
        goto error_exit;
    }

    OSA_prfReset(&prfVnf);

    createPrm.dataFormat = DRV_DATA_FORMAT_YUV420;  //DRV_DATA_FORMAT_YUV422;
    createPrm.width = WIDTH;
    createPrm.height = HEIGHT;
    createPrm.offsetH = LINE_IN_OFFSET_H;
    createPrm.offsetV = LINE_IN_OFFSET_V;
    createPrm.mode = ALG_VNF_MODE_KTNF; //ALG_VNF_MODE_NO_NF, ALG_VNF_MODE_ONLY_TNF, ALG_VNF_MODE_ONLY_KNF, ALG_VNF_MODE_KNF_TNF, ALG_VNF_MODE_KTNF
    createPrm.pVnfParams = NULL;
    createPrm.sysBaseAddr = sysRegBase; // AYK - 0416

    algVidEncHndl = ALG_vnfCreate(&createPrm);
    if (algVidEncHndl == NULL)
    {
        OSA_ERROR("ALG_vnfCreate()\n");
        goto error_exit;
    }
    pObj = (ALG_VnfObj *) algVidEncHndl;

    pObj->inPastAddr = inVirtAddr;
    runPrm.mode = createPrm.mode;
    runPrm.pVnfParams = NULL;

    /* Setting of Run time parameters */
    runPrm.inStartX = 0;
    runPrm.inStartY = 0;
    runPrm.inAddr = inVirtAddr;
    runPrm.mode = createPrm.mode;
    runPrm.pVnfParams = createPrm.pVnfParams;

    for (i = 0; i < numFrames * NUMFRAMESLOOPCOUNT; i++)
    {

        runPrm.outAddr = outVirtAddr[curOutBuf];
        curOutBuf ^= 1;

        /* Input buffers */
        offset = createPrm.offsetH * createPrm.offsetV;
        inBufLuma = (runPrm.inAddr + runPrm.inStartY * createPrm.offsetH + runPrm.inStartX);
        inBufChroma = (runPrm.inAddr + offset + (runPrm.inStartY / 2) * createPrm.offsetH + runPrm.inStartX);

        /* Open input file */
        //finFile = fopen("videoJVC.yuv420", "rb");
        //finFile = fopen("In5_420sp.yuv", "rb");
        //finFile = fopen("foreman_608x480_420sp.yuv", "rb");
        //finFile = fopen("station2_752x512_420sp.yuv", "rb");
        finFile = fopen("station2_1312x752_420sp.yuv", "rb");
        //finFile = fopen("station2_1312x752_pad2_420sp.yuv", "rb");
        //finFile = fopen("station2_1920x1080_420sp.yuv", "rb");
        bufferSize = (createPrm.offsetH * createPrm.height );
        if (!finFile)
        {
            OSA_printf("\n Couldn't open Input file...  ");
            OSA_ERROR("ALG_vnfCreate()\n");
            goto error_exit;
        } else
        {
            /*Read the input buffers from FILE */
            if (ALG_vnfTestMain_ReadInputData(finFile, i % numFrames, bufferSize, inBufLuma, inBufChroma) == -1)
            {
                fclose(finFile);
                OSA_printf("\n File read error...  ");
                OSA_ERROR("ALG_vnfCreate()\n");
                goto error_exit;
            } else
            {
                fclose(finFile);
            }
        }

        /* Example condition to change a parameter dynamically */
#if 0
        if ((i==8) || (i==16))
        {

            if (i==8)
                runPrm.mode = ALG_VNF_MODE_NO_NF;
            if (i==16)
                runPrm.mode = ALG_VNF_MODE_KNF_TNF;

        }
#endif

        OSA_prfBegin(&prfVnf);
        status = ALG_vnfRun(algVidEncHndl, &runPrm, &runStatus);
        OSA_prfEnd(&prfVnf, 1);

        if (status == OSA_SOK)
        {
            OSA_printf(" ALG: Vnf: %d: \n", i);

            /* Open output file */
            if (i > 0)
            {
                foutFile = fopen("output.yuv", "ab+");
            } else
            {
                foutFile = fopen("output.yuv", "wb");
            }

            /* Write the output frame in the file */
            sizeFrame = createPrm.offsetH * runStatus.outHeight;

            /* Writing Luma data */
            curOutAddr = runPrm.outAddr + runStatus.outStartY * createPrm.offsetH;

            fwrite(curOutAddr, 1, sizeFrame, foutFile);
            /* Writing Chroma data */
            curOutAddr = runPrm.outAddr + (createPrm.offsetH * createPrm.offsetV) + runStatus.outStartY/2 * createPrm.offsetH;

            fwrite(curOutAddr, 1, sizeFrame / 2, foutFile);

            fclose(foutFile);

        } else
        {
            OSA_printf(" ALG: VidEnc: ALG_vnfRun() ERROR !!!\n");
            break;
        }
    }                             /* end of processing */

    ALG_vnfDelete(algVidEncHndl);

    OSA_prfPrint(&prfVnf, "VNF", 0);

    error_exit:
    if (inVirtAddr)
        OSA_cmemFree(inVirtAddr);

    if (outVirtAddr[0])
        OSA_cmemFree(outVirtAddr[0]);

    if (outVirtAddr[1])
        OSA_cmemFree(outVirtAddr[1]);

    ALG_sysExit();

    DM365MM_ummap(sysRegBase,0x4000);

    DM365MM_exit();
    return status;
}

ALG_vnfParams gAlg_vnfPrm = 
{
    /* default params  - light box, aperture wide open, 2x isif d-gain, 50% knob */
    26,     //color 0 level 1 offset
    -18,    //color 0 level 1 slope
    74,     //color 0 level 2 offset
    -52,    //color 0 level 2 slope
    37,     //color 0 level 3 offset
    -24,    //color 0 level 3 slope
    35,     //color 1 level 1 offset
    37,     //color 1 level 1 slope
    30,     //color 1 level 2 offset
    24,     //color 1 level 2 slope
    19,     //color 1 level 3 offset
    2,      //color 1 level 3 slope
    25,     //color 2 level 1 offset
    58,     //color 2 level 1 slope
    22,     //color 2 level 2 offset
    30,     //color 2 level 2 slope
    15,     //color 2 level 3 offset
    12,     //color 2 level 3 slope
    2,
    1,      /* Setting of Edge Enhancement parameters */
    26,
    36,
    36,
    3,
    74,
    84,
    94,
    3,
    37,
    47,
    57,
    0,       /* Control Param: 0 to 255 */ /* TNF Parameters */
    17,     /* Control Param: 1 to 255 */
    1        /* 1: TNF applied on Luma Only, 0: TNF applied on both Luma and Chroma */
};

ALG_vnfParams* ImageTune_imgsGetVnfConfig(int mode)
{
    return &gAlg_vnfPrm;
}
