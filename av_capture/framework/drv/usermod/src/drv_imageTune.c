
#include <drv_imageTunePriv.h>
#include <drv_ldcPriv.h>
#include <drv_isifPriv.h>
#include <drv_imgs.h>

DRV_ImageTuneObj gDRV_imageTuneObj;

void *DRV_imageTuneThrMain(void *prm)
{
  IMAGE_TUNE_ServerRun();

  return NULL;
}

int DRV_imageTuneInit()
{
  DRV_ImgsIsifConfig      *pIsifCfg;
  DRV_ImgsIpipeConfig     *pIpipeCfg;
  DRV_ImgsLdcConfig       *pLdcCfg;
  awb_calc_data_t         *pAwbCfg;
  ALG_vnfParams           *pVnfCfg;
  IMAGE_TUNE_ParamSet *prm;

  int status = OSA_SOK, paramSetId = 0;

  memset(&gDRV_imageTuneObj, 0, sizeof(gDRV_imageTuneObj));

  // init and set default handlers
  status = IMAGE_TUNE_Init();
  if(status!=OSA_SOK) {
    OSA_ERROR("IMAGE TUNE: init\n");
    return OSA_EFAIL;
  }

  // override with system specific handlers
  IMAGE_TUNE_AttachCmdHandler(IMAGE_TUNE_SYS_SAVE_RAW_DATA, DRV_imageTuneCmdExecuteSaveRawData);
  IMAGE_TUNE_AttachCmdHandler(IMAGE_TUNE_SYS_SAVE_YUV_DATA, DRV_imageTuneCmdExecuteSaveYuvData);
  IMAGE_TUNE_AttachCmdHandler(IMAGE_TUNE_LDC_PRM          , DRV_ldcImageTuneCmdExecuteLdc     );
  IMAGE_TUNE_AttachCmdHandler(IMAGE_TUNE_CCDC_LSC         , DRV_isifImageTuneCmdExecuteLsc    );
  IMAGE_TUNE_AttachCmdHandler(IMAGE_TUNE_SYS_SEND_RAW_DATA, DRV_imageTuneCmdExecuteSendRawData);
  IMAGE_TUNE_AttachCmdHandler(IMAGE_TUNE_SYS_SEND_YUV_DATA, DRV_imageTuneCmdExecuteSendYuvData);

  pIsifCfg  = DRV_imgsGetIsifConfig(DRV_IMGS_SENSOR_MODE_DEFAULT);
  pIpipeCfg = DRV_imgsGetIpipeConfig(DRV_IMGS_SENSOR_MODE_DEFAULT, 0);
  pLdcCfg   = DRV_imgsGetLdcConfig(DRV_IMGS_SENSOR_MODE_DEFAULT, 0, 0);
  pAwbCfg   = ImageTune_imgsGetAwbConfig(0);
  pVnfCfg   = ImageTune_imgsGetVnfConfig(0);
   
  if(pIsifCfg!=NULL)
    gDRV_imageTuneObj.imgsDefaultPrm.pCcdcPrm = &pIsifCfg->ccdcParams;

  if(pIpipeCfg!=NULL) {
    gDRV_imageTuneObj.imgsDefaultPrm.pIpipePrm = &pIpipeCfg->ipipeParams;
    gDRV_imageTuneObj.imgsDefaultPrm.pIpipeifPrm = &pIpipeCfg->ipipeifParams;
  }

  if(pLdcCfg!=NULL)
    gDRV_imageTuneObj.imgsDefaultPrm.pLdcPrm = &pLdcCfg->ldcParams;

  if(pAwbCfg!=NULL)
    gDRV_imageTuneObj.imgsDefaultPrm.pAwbPrm = pAwbCfg;
  
  if(pVnfCfg!=NULL)
	 gDRV_imageTuneObj.imgsDefaultPrm.pVnfPrm= &pVnfCfg;
  
  status = IMAGE_TUNE_ReadParamset(0);                                    /* Read Paramset file */
  status = IMAGE_TUNE_SetParams(&gDRV_imageTuneObj.imgsDefaultPrm);       /* Get Default vaule */
  status = IMAGE_TUNE_SaveParamSetList(prm, IMAGE_TUNE_DEFAULT_PARAMSET); /* Set Default vaule to Paramset "0" */

#ifndef IMAGE_TUNE_USE_FACTORY_PARAMSET
  /*if IMAGE_TUNE_USE_FACTORY_PARAMSET enabled, uses the paramset in the code (hard coded) as
	default paramset during boot else IMAGE_TUNE_USE_DEFAULT_PARAMSET as boot paramset if it is available
	and may change based on the boot paramset ID - now in vnf - Revisit*/	
  status = DRV_imageTuneLoadParamSet(0, IMAGE_TUNE_USE_DEFAULT_PARAMSET);
#endif

  status = OSA_flgCreate(&gDRV_imageTuneObj.dataSaveFlg, 0);
  if(status!=OSA_SOK) {
    OSA_ERROR("OSA_flgCreate()\n");
    return status;
  }

  status = IMAGE_TUNE_ServerOpen(IMAGE_TUNE_SERVER_DEFAULT_PORT);
  if(status != OSA_SOK) {
    OSA_ERROR("IMAGE_TUNE_ServerOpen()\n");
    OSA_flgDelete(&gDRV_imageTuneObj.dataSaveFlg);
    return status;
  }

  status = OSA_thrCreate(&gDRV_imageTuneObj.thrHndl, DRV_imageTuneThrMain, DRV_IMAGE_TUNE_THR_PRI, DRV_IMAGE_TUNE_THR_STACK_SIZE, NULL);
  if(status!=OSA_SOK) {
    OSA_ERROR("OSA_thrCreate()\n");
    OSA_flgDelete(&gDRV_imageTuneObj.dataSaveFlg);
    IMAGE_TUNE_ServerClose();
    return status;
  }

  return status;
}

int DRV_imageTuneExit()
{
  OSA_thrDelete(&gDRV_imageTuneObj.thrHndl);
  IMAGE_TUNE_ServerClose();
  IMAGE_TUNE_Exit();

  return OSA_SOK;
}

Bool DRV_imageTuneIsSaveRawData()
{
  return gDRV_imageTuneObj.isRawDataSave;
}

Bool DRV_imageTuneIsSaveYuvData()
{
  return gDRV_imageTuneObj.isYuvDataSave;
}

Bool DRV_imageTuneIsSendRawData()
{
  return gDRV_imageTuneObj.isRawDataSend;
}

Bool DRV_imageTuneIsSendYuvData()
{
  return gDRV_imageTuneObj.isYuvDataSend;
}

int DRV_imageTuneSaveDataToFile(int format, int fileIndex)
{
  IMAGE_TUNE_CmdInfo cmdInfo;
  char filename[DRV_IMAGE_TUNE_FILENAME_LEN_MAX];

  cmdInfo.commandFlags = 0;
  cmdInfo.prm          = (Uint32*)filename;

  if(format==DRV_DATA_FORMAT_RAW) {

    sprintf(filename, "DATA%04d.rawTI", fileIndex);

    cmdInfo.commandId = IMAGE_TUNE_SYS_SAVE_RAW_DATA;
    cmdInfo.prmSize = strlen(filename)+1;

    DRV_imageTuneCmdExecuteSaveRawData(&cmdInfo, NULL);

  } else {

    sprintf(filename, "DATA%04d.yuvTI", fileIndex);

    cmdInfo.commandId = IMAGE_TUNE_SYS_SAVE_YUV_DATA;
    cmdInfo.prmSize = strlen(filename)+1;

    DRV_imageTuneCmdExecuteSaveYuvData(&cmdInfo, NULL);
  }

  return OSA_SOK;
}

int DRV_imageTuneCmdExecuteSaveRawData(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo)
{
  if( cmdInfo->prmSize > (DRV_IMAGE_TUNE_FILENAME_LEN_MAX-1) ) {
    return OSA_EFAIL;
  }

  memcpy(gDRV_imageTuneObj.rawDataSaveFileName, cmdInfo->prm, cmdInfo->prmSize);

  gDRV_imageTuneObj.rawDataSaveFileName[cmdInfo->prmSize] = 0;

  OSA_flgClear(&gDRV_imageTuneObj.dataSaveFlg, DRV_IMAGE_TUNE_DAVE_SAVE_FLAG_RAW);

  gDRV_imageTuneObj.isRawDataSave = TRUE;

  OSA_flgWait(&gDRV_imageTuneObj.dataSaveFlg, DRV_IMAGE_TUNE_DAVE_SAVE_FLAG_RAW, OSA_FLG_MODE_AND_CLR, OSA_TIMEOUT_FOREVER);

  return OSA_SOK;
}

int DRV_imageTuneCmdExecuteSaveYuvData(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo)
{
  if( cmdInfo->prmSize > (DRV_IMAGE_TUNE_FILENAME_LEN_MAX-1) ) {
    return OSA_EFAIL;
  }

  memcpy(gDRV_imageTuneObj.yuvDataSaveFileName, cmdInfo->prm, cmdInfo->prmSize);

  gDRV_imageTuneObj.yuvDataSaveFileName[cmdInfo->prmSize] = 0;

  OSA_flgClear(&gDRV_imageTuneObj.dataSaveFlg, DRV_IMAGE_TUNE_DAVE_SAVE_FLAG_YUV);

  gDRV_imageTuneObj.isYuvDataSave = TRUE;
 
  OSA_flgWait(&gDRV_imageTuneObj.dataSaveFlg, DRV_IMAGE_TUNE_DAVE_SAVE_FLAG_YUV, OSA_FLG_MODE_AND_CLR, OSA_TIMEOUT_FOREVER);

  return OSA_SOK;
}


//ETH
int DRV_imageTuneCmdExecuteSendRawData(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo)
{

  OSA_flgClear(&gDRV_imageTuneObj.dataSaveFlg, DRV_IMAGE_TUNE_DAVE_SEND_FLAG_RAW);

  gDRV_imageTuneObj.isRawDataSend = TRUE;
  IMAGE_TUNE_SetSendRawData(1);
  
  OSA_flgWait(&gDRV_imageTuneObj.dataSaveFlg, DRV_IMAGE_TUNE_DAVE_SEND_FLAG_RAW, OSA_FLG_MODE_AND_CLR, OSA_TIMEOUT_FOREVER);

  return OSA_SOK;
}

//ETH
int DRV_imageTuneCmdExecuteSendYuvData(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo)
{

  gDRV_imageTuneObj.yuvDataSaveFileName[cmdInfo->prmSize] = 0;

  OSA_flgClear(&gDRV_imageTuneObj.dataSaveFlg, DRV_IMAGE_TUNE_DAVE_SEND_FLAG_YUV);

  gDRV_imageTuneObj.isYuvDataSend = TRUE;
  IMAGE_TUNE_SetSendYuvData(1);
  
  OSA_flgWait(&gDRV_imageTuneObj.dataSaveFlg, DRV_IMAGE_TUNE_DAVE_SEND_FLAG_YUV, OSA_FLG_MODE_AND_CLR, OSA_TIMEOUT_FOREVER);

  return OSA_SOK;
}


int DRV_imageTuneGetRawDataStartPhase()
{
  int phase, regValue, startX, startY;
//  static char *phaseName[] = { "R ", "Gr", "Gb", "B " };

  regValue = CSL_FEXT(gCSL_vpssHndl.ipipeRegs->SRC_COL, IPIPE_SRC_COL_EE);
  regValue = regValue%4;

  startX = gCSL_vpssHndl.ipipeRegs->SRC_HPS%2;
  startY = gCSL_vpssHndl.ipipeRegs->SRC_VPS%2;

  phase = regValue;

  if(startX) {
    switch(phase) {
      case CSL_IPIPE_SOURCE_COLOUR_R:
        phase = CSL_IPIPE_SOURCE_COLOUR_GR;
        break;
      case CSL_IPIPE_SOURCE_COLOUR_GR:
        phase = CSL_IPIPE_SOURCE_COLOUR_R;
        break;
      case CSL_IPIPE_SOURCE_COLOUR_GB:
        phase = CSL_IPIPE_SOURCE_COLOUR_B;
        break;
      case CSL_IPIPE_SOURCE_COLOUR_B:
        phase = CSL_IPIPE_SOURCE_COLOUR_GB;
        break;
    }
  }

  if(startY) {
    switch(phase) {
      case CSL_IPIPE_SOURCE_COLOUR_R:
        phase = CSL_IPIPE_SOURCE_COLOUR_GB;
        break;
      case CSL_IPIPE_SOURCE_COLOUR_GR:
        phase = CSL_IPIPE_SOURCE_COLOUR_B;
        break;
      case CSL_IPIPE_SOURCE_COLOUR_GB:
        phase = CSL_IPIPE_SOURCE_COLOUR_R;
        break;
      case CSL_IPIPE_SOURCE_COLOUR_B:
        phase = CSL_IPIPE_SOURCE_COLOUR_GR;
        break;
    }
  }

  #if 0
  OSA_printf(" IMAGE TUNE: Phase Reg = %s : Start x,y = %d,%d : Phase = %s \n",
    phaseName[regValue], startX, startY, phaseName[phase]
    );
  #endif

  return phase;
}

extern Uint32* csl_h3aRegs;

int DRV_imageTuneSaveData(IMAGE_TUNE_SaveDataInfo *info)
{
  char *filename;
  Uint32 doneFlag;
  int status;

  switch(info->dataFormat) {
    case DRV_DATA_FORMAT_RAW:
      if(!DRV_imageTuneIsSaveRawData() && !DRV_imageTuneIsSendRawData())
        return OSA_EFAIL;

      filename = gDRV_imageTuneObj.rawDataSaveFileName;
      doneFlag = DRV_IMAGE_TUNE_DAVE_SAVE_FLAG_RAW;
      break;

    case DRV_DATA_FORMAT_YUV422:
    case DRV_DATA_FORMAT_YUV420:
      if(!DRV_imageTuneIsSaveYuvData() && !DRV_imageTuneIsSendYuvData())
        return OSA_EFAIL;

      filename = gDRV_imageTuneObj.yuvDataSaveFileName;
      doneFlag = DRV_IMAGE_TUNE_DAVE_SAVE_FLAG_YUV;
      break;
    default:
      return OSA_EFAIL;
  }

  info->rawDataStartPhase = 0;
  info->rawDataBitsPerPixel = 0;
  info->rawDataFormat = 0;

  if(info->dataFormat==DRV_DATA_FORMAT_RAW) {
    // extract info from ISIF registers

    info->rawDataStartPhase = DRV_imageTuneGetRawDataStartPhase();
    //info->rawDataStartPhase = CSL_FEXT(gCSL_vpssHndl.isifRegs->CCOLP, ISIF_CCOLP_CP01_0);

    info->rawDataBitsPerPixel = 16 - CSL_FEXT(gCSL_vpssHndl.isifRegs->CGAMMAWD, ISIF_CGAMMAWD_GWDI);

    info->rawDataFormat = 0; // normal (no compression)

    if(CSL_FEXT(gCSL_vpssHndl.isifRegs->MISC, ISIF_MISC_DPCMEN)) {
      info->rawDataFormat = 2; // DPCM compression 12bits/pixel packed in DDR
      info->rawDataBitsPerPixel = 12; // after Inverse DPCM
    } else
    if(CSL_FEXT(gCSL_vpssHndl.isifRegs->CGAMMAWD, ISIF_CGAMMAWD_CCDTBL)) {
      //  A-law compression, 8bits/pixel packed in DDR
      info->rawDataFormat = 1;
      info->rawDataBitsPerPixel = 10; // after Inverse A-law
    }

    #if 0
    OSA_printf(" DRV:IMAGE TUNE: phase = %d, format = %d, bits/pixel = %d\n",
      info->rawDataStartPhase,
      info->rawDataFormat,
      info->rawDataBitsPerPixel
     );
    #endif
  }

  status = ALG_aewbGetTTawb(&info->awbData);  //  IT

  /* H3A Regs */
  memcpy(info->awbData.h3aRegs, csl_h3aRegs,sizeof(info->awbData.h3aRegs)); //(Uint32*)CSL_H3A_0_REGS

  status = IMAGE_TUNE_SaveData(filename, info);

  if(DRV_imageTuneIsSaveRawData() ||DRV_imageTuneIsSaveYuvData()){

    if(info->dataFormat==DRV_DATA_FORMAT_RAW)
      gDRV_imageTuneObj.isRawDataSave = FALSE;
    else
      gDRV_imageTuneObj.isYuvDataSave = FALSE;

  }else if(DRV_imageTuneIsSendRawData() ||DRV_imageTuneIsSendYuvData()){  //ETH

    if(info->dataFormat==DRV_DATA_FORMAT_RAW){
      gDRV_imageTuneObj.isRawDataSend = FALSE;
      doneFlag = DRV_IMAGE_TUNE_DAVE_SEND_FLAG_RAW;
    }else{
      gDRV_imageTuneObj.isYuvDataSend = FALSE;
      doneFlag = DRV_IMAGE_TUNE_DAVE_SEND_FLAG_YUV;
    } 
  }
  
  OSA_flgSet(&gDRV_imageTuneObj.dataSaveFlg, doneFlag);
  
  return status;
}


int DRV_imageTuneLoadParams(int mode, int paramSetId)
{
  int retVal;
  DRV_ImgsIsifConfig      *pIsifCfg;
  DRV_ImgsIpipeConfig     *pIpipeCfg;
  DRV_ImgsLdcConfig       *pLdcCfg;
  awb_calc_data_t         *pAwbCfg;
  ALG_vnfParams           *pVnfCfg;

  retVal = IMAGE_TUNE_LoadParams(paramSetId);
  if(retVal==OSA_SOK) {

    pIsifCfg  = DRV_imgsGetIsifConfig(mode);
    pIpipeCfg = DRV_imgsGetIpipeConfig(mode, 0);
    pLdcCfg   = DRV_imgsGetLdcConfig(mode, 0, 0);
    pAwbCfg = ImageTune_imgsGetAwbConfig(mode);
    pVnfCfg = ImageTune_imgsGetVnfConfig(mode);
		
    if(pIsifCfg!=NULL)
      gDRV_imageTuneObj.imgsDefaultPrm.pCcdcPrm = &pIsifCfg->ccdcParams;

    if(pIpipeCfg!=NULL) {
      gDRV_imageTuneObj.imgsDefaultPrm.pIpipePrm = &pIpipeCfg->ipipeParams;
      gDRV_imageTuneObj.imgsDefaultPrm.pIpipeifPrm = &pIpipeCfg->ipipeifParams;
    }

    if(pLdcCfg!=NULL) {
      gDRV_imageTuneObj.imgsDefaultPrm.pLdcPrm = &pLdcCfg->ldcParams;
    }

   if(pAwbCfg!=NULL){
      gDRV_imageTuneObj.imgsDefaultPrm.pAwbPrm= &pAwbCfg;

   }
   
   if(pVnfCfg!=NULL){
      gDRV_imageTuneObj.imgsDefaultPrm.pVnfPrm= &pVnfCfg;

   }

   retVal = IMAGE_TUNE_GetParams(&gDRV_imageTuneObj.imgsDefaultPrm);
   if(retVal  == OSA_SOK)
      OSA_printf("\n DRV_ImageTune: Paramset loaded !!!\n");
   }

  return OSA_SOK;  //to avoid avserver crash
}

int DRV_imageTuneSaveParams(int mode, int paramSetId, Bool saveDefaultParams)
{
  int retVal;
  DRV_ImgsIsifConfig      *pIsifCfg;
  DRV_ImgsIpipeConfig     *pIpipeCfg;
  DRV_ImgsLdcConfig       *pLdcCfg;
  awb_calc_data_t       *pAwbCfg;
  ALG_vnfParams           *pVnfCfg;

  pIsifCfg  = DRV_imgsGetIsifConfig(mode);
  pIpipeCfg = DRV_imgsGetIpipeConfig(mode, 0);
  pLdcCfg   = DRV_imgsGetLdcConfig(mode, 0, 0);
  pAwbCfg   = ImageTune_imgsGetAwbConfig(mode);
  pVnfCfg   = ImageTune_imgsGetVnfConfig(mode);

  if(pIsifCfg!=NULL)
    gDRV_imageTuneObj.imgsDefaultPrm.pCcdcPrm = &pIsifCfg->ccdcParams;

  if(pIpipeCfg!=NULL) {
    gDRV_imageTuneObj.imgsDefaultPrm.pIpipePrm = &pIpipeCfg->ipipeParams;
    gDRV_imageTuneObj.imgsDefaultPrm.pIpipeifPrm = &pIpipeCfg->ipipeifParams;
  }

  if(pLdcCfg!=NULL)
    gDRV_imageTuneObj.imgsDefaultPrm.pLdcPrm = &pLdcCfg->ldcParams;

  //awb
  if(pAwbCfg!=NULL)
    gDRV_imageTuneObj.imgsDefaultPrm.pAwbPrm = pAwbCfg;
  
  if(pVnfCfg!=NULL)
	 gDRV_imageTuneObj.imgsDefaultPrm.pVnfPrm= &pVnfCfg;

  if(saveDefaultParams) {
    IMAGE_TUNE_SetParams(&gDRV_imageTuneObj.imgsDefaultPrm);
  }

  retVal = IMAGE_TUNE_SaveParams(paramSetId);

  return retVal;
}


int DRV_imageTuneLoadParamSet(int mode, int paramSetId){

	int retVal = OSA_EFAIL;
	retVal = DRV_imageTuneLoadParams( mode, paramSetId);

#if 0  //test for boot
	retVal = IMAGE_TUNE_LoadParams(paramSetId);

    if(retVal == OSA_SOK){
      retVal = IMAGE_TUNE_CmdExecuteIpipeRgb2Yuv_new(paramSetId);
  	  OSA_printf("paramSetId %d, DRV_imageTuneLoadParamSet done... \n", paramSetId);
    }
#endif
    return retVal;
}


