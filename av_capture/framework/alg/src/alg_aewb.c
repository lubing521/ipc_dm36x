#include "alg_aewb_priv.h"
#include "alg_ti_aewb_priv.h"
#include "alg_ti_flicker_detect.h"

#ifdef AR0331_WDR
#include "ae_tiar0331.h"
#include <drv_imgs.h>
#else
#include "ae_ti.h"
#endif

#include "awb_ti.h"
#include "ae_appro.h"
#include "awb_appro.h"

#include "Appro_aewb.h"
#include "TI_aewb.h"

#include <drv_gpio.h>

ALG_AewbObj gALG_aewbObj;
AEW_EXT_PARAM Aew_ext_parameter;

#ifdef AR0331_WDR
#define AE_TI_AE AE_TIAR0331_AE	 
extern void getSensorStatistic();
#else 
extern IAE_Fxns AE_TI_AE;
#endif

int TI_fd_sequence(int sensorMode, void* h3aDataVirtAddr, 
        int h3a_w, int h3a_h, int saldre, 
        IAEWB_Ae *curAe, IAEWB_Ae *nextAe, int env_50_60Hz,
        int *fd_mem, int fd_mem_size);

#define GIO_AUTO_IRIS	(61)

#define FD_DEBUG_MSG     // test by fan


typedef struct {
    Uint32 awbNumWinH;
    Uint32 awbNumWinV;
    Uint32 aewbNumSPCInWin;
    Uint32 awbMiscData[16];
    IAEWB_Rgb *awbRgbData;
    OSA_MutexHndl statusLock;

}ALG_AewbData_AwbSrc;

static ALG_AewbData_AwbSrc gITTAwb;

typedef struct {
    int y;
    int u;
    int v;
}IAWB_Yuv;

void ALG_aewbConvert_RGB_YUV(IAEWB_Rgb *rgbData, int pix_in_pax, int awb_h3a_paxels, IAEWB_Rgb *norm_rgbData, IAWB_Yuv *yuvData);

static int *g_fd_mem = NULL; //algorithm persistent memory
static int g_fd_mem_size = 6*1024; //algorithm persistent memory
static int aewbFrames = 0;
static IAEWB_Rgb *rgbData = NULL;
static aewDataEntry *aew_data = NULL;
static int env_50_60Hz = -1;
static AWB_PARAM ipipe_awb_gain = {
    200,
    256,
    256,
    350,
    256
};

int ALG_aewbPlatformCheck( void )
{
    FILE *pfile = NULL;
    char tempbuff[100];
    char filename[]="/proc/version";
    int	 ret = 0;
    char *pStr = NULL;

    pfile = fopen(filename,"r");
    if( pfile == NULL )
    {
        ret = -1;
        goto CHECK_END;
    }

    if( fread(tempbuff, sizeof(tempbuff),1,pfile) <= 0 )
    {
        ret = -1;
        goto CHECK_END;
    }

    tempbuff[sizeof(tempbuff)-1]='\0';

    /*Linux version 2.6.10_mvl401_IPNC_1.0.0.0 (root@localhost.localdomain) (gcc ve.....*/

    pStr = strstr(tempbuff,"IPNC");
    if( pStr != NULL )
    {
        ret = 1;
        goto CHECK_END;
    }
    pStr = strstr(tempbuff,"ipnc");
    if( pStr != NULL )
    {
        ret = 1;
        goto CHECK_END;
    }

    pStr = strstr(tempbuff,"EVM");
    if( pStr != NULL )
    {
        ret = 0;
        goto CHECK_END;
    }
    pStr = strstr(tempbuff,"evm");
    if( pStr != NULL )
    {
        ret = 0;
        goto CHECK_END;
    }


CHECK_END:

    if( pStr )
    {
        fprintf(stderr,"%s \n",pStr);
    }
    if( pfile )
    {
        fclose(pfile);
    }
    return ret;
}

int ALG_aewbCheckAutoIris(void)
{
    int ret = 0;

    DRV_gpioSetMode(GIO_AUTO_IRIS, CSL_GPIO_INPUT);

    if( ALG_aewbPlatformCheck() == 1 )
    {
        ret = DRV_gpioGet(GIO_AUTO_IRIS);
        OSA_printf("IPNC AUTO_IRIS = %d \n",!ret);
        return !ret;
    }
    else {
        ret = MANUAL_IRIS;
        OSA_printf("EVM AUTO_IRIS = %d \n",ret);
        return ret;
    }
}

void* ALG_aewbCreate(ALG_AewbCreate *create)
{
    IAE_Params aeParams;
    IAWB_Params awbParams;

    int numMem;
    int retval;
    int sensorMode;

    memset(&gALG_aewbObj, 0, sizeof(gALG_aewbObj));
    memset(&Aew_ext_parameter, 0, sizeof(Aew_ext_parameter));

    rgbData = calloc(sizeof(IAEWB_Rgb), create->pH3aInfo->aewbNumWinH * create->pH3aInfo->aewbNumWinV);
    aew_data = calloc(sizeof(aewDataEntry), (create->pH3aInfo->aewbNumWinH * create->pH3aInfo->aewbNumWinV + 7) >> 3);
    g_fd_mem = calloc(sizeof(int), g_fd_mem_size);

    sensorMode = (create->sensorMode & 0xFF);

    gALG_aewbObj.vsEnable 		= (create->sensorMode & DRV_IMGS_SENSOR_MODE_VSTAB) ? 1 : 0;
    gALG_aewbObj.aewbVendor   	= create->aewbVendor;
    gALG_aewbObj.reduceShutter   	= create->reduceShutter;
    gALG_aewbObj.saldre   		= create->saldre;
     // test by fan
      OSA_printf(" ****fan2:ALG_aewbCreate starting***** \n");
	   // aew thread start from here
	   // 很多初始化的参数要改动
	 
   Aew_ext_parameter.GAIN_SETUP            = ALG_aewbSetSensorGain        ;
	  
    Aew_ext_parameter.SHUTTER_SETUP         = ALG_aewbSetSensorExposure    ;
	//上面两个是回调函数，如图像偏暗或偏量，可适当调整这里的参数。
    Aew_ext_parameter.AWB_SETUP             = ALG_aewbSetIpipeWb           ;
    Aew_ext_parameter.DCSUB_SETUP           = ALG_aewbSetSensorDcsub       ;
    Aew_ext_parameter.BIN_SETUP             = ALG_aewbSetSensorBin         ;
    Aew_ext_parameter.RGB2RGB_SETUP         = ALG_aewbSetRgb2Rgb           ;
    Aew_ext_parameter.MEDIAN_SETUP          = ALG_aewbSetOtfCorrect        ;
    Aew_ext_parameter.EDGE_SETUP            = ALG_aewbSetEdgeEnhancement   ;
    //Aew_ext_parameter.BRT_CTR_SET           = ALG_aewbSetContrastBrightness;
    Aew_ext_parameter.BINNING_SKIP_SETUP    = ALG_aewbSetSensorBinSkip     ;
    Aew_ext_parameter.ENV_50_60HZ_SETUP     = ALG_aewbSetSensor50_60Hz     ;
    Aew_ext_parameter.SENSOR_FRM_RATE_SETUP = ALG_aewbSetSensorFrameRate   ;

    Aew_ext_parameter.H3ABuffer         = NULL;
    Aew_ext_parameter.sensor_frame_rate = 0;
    Aew_ext_parameter.ipipe_dgain_base  = 1;
    Aew_ext_parameter.special_mode      = 0;
    Aew_ext_parameter.auto_iris         = ALG_aewbCheckAutoIris();
    Aew_ext_parameter.day_night         = AE_DAY;
    Aew_ext_parameter.awb_mode          = AWB_AUTO;
    Aew_ext_parameter.saturation        =  100;  //128;  by fan
    Aew_ext_parameter.blc               = BACKLIGHT_NORMAL;
    Aew_ext_parameter.sharpness         = 110 ; //128;
    Aew_ext_parameter.brightness        = 140; //128;
    Aew_ext_parameter.contrast          =  135 ;//128;
    Aew_ext_parameter.aew_enable        = AEW_ENABLE;
    Aew_ext_parameter.env_50_60Hz       = create->flickerType;
    Aew_ext_parameter.binning_mode      = create->aewbBinEnable?SENSOR_BINNING:SENSOR_SKIP;

    Aew_ext_parameter.MEDIAN_SETUP(1);

    gITTAwb.awbNumWinH = create->pH3aInfo->aewbNumWinH;
    gITTAwb.awbNumWinV = create->pH3aInfo->aewbNumWinV;
    gITTAwb.aewbNumSPCInWin= create->pH3aInfo->aewbNumSamplesPerColorInWin;
    gITTAwb.awbRgbData = rgbData;
    OSA_mutexCreate(&gITTAwb.statusLock);

    if(create->aewbVendor == ALG_AEWB_ID_APPRO) {
        //Initial AE
        gALG_aewbObj.weight = APPRO_WEIGHTING_MATRIX;
        aeParams.size = sizeof(aeParams);
        aeParams.numHistory = 0;
        aeParams.numSmoothSteps = 1;
        numMem = AE_APPRO_AE.ialg.algAlloc((IALG_Params *)&aeParams, NULL, gALG_aewbObj.memTab_ae);
        while(numMem > 0){
            gALG_aewbObj.memTab_ae[numMem-1].base = malloc(gALG_aewbObj.memTab_ae[numMem-1].size);
            numMem --;
        }

        gALG_aewbObj.handle_ae = (IALG_Handle)gALG_aewbObj.memTab_ae[0].base;
        retval = AE_APPRO_AE.ialg.algInit(gALG_aewbObj.handle_ae, gALG_aewbObj.memTab_ae, NULL, (IALG_Params *)&aeParams);
        if(retval == -1) {
            OSA_ERROR("AE_APPRO_AE.ialg.algInit()\n");
            return NULL;
        }

        //Initial AWB
        awbParams.size = sizeof(awbParams);
        awbParams.numHistory = 0;
        awbParams.numSmoothSteps = 1;
        numMem = AWB_APPRO_AWB.ialg.algAlloc((IALG_Params *)&awbParams, NULL, gALG_aewbObj.memTab_awb);
        while(numMem > 0){
            gALG_aewbObj.memTab_awb[numMem-1].base = malloc(gALG_aewbObj.memTab_awb[numMem-1].size);
            numMem --;
        }

        gALG_aewbObj.handle_awb = (IALG_Handle)gALG_aewbObj.memTab_awb[0].base;
        retval = AWB_APPRO_AWB.ialg.algInit(gALG_aewbObj.handle_awb, gALG_aewbObj.memTab_awb, NULL, (IALG_Params *)&awbParams);
        if(retval == -1) {
            OSA_ERROR("AWB_APPRO_AWB.ialg.algInit()\n");
            return NULL;
        }

        gALG_aewbObj.IAEWB_StatMatdata.winCtVert  = create->pH3aInfo->aewbNumWinV;
        gALG_aewbObj.IAEWB_StatMatdata.winCtHorz  = create->pH3aInfo->aewbNumWinH;
        gALG_aewbObj.IAEWB_StatMatdata.pixCtWin   = create->pH3aInfo->aewbNumSamplesPerColorInWin;

        gALG_aewbObj.AE_OutArgs.nextAe.exposureTime = 33333;
        gALG_aewbObj.AE_OutArgs.nextAe.apertureLevel = 1;
        gALG_aewbObj.AE_OutArgs.nextAe.ipipeGain = 1024;
        gALG_aewbObj.AE_OutArgs.nextAe.sensorGain = 1231;
 

        if(create->vnfDemoCfg)
        {
            IAE_DynamicParam.exposureTimeStepSize = 20;
            IAE_DynamicParam.targetBrightness = 100;
            IAE_DynamicParam.targetBrightnessRange.min = 90;
            IAE_DynamicParam.targetBrightnessRange.max = 110;
            memcpy( (void *)&IAE_DynamicParam.exposureTimeRange,
                    (void *)&APPRO_shutter_List_NF,
                    sizeof(IAE_DynamicParam.exposureTimeRange) );

            memcpy( (void *)&IAE_DynamicParam.sensorGainRange,
                    (void *)&APPRO_agc_List_NF,
                    sizeof(IAE_DynamicParam.sensorGainRange) );

            memcpy( (void *)&IAE_DynamicParam.ipipeGainRange,
                    (void *)&APPRO_dgain_List_NF,
                    sizeof(IAE_DynamicParam.ipipeGainRange) );

        }else if(sensorMode <= DRV_IMGS_SENSOR_MODE_800x600) {

            memcpy( (void *)&IAE_DynamicParam.exposureTimeRange,
                    (void *)&APPRO_shutter_List_480P_Bin,
                    sizeof(IAE_DynamicParam.exposureTimeRange) );

            memcpy( (void *)&IAE_DynamicParam.sensorGainRange,
                    (void *)&APPRO_agc_List_480P,
                    sizeof(IAE_DynamicParam.sensorGainRange) );

            memcpy( (void *)&IAE_DynamicParam.ipipeGainRange,
                    (void *)&APPRO_dgain_List_480P,
                    sizeof(IAE_DynamicParam.ipipeGainRange) );

        } else {

            memcpy( (void *)&IAE_DynamicParam.exposureTimeRange,
                    (void *)&APPRO_shutter_List_720P,
                    sizeof(APPRO_shutter_List_720P) );

            memcpy( (void *)&IAE_DynamicParam.sensorGainRange,
                    (void *)&APPRO_agc_List_720P,
                    sizeof(APPRO_agc_List_720P) );

            memcpy( (void *)&IAE_DynamicParam.ipipeGainRange,
                    (void *)&APPRO_dgain_List_720P,
                    sizeof(APPRO_dgain_List_720P) );
        }

        memcpy( (void *)&gALG_aewbObj.AE_InArgs.statMat,
                (void *)&gALG_aewbObj.IAEWB_StatMatdata,
                sizeof(IAEWB_StatMat) );

        memcpy( (void *)&gALG_aewbObj.AWB_InArgs.statMat,
                (void *)&gALG_aewbObj.IAEWB_StatMatdata,
                sizeof(IAEWB_StatMat) );


        IAE_DynamicParam.size = sizeof(IAE_DynamicParams);
        retval = AE_APPRO_AE.control((IAE_Handle)gALG_aewbObj.handle_ae, IAE_CMD_SET_CONFIG, &IAE_DynamicParam, NULL);
        if(retval == -1) {
            OSA_ERROR("AE_APPRO_AE.control()\n");
            return NULL;
        }

        IAWB_DynamicParam.size = sizeof(IAWB_DynamicParams);
        retval = AWB_APPRO_AWB.control((IAWB_Handle)gALG_aewbObj.handle_awb, IAWB_CMD_SET_CONFIG, &IAWB_DynamicParam, NULL);
        if(retval == -1) {
            OSA_ERROR("AWB_APPRO_AWB.control()\n");
            return NULL;
        }

        CONTROL_DRIVER_initial(
                (IAE_Handle)gALG_aewbObj.handle_ae,
                (IAWB_Handle)gALG_aewbObj.handle_awb,
                &gALG_aewbObj.AE_OutArgs,
                &gALG_aewbObj.AWB_OutArgs
                );
    }
    else if(create->aewbVendor == ALG_AEWB_ID_TI) {
        TI_2A_init_tables(create->pH3aInfo->aewbNumWinH, create->pH3aInfo->aewbNumWinV);
        //Initial AE
        gALG_aewbObj.weight = TI_WEIGHTING_MATRIX;
        aeParams.size = sizeof(aeParams);
        aeParams.numHistory = 10;
        aeParams.numSmoothSteps = 6;
        numMem = AE_TI_AE.ialg.algAlloc((IALG_Params *)&aeParams, NULL, gALG_aewbObj.memTab_ae);
        while(numMem > 0){
            gALG_aewbObj.memTab_ae[numMem-1].base = malloc(gALG_aewbObj.memTab_ae[numMem-1].size);
            numMem --;
        }

        gALG_aewbObj.handle_ae = (IALG_Handle)gALG_aewbObj.memTab_ae[0].base;
        retval = AE_TI_AE.ialg.algInit(gALG_aewbObj.handle_ae, gALG_aewbObj.memTab_ae, NULL, (IALG_Params *)&aeParams);
        if(retval == -1) {
            OSA_ERROR("AE_TI_AE.ialg.algInit()\n");
            return NULL;
        }

        //Initial AWB
        awbParams.size = sizeof(awbParams);
        awbParams.numHistory = 6;
        numMem = AWB_TI_AWB.ialg.algAlloc((IALG_Params *)&awbParams, NULL, gALG_aewbObj.memTab_awb);
        while(numMem > 0){
            gALG_aewbObj.memTab_awb[numMem-1].base = malloc(gALG_aewbObj.memTab_awb[numMem-1].size);
            numMem --;
        }

        gALG_aewbObj.handle_awb = (IALG_Handle)gALG_aewbObj.memTab_awb[0].base;
        retval = AWB_TI_AWB.ialg.algInit(gALG_aewbObj.handle_awb, gALG_aewbObj.memTab_awb, NULL, (IALG_Params *)&awbParams);
        if(retval == -1) {
            OSA_ERROR("AWB_TI_AWB.ialg.algInit()\n");
            return NULL;
        }

        gALG_aewbObj.IAEWB_StatMatdata.winCtVert  = create->pH3aInfo->aewbNumWinV;
        gALG_aewbObj.IAEWB_StatMatdata.winCtHorz  = create->pH3aInfo->aewbNumWinH;
        gALG_aewbObj.IAEWB_StatMatdata.pixCtWin   = create->pH3aInfo->aewbNumSamplesPerColorInWin;

        retval = TI_2A_config(1, create->saldre);

        if(retval == -1) {
            return NULL;
        }

        TI_2A_SetEEValues(create->shiftValue);

    }

    return &gALG_aewbObj;
}

int TI_2A_config(int flicker_detection, int saldre)
{
    IAE_DynamicParams aeDynamicParams;
    int i;
    int retval;

    i = 0; aewbFrames = 0;

#ifdef FD_DEBUG_MSG
    OSA_printf("\n\n\n\n\nTI_2A_config: flicker detection = %d\n\n", flicker_detection);
    OSA_printf("Aew_ext_parameter.env_50_60Hz = %d\n", Aew_ext_parameter.env_50_60Hz);
#endif

    /* set stepSize based on input from Flicker detectiom and PAL/NTSC environment */
    int step_size = 8333, min_exp = 200;
    if (Aew_ext_parameter.env_50_60Hz == VIDEO_NTSC)
    {
        step_size = 8333;
        if (flicker_detection == 3)
        {
            //min_exp = 8333;
        }
    }
    else if (Aew_ext_parameter.env_50_60Hz == VIDEO_PAL)
    {
        step_size = 8333; // 10000; by fan
        if (flicker_detection == 2)
        {
            //min_exp = 10000;
        }
    }
    else if (Aew_ext_parameter.env_50_60Hz == VIDEO_NONE)
    {
        step_size = 0;
    }

#ifdef FD_DEBUG_MSG
    OSA_printf("min_exp = %d, step_size = %d final\n", min_exp, step_size);
#endif


    TI_2A_AE_config(&aeDynamicParams, min_exp, step_size);

    memcpy( (void *)&gALG_aewbObj.AE_InArgs.statMat,
            (void *)&gALG_aewbObj.IAEWB_StatMatdata,
            sizeof(IAEWB_StatMat) );

    memcpy( (void *)&gALG_aewbObj.AWB_InArgs.statMat,
            (void *)&gALG_aewbObj.IAEWB_StatMatdata,
            sizeof(IAEWB_StatMat) );

    retval = AE_TI_AE.control((IAE_Handle)gALG_aewbObj.handle_ae, IAE_CMD_SET_CONFIG, &aeDynamicParams, NULL);
    if(retval == -1) {
        OSA_ERROR("AE_TI_AE.control()\n");
        return retval;
    }

    /* Pass calibration data to TI AWB */
    retval = IMAGE_TUNE_GetAwbParams(&awb_calc_data);   
	//上面要改动如颜色偏色，就改这个表。
    retval = AWB_TI_AWB.control((IAWB_Handle)gALG_aewbObj.handle_awb, TIAWB_CMD_CALIBRATION, &awb_calc_data, NULL);
    if(retval == -1) {
        OSA_ERROR("AWB_TI_AWB.control()\n");
        return retval;
    }

    return 0;
}

static void GETTING_RGB_BLOCK_VALUE(unsigned short * BLOCK_DATA_ADDR,IAEWB_Rgb *rgbData, aewDataEntry *aew_data, int shift)
{
    unsigned short i,j,k, numWin, idx1, idx2;
    Uint8 *curAewbAddr;
    CSL_H3aAewbOutUnsatBlkCntOverlay *pAewbUnsatBlk;
    CSL_H3aAewbOutSumModeOverlay *pAewbWinData;
    int accValue[4];
    int aew_win_vt_cnt = gALG_aewbObj.IAEWB_StatMatdata.winCtVert;
    int aew_win_hz_cnt = gALG_aewbObj.IAEWB_StatMatdata.winCtHorz;

    curAewbAddr = (Uint8*)BLOCK_DATA_ADDR;
    numWin=0;

    accValue[0]=accValue[1]=accValue[2]=accValue[3]=0;

    for(i=0;i<aew_win_vt_cnt; i++) {
        for(j=0;j<aew_win_hz_cnt; j++) {

            pAewbWinData = (CSL_H3aAewbOutSumModeOverlay *)curAewbAddr;

            idx1 = numWin/8;
            idx2 = numWin%8;

            aew_data[idx1].window_data[idx2][0] = pAewbWinData->subSampleAcc[0];
            aew_data[idx1].window_data[idx2][1] = pAewbWinData->subSampleAcc[1];
            aew_data[idx1].window_data[idx2][2] = pAewbWinData->subSampleAcc[2];
            aew_data[idx1].window_data[idx2][3] = pAewbWinData->subSampleAcc[3];

            accValue[0] += pAewbWinData->subSampleAcc[0];
            accValue[1] += pAewbWinData->subSampleAcc[1];
            accValue[2] += pAewbWinData->subSampleAcc[2];
            accValue[3] += pAewbWinData->subSampleAcc[3];

            curAewbAddr += sizeof(CSL_H3aAewbOutSumModeOverlay);

            numWin++;

            if(numWin%8==0) {
                pAewbUnsatBlk = (CSL_H3aAewbOutUnsatBlkCntOverlay*)curAewbAddr;

                for(k=0; k<8;k++)
                    aew_data[idx1].unsat_block_ct[k] = pAewbUnsatBlk->unsatCount[k];

                curAewbAddr += sizeof(CSL_H3aAewbOutUnsatBlkCntOverlay);
            }
        }
        curAewbAddr = (Uint8*)OSA_align( (Uint32)curAewbAddr, 32);
    }

    OSA_mutexLock(&gITTAwb.statusLock);  //for ITT - rgb data cpy yo ITT
    for(i = 0; i < (aew_win_hz_cnt * aew_win_vt_cnt)>>3;i ++){
        for(j = 0; j < 8; j ++){
            rgbData[i * 8 + j].r = aew_data[i].window_data[j][1] >> shift;
            rgbData[i * 8 + j].b = aew_data[i].window_data[j][2] >> shift;
            rgbData[i * 8 + j].g = (aew_data[i].window_data[j][0]
                    + aew_data[i].window_data[j][3]+ 1) >> (1 + shift) ;
        }
    }
    OSA_mutexUnlock(&gITTAwb.statusLock);

    accValue[0] /= numWin*gALG_aewbObj.IAEWB_StatMatdata.pixCtWin;
    accValue[1] /= numWin*gALG_aewbObj.IAEWB_StatMatdata.pixCtWin;
    accValue[2] /= numWin*gALG_aewbObj.IAEWB_StatMatdata.pixCtWin;
    accValue[3] /= numWin*gALG_aewbObj.IAEWB_StatMatdata.pixCtWin;

#ifdef ALG_AEWB_DEBUG
    OSA_printf(" AEWB: Avg Color: %5d, %5d, %5d, %5d\n", accValue[0], accValue[1], accValue[2], accValue[3]);
#endif
}

void AEW_SETUP_CONTROL( CONTROL3AS *CONTROL3A )
{
    CONTROL3A->IMAGE_SHARPNESS  = Aew_ext_parameter.sharpness;
    CONTROL3A->IMAGE_CONTRAST   = Aew_ext_parameter.contrast;
    CONTROL3A->IMAGE_BRIGHTNESS = Aew_ext_parameter.brightness;
    CONTROL3A->IMAGE_SATURATION = Aew_ext_parameter.saturation;
    CONTROL3A->IMAGE_BACKLIGHT  = Aew_ext_parameter.blc;
    CONTROL3A->INDOUTDOOR       = Aew_ext_parameter.awb_mode;
    CONTROL3A->VIDEO_MODE       = Aew_ext_parameter.env_50_60Hz;
    CONTROL3A->AUTO_IRIS        = Aew_ext_parameter.auto_iris;
    CONTROL3A->DAY_NIGHT        = Aew_ext_parameter.day_night;

    if( Aew_ext_parameter.binning_mode == SENSOR_BINNING )
        CONTROL3A->SKIP_BINNING_MODE = 0;
    else
        CONTROL3A->SKIP_BINNING_MODE = 1;

    if( Aew_ext_parameter.aew_enable == AEW_ENABLE )
        CONTROL3A->PAUSE_AWWB = 0;
    else
        CONTROL3A->PAUSE_AWWB = 1;

    if(CONTROL3A->IMAGE_BACKLIGHT==BACKLIGHT_LOW ||
            CONTROL3A->IMAGE_BACKLIGHT==BACKLIGHT_LOW2 )
    {
        if(gALG_aewbObj.aewbVendor==ALG_AEWB_ID_APPRO) {
            gALG_aewbObj.weight= APPRO_WEIGHTING_MATRIX;
        }
        else   if(gALG_aewbObj.aewbVendor==ALG_AEWB_ID_TI) {
            gALG_aewbObj.weight= TI_WEIGHTING_MATRIX;
        }
    }
    else if(CONTROL3A->IMAGE_BACKLIGHT==BACKLIGHT_HIGH ||
            CONTROL3A->IMAGE_BACKLIGHT==BACKLIGHT_HIGH2 )
    {
        if(gALG_aewbObj.aewbVendor==ALG_AEWB_ID_APPRO) {
            gALG_aewbObj.weight=APPRO_WEIGHTING_SPOT;
        }
        else   if(gALG_aewbObj.aewbVendor==ALG_AEWB_ID_TI) {
            gALG_aewbObj.weight=TI_WEIGHTING_SPOT;
        }
    }
    else
    {
        if(gALG_aewbObj.aewbVendor==ALG_AEWB_ID_APPRO) {
            gALG_aewbObj.weight=APPRO_WEIGHTING_CENTER;
        }
        else   if(gALG_aewbObj.aewbVendor==ALG_AEWB_ID_TI) {
            gALG_aewbObj.weight=TI_WEIGHTING_CENTER;
        }
    }

    /* 50/60Hz switch & brightness & contrast support for TI 2A */
    if(gALG_aewbObj.aewbVendor==ALG_AEWB_ID_TI) {
        ALG_aewbSetBrightness(CONTROL3A->IMAGE_BRIGHTNESS);
        ALG_aewbSetContrast(CONTROL3A->IMAGE_CONTRAST >> 3);
        
        if(env_50_60Hz != CONTROL3A->VIDEO_MODE) {
            env_50_60Hz = CONTROL3A->VIDEO_MODE;

			///zeng add 2013-09-22
			Aew_ext_parameter.ENV_50_60HZ_SETUP(env_50_60Hz);
			
            if (env_50_60Hz == VIDEO_NTSC)
            {
                TI_2A_config(1, gALG_aewbObj.saldre);
            }
            else if (env_50_60Hz == VIDEO_PAL)
            {
                TI_2A_config(1, gALG_aewbObj.saldre);
            }
            else
            {
                TI_2A_config(0, gALG_aewbObj.saldre);
            }
        }
    }
}

static void TIAWB_applySettings(IAEWB_Wb* nextWb)
{
    AWB_PARAM awb_gain;
    awb_gain.rGain = nextWb->rGain >> 3;
    awb_gain.grGain = nextWb->gGain >> 3;
    awb_gain.gbGain = nextWb->gGain >> 3;
    awb_gain.bGain = nextWb->bGain >> 3;
#ifdef _AEWB_DEBUG_PRINT_
    printf("AEWB debug: r=%d, g=%d, b=%d\n", nextWb->rGain, nextWb->gGain, nextWb->bGain);
#endif
    ALG_aewbSetIpipeWb2(&awb_gain);
}

static void TIAE_applySettings(IAEWB_Ae *curAe, IAEWB_Ae *nextAe, int actStep, int step)
{
    if (step == actStep)
    {
#ifdef _AEWB_DEBUG_PRINT_
        if (       (nextAe->sensorGain != curAe->sensorGain)
                || (nextAe->exposureTime != curAe->exposureTime)
                || (nextAe->ipipeGain != curAe->ipipeGain)
           )
        {
            printf("AEWB debug: T=(%d->%d), AG=(%d->%d), DG=(%d->%d)\n",
                    (int)curAe->exposureTime,
                    (int)nextAe->exposureTime,
                    (int)curAe->sensorGain,
                    (int)nextAe->sensorGain,
                    (int)curAe->ipipeGain,
                    (int)nextAe->ipipeGain);
        }
#endif
        int AG = nextAe->sensorGain;
        int DG = nextAe->ipipeGain;
        int EX = nextAe->exposureTime;
        ALG_aewbSetExposureGain(EX, AG, DG, 1);
    }
    else
    {
        ALG_aewbSetExposureGain(0, 0, 0, 0);
    }
}

void TI2AFunc(void *pAddr)
{
    int i = 0, retval = OSA_SOK ;
    CONTROL3AS TI_Control3A;
    EDGE_PARAM EE_Param;
    
#ifdef AR0331_WDR
    getSensorStatistics();
#endif
    GETTING_RGB_BLOCK_VALUE(pAddr, rgbData, aew_data, 2);

    /* Xiangdong: we need a flag from the tuning serser to signal to the AWB thread that a new set of
       calibration data has been created by the tuning tool and need to be used,
       the following code needs to be enabled for to pass new tuning data in */
    if(IMAGE_TUNE_CmdGetAwbPrmStatus(&i) )
    {
        retval = IMAGE_TUNE_GetAwbParams(&awb_calc_data);
        retval = AWB_TI_AWB.control((IAWB_Handle)gALG_aewbObj.handle_awb, TIAWB_CMD_CALIBRATION, &awb_calc_data, NULL);
        IMAGE_TUNE_CmdSetAwbPrmStatus(0); //reset flag
    }

    if (Aew_ext_parameter.aew_enable == AEW_ENABLE)
    {
        //AE
        if (gALG_aewbObj.aewbType == ALG_AEWB_AE || gALG_aewbObj.aewbType == ALG_AEWB_AEWB)
        {
            if (aewbFrames % NUM_STEPS == 0) 
            {
                gALG_aewbObj.AE_InArgs.curAe.apertureLevel = 1;
                gALG_aewbObj.AE_InArgs.curAe.exposureTime = gALG_aewbObj.AE_OutArgs.nextAe.exposureTime;
                gALG_aewbObj.AE_InArgs.curAe.sensorGain = gALG_aewbObj.AE_OutArgs.nextAe.sensorGain;
                gALG_aewbObj.AE_InArgs.curAe.ipipeGain = gALG_aewbObj.AE_OutArgs.nextAe.ipipeGain;
                gALG_aewbObj.AE_InArgs.curWb.rGain = gALG_aewbObj.AWB_OutArgs.nextWb.rGain; 
                gALG_aewbObj.AE_InArgs.curWb.gGain = gALG_aewbObj.AWB_OutArgs.nextWb.gGain; 
                gALG_aewbObj.AE_InArgs.curWb.bGain = gALG_aewbObj.AWB_OutArgs.nextWb.bGain; 

                AE_TI_AE.process(
                        (IAE_Handle)gALG_aewbObj.handle_ae,
                        &gALG_aewbObj.AE_InArgs,
                        &gALG_aewbObj.AE_OutArgs,
                        rgbData,
                        gALG_aewbObj.weight,
                        NULL);
                ipipe_awb_gain.dGain = gALG_aewbObj.AE_OutArgs.nextAe.ipipeGain>> 2;
            }
            else
            {
                TIAE_applySettings(&gALG_aewbObj.AE_InArgs.curAe, &gALG_aewbObj.AE_OutArgs.nextAe, 1, aewbFrames % NUM_STEPS);
            }
        }
        else if (aewbFrames % NUM_STEPS == 0)
        {
            gALG_aewbObj.AE_OutArgs.nextAe = gALG_aewbObj.AE_InArgs.curAe;
        }

        //AWB
        if ((gALG_aewbObj.aewbType == ALG_AEWB_AEWB) && (aewbFrames % NUM_STEPS == NUM_STEPS-1))
        {
            gALG_aewbObj.AWB_InArgs.curWb = gALG_aewbObj.AE_InArgs.curWb;
            gALG_aewbObj.AWB_InArgs.curAe = gALG_aewbObj.AE_InArgs.curAe;

            AWB_TI_AWB.process(
                    (IAWB_Handle)gALG_aewbObj.handle_awb,
                    &gALG_aewbObj.AWB_InArgs,
                    &gALG_aewbObj.AWB_OutArgs,
                    rgbData,
                    NULL);
            ipipe_awb_gain.rGain = gALG_aewbObj.AWB_OutArgs.nextWb.rGain >> 3;
            ipipe_awb_gain.grGain = gALG_aewbObj.AWB_OutArgs.nextWb.gGain >> 3;
            ipipe_awb_gain.gbGain = gALG_aewbObj.AWB_OutArgs.nextWb.gGain >> 3;
            ipipe_awb_gain.bGain = gALG_aewbObj.AWB_OutArgs.nextWb.bGain >> 3;

            TIAWB_applySettings(&gALG_aewbObj.AWB_OutArgs.nextWb);
        }

        //ISP
        if ( (gALG_aewbObj.aewbType == ALG_AEWB_AWB || gALG_aewbObj.aewbType == ALG_AEWB_AEWB) &&
                gALG_aewbObj.AE_OutArgs.nextAe.exposureTime == gALG_aewbObj.AE_InArgs.curAe.exposureTime &&
                gALG_aewbObj.AE_OutArgs.nextAe.sensorGain == gALG_aewbObj.AE_InArgs.curAe.sensorGain)
        {
            int i = TI_2A_ISP_control(gALG_aewbObj.AE_OutArgs.nextAe.exposureTime,
                    gALG_aewbObj.AE_OutArgs.nextAe.sensorGain,
                    gALG_aewbObj.AE_OutArgs.nextAe.ipipeGain,
                    gALG_aewbObj.AWB_OutArgs.nextWb.colorTemp);
#ifdef _AEWB_DEBUG_PRINT_
            {
                static int ii = -1;
                if (ii != i)
                {
                    printf("AEWB debug: color temp=%d (%d, %d, %d), id=%d\n",
                            gALG_aewbObj.AWB_OutArgs.nextWb.colorTemp,
                            ipipe_awb_gain.rGain, ipipe_awb_gain.grGain, ipipe_awb_gain.bGain, i);
                }
                ii = i;
            }
#endif
        }

        if (aewbFrames % NUM_STEPS == 0)
        {
            AEW_SETUP_CONTROL( &TI_Control3A );
            if (gALG_aewbObj.AE_InArgs.curAe.sensorGain < 10000)
            {
                EE_Param.es_gain = Aew_ext_parameter.sharpness - 20;  
            }
            else
            {
                EE_Param.es_gain = 125 - (gALG_aewbObj.AE_InArgs.curAe.sensorGain - 10000) / 400;  
            }
            
            ALG_aewbSetEdgeEnhancement(&EE_Param);
        }
        ALG_aewbSetDayNight();
    }

    /* remove the count and put it into the process */
    aewbFrames ++;
}

short ALG_aewbDummy(int setval)
{
    return 0;
}

void Appro3AFunc(void *pAddr)
{
    CONTROL3AS Appro_Control3A;
    static int firstApproflg = 1;
    static IAWB_InArgs   AWB_InArgs;
    static IAWB_OutArgs  AWB_OutArgs;

    if( firstApproflg )
    {
        firstApproflg = 0;
        AWB_InArgs	= gALG_aewbObj.AWB_InArgs;
    }

    GETTING_RGB_BLOCK_VALUE(pAddr, rgbData, aew_data,0);

    /* remove the count and put it into the process */
    if(Aew_ext_parameter.aew_enable == AEW_ENABLE )
    {

        gALG_aewbObj.AE_InArgs.curWb = gALG_aewbObj.AWB_OutArgs.nextWb;
        gALG_aewbObj.AWB_InArgs.curWb = gALG_aewbObj.AWB_OutArgs.nextWb;

        AE_APPRO_AE.process(
                (IAE_Handle)gALG_aewbObj.handle_ae,
                &gALG_aewbObj.AE_InArgs,
                &gALG_aewbObj.AE_OutArgs,
                rgbData,
                gALG_aewbObj.weight,
                NULL
                );

        gALG_aewbObj.AE_InArgs.curAe = gALG_aewbObj.AE_OutArgs.nextAe;
        gALG_aewbObj.AWB_InArgs.curAe = gALG_aewbObj.AE_OutArgs.nextAe;

        AWB_APPRO_AWB.process(
                (IAWB_Handle)gALG_aewbObj.handle_awb,
                &gALG_aewbObj.AWB_InArgs,
                &gALG_aewbObj.AWB_OutArgs,
                rgbData,
                aew_data
                );

#ifdef ALG_AEWB_DEBUG
        OSA_printf(" ALG: Aewb: Exposure = %ld, Gain = %ld\n", gALG_aewbObj.AE_OutArgs.nextAe.exposureTime, gALG_aewbObj.AE_OutArgs.nextAe.sensorGain);
#endif

        // If 2A algo is replacd, recommednd to replace this control of shutter also
        // replacement ths function name
        AEW_SETUP_CONTROL( &Appro_Control3A );

        CONTROL_DRIVER_process(
                (IAE_Handle)gALG_aewbObj.handle_ae,
                (IAWB_Handle)gALG_aewbObj.handle_awb,
                &gALG_aewbObj.AE_OutArgs,
                &gALG_aewbObj.AWB_OutArgs,
                &Appro_Control3A
                );
    }
}

int ALG_aewbRun(void *hndl, ALG_AewbRunPrm *prm, ALG_AewbStatus *status)
{
    memset(status, 0, sizeof(*status));

    gALG_aewbObj.vnfDemoCfg   	= prm->vnfDemoCfg;
    gALG_aewbObj.aewbType     	= prm->aewbType;
    gALG_aewbObj.aewbVendor   	= prm->aewbVendor;
    gALG_aewbObj.aewbPriority   	= prm->aewbPriority;
    gALG_aewbObj.reduceShutter   	= prm->reduceShutter;
    gALG_aewbObj.saldre   		= prm->saldre;

    if(prm->aewbVendor == ALG_AEWB_ID_APPRO) {
        if(Aew_ext_parameter.aew_enable == AEW_ENABLE )
        {
            ApproSend3A(
                    (IAE_Handle)gALG_aewbObj.handle_ae,
                    (IAWB_Handle)gALG_aewbObj.handle_awb,
                    &gALG_aewbObj.AE_OutArgs,
                    &gALG_aewbObj.AWB_OutArgs
                    );
        }

        Aew_ext_parameter.H3ABuffer = (void*)prm->h3aDataVirtAddr;
        Appro3AFunc( Aew_ext_parameter.H3ABuffer );
    }
    else if(prm->aewbVendor == ALG_AEWB_ID_TI) {

        int fd_status = TI_fd_sequence(prm->sensorMode, prm->h3aDataVirtAddr,
                gALG_aewbObj.IAEWB_StatMatdata.winCtHorz, gALG_aewbObj.IAEWB_StatMatdata.winCtVert,
                gALG_aewbObj.saldre,
                &gALG_aewbObj.AE_InArgs.curAe, &gALG_aewbObj.AE_OutArgs.nextAe, Aew_ext_parameter.env_50_60Hz,
                g_fd_mem, g_fd_mem_size*4);

        if (fd_status == 0 || fd_status == 2)
        {
            TI2AFunc( (void *)prm->h3aDataVirtAddr );
        }

    }

    return 0;
}

int ALG_aewbDelete(void *hndl)
{
    int numMem;

    if(gALG_aewbObj.aewbVendor==ALG_AEWB_ID_APPRO) {
        numMem = AE_APPRO_AE.ialg.algFree(gALG_aewbObj.handle_ae, gALG_aewbObj.memTab_ae);
        while(numMem > 0){
            free( gALG_aewbObj.memTab_ae[numMem-1].base );
            numMem --;
        }

        numMem = AWB_APPRO_AWB.ialg.algFree(gALG_aewbObj.handle_awb, gALG_aewbObj.memTab_awb);
        while(numMem > 0){
            free( gALG_aewbObj.memTab_awb[numMem-1].base );
            numMem --;
        }
    }
    else if(gALG_aewbObj.aewbVendor == ALG_AEWB_ID_TI) {

        numMem = AE_TI_AE.ialg.algFree(gALG_aewbObj.handle_ae, gALG_aewbObj.memTab_ae);
        while(numMem > 0){
            free( gALG_aewbObj.memTab_ae[numMem-1].base );
            numMem --;
        }

        numMem = AWB_TI_AWB.ialg.algFree(gALG_aewbObj.handle_awb, gALG_aewbObj.memTab_awb);
        while(numMem > 0){
            free( gALG_aewbObj.memTab_awb[numMem-1].base );
            numMem --;
        }
    }

    free(rgbData);
    free(aew_data);
    free(g_fd_mem);

    OSA_mutexDelete(&gITTAwb.statusLock);

    return 0;
}

int ALG_aewbSetTTawb( ALG_AewbData_ITTAwb ipipe_awb_gain){

    //TBD

    return 0;
}

/* get AWB data for Image Tuning tool */
int ALG_aewbGetTTawb( ALG_AewbData_ITTAwb *itt_AwbData)
{
    IAEWB_Rgb *tRgb;
    Uint32 awbDataSize = 0;

    OSA_mutexLock(&gITTAwb.statusLock);


    itt_AwbData->awbNumWinH = gITTAwb.awbNumWinH;
    itt_AwbData->awbNumWinV = gITTAwb.awbNumWinV;
    memset(itt_AwbData->awbMiscData ,0x0, sizeof(gITTAwb.awbMiscData));  //Temp

    awbDataSize = (itt_AwbData->awbNumWinH * itt_AwbData->awbNumWinV);
    tRgb = gITTAwb.awbRgbData;

    if(awbDataSize > (IMAGE_TUNE_AWB_RGB_SIZE))
        awbDataSize = (IMAGE_TUNE_AWB_RGB_SIZE);
    if( tRgb != NULL) {
        ALG_aewbConvert_RGB_YUV(tRgb, (int)gITTAwb.aewbNumSPCInWin, (int)awbDataSize, (IAEWB_Rgb *)itt_AwbData->awbRgbData, (IAWB_Yuv *)itt_AwbData->awbYuvData);
    }
    else{
        memset(itt_AwbData->awbRgbData ,0x0, (IMAGE_TUNE_AWB_RGB_SIZE*3*4));
        memset(itt_AwbData->awbYuvData ,0x0, (IMAGE_TUNE_AWB_RGB_SIZE*3*4));
    }

    OSA_mutexUnlock(&gITTAwb.statusLock);

    return 0;
}

/*********************************************************************************************************************************
 * README
 *
 * ALG_aewbConvert_RGB_YUV - CONVERT_RGB_YUV(IAWB_RGB *rgbData, IAWB_RGB *norm_rgbData, IAWB_YUV *yuvData, int pix_in_pax, int awb_h3a_paxels)
 *
 * IAWB_RGB *rgbData: pointer to the RGB H3A data passed to AWB library
 * IAWB_RGB *norm_rgbData: pointer to the normalized RGB H3A data, that is, rgbData divided by pix_in_pax (number of pixels per H3A paxel)
 * IAWB_YUV *yuvData: pointer to the converted H3A data in Y, Cb, Cr format
 * int pix_in_pax: number of accumulated pixels per H3A paxel
 * int awb_h3a_paxels: total number of H3A paxels (i.e., total number of H3A windows)
 *
 * Note:
 * 1. IAWB_RGB *rgbData should points to the "IAEWB_Rgb *rgbData" produced by function
 *			static void GETTING_RGB_BLOCK_VALUE(unsigned short * BLOCK_DATA_ADDR,IAEWB_Rgb *rgbData, aewDataEntry *aew_data, int shift)
 * 2. IAWB_RGB *norm_rgbData and IAWB_YUV *yuvData are the RGB data and YUV data needed by IPNC tuning tool for AWB tuning
 *
 * Source - Buyue
 *********************************************************************************************************************************/

void ALG_aewbConvert_RGB_YUV(IAEWB_Rgb *rgbData, int pix_in_pax, int awb_h3a_paxels, IAEWB_Rgb *norm_rgbData, IAWB_Yuv *yuvData)
{
    int i;
    int current_R, current_G, current_B;
    int current_Y, current_Cb, current_Cr;
    int temp;


    for ( i = 0; i < awb_h3a_paxels; i++ )
    {
        current_R = rgbData[i].r;
        current_G = rgbData[i].g;
        current_B = rgbData[i].b;

        current_R = (current_R + pix_in_pax / 2) / pix_in_pax;
        current_G = (current_G + pix_in_pax / 2) / pix_in_pax;
        current_B = (current_B + pix_in_pax / 2) / pix_in_pax;

        current_Y = (( 0x4D * current_R ) + ( 0x96 * current_G ) + ( 0x1D * current_B ) + 128 ) / 256;

        temp = -0x2B * current_R - 0x55 * current_G + 0x80 * current_B;
        if ( temp > 0 ) temp += ( current_Y + 1 ) / 2;
        else if (temp < 0 ) temp -= ( current_Y + 1 ) / 2;
        current_Cb = temp / ( current_Y + 1 );

        temp = 0x80 * current_R - 0x6B * current_G - 0x15 * current_B;
        if ( temp > 0 ) temp += ( current_Y + 1 ) / 2;
        else if ( temp < 0 ) temp -= ( current_Y + 1 ) / 2;
        current_Cr = temp / ( current_Y + 1 );

        norm_rgbData[i].r = current_R;
        norm_rgbData[i].g = current_G;
        norm_rgbData[i].b = current_B;

        yuvData[i].y = current_Y;
        yuvData[i].u = current_Cb;
        yuvData[i].v = current_Cr;

    }
}

awb_calc_data_t *ImageTune_imgsGetAwbConfig(int mod)
{
    //Revisit
    //OSA_printf("IT AlgAwb: GetAwbConfig done \n");

    return &awb_calc_data;

}
