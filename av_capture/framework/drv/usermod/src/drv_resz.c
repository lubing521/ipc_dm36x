
#include <drv_resz.h>
#include <osa_mutex.h>
#include <osa_cmem.h>
#include <osa_file.h>
#include <drv_ipipe.h>

//#define DRV_RESZ_TEST_IPIPE

#define DRV_VPSS_CLK_IN_MHZ   (121)

#define DRV_RESZ_MAGIC_NUM  (0xDEADBEEF)

#define DRV_RESZ_RETRY_COUNT  (8)

#define DRV_RESZ_MAGIC_NUM_ENABLE
//#define DRV_RESZ_CACHE_WBINV_ENABLE

//#define DRV_RESZ_DEBUG

typedef struct {

  OSA_MutexHndl lock;

  CSL_RszHwSetup              setup;
  CSL_RszOutConfig            outConfig[CSL_RSZ_CH_MAX];
  CSL_RszMinMaxConfig         minMaxConfig;
  CSL_RszClkConfig            clkConfig;
  CSL_RszInFrameConfig        inFrameConfig;
  CSL_RszInConfig             inConfig;

  CSL_IpipeifClkConfig        ipipeifClk;
  CSL_IpipeifDpcmConfig       dpcmConfig;
  CSL_IpipeifSdramInConfig    ddrInConfig;
  CSL_IpipeifInSource2Config  inSrcConfig;

  DRV_ClkHz clkHz;

} DRV_ReszObj;

DRV_ReszObj gDRV_reszObj;

int DRV_reszInit()
{
  int status, i;
  CSL_RszOutConfig *pOutConfig;

  memset(&gDRV_reszObj, 0, sizeof(gDRV_reszObj));

  status = OSA_mutexCreate(&gDRV_reszObj.lock);
  if(status!=OSA_SOK) {
    OSA_ERROR("OSA_mutexCreate()\n");
    return status;
  }

  gDRV_reszObj.setup.enable         = TRUE;
  gDRV_reszObj.setup.oneShotEnable  = TRUE;
  gDRV_reszObj.setup.inConfig       = &gDRV_reszObj.inConfig;
  gDRV_reszObj.setup.inFrameConfig  = &gDRV_reszObj.inFrameConfig;
  gDRV_reszObj.setup.clkConfig      = &gDRV_reszObj.clkConfig;
  gDRV_reszObj.setup.minMaxConfig   = &gDRV_reszObj.minMaxConfig;
  gDRV_reszObj.setup.rszAConfig     = &gDRV_reszObj.outConfig[0];
  gDRV_reszObj.setup.rszBConfig     = &gDRV_reszObj.outConfig[1];

  gDRV_reszObj.inConfig.wenUseEnable       = FALSE;
  gDRV_reszObj.inConfig.passThruEnable     = FALSE;
  gDRV_reszObj.inConfig.passThruInFormat   = CSL_RSZ_PASS_THROUGH_MODE_INPUT_YCbCr;
  gDRV_reszObj.inConfig.inputDataPath      = 0;
  gDRV_reszObj.inConfig.chormaFormatType   = 0;
  gDRV_reszObj.inConfig.yuv420Color        = 0;
  gDRV_reszObj.inConfig.chormaPos          = CSL_RSZ_CHROMA_POS_SAME_AS_Y;
  gDRV_reszObj.inConfig.chormaFlip         = FALSE;

  gDRV_reszObj.inFrameConfig.inStartX     = 0;
  gDRV_reszObj.inFrameConfig.inStartY     = 0;
  gDRV_reszObj.inFrameConfig.inWidth      = 0;
  gDRV_reszObj.inFrameConfig.inHeight     = 0;

  gDRV_reszObj.clkConfig.mmrClkEnable  = TRUE;
  gDRV_reszObj.clkConfig.coreClkEnable = TRUE;

  gDRV_reszObj.minMaxConfig.minY       = 0;
  gDRV_reszObj.minMaxConfig.maxY       = 0xFF;
  gDRV_reszObj.minMaxConfig.minC       = 0;
  gDRV_reszObj.minMaxConfig.maxC       = 0xFF;

  for(i=0; i<CSL_RSZ_CH_MAX; i++)
  {
    pOutConfig = &gDRV_reszObj.outConfig[i];


    pOutConfig->flipH                  = 0;
    pOutConfig->flipV                  = 0;
    pOutConfig->irqInterval            = 0;

    if(i==0)
      pOutConfig->dmaInterval            = 0x8;
    else
      pOutConfig->dmaInterval            = 0x8;

    pOutConfig->enable                 = TRUE;
    pOutConfig->oneShotEnable          = TRUE;
    pOutConfig->yuv420YOutEnable       = 0;
    pOutConfig->yuv420COutEnable       = 0;
    pOutConfig->inStartX               = 0;
    pOutConfig->inStartY               = 0;
    pOutConfig->outWidth               = 0;
    pOutConfig->outHeight              = 0;
    pOutConfig->vertStartPhaseY        = 0;
    pOutConfig->vertStartPhaseC        = 0;
    pOutConfig->vertReszRatio          = 0;
    pOutConfig->vertReszTypeY          = CSL_RSZ_TYPE_4_TAP_CUBIC_CONVOLUTION;
    pOutConfig->vertReszTypeC          = CSL_RSZ_TYPE_4_TAP_CUBIC_CONVOLUTION;
    pOutConfig->vertLpfIntensityY      = 0;
    pOutConfig->vertLpfIntensityC      = 0;
    pOutConfig->horzStartPhase         = 0;
    pOutConfig->horzStartPhaseAdd      = 0;
    pOutConfig->horzReszRatio          = 0;
    pOutConfig->horzReszTypeY          = CSL_RSZ_TYPE_4_TAP_CUBIC_CONVOLUTION;
    pOutConfig->horzReszTypeC          = CSL_RSZ_TYPE_4_TAP_CUBIC_CONVOLUTION;
    pOutConfig->horzLpfIntensityY      = 0;
    pOutConfig->horzLpfIntensityC      = 0;
    pOutConfig->downScaleModeEnable    = FALSE;
    pOutConfig->downScaleModeAvgSizeV  = 0;
    pOutConfig->downScaleModeAvgSizeH  = 0;
    pOutConfig->rgbOutEnable           = FALSE;
    pOutConfig->rgbOutMaskFirstEnable  = FALSE;
    pOutConfig->rgbOutMaskLastEnable   = FALSE;
    pOutConfig->rgbOutType             = 0;
    pOutConfig->rgbOutAlphaVal         = 0;
    pOutConfig->outStartAddr           = 0;
    pOutConfig->outLineOffset          = 0;
    pOutConfig->outBaseAddr            = 0;
    pOutConfig->outStartLine           = 0;
    pOutConfig->outEndLine             = 0;
    pOutConfig->yuv420CoutStartAddr    = 0;
    pOutConfig->yuv420CoutLineOffset   = 0;
    pOutConfig->yuv420CoutBaseAddr     = 0;
    pOutConfig->yuv420CoutStartLine    = 0;
    pOutConfig->yuv420CoutEndLine      = 0;
  }

  gDRV_reszObj.ipipeifClk.clkSel        = CSL_IPIPEIF_IPIPEIF_CLOCK_SELECT_SDRAM_DIV_CLK;
  gDRV_reszObj.ipipeifClk.clkDivM       = 0;
  gDRV_reszObj.ipipeifClk.clkDivN       = 0;
  gDRV_reszObj.ipipeifClk.pixelsPerLine = 0;
  gDRV_reszObj.ipipeifClk.linesPerFrame = 0;

  gDRV_reszObj.dpcmConfig.enable        = 0;
  gDRV_reszObj.dpcmConfig.bits          = CSL_IPIPEIF_IPIPEIF_DPCM_BITS_10_BIT_TO_12_BIT;
  gDRV_reszObj.dpcmConfig.predType      = CSL_IPIPEIF_IPIPEIF_DPCM_PERDICTION_MODE_ADVANCED;

  gDRV_reszObj.ddrInConfig.enable          = TRUE;
  gDRV_reszObj.ddrInConfig.oneShotEnable   = TRUE;
  gDRV_reszObj.ddrInConfig.rawDataShift    = CSL_IPIPEIF_SDRAM_IN_DATASHIFT_BITS_11_0;
  gDRV_reszObj.ddrInConfig.rawUnpackMode   = 0;
  gDRV_reszObj.ddrInConfig.inputAddr       = 0;
  gDRV_reszObj.ddrInConfig.inputWidth      = 0;
  gDRV_reszObj.ddrInConfig.inputHeight     = 0;
  gDRV_reszObj.ddrInConfig.inputLineOffset = 0;
  gDRV_reszObj.ddrInConfig.dpcEnable       = FALSE;
  gDRV_reszObj.ddrInConfig.dpcThreshold    = 0;
  gDRV_reszObj.ddrInConfig.dpcmConfig      = &gDRV_reszObj.dpcmConfig;
  gDRV_reszObj.ddrInConfig.clkConfig       = &gDRV_reszObj.ipipeifClk;

  gDRV_reszObj.inSrcConfig.inputSource2           = 0;
  gDRV_reszObj.inSrcConfig.pixelDecimationEnable  = FALSE;
  gDRV_reszObj.inSrcConfig.pixelDecimationRatio   = 16;
  gDRV_reszObj.inSrcConfig.avgFilterEnable        = FALSE;
  gDRV_reszObj.inSrcConfig.alignHsyncVsync        = FALSE;
  gDRV_reszObj.inSrcConfig.initReszPosH           = 0;
  gDRV_reszObj.inSrcConfig.outClip                = 0xFFFFFFFF;
  gDRV_reszObj.inSrcConfig.gain                   = 0x200;
#ifdef YUV_MODE_PROGRESSIVE
  gDRV_reszObj.inSrcConfig.isifInType             = CSL_IPIPEIF_ISIF_INPUT_TYPE_YUV;  //Tuff
  gDRV_reszObj.inSrcConfig.yuvConvertEnable       = TRUE;
#else
  gDRV_reszObj.inSrcConfig.isifInType             = CSL_IPIPEIF_ISIF_INPUT_TYPE_RAW;
  gDRV_reszObj.inSrcConfig.yuvConvertEnable       = FALSE;
#endif
  gDRV_reszObj.inSrcConfig.vsyncMaskEnable        = FALSE;

  DRV_clkGetClkHz(&gDRV_reszObj.clkHz, FALSE);

  return status;
}

int DRV_reszExit()
{
  OSA_mutexDelete(&gDRV_reszObj.lock);

  return OSA_SOK;
}

int DRV_reszLock()
{
  return OSA_mutexLock(&gDRV_reszObj.lock);
}

int DRV_reszUnlock()
{
  return OSA_mutexUnlock(&gDRV_reszObj.lock);
}

int DRV_reszCheckMagicNum(DRV_ReszRunPrm *pPrm, int bytesPerPixel, int chroma)
{
  int i;
  volatile Uint32 *pAddr;

  #ifdef DRV_RESZ_MAGIC_NUM_ENABLE
  for(i=0; i<CSL_RSZ_CH_MAX; i++) {
    if(pPrm->pOut[i]!=NULL) {
      if(pPrm->pOut[i]->outVirtAddr!=NULL) {
        pAddr = (Uint32*)(
          (Uint32)pPrm->pOut[i]->outVirtAddr + pPrm->pOut[i]->outOffsetH*(pPrm->pOut[i]->outHeight-2)*bytesPerPixel
          );

        #ifdef DRV_RESZ_CACHE_WBINV_ENABLE
        OSA_cmemCacheInv(pAddr, 64);
        #endif

        if(*pAddr == DRV_RESZ_MAGIC_NUM)
          return OSA_EFAIL;
      }
    }
  }
  #endif

  return OSA_SOK;
}

int DRV_reszInsertMagicNum(DRV_ReszRunPrm *pPrm, int bytesPerPixel, int chroma)
{
  int i;
  volatile Uint32 *pAddr;

  #ifdef DRV_RESZ_MAGIC_NUM_ENABLE
  for(i=0; i<CSL_RSZ_CH_MAX; i++) {
    if(pPrm->pOut[i]!=NULL) {
      if(pPrm->pOut[i]->outVirtAddr!=NULL) {
        pAddr = (Uint32*)(
          (Uint32)pPrm->pOut[i]->outVirtAddr + pPrm->pOut[i]->outOffsetH*(pPrm->pOut[i]->outHeight-2)*bytesPerPixel
          );

        *pAddr = DRV_RESZ_MAGIC_NUM;

        #ifdef DRV_RESZ_CACHE_WBINV_ENABLE
        OSA_cmemCacheWb(pAddr, 64);
        #endif
      }
    }
  }
  #endif

  return OSA_SOK;
}

int DRV_reszRun(DRV_ReszRunPrm *pPrm)
{
  Uint8 *inPhysAddr;
  Uint8 *inPhysAddrC;

  Uint16 bytesPerLine;
  Uint16 bytesPerPixel[2];

  int status, i, startX, startY;
  CSL_RszOutConfig *pOutConfig;
  Uint16 inType, numIterations;
  Bool isInterlaced;

  DRV_ReszRunPrm prm;
  DRV_ReszOutPrm outPrm[CSL_RSZ_CH_MAX];
  int retry;

  Uint32 oldWbGain[4]={0x200, 0x200, 0x200, 0x200};

  if(pPrm->pOut[0]==NULL && pPrm->pOut[1]==NULL)
    return OSA_EFAIL;

  memcpy(&prm, pPrm, sizeof(prm));

  prm.pOut[0]   = NULL;
  prm.pOut[1]   = NULL;

  if(pPrm->pOut[0]!=NULL) {
    memcpy(&outPrm[0], pPrm->pOut[0], sizeof(outPrm[0]));
    prm.pOut[0]= &outPrm[0];
  }

  if(pPrm->pOut[1]!=NULL) {
    memcpy(&outPrm[1], pPrm->pOut[1], sizeof(outPrm[1]));
    prm.pOut[1]= &outPrm[1];
  }

  if(prm.enableInvAlaw)
    prm.enableInvDpcm = FALSE;

  inType = prm.inType & 0xFF;
  isInterlaced = FALSE;
  numIterations = 1;

  if(inType!=DRV_DATA_FORMAT_RAW) {
    isInterlaced = (prm.inType & DRV_DATA_FORMAT_INTERLACED)? TRUE : FALSE;

    if(isInterlaced) {
      numIterations = 2;
      prm.inOffsetH *= 2;
      prm.inOffsetV /= 2;
      prm.inStartY = OSA_floor(prm.inStartY, 2)/2;
      prm.inHeight = OSA_floor(prm.inHeight, 2)/2;

      if(prm.pOut[0]!=NULL) {
        prm.pOut[0]->outHeight = OSA_floor(prm.pOut[0]->outHeight, 2)/2;
        prm.pOut[0]->outOffsetH *= 2;
        prm.pOut[0]->outOffsetV /= 2;
      }
      if(prm.pOut[1]!=NULL) {
        prm.pOut[1]->outHeight = OSA_floor(prm.pOut[1]->outHeight, 2)/2;
        prm.pOut[1]->outOffsetH *= 2;
        prm.pOut[1]->outOffsetV /= 2;
      }
    }
  }

  if(inType==DRV_DATA_FORMAT_RAW) {
  	DRV_ipipeLock();
       DRV_isifLock();  //ANR - ISIF

  }
  DRV_reszLock();

  while(numIterations--) {



    if(inType==DRV_DATA_FORMAT_RAW)
  {
      if(prm.enableInvAlaw) {
      bytesPerPixel[0] = 1;
      bytesPerPixel[1] = 1;
    } else
      if(prm.enableInvDpcm) {
      bytesPerPixel[0] = 3;
      bytesPerPixel[1] = 2;
    } else {
      bytesPerPixel[0] = 2;
      bytesPerPixel[1] = 1;
    }

  } else
    if(inType==DRV_DATA_FORMAT_YUV422)
  {
    bytesPerPixel[0] = 2;
    bytesPerPixel[1] = 1;

  } else
    if(inType==DRV_DATA_FORMAT_YUV420)
  {
    bytesPerPixel[0] = 1;
    bytesPerPixel[1] = 1;

  } else {

    goto error_exit;
  }

    bytesPerLine = (prm.inOffsetH*bytesPerPixel[0])/bytesPerPixel[1];

    if(isInterlaced) {
      if(numIterations==1) {

      }
      if(numIterations==0) {

        prm.inPhysAddr += bytesPerLine/2;
        prm.inVirtAddr += bytesPerLine/2;
        if(prm.pOut[0]!=NULL) {
          prm.pOut[0]->outPhysAddr += (prm.pOut[0]->outOffsetH/2);
          prm.pOut[0]->outVirtAddr += (prm.pOut[0]->outOffsetH/2);
        }
        if(prm.pOut[1]!=NULL) {
          prm.pOut[1]->outPhysAddr += (prm.pOut[1]->outOffsetH/2);
          prm.pOut[1]->outVirtAddr += (prm.pOut[1]->outOffsetH/2);
        }
      }
    }


    startX = OSA_floor(prm.inStartX, 32);
    startY = OSA_floor(prm.inStartY, 2);

    inPhysAddr  = prm.inPhysAddr;
    inPhysAddrC = prm.inPhysAddr + prm.inOffsetH*prm.inOffsetV;

  inPhysAddr += (bytesPerLine*startY) + (startX*bytesPerPixel[0])/bytesPerPixel[1];

    if(inType==DRV_DATA_FORMAT_YUV420)
    inPhysAddrC += (bytesPerLine*startY/2) + (startX*bytesPerPixel[0])/bytesPerPixel[1];

    startY = (prm.inStartY%2);
    startX = prm.inStartX%32;
  startX = OSA_floor(startX, 2);

    if(startY<1)
      startY=1;

    if(prm.pOut[0]==NULL)
    gDRV_reszObj.setup.rszAConfig     = NULL;
  else
    gDRV_reszObj.setup.rszAConfig     = &gDRV_reszObj.outConfig[0];

    if(prm.pOut[1]==NULL)
    gDRV_reszObj.setup.rszBConfig     = NULL;
  else
    gDRV_reszObj.setup.rszBConfig     = &gDRV_reszObj.outConfig[1];

    if(inType==DRV_DATA_FORMAT_RAW)
    gDRV_reszObj.inConfig.inputDataPath      = CSL_RSZ_INPUT_DATA_PATH_IPIPE;
  else
    gDRV_reszObj.inConfig.inputDataPath      = CSL_RSZ_INPUT_DATA_PATH_IPIPEIF;

  gDRV_reszObj.inConfig.chormaFormatType   = CSL_RSZ_CHROMA_FORMAT_422;

  gDRV_reszObj.inConfig.yuv420Color        = CSL_RSZ_420_COL_SELECT_Y;

    if(inType==DRV_DATA_FORMAT_RAW) {
    gDRV_reszObj.inFrameConfig.inStartX     = 0;
    gDRV_reszObj.inFrameConfig.inStartY     = 0;
  } else {

    gDRV_reszObj.inFrameConfig.inStartX     = startX;
      gDRV_reszObj.inFrameConfig.inStartY     = startY; // jbl 0;
  }

    gDRV_reszObj.inFrameConfig.inWidth      = prm.inWidth;
    gDRV_reszObj.inFrameConfig.inHeight     = prm.inHeight;

  for(i=0; i<CSL_RSZ_CH_MAX; i++)
  {
    pOutConfig = &gDRV_reszObj.outConfig[i];

    pOutConfig->inStartX               = 0;
    pOutConfig->inStartY               = startY;

      if(prm.pOut[i]) {

        pOutConfig->flipH                  = prm.pOut[i]->flipH;
        pOutConfig->flipV                  = prm.pOut[i]->flipV;

      pOutConfig->yuv420YOutEnable       = FALSE;
      pOutConfig->yuv420COutEnable       = FALSE;

        if(prm.pOut[i]->outType==DRV_DATA_FORMAT_YUV420) {

        pOutConfig->yuv420YOutEnable       = TRUE;
        pOutConfig->yuv420COutEnable       = TRUE;

        #if 1
          if(inType==DRV_DATA_FORMAT_YUV420)
          pOutConfig->yuv420COutEnable       = FALSE;
        #endif
      }


        pOutConfig->outWidth               = prm.pOut[i]->outWidth;
        pOutConfig->outHeight              = prm.pOut[i]->outHeight;

        pOutConfig->vertReszRatio          = ((Uint32)prm.inHeight*256)/prm.pOut[i]->outHeight;
        pOutConfig->horzReszRatio          = ((Uint32)prm.inWidth*256)/prm.pOut[i]->outWidth;

        pOutConfig->outStartAddr           = prm.pOut[i]->outPhysAddr;

        if(prm.pOut[i]->outType==DRV_DATA_FORMAT_YUV420)
          pOutConfig->outLineOffset        = prm.pOut[i]->outOffsetH;
      else
          pOutConfig->outLineOffset        = prm.pOut[i]->outOffsetH*2;

        if(prm.pOut[i]->flipV) {
          pOutConfig->outStartAddr        += pOutConfig->outLineOffset*(prm.pOut[i]->outHeight-1);
      }

        if(prm.pOut[i]->flipH) {
          if(prm.pOut[i]->outType==DRV_DATA_FORMAT_YUV420)
            pOutConfig->outStartAddr      += prm.pOut[i]->outWidth - 1;
        else
            pOutConfig->outStartAddr      += prm.pOut[i]->outWidth*2 - 1;
      }

      pOutConfig->outBaseAddr            = pOutConfig->outStartAddr;
      pOutConfig->outStartLine           = 0;
        pOutConfig->outEndLine             = prm.pOut[i]->outHeight;

        if(prm.pOut[i]->outType==DRV_DATA_FORMAT_YUV420)
      {
          pOutConfig->yuv420CoutStartAddr    = prm.pOut[i]->outPhysAddr + prm.pOut[i]->outOffsetH*prm.pOut[i]->outOffsetV;

          if(prm.pOut[i]->flipV) {
            pOutConfig->yuv420CoutStartAddr += pOutConfig->outLineOffset*(prm.pOut[i]->outHeight/2-1);
        }

          if(prm.pOut[i]->flipH) {
            if(prm.pOut[i]->outType==DRV_DATA_FORMAT_YUV420)
              pOutConfig->yuv420CoutStartAddr  += prm.pOut[i]->outWidth - 1;
          else
              pOutConfig->yuv420CoutStartAddr  += prm.pOut[i]->outWidth*2 - 1;
        }

          pOutConfig->yuv420CoutLineOffset   = prm.pOut[i]->outOffsetH;
        pOutConfig->yuv420CoutBaseAddr     = pOutConfig->yuv420CoutStartAddr;
        pOutConfig->yuv420CoutStartLine    = 0;
          pOutConfig->yuv420CoutEndLine      = prm.pOut[i]->outHeight/2;
      }
    }
  }

    if(prm.clkDivM==0 || prm.clkDivN==0) {

    gDRV_reszObj.ipipeifClk.clkDivM       = 10;
    gDRV_reszObj.ipipeifClk.clkDivN       = 80;

  } else {

      gDRV_reszObj.ipipeifClk.clkDivM       = prm.clkDivM;
      gDRV_reszObj.ipipeifClk.clkDivN       = prm.clkDivN;
  }

    gDRV_reszObj.ipipeifClk.pixelsPerLine = prm.inWidth+startX+16;
    gDRV_reszObj.ipipeifClk.linesPerFrame = prm.inHeight+16;

  gDRV_reszObj.dpcmConfig.enable        = FALSE;

  gDRV_reszObj.ddrInConfig.rawUnpackMode = CSL_IPIPEIF_SDRAM_IN_RAW_UNPACK_MODE_NORMAL_16;

  gDRV_reszObj.inSrcConfig.gain          = 0x200;
  gDRV_reszObj.ddrInConfig.rawDataShift  = CSL_IPIPEIF_SDRAM_IN_DATASHIFT_BITS_11_0;

    if(inType==DRV_DATA_FORMAT_RAW)
  {
      if(prm.enableInvAlaw) {

      gDRV_reszObj.ddrInConfig.rawUnpackMode = CSL_IPIPEIF_SDRAM_IN_RAW_UNPACK_MODE_PACK_10;

      // need to put 4x gain to make 10-bit inverse A-law data to 12-bit raw data

      // 2x gain in IPIPEIF
      gDRV_reszObj.inSrcConfig.gain          = 0x3FF;

      // 2x gain in IPIPEWB
      oldWbGain[0] = gCSL_vpssHndl.ipipeRegs->WB2_WGN_R;
      oldWbGain[1] = gCSL_vpssHndl.ipipeRegs->WB2_WGN_GR;
      oldWbGain[2] = gCSL_vpssHndl.ipipeRegs->WB2_WGN_GB;
      oldWbGain[3] = gCSL_vpssHndl.ipipeRegs->WB2_WGN_B;

      gCSL_vpssHndl.ipipeRegs->WB2_WGN_R <<= 1;
      gCSL_vpssHndl.ipipeRegs->WB2_WGN_GR <<= 1;
      gCSL_vpssHndl.ipipeRegs->WB2_WGN_GB <<= 1;
      gCSL_vpssHndl.ipipeRegs->WB2_WGN_B <<= 1;

    } else
      if(prm.enableInvDpcm) {
      gDRV_reszObj.dpcmConfig.enable         = TRUE;
      gDRV_reszObj.ddrInConfig.rawUnpackMode = CSL_IPIPEIF_SDRAM_IN_RAW_UNPACK_MODE_PACK_12;
    }
  }

  gDRV_reszObj.ddrInConfig.inputAddr       = inPhysAddr;
    gDRV_reszObj.ddrInConfig.inputWidth      = prm.inWidth+startX;
    gDRV_reszObj.ddrInConfig.inputHeight     = prm.inHeight;
  gDRV_reszObj.ddrInConfig.inputLineOffset = bytesPerLine;

  gDRV_reszObj.inSrcConfig.isifInType       = CSL_IPIPEIF_ISIF_INPUT_TYPE_RAW;

    if(inType==DRV_DATA_FORMAT_RAW) {
    gDRV_reszObj.inSrcConfig.inputSource2           = CSL_IPIPEIF_INPUT_SOURCE_SDRAM_RAW;
  } else {
    gDRV_reszObj.inSrcConfig.inputSource2           = CSL_IPIPEIF_INPUT_SOURCE_SDRAM_YUV;
  }

    if(inType==DRV_DATA_FORMAT_YUV420) {
    gDRV_reszObj.inConfig.chormaFormatType    = CSL_RSZ_CHROMA_FORMAT_420;
    gDRV_reszObj.ddrInConfig.rawUnpackMode    = CSL_IPIPEIF_SDRAM_IN_RAW_UNPACK_MODE_PACK_08;
    gDRV_reszObj.inSrcConfig.inputSource2     = CSL_IPIPEIF_INPUT_SOURCE_SDRAM_RAW;
    gDRV_reszObj.inSrcConfig.isifInType       = CSL_IPIPEIF_ISIF_INPUT_TYPE_YUV;
  }

    DRV_reszInsertMagicNum(&prm, 1, 0);

    retry=0;

    do {

  CSL_ipipeifSdramInEnable(&gCSL_ipipeifHndl, FALSE);
  CSL_ipipeEnable(&gCSL_ipipeHndl, FALSE);
  CSL_rszEnable(&gCSL_rszHndl, FALSE);
  CSL_rszIntClear(&gCSL_rszHndl);

  status = CSL_rszHwSetup(&gCSL_rszHndl, &gDRV_reszObj.setup);
  if(status!=OSA_SOK) {
    OSA_ERROR("CSL_rszHwSetup()\n");
    goto error_exit;
  }

  status = CSL_ipipeifSetInSource2Config(&gCSL_ipipeifHndl, &gDRV_reszObj.inSrcConfig);
  if(status!=OSA_SOK) {
    OSA_ERROR("CSL_ipipeifSetInSource2Config()\n");
    goto error_exit;
  }

      if(inType==DRV_DATA_FORMAT_RAW) {

    CSL_IpipeInFrameConfig ipipeInConfig;

    ipipeInConfig.inStartX  = startX;
    ipipeInConfig.inStartY  = startY;
        ipipeInConfig.inWidth   = prm.inWidth;
        ipipeInConfig.inHeight  = prm.inHeight;

    status = CSL_ipipeSetInFrameConfig(&gCSL_ipipeHndl, &ipipeInConfig);
    if(status!=OSA_SOK) {
      OSA_ERROR("CSL_ipipeifSetInSource2Config()\n");
      goto error_exit;
    }

    CSL_ipipeOneShotEnable(&gCSL_ipipeHndl, TRUE);
    CSL_ipipeSetDataPath(&gCSL_ipipeHndl, CSL_IPIPE_IPIPE_DATA_PATH_BAYER_IN_YCBCR_RGB_OUT);
    CSL_ipipeEnable(&gCSL_ipipeHndl, TRUE);
  }

  CSL_rszIntEnable(&gCSL_rszHndl, TRUE);

  status = CSL_ipipeifSetSdramInConfig(&gCSL_ipipeifHndl, &gDRV_reszObj.ddrInConfig);
  if(status!=OSA_SOK) {
    OSA_ERROR("CSL_ipipeifSetSdramInConfig()\n");
    goto error_exit;
  }

  status = CSL_rszIntWait(&gCSL_rszHndl);
  if(status!=OSA_SOK) {
    OSA_ERROR("CSL_rszIntWait()\n");
    goto error_exit;
  }

      status = DRV_reszCheckMagicNum(&prm, 1, 0);
      if(status==OSA_SOK) {
        break;
      }

      status = OSA_SOK;

      retry++;

      #ifdef DRV_RESZ_DEBUG
      OSA_printf(" RESZ: Retrying %d ...\n", retry);
      #endif

    } while(retry<DRV_RESZ_RETRY_COUNT);

    if(inType==DRV_DATA_FORMAT_YUV420) {
    // run for Chroma input
    gDRV_reszObj.inSrcConfig.isifInType = CSL_IPIPEIF_ISIF_INPUT_TYPE_RAW;
    gDRV_reszObj.inConfig.yuv420Color   = CSL_RSZ_420_COL_SELECT_C;

    gDRV_reszObj.inFrameConfig.inStartY     /= 2;
    gDRV_reszObj.inFrameConfig.inHeight     /= 2;

    for(i=0; i<CSL_RSZ_CH_MAX; i++)
    {
      pOutConfig = &gDRV_reszObj.outConfig[i];
        if(prm.pOut[i]) {

        pOutConfig->yuv420YOutEnable       = FALSE;
        pOutConfig->yuv420COutEnable       = TRUE;
      }
    }

    gDRV_reszObj.ipipeifClk.linesPerFrame  = gDRV_reszObj.inFrameConfig.inHeight+16;

    gDRV_reszObj.ddrInConfig.inputAddr     = inPhysAddrC;
    gDRV_reszObj.ddrInConfig.inputHeight   /= 2;

    status = CSL_rszHwSetup(&gCSL_rszHndl, &gDRV_reszObj.setup);
    if(status!=OSA_SOK) {
      OSA_ERROR("CSL_rszHwSetup()\n");
      goto error_exit;
    }

    status = CSL_ipipeifSetInSource2Config(&gCSL_ipipeifHndl, &gDRV_reszObj.inSrcConfig);
    if(status!=OSA_SOK) {
      OSA_ERROR("CSL_ipipeifSetInSource2Config()\n");
      goto error_exit;
    }

    status = CSL_ipipeifSetSdramInConfig(&gCSL_ipipeifHndl, &gDRV_reszObj.ddrInConfig);
    if(status!=OSA_SOK) {
      OSA_ERROR("CSL_ipipeifSetSdramInConfig()\n");
      goto error_exit;
    }
    status = CSL_rszIntWait(&gCSL_rszHndl);
    if(status!=OSA_SOK) {
      OSA_ERROR("CSL_rszIntWait()\n");
      goto error_exit;
    }
  }

error_exit:

    if(inType==DRV_DATA_FORMAT_RAW)
  {
      if(prm.enableInvAlaw) {
      // restore original gains
      gCSL_vpssHndl.ipipeRegs->WB2_WGN_R  = oldWbGain[0];
      gCSL_vpssHndl.ipipeRegs->WB2_WGN_GR = oldWbGain[1];
      gCSL_vpssHndl.ipipeRegs->WB2_WGN_GB = oldWbGain[2];
      gCSL_vpssHndl.ipipeRegs->WB2_WGN_B  = oldWbGain[3];
    }
  }

  CSL_rszIntEnable(&gCSL_rszHndl, FALSE);
  CSL_ipipeifSdramInEnable(&gCSL_ipipeifHndl, FALSE);
  CSL_ipipeEnable(&gCSL_ipipeHndl, FALSE);
  CSL_rszEnable(&gCSL_rszHndl, FALSE);

    if(status!=OSA_SOK)
      break;
  }

  DRV_reszUnlock();

  if(inType==DRV_DATA_FORMAT_RAW) {
  	DRV_ipipeUnlock();
	  DRV_isifUnlock();  //ANR - ISIF

  }

  return status;
}

//Anshuman - Dec 11 2009 - To support digital PTZ in DDR In mode
int DRV_reszCalcDigitalPtz(Uint16 dzoom, Int16 pan, Int16 tilt, DRV_ReszRunPrm *rszPrm)
{

  int inStartX, inStartY, inWidth, inHeight, dx, dy;

  if(dzoom < 100 )
    dzoom = 100;

  if(dzoom > 400 )
    dzoom = 400;

  if(pan < -10)
    pan = -10;

  if(pan > 10)
    pan = 10;

  if(tilt < -10)
    tilt = -10;

  if(tilt > 10)
    tilt = 10;

  inWidth  = (rszPrm->inWidth*100)/dzoom;
  inHeight = (rszPrm->inHeight*100)/dzoom;

  inWidth = OSA_floor(inWidth, 2);
  inHeight= OSA_floor(inHeight, 2);

  inStartX = (rszPrm->inWidth  - inWidth)/2;
  inStartY = (rszPrm->inHeight - inHeight)/2;

  dx = OSA_align(rszPrm->inWidth/10, 2);
  dy = OSA_align(rszPrm->inHeight/10, 2);

  // increment or decrement X, Y by dx, dt amount
  inStartX = pan*dx;
  inStartY = tilt*dy;

  // bound X, Y within the frame
  if(inStartX < 0) {
    inStartX = 0;
  }

  if(inStartX + inWidth > rszPrm->inWidth) {
    inStartX = (rszPrm->inWidth - inWidth);
  }

  if(inStartY < 0) {
    inStartY = 0;
  }

  if(inStartY + inHeight > rszPrm->inHeight) {
    inStartY = (rszPrm->inHeight - inHeight);
  }

  inStartX = OSA_floor(inStartX, 2);
  inStartY = OSA_floor(inStartY, 2);

  rszPrm->inStartX = inStartX;
  rszPrm->inStartY = inStartY;
  rszPrm->inWidth  = inWidth;
  rszPrm->inHeight = inHeight;

  return OSA_SOK;
}

int DRV_reszTestMain(int argc, char **argv)
{
  int status;
  Uint8 *inVirtAddr=NULL, *inPhysAddr, *outVirtAddr[2]={NULL, NULL}, *outPhysAddr[2];
  OSA_PrfHndl prfResz;
  int i, count=10000;

  Uint32 size=1*MB, fileSize;

  DRV_ReszRunPrm prm;
  DRV_ReszOutPrm outPrm[2];

  status = DRV_init();
  if(status!=OSA_SOK)
    return status;

  OSA_prfReset(&prfResz);

  inVirtAddr     = OSA_cmemAlloc(size, 32);
  outVirtAddr[0] = OSA_cmemAlloc(size, 32);
  outVirtAddr[1] = OSA_cmemAlloc(size, 32);

  inPhysAddr     = OSA_cmemGetPhysAddr(inVirtAddr);
  outPhysAddr[0] = OSA_cmemGetPhysAddr(outVirtAddr[0]);
  outPhysAddr[1] = OSA_cmemGetPhysAddr(outVirtAddr[1]);

  if(   inVirtAddr==NULL || outVirtAddr[0] == NULL || outVirtAddr[1] == NULL
     || inPhysAddr==NULL || outPhysAddr[0] == NULL || outPhysAddr[1] == NULL
    ) {
    OSA_ERROR("OSA_cmemAlloc()\n");
    goto error_exit;
  }

  #if 0
  memset(outVirtAddr[0], 0xAA, size);
  memset(outVirtAddr[1], 0xAA, size);
  #endif

  #if 0
  status = OSA_fileReadFile("video.yuv420", inVirtAddr, size, &fileSize);
  if(status!=OSA_SOK) {
    OSA_ERROR("OSA_fileReadFile()\n");
    goto error_exit;
  }
  #endif

  OSA_printf(" inAddr=%x, outAddr[0]=%x outAddr[1]=%x\n", (Uint32)inPhysAddr, (Uint32)outPhysAddr[0], (Uint32)outPhysAddr[1] );

  prm.inType = DRV_DATA_FORMAT_YUV422 | DRV_DATA_FORMAT_INTERLACED;
  prm.inPhysAddr = inPhysAddr;
  prm.inVirtAddr = inVirtAddr;
  prm.inStartX = 0;
  prm.inStartY = 0;
  prm.inWidth  = 352;
  prm.inHeight = 240;
  prm.inOffsetH= OSA_align(prm.inWidth, 16);
  prm.inOffsetV= prm.inHeight;
  prm.enableInvAlaw = FALSE;
  prm.enableInvDpcm = FALSE;
  prm.clkDivM = 10;
  prm.clkDivN = 80;

  prm.pOut[0] = NULL;
  prm.pOut[1] = NULL;

  prm.pOut[0] = &outPrm[0];
  //prm.pOut[1] = &outPrm[1];

  outPrm[0].outType = DRV_DATA_FORMAT_YUV420;
  outPrm[0].flipH = FALSE;
  outPrm[0].flipV = FALSE;
  outPrm[0].outPhysAddr = outPhysAddr[0];
  outPrm[0].outVirtAddr = outVirtAddr[0];
  outPrm[0].outWidth = 352;
  outPrm[0].outHeight = 240;
  outPrm[0].outOffsetH = OSA_align(outPrm[0].outWidth, 32);
  outPrm[0].outOffsetV = outPrm[0].outHeight;

  outPrm[1].outType = outPrm[0].outType;
  outPrm[1].flipH = outPrm[0].flipH;
  outPrm[1].flipV = outPrm[0].flipV;
  outPrm[1].outPhysAddr = outPhysAddr[1];
  outPrm[1].outVirtAddr = outVirtAddr[1];
  outPrm[1].outWidth = 352;
  outPrm[1].outHeight = 240;
  outPrm[1].outOffsetH = OSA_align(outPrm[1].outWidth, 32);
  outPrm[1].outOffsetV = outPrm[1].outHeight;

  #ifdef DRV_RESZ_TEST_IPIPE
  {
    DRV_IpipeConfig ipipeConfig;

    ipipeConfig.sensorMode              = DRV_IMGS_SENSOR_MODE_720x480;
    ipipeConfig.inputSrc                = DRV_IPIPE_INPUT_SRC_DDR;
    ipipeConfig.boxcarBlockSize         = DRV_IPIPE_BOXCAR_BLOCK_SIZE_NONE;
    ipipeConfig.histogramMode           = DRV_IPIPE_HISTO_MODE_NONE;

    ipipeConfig.bscNumVectorsRow        = 0;
    ipipeConfig.bscNumVectorsCol        = ipipeConfig.bscNumVectorsRow;
    ipipeConfig.rszValidDataStartOffset = 0;

    ipipeConfig.rszOutConfig[0].enable    = FALSE;
    ipipeConfig.rszOutConfig[0].outFormat = outPrm[0].outType;
    ipipeConfig.rszOutConfig[0].width     = outPrm[0].outWidth;
    ipipeConfig.rszOutConfig[0].height    = outPrm[0].outHeight;
    ipipeConfig.rszOutConfig[0].flipH     = FALSE;
    ipipeConfig.rszOutConfig[0].flipV     = FALSE;
    ipipeConfig.rszOutConfig[0].numBuf    = 0;

    ipipeConfig.rszOutConfig[1].enable    = FALSE;
    ipipeConfig.rszOutConfig[1].outFormat = outPrm[1].outType;
    ipipeConfig.rszOutConfig[1].width     = outPrm[1].outWidth;
    ipipeConfig.rszOutConfig[1].height    = outPrm[1].outHeight;
    ipipeConfig.rszOutConfig[1].flipH     = FALSE;
    ipipeConfig.rszOutConfig[1].flipV     = FALSE;
    ipipeConfig.rszOutConfig[1].numBuf    = 0;

    status = DRV_ipipeOpen(&ipipeConfig);
  }
  #endif

  OSA_prfBegin(&prfResz);

  for(i=0;i<count; i++)
    status = DRV_reszRun(&prm);

  OSA_prfEnd(&prfResz, count);

  #ifdef DRV_RESZ_TEST_IPIPE
  DRV_ipipeClose();
  #endif

  if(status!=OSA_SOK) {
    OSA_ERROR("DRV_reszRun()\n");
    goto error_exit;
  }

  OSA_prfPrint(&prfResz, "Resz", 0);

  #if 0

  if(prm.pOut[0]) {
    OSA_printf(" Writing size=(%dx%d) offset=(%dx%d)\n", outPrm[0].outWidth, outPrm[0].outHeight, outPrm[0].outOffsetH, outPrm[0].outOffsetV);

    fileSize = outPrm[0].outOffsetH*outPrm[0].outOffsetV*2;

    status = OSA_fileWriteFile("output0.yuv", outVirtAddr[0], fileSize);
    if(status!=OSA_SOK) {
      goto error_exit;
    }
  }

  if(prm.pOut[1]) {
    OSA_printf(" Writing size=(%dx%d) offset=(%dx%d)\n", outPrm[1].outWidth, outPrm[1].outHeight, outPrm[1].outOffsetH, outPrm[1].outOffsetV);

    fileSize = outPrm[1].outOffsetH*outPrm[1].outOffsetV*2;

    status = OSA_fileWriteFile("output1.yuv", outVirtAddr[1], fileSize);
    if(status!=OSA_SOK) {
      goto error_exit;
    }
  }
  #endif

error_exit:
  if(inVirtAddr)
    OSA_cmemFree(inVirtAddr);

  if(outVirtAddr[0])
    OSA_cmemFree(outVirtAddr[0]);

  if(outVirtAddr[1])
    OSA_cmemFree(outVirtAddr[1]);

  status = DRV_exit();

  return status;
}

