/*
    DM360 Evaluation Software

    (c)Texas Instruments 2007
*/
#include <csl_ccdc.h>

CSL_Status CSL_ccdcEnable(CSL_CcdcHandle hndl, Bool32 enable)
{
  if (hndl == NULL)
    return CSL_EFAIL;

  if(enable==FALSE)
    hndl->count = 0;

  CSL_FINS(hndl->regs->SYNCEN, ISIF_SYNCEN_SYEN, enable);

  return CSL_SOK;
}

CSL_Status CSL_ccdcSetInDataConfig(CSL_CcdcHandle hndl, CSL_CcdcInDataConfig * data)
{
  if (hndl == NULL || data == NULL)
    return CSL_EFAIL;

  CSL_FINS(hndl->regs->MODESET, ISIF_MODESET_HLPF, data->hLpfEnable);
  CSL_FINS(hndl->regs->MODESET, ISIF_MODESET_INPMOD, data->inDataType);
  CSL_FINS(hndl->regs->MODESET, ISIF_MODESET_DPOL, data->dataPolarity);
  CSL_FINS(hndl->regs->CCDCFG, ISIF_CCDCFG_Y8POS, data->yPos);
  CSL_FINS(hndl->regs->CGAMMAWD, ISIF_CGAMMAWD_GWDI, data->inDataMsbPosition);

  return CSL_SOK;
}

CSL_Status CSL_ccdcSdramOutEnable(CSL_CcdcHandle hndl, Bool32 enable)
{
  if (hndl == NULL)
    return CSL_EFAIL;

  CSL_FINS(hndl->regs->SYNCEN, ISIF_SYNCEN_DWEN, enable);

  return CSL_SOK;
}

CSL_Status CSL_ccdcSetSyncConfig(CSL_CcdcHandle hndl, CSL_CcdcSyncConfig * data)
{
  if (hndl == NULL || data == NULL)
    return CSL_EFAIL;

  CSL_FINS(hndl->regs->MODESET, ISIF_MODESET_CCDMD, data->interlaceMode);
  CSL_FINS(hndl->regs->MODESET, ISIF_MODESET_SWEN, data->wenUseEnable);

  CSL_FINS(hndl->regs->MODESET, ISIF_MODESET_FIPOL, data->fidPolarity);
  CSL_FINS(hndl->regs->MODESET, ISIF_MODESET_HDPOL, data->hdPolarity);
  CSL_FINS(hndl->regs->MODESET, ISIF_MODESET_VDPOL, data->vdPolarity);
  CSL_FINS(hndl->regs->MODESET, ISIF_MODESET_HDVDD, data->hdVdDir);
  CSL_FINS(hndl->regs->MODESET, ISIF_MODESET_FIDD, data->fidDir);

  if(data->hdWidth==0)
    data->hdWidth = 1;

  CSL_FINS(hndl->regs->HDW, ISIF_HDW_HDW, data->hdWidth - 1);

  if(data->vdWidth==0)
    data->vdWidth = 1;

  CSL_FINS(hndl->regs->VDW, ISIF_VDW_VDW, data->vdWidth - 1);

  if(data->pixelsPerLine==0)
    data->pixelsPerLine=1;

  CSL_FINS(hndl->regs->PPLN, ISIF_PPLN_PPLN, data->pixelsPerLine - 1);

  if(data->linesPerFrame==0)
    data->linesPerFrame=1;

  CSL_FINS(hndl->regs->LPFR, ISIF_LPFR_LPFR, 2 * data->linesPerFrame - 1);

  return CSL_SOK;
}


CSL_Status CSL_ccdcSetMiscConfig(CSL_CcdcHandle hndl, CSL_CcdcMiscConfig * data)
{
  if (hndl == NULL || data == NULL)
    return CSL_EFAIL;

  CSL_FINS(hndl->regs->CGAMMAWD, ISIF_CGAMMAWD_CFAP, data->cfaPattern);

  CSL_FINS(hndl->regs->CCDCFG, ISIF_CCDCFG_VDLC, data->vdLatchDisable);
  CSL_FINS(hndl->regs->CCDCFG, ISIF_CCDCFG_MSBINVO, data->inverseMsbCout);
  CSL_FINS(hndl->regs->CCDCFG, ISIF_CCDCFG_MSBINVI, data->inverseMsbCin);

  CSL_FINS(hndl->regs->CCDCFG, ISIF_CCDCFG_EXTRG, data->sdramAddrInitExtTiggerEnable);
  CSL_FINS(hndl->regs->CCDCFG, ISIF_CCDCFG_TRGSEL, data->sdramAddrInitTiggerSource);

  CSL_FINS(hndl->regs->CCDCFG, ISIF_CCDCFG_WENLOG, data->orWenIntExt);
  CSL_FINS(hndl->regs->CCDCFG, ISIF_CCDCFG_FIDMD, data->fidLatchAtVdDisable);

  CSL_FINS(hndl->regs->CCDCFG, ISIF_CCDCFG_YCINSWP, data->ycInSwap);
  CSL_FINS(hndl->regs->CCDCFG, ISIF_CCDCFG_YCOUTSWP, data->ycOutSwap);

  return CSL_SOK;
}

CSL_Status CSL_ccdcSetColPatConfig(CSL_CcdcHandle hndl, CSL_CcdcColPatConfig * data)
{
  if (hndl == NULL || data == NULL)
    return CSL_EFAIL;

  CSL_FINS(hndl->regs->CCOLP, ISIF_CCOLP_CP01_0, data->colPat0[1][1]);
  CSL_FINS(hndl->regs->CCOLP, ISIF_CCOLP_CP03_2, data->colPat0[1][0]);
  CSL_FINS(hndl->regs->CCOLP, ISIF_CCOLP_CP05_4, data->colPat0[0][1]);
  CSL_FINS(hndl->regs->CCOLP, ISIF_CCOLP_CP07_6, data->colPat0[0][0]);

  CSL_FINS(hndl->regs->CCOLP, ISIF_CCOLP_CP11_0, data->colPat1[1][1]);
  CSL_FINS(hndl->regs->CCOLP, ISIF_CCOLP_CP13_2, data->colPat1[1][0]);
  CSL_FINS(hndl->regs->CCOLP, ISIF_CCOLP_CP15_4, data->colPat1[0][1]);
  CSL_FINS(hndl->regs->CCOLP, ISIF_CCOLP_CP17_6, data->colPat1[0][0]);

  return CSL_SOK;
}

CSL_Status CSL_ccdcSetLinerizationConfig(CSL_CcdcHandle hndl, CSL_CcdcLinerizationConfig * data)
{
  Uint16  i;
  volatile Uint32 *tableMemAddr[2];

  if (hndl == NULL || data == NULL)
    return CSL_EFAIL;

  CSL_FINS(hndl->regs->LINCFG0, ISIF_LINCFG0_CORRSFT, data->corrValShift);
  CSL_FINS(hndl->regs->LINCFG1, ISIF_LINCFG1_LUTSCL, data->lutInScale);

  if(data->numLutEntries > CSL_ISIF_LIN_TBL_MAX_ENTRIES)
    data->numLutEntries = CSL_ISIF_LIN_TBL_MAX_ENTRIES;

  if(data->lutTable!=NULL) {

    tableMemAddr[0] = (Uint32*)CSL_ISIF_LIN_TBL_0_ADDR;
    tableMemAddr[1] = (Uint32*)CSL_ISIF_LIN_TBL_1_ADDR;

    for (i = 0; i < data->numLutEntries; i+= 2) {
      *tableMemAddr[0]++ = data->lutTable[i];
      *tableMemAddr[1]++ = data->lutTable[i+1];
    }
  }

  CSL_FINS(hndl->regs->LINCFG0, ISIF_LINCFG0_LINEN, data->enable);

  return CSL_SOK;
}


CSL_Status CSL_ccdcSetFmtCscInFrameConfig(CSL_CcdcHandle hndl, CSL_CcdcFmtCscInFrameConfig * data)
{
  if (hndl == NULL || data == NULL)
    return CSL_EFAIL;

  CSL_FINS(hndl->regs->FMTSPH, ISIF_FMTSPH_FMTSPH, data->fmtsph);
  CSL_FINS(hndl->regs->FMTLNH, ISIF_FMTLNH_FMTLNH, data->fmtlnh - 1);
  CSL_FINS(hndl->regs->FMTSLV, ISIF_FMTSLV_FMTSLV, data->fmtslv);
  CSL_FINS(hndl->regs->FMTLNV, ISIF_FMTLNV_FMTLNV, data->fmtlnv - 1);

  return CSL_SOK;
}

CSL_Status CSL_ccdcSetFmtConfig(CSL_CcdcHandle hndl, CSL_CcdcFmtConfig * data)
{
  Uint32  val;
  CSL_Status status = CSL_SOK;

  if (hndl == NULL || data == NULL)
    return CSL_EFAIL;

  switch (data->fmtMode) {

  case CSL_CCDC_FMT_LINE_SPLITING:
    CSL_FINS(hndl->regs->FMTCFG, ISIF_FMTCFG_FMTCBL, 0);
    break;
  case CSL_CCDC_FMT_LINE_COMBINING:
    CSL_FINS(hndl->regs->FMTCFG, ISIF_FMTCFG_FMTCBL, 1);
    break;
  case CSL_CCDC_FMT_LINE_ALTERNATING:
    CSL_FINS(hndl->regs->FMTCFG, ISIF_FMTCFG_FMTCBL, 0);
    CSL_FINS(hndl->regs->FMTCFG, ISIF_FMTCFG_LNALT, 1);
    break;
  default:
    status = CSL_EFAIL;
    break;
  }

  CSL_FINS(hndl->regs->FMTCFG, ISIF_FMTCFG_LNUM, data->numLines - 1);
  CSL_FINS(hndl->regs->FMTCFG, ISIF_FMTCFG_FMTAINC, data->addrInc - 1);

  CSL_FINS(hndl->regs->FMTPLEN, ISIF_FMTPLEN_FMTPLEN0, data->progLength[0] - 1);
  CSL_FINS(hndl->regs->FMTPLEN, ISIF_FMTPLEN_FMTPLEN1, data->progLength[1] - 1);
  CSL_FINS(hndl->regs->FMTPLEN, ISIF_FMTPLEN_FMTPLEN2, data->progLength[2] - 1);
  CSL_FINS(hndl->regs->FMTPLEN, ISIF_FMTPLEN_FMTPLEN3, data->progLength[3] - 1);


  CSL_FINS(hndl->regs->FMTRLEN, ISIF_FMTRLEN_FMTRLEN, data->outLineLength);

  CSL_FINS(hndl->regs->FMTHCNT, ISIF_FMTHCNT_FMTHCNT, data->hCnt);

  CSL_FINS(hndl->regs->FMTAPTR0, ISIF_FMTAPTR0_LINE, data->fmtAddrLine[0]);
  CSL_FINS(hndl->regs->FMTAPTR1, ISIF_FMTAPTR1_LINE, data->fmtAddrLine[1]);
  CSL_FINS(hndl->regs->FMTAPTR2, ISIF_FMTAPTR2_LINE, data->fmtAddrLine[2]);
  CSL_FINS(hndl->regs->FMTAPTR3, ISIF_FMTAPTR3_LINE, data->fmtAddrLine[3]);
  CSL_FINS(hndl->regs->FMTAPTR4, ISIF_FMTAPTR4_LINE, data->fmtAddrLine[4]);
  CSL_FINS(hndl->regs->FMTAPTR5, ISIF_FMTAPTR5_LINE, data->fmtAddrLine[5]);
  CSL_FINS(hndl->regs->FMTAPTR6, ISIF_FMTAPTR6_LINE, data->fmtAddrLine[6]);
  CSL_FINS(hndl->regs->FMTAPTR7, ISIF_FMTAPTR7_LINE, data->fmtAddrLine[7]);
  CSL_FINS(hndl->regs->FMTAPTR8, ISIF_FMTAPTR8_LINE, data->fmtAddrLine[8]);
  CSL_FINS(hndl->regs->FMTAPTR9, ISIF_FMTAPTR9_LINE, data->fmtAddrLine[9]);
  CSL_FINS(hndl->regs->FMTAPTR10, ISIF_FMTAPTR10_LINE, data->fmtAddrLine[10]);
  CSL_FINS(hndl->regs->FMTAPTR11, ISIF_FMTAPTR11_LINE, data->fmtAddrLine[11]);
  CSL_FINS(hndl->regs->FMTAPTR12, ISIF_FMTAPTR12_LINE, data->fmtAddrLine[12]);
  CSL_FINS(hndl->regs->FMTAPTR13, ISIF_FMTAPTR13_LINE, data->fmtAddrLine[13]);
  CSL_FINS(hndl->regs->FMTAPTR14, ISIF_FMTAPTR14_LINE, data->fmtAddrLine[14]);
  CSL_FINS(hndl->regs->FMTAPTR15, ISIF_FMTAPTR15_LINE, data->fmtAddrLine[15]);

  CSL_FINS(hndl->regs->FMTAPTR0, ISIF_FMTAPTR0_INIT, data->fmtAddrInit[0]);
  CSL_FINS(hndl->regs->FMTAPTR1, ISIF_FMTAPTR1_INIT, data->fmtAddrInit[1]);
  CSL_FINS(hndl->regs->FMTAPTR2, ISIF_FMTAPTR2_INIT, data->fmtAddrInit[2]);
  CSL_FINS(hndl->regs->FMTAPTR3, ISIF_FMTAPTR3_INIT, data->fmtAddrInit[3]);
  CSL_FINS(hndl->regs->FMTAPTR4, ISIF_FMTAPTR4_INIT, data->fmtAddrInit[4]);
  CSL_FINS(hndl->regs->FMTAPTR5, ISIF_FMTAPTR5_INIT, data->fmtAddrInit[5]);
  CSL_FINS(hndl->regs->FMTAPTR6, ISIF_FMTAPTR6_INIT, data->fmtAddrInit[6]);
  CSL_FINS(hndl->regs->FMTAPTR7, ISIF_FMTAPTR7_INIT, data->fmtAddrInit[7]);
  CSL_FINS(hndl->regs->FMTAPTR8, ISIF_FMTAPTR8_INIT, data->fmtAddrInit[8]);
  CSL_FINS(hndl->regs->FMTAPTR9, ISIF_FMTAPTR9_INIT, data->fmtAddrInit[9]);
  CSL_FINS(hndl->regs->FMTAPTR10, ISIF_FMTAPTR10_INIT, data->fmtAddrInit[10]);
  CSL_FINS(hndl->regs->FMTAPTR11, ISIF_FMTAPTR11_INIT, data->fmtAddrInit[11]);
  CSL_FINS(hndl->regs->FMTAPTR12, ISIF_FMTAPTR12_INIT, data->fmtAddrInit[12]);
  CSL_FINS(hndl->regs->FMTAPTR13, ISIF_FMTAPTR13_INIT, data->fmtAddrInit[13]);
  CSL_FINS(hndl->regs->FMTAPTR14, ISIF_FMTAPTR14_INIT, data->fmtAddrInit[14]);
  CSL_FINS(hndl->regs->FMTAPTR15, ISIF_FMTAPTR15_INIT, data->fmtAddrInit[15]);

  val = CSL_FMK(ISIF_FMTPGMVF0_PGM00EN, data->valid[0])|
        CSL_FMK(ISIF_FMTPGMVF0_PGM01EN, data->valid[1])|
        CSL_FMK(ISIF_FMTPGMVF0_PGM02EN, data->valid[2])|
        CSL_FMK(ISIF_FMTPGMVF0_PGM03EN, data->valid[3])|
        CSL_FMK(ISIF_FMTPGMVF0_PGM04EN, data->valid[4])|
        CSL_FMK(ISIF_FMTPGMVF0_PGM05EN, data->valid[5])|
        CSL_FMK(ISIF_FMTPGMVF0_PGM06EN, data->valid[6])|
        CSL_FMK(ISIF_FMTPGMVF0_PGM07EN, data->valid[7])|
        CSL_FMK(ISIF_FMTPGMVF0_PGM08EN, data->valid[8])|
        CSL_FMK(ISIF_FMTPGMVF0_PGM09EN, data->valid[9])|
        CSL_FMK(ISIF_FMTPGMVF0_PGM10EN, data->valid[10])|
        CSL_FMK(ISIF_FMTPGMVF0_PGM11EN, data->valid[11])|
        CSL_FMK(ISIF_FMTPGMVF0_PGM12EN, data->valid[12])|
        CSL_FMK(ISIF_FMTPGMVF0_PGM13EN, data->valid[13])|
        CSL_FMK(ISIF_FMTPGMVF0_PGM14EN, data->valid[14])|
        CSL_FMK(ISIF_FMTPGMVF0_PGM15EN, data->valid[15]);

  hndl->regs->FMTPGMVF0 = val;

  val = CSL_FMK(ISIF_FMTPGMVF1_PGM16EN, data->valid[16])|
        CSL_FMK(ISIF_FMTPGMVF1_PGM17EN, data->valid[17])|
        CSL_FMK(ISIF_FMTPGMVF1_PGM18EN, data->valid[18])|
        CSL_FMK(ISIF_FMTPGMVF1_PGM19EN, data->valid[19])|
        CSL_FMK(ISIF_FMTPGMVF1_PGM20EN, data->valid[20])|
        CSL_FMK(ISIF_FMTPGMVF1_PGM21EN, data->valid[21])|
        CSL_FMK(ISIF_FMTPGMVF1_PGM22EN, data->valid[22])|
        CSL_FMK(ISIF_FMTPGMVF1_PGM23EN, data->valid[23])|
        CSL_FMK(ISIF_FMTPGMVF1_PGM24EN, data->valid[24])|
        CSL_FMK(ISIF_FMTPGMVF1_PGM25EN, data->valid[25])|
        CSL_FMK(ISIF_FMTPGMVF1_PGM26EN, data->valid[26])|
        CSL_FMK(ISIF_FMTPGMVF1_PGM27EN, data->valid[27])|
        CSL_FMK(ISIF_FMTPGMVF1_PGM28EN, data->valid[28])|
        CSL_FMK(ISIF_FMTPGMVF1_PGM29EN, data->valid[29])|
        CSL_FMK(ISIF_FMTPGMVF1_PGM30EN, data->valid[30])|
        CSL_FMK(ISIF_FMTPGMVF1_PGM31EN, data->valid[31]);

  hndl->regs->FMTPGMVF1 = val;

  val = CSL_FMK(ISIF_FMTPGMAPU0_PGM0UPDT, data->update[0])|
        CSL_FMK(ISIF_FMTPGMAPU0_PGM1UPDT, data->update[1])|
        CSL_FMK(ISIF_FMTPGMAPU0_PGM2UPDT, data->update[2])|
        CSL_FMK(ISIF_FMTPGMAPU0_PGM3UPDT, data->update[3])|
        CSL_FMK(ISIF_FMTPGMAPU0_PGM4UPDT, data->update[4])|
        CSL_FMK(ISIF_FMTPGMAPU0_PGM5UPDT, data->update[5])|
        CSL_FMK(ISIF_FMTPGMAPU0_PGM6UPDT, data->update[6])|
        CSL_FMK(ISIF_FMTPGMAPU0_PGM7UPDT, data->update[7])|
        CSL_FMK(ISIF_FMTPGMAPU0_PGM8UPDT, data->update[8])|
        CSL_FMK(ISIF_FMTPGMAPU0_PGM9UPDT, data->update[9])|
        CSL_FMK(ISIF_FMTPGMAPU0_PGM10UPDT, data->update[10])|
        CSL_FMK(ISIF_FMTPGMAPU0_PGM11UPDT, data->update[11])|
        CSL_FMK(ISIF_FMTPGMAPU0_PGM12UPDT, data->update[12])|
        CSL_FMK(ISIF_FMTPGMAPU0_PGM13UPDT, data->update[13])|
        CSL_FMK(ISIF_FMTPGMAPU0_PGM14UPDT, data->update[14])|
        CSL_FMK(ISIF_FMTPGMAPU0_PGM15UPDT, data->update[15]);

  hndl->regs->FMTPGMAPU0 = val;

  val = CSL_FMK(ISIF_FMTPGMAPU1_PGM16UPDT, data->update[16])|
        CSL_FMK(ISIF_FMTPGMAPU1_PGM17UPDT, data->update[17])|
        CSL_FMK(ISIF_FMTPGMAPU1_PGM18UPDT, data->update[18])|
        CSL_FMK(ISIF_FMTPGMAPU1_PGM19UPDT, data->update[19])|
        CSL_FMK(ISIF_FMTPGMAPU1_PGM20UPDT, data->update[20])|
        CSL_FMK(ISIF_FMTPGMAPU1_PGM21UPDT, data->update[21])|
        CSL_FMK(ISIF_FMTPGMAPU1_PGM22UPDT, data->update[22])|
        CSL_FMK(ISIF_FMTPGMAPU1_PGM23UPDT, data->update[23])|
        CSL_FMK(ISIF_FMTPGMAPU1_PGM24UPDT, data->update[24])|
        CSL_FMK(ISIF_FMTPGMAPU1_PGM25UPDT, data->update[25])|
        CSL_FMK(ISIF_FMTPGMAPU1_PGM26UPDT, data->update[26])|
        CSL_FMK(ISIF_FMTPGMAPU1_PGM27UPDT, data->update[27])|
        CSL_FMK(ISIF_FMTPGMAPU1_PGM28UPDT, data->update[28])|
        CSL_FMK(ISIF_FMTPGMAPU1_PGM29UPDT, data->update[29])|
        CSL_FMK(ISIF_FMTPGMAPU1_PGM30UPDT, data->update[30])|
        CSL_FMK(ISIF_FMTPGMAPU1_PGM31UPDT, data->update[31]);

  hndl->regs->FMTPGMAPU1 = val;

  CSL_FINS(hndl->regs->FMTPGMAPS0, ISIF_FMTPGMAPS0_PGM0APTR, data->select[0]);
  CSL_FINS(hndl->regs->FMTPGMAPS0, ISIF_FMTPGMAPS0_PGM1APTR, data->select[1]);
  CSL_FINS(hndl->regs->FMTPGMAPS0, ISIF_FMTPGMAPS0_PGM2APTR, data->select[2]);
  CSL_FINS(hndl->regs->FMTPGMAPS0, ISIF_FMTPGMAPS0_PGM3APTR, data->select[3]);
  CSL_FINS(hndl->regs->FMTPGMAPS1, ISIF_FMTPGMAPS1_PGM4APTR, data->select[4]);
  CSL_FINS(hndl->regs->FMTPGMAPS1, ISIF_FMTPGMAPS1_PGM5APTR, data->select[5]);
  CSL_FINS(hndl->regs->FMTPGMAPS1, ISIF_FMTPGMAPS1_PGM6APTR, data->select[6]);
  CSL_FINS(hndl->regs->FMTPGMAPS1, ISIF_FMTPGMAPS1_PGM7APTR, data->select[7]);
  CSL_FINS(hndl->regs->FMTPGMAPS2, ISIF_FMTPGMAPS2_PGM8APTR, data->select[8]);
  CSL_FINS(hndl->regs->FMTPGMAPS2, ISIF_FMTPGMAPS2_PGM9APTR, data->select[9]);
  CSL_FINS(hndl->regs->FMTPGMAPS2, ISIF_FMTPGMAPS2_PGM10APTR, data->select[10]);
  CSL_FINS(hndl->regs->FMTPGMAPS2, ISIF_FMTPGMAPS2_PGM11APTR, data->select[11]);
  CSL_FINS(hndl->regs->FMTPGMAPS3, ISIF_FMTPGMAPS3_PGM12APTR, data->select[12]);
  CSL_FINS(hndl->regs->FMTPGMAPS3, ISIF_FMTPGMAPS3_PGM13APTR, data->select[13]);
  CSL_FINS(hndl->regs->FMTPGMAPS3, ISIF_FMTPGMAPS3_PGM14APTR, data->select[14]);
  CSL_FINS(hndl->regs->FMTPGMAPS3, ISIF_FMTPGMAPS3_PGM15APTR, data->select[15]);
  CSL_FINS(hndl->regs->FMTPGMAPS4, ISIF_FMTPGMAPS4_PGM16APTR, data->select[16]);
  CSL_FINS(hndl->regs->FMTPGMAPS4, ISIF_FMTPGMAPS4_PGM17APTR, data->select[17]);
  CSL_FINS(hndl->regs->FMTPGMAPS4, ISIF_FMTPGMAPS4_PGM18APTR, data->select[18]);
  CSL_FINS(hndl->regs->FMTPGMAPS4, ISIF_FMTPGMAPS4_PGM19APTR, data->select[19]);
  CSL_FINS(hndl->regs->FMTPGMAPS5, ISIF_FMTPGMAPS5_PGM20APTR, data->select[20]);
  CSL_FINS(hndl->regs->FMTPGMAPS5, ISIF_FMTPGMAPS5_PGM21APTR, data->select[21]);
  CSL_FINS(hndl->regs->FMTPGMAPS5, ISIF_FMTPGMAPS5_PGM22APTR, data->select[22]);
  CSL_FINS(hndl->regs->FMTPGMAPS5, ISIF_FMTPGMAPS5_PGM23APTR, data->select[23]);
  CSL_FINS(hndl->regs->FMTPGMAPS6, ISIF_FMTPGMAPS6_PGM24APTR, data->select[24]);
  CSL_FINS(hndl->regs->FMTPGMAPS6, ISIF_FMTPGMAPS6_PGM25APTR, data->select[25]);
  CSL_FINS(hndl->regs->FMTPGMAPS6, ISIF_FMTPGMAPS6_PGM26APTR, data->select[26]);
  CSL_FINS(hndl->regs->FMTPGMAPS6, ISIF_FMTPGMAPS6_PGM27APTR, data->select[27]);
  CSL_FINS(hndl->regs->FMTPGMAPS7, ISIF_FMTPGMAPS7_PGM28APTR, data->select[28]);
  CSL_FINS(hndl->regs->FMTPGMAPS7, ISIF_FMTPGMAPS7_PGM29APTR, data->select[29]);
  CSL_FINS(hndl->regs->FMTPGMAPS7, ISIF_FMTPGMAPS7_PGM30APTR, data->select[30]);
  CSL_FINS(hndl->regs->FMTPGMAPS7, ISIF_FMTPGMAPS7_PGM31APTR, data->select[31]);

  CSL_FINS(hndl->regs->FMTCFG, ISIF_FMTCFG_FMTEN, data->fmtEnable);

  return status;
}



CSL_Status CSL_ccdcSetCscConfig(CSL_CcdcHandle hndl, CSL_CcdcCscConfig * data)
{
  if (hndl == NULL || data == NULL)
    return CSL_EFAIL;

  CSL_FINS(hndl->regs->CSCM0, ISIF_CSCM0_CSCM00, data->matrix[0][0]);
  CSL_FINS(hndl->regs->CSCM0, ISIF_CSCM0_CSCM01, data->matrix[0][1]);
  CSL_FINS(hndl->regs->CSCM1, ISIF_CSCM1_CSCM02, data->matrix[0][2]);
  CSL_FINS(hndl->regs->CSCM1, ISIF_CSCM1_CSCM03, data->matrix[0][3]);
  CSL_FINS(hndl->regs->CSCM2, ISIF_CSCM2_CSCM10, data->matrix[1][0]);
  CSL_FINS(hndl->regs->CSCM2, ISIF_CSCM2_CSCM11, data->matrix[1][1]);
  CSL_FINS(hndl->regs->CSCM3, ISIF_CSCM3_CSCM12, data->matrix[1][2]);
  CSL_FINS(hndl->regs->CSCM3, ISIF_CSCM3_CSCM13, data->matrix[1][3]);
  CSL_FINS(hndl->regs->CSCM4, ISIF_CSCM4_CSCM20, data->matrix[2][0]);
  CSL_FINS(hndl->regs->CSCM4, ISIF_CSCM4_CSCM21, data->matrix[2][1]);
  CSL_FINS(hndl->regs->CSCM5, ISIF_CSCM5_CSCM22, data->matrix[2][2]);
  CSL_FINS(hndl->regs->CSCM5, ISIF_CSCM5_CSCM23, data->matrix[2][3]);
  CSL_FINS(hndl->regs->CSCM6, ISIF_CSCM6_CSCM30, data->matrix[3][0]);
  CSL_FINS(hndl->regs->CSCM6, ISIF_CSCM6_CSCM31, data->matrix[3][1]);
  CSL_FINS(hndl->regs->CSCM7, ISIF_CSCM7_CSCM32, data->matrix[3][2]);
  CSL_FINS(hndl->regs->CSCM7, ISIF_CSCM7_CSCM33, data->matrix[3][3]);

  CSL_FINS(hndl->regs->CSCCTL, ISIF_CSCCTL_CSCEN, data->enable);

  return CSL_SOK;
}

CSL_Status CSL_ccdcSetClampConfig(CSL_CcdcHandle hndl, CSL_CcdcClampConfig * data)
{
  CSL_Status status = CSL_SOK;

  if (hndl == NULL || data == NULL)
    return CSL_EFAIL;

  CSL_FINS(hndl->regs->CLAMPCFG, ISIF_CLAMPCFG_CLEN, data->enable);
  CSL_FINS(hndl->regs->CLAMPCFG, ISIF_CLAMPCFG_CLMD, data->mode);
  CSL_FINS(hndl->regs->CLDCOFST, ISIF_CLDCOFST_CLDC, data->dcOffset);

  CSL_FINS(hndl->regs->CLSV, ISIF_CLSV_CLSV, data->clampStartV);

  CSL_FINS(hndl->regs->CLAMPCFG, ISIF_CLAMPCFG_CLHMD, data->horzClampMode);

  CSL_FINS(hndl->regs->CLHWIN1, ISIF_CLHWIN1_CLHSH, data->horzClampWinStartH);
  CSL_FINS(hndl->regs->CLHWIN2, ISIF_CLHWIN2_CLHSV, data->horzClampWinStartV);

  switch (data->horzClampWinSizeV) {
  case 2:
    CSL_FINS(hndl->regs->CLHWIN0, ISIF_CLHWIN0_CLHWN, 0);
    break;
  case 4:
    CSL_FINS(hndl->regs->CLHWIN0, ISIF_CLHWIN0_CLHWN, 1);
    break;
  case 8:
    CSL_FINS(hndl->regs->CLHWIN0, ISIF_CLHWIN0_CLHWN, 2);
    break;
  case 16:
    CSL_FINS(hndl->regs->CLHWIN0, ISIF_CLHWIN0_CLHWN, 3);
    break;
  default:
    status = CSL_EFAIL;
    break;
  }

  switch (data->horzClampWinSizeH) {
  case 32:
    CSL_FINS(hndl->regs->CLHWIN0, ISIF_CLHWIN0_CLHWM, 0);
    break;
  case 64:
    CSL_FINS(hndl->regs->CLHWIN0, ISIF_CLHWIN0_CLHWM, 1);
    break;
  case 128:
    CSL_FINS(hndl->regs->CLHWIN0, ISIF_CLHWIN0_CLHWM, 2);
    break;
  case 256:
    CSL_FINS(hndl->regs->CLHWIN0, ISIF_CLHWIN0_CLHWM, 3);
    break;
  default:
    status = CSL_EFAIL;
    break;
  }

  CSL_FINS(hndl->regs->CLHWIN0, ISIF_CLHWIN0_CLHWBS, data->horzClampBaseWin);
  CSL_FINS(hndl->regs->CLHWIN0, ISIF_CLHWIN0_CLHWC, data->horzClampNumWin - 1);

  CSL_FINS(hndl->regs->CLVRV, ISIF_CLVRV_CLVRV, data->vertClampResetValue);
  CSL_FINS(hndl->regs->CLVWIN0, ISIF_CLVWIN0_CLVRVSL, data->vertClampResetValueSource);
  CSL_FINS(hndl->regs->CLVWIN0, ISIF_CLVWIN0_CLVCOEF, data->vertClampLineAvgCoeff);

  CSL_FINS(hndl->regs->CLHWIN0, ISIF_CLHWIN0_CLHLMT, data->horzClampBlack);
  CSL_FINS(hndl->regs->CLVWIN0, ISIF_CLVWIN0_CLVOBH, data->vertClampBlackOptical);

  CSL_FINS(hndl->regs->CLVWIN1, ISIF_CLVWIN1_CLVSH, data->vertClampWinStartH);
  CSL_FINS(hndl->regs->CLVWIN2, ISIF_CLVWIN2_CLVSV, data->vertClampWinStartV);

  CSL_FINS(hndl->regs->CLVWIN3, ISIF_CLVWIN3_CLVOBV, data->vertClampNumWin - 1);

  return status;
}


CSL_Status CSL_ccdcSetDfcLscOffsetConfig(CSL_CcdcHandle hndl, CSL_CcdcDfcLscOffsetConfig * data)
{
  if (hndl == NULL || data == NULL)
    return CSL_EFAIL;

  CSL_FINS(hndl->regs->DATAHOFST, ISIF_DATAHOFST_HOFST, data->hOffset);
  CSL_FINS(hndl->regs->DATAVOFST, ISIF_DATAVOFST_VOFST, data->vOffset);

  return CSL_SOK;
}

CSL_Status CSL_ccdcSetDfcConfig(CSL_CcdcHandle hndl, CSL_CcdcDfcConfig * data)
{
  CSL_Status status=CSL_SOK;

  if (hndl == NULL || data == NULL)
    return CSL_EFAIL;

  CSL_FINS(hndl->regs->DFCCTL, ISIF_DFCCTL_VDFCEN, data->enable);
  CSL_FINS(hndl->regs->DFCCTL, ISIF_DFCCTL_VDFCSL, data->mode);
  CSL_FINS(hndl->regs->DFCCTL, ISIF_DFCCTL_VDFCUDA, data->upperPixelsDisable);
  CSL_FINS(hndl->regs->DFCCTL, ISIF_DFCCTL_VDFLSFT, data->levelValueShift);
  CSL_FINS(hndl->regs->VDFSATLV, ISIF_VDFSATLV_VDFSLV, data->satLevel);

  if(data->enable) {
    status = CSL_ccdcWriteDfcTable(hndl, data->vdfcTable);
  }

  return status;
}

CSL_Status CSL_ccdcWriteDfcTable(CSL_CcdcHandle hndl, CSL_CcdcVdfcTable * data)
{
  Uint16  i;

  if (hndl == NULL || data == NULL)
    return CSL_EFAIL;

  if (data->numDefects == 0)
    return CSL_SOK;

  if (data->numDefects > 8)
    data->numDefects = 8;

  // clear table
  CSL_FINS(hndl->regs->DFCMEMCTL, ISIF_DFCMEMCTL_DFCMCLR, 1);
  while(CSL_FEXT(hndl->regs->DFCMEMCTL, ISIF_DFCMEMCTL_DFCMCLR) )
    ;

  // set address to 0
  CSL_FINS(hndl->regs->DFCMEMCTL, ISIF_DFCMEMCTL_DFCMARST, 1);

  // write table
  for (i = 0; i < data->numDefects; i++) {

    hndl->regs->DFCMEM0 = data->startY[i];
    hndl->regs->DFCMEM1 = data->startX[i];
    hndl->regs->DFCMEM2 = data->levelEqual[i];
    hndl->regs->DFCMEM3 = data->levelUpper[i];
    hndl->regs->DFCMEM4 = data->levelLower[i];

    CSL_FINS(hndl->regs->DFCMEMCTL, ISIF_DFCMEMCTL_DFCMWR, 1);
    while(CSL_FEXT(hndl->regs->DFCMEMCTL, ISIF_DFCMEMCTL_DFCMWR) )
      ;

    // increment address
    CSL_FINS(hndl->regs->DFCMEMCTL, ISIF_DFCMEMCTL_DFCMARST, 0);
  }

  return CSL_SOK;
}

CSL_Status CSL_ccdcReadDfcTable(CSL_CcdcHandle hndl, CSL_CcdcVdfcTable * data)
{
  Uint16  i;

  if (hndl == NULL || data == NULL)
    return CSL_EFAIL;

  if (data->numDefects > 8)
    data->numDefects = 8;

  CSL_FINS(hndl->regs->DFCMEMCTL, ISIF_DFCMEMCTL_DFCMARST, 1);

  for (i = 0; i < data->numDefects; i++) {

    CSL_FINS(hndl->regs->DFCMEMCTL, ISIF_DFCMEMCTL_DFCMRD, 1);
    while(CSL_FEXT(hndl->regs->DFCMEMCTL, ISIF_DFCMEMCTL_DFCMRD) )
      ;

    data->startY[i] = hndl->regs->DFCMEM0;
    data->startX[i] = hndl->regs->DFCMEM1;
    data->levelEqual[i] = hndl->regs->DFCMEM2;
    data->levelUpper[i] = hndl->regs->DFCMEM3;
    data->levelLower[i] = hndl->regs->DFCMEM4;

    CSL_FINS(hndl->regs->DFCMEMCTL, ISIF_DFCMEMCTL_DFCMARST, 0);
  }

  return CSL_SOK;
}

CSL_Status CSL_ccdcSetLscConfig(CSL_CcdcHandle hndl, CSL_CcdcLscConfig * data)
{
  CSL_Status status = CSL_SOK;

  if (hndl == NULL || data == NULL)
    return CSL_EFAIL;

  CSL_ccdcLscEnable(hndl, FALSE);

  /* HVAL, VVAL set during enable based on these values */
  hndl->lscHval = data->inWidth - 1;
  hndl->lscVval = data->inHeight - 1;

  CSL_FINS(hndl->regs->LSCHVAL, ISIF_LSCHVAL_HVAL, 0);
  CSL_FINS(hndl->regs->LSCVVAL, ISIF_LSCVVAL_VVAL, 0);

  switch (data->paxelWidth) {
  case 8:
    CSL_FINS(hndl->regs->DLSCCFG, ISIF_DLSCCFG_GAIN_MODE_M, 3);
    break;
  case 16:
    CSL_FINS(hndl->regs->DLSCCFG, ISIF_DLSCCFG_GAIN_MODE_M, 4);
    break;
  case 32:
    CSL_FINS(hndl->regs->DLSCCFG, ISIF_DLSCCFG_GAIN_MODE_M, 5);
    break;
  case 64:
    CSL_FINS(hndl->regs->DLSCCFG, ISIF_DLSCCFG_GAIN_MODE_M, 6);
    break;
  case 128:
    CSL_FINS(hndl->regs->DLSCCFG, ISIF_DLSCCFG_GAIN_MODE_M, 7);
    break;
  default:
    status = CSL_EFAIL;
    break;
  }

  switch (data->paxelHeight) {
  case 8:
    CSL_FINS(hndl->regs->DLSCCFG, ISIF_DLSCCFG_GAIN_MODE_N, 3);
    break;
  case 16:
    CSL_FINS(hndl->regs->DLSCCFG, ISIF_DLSCCFG_GAIN_MODE_N, 4);
    break;
  case 32:
    CSL_FINS(hndl->regs->DLSCCFG, ISIF_DLSCCFG_GAIN_MODE_N, 5);
    break;
  case 64:
    CSL_FINS(hndl->regs->DLSCCFG, ISIF_DLSCCFG_GAIN_MODE_N, 6);
    break;
  case 128:
    CSL_FINS(hndl->regs->DLSCCFG, ISIF_DLSCCFG_GAIN_MODE_N, 7);
    break;
  default:
    status = CSL_EFAIL;
    break;
  }

  CSL_FINS(hndl->regs->DLSCCFG, ISIF_DLSCCFG_GAIN_FORMAT, data->gainFormat);

  CSL_FINS(hndl->regs->DLSCOFST, ISIF_DLSCOFST_OFSTEN, data->offsetEnable);
  CSL_FINS(hndl->regs->DLSCOFST, ISIF_DLSCOFST_OFSTSF, data->offsetScale);
  CSL_FINS(hndl->regs->DLSCOFST, ISIF_DLSCOFST_OFSTSFT, data->offsetShift);

  if (data->initX % 2 != 0)
    status = CSL_EFAIL;

  if (data->initY % 2 != 0)
    status = CSL_EFAIL;

  CSL_FINS(hndl->regs->DLSCINI, ISIF_DLSCINI_X, data->initX);
  CSL_FINS(hndl->regs->DLSCINI, ISIF_DLSCINI_Y, data->initY);

  status |= CSL_ccdcLscSetGainTableAddr(hndl, data->gainTableAddr);

  if (data->gainTableLineOffset % 4 != 0)
    status = CSL_EFAIL;

  CSL_FINS(hndl->regs->DLSCGROF, ISIF_DLSCGROF_OFFSET, data->gainTableLineOffset);

  status |= CSL_ccdcLscSetOffsetTableAddr(hndl, data->offsetTableAddr);

  if (data->offsetTableLineOffset % 4 != 0)
    status = CSL_EFAIL;

  CSL_FINS(hndl->regs->DLSCOROF, ISIF_DLSCOROF_OFFSET, data->offsetTableLineOffset);

  hndl->regs->DLSCIRQEN = 0x0;

  CSL_FINS(hndl->regs->DLSCIRQEN, ISIF_DLSCIRQEN_SOF, data->intSofEnable);
  CSL_FINS(hndl->regs->DLSCIRQEN, ISIF_DLSCIRQEN_PREFETCH_ERROR, data->intPreFetchErrorEnable);
  CSL_FINS(hndl->regs->DLSCIRQEN, ISIF_DLSCIRQEN_DONE, data->intDoneEnable);

  // clear old LSC status
  hndl->regs->DLSCIRQST = 0xF;
  
  CSL_ccdcLscEnable(hndl, data->enable);

  #if 0
  if(hndl->ldcPrefecthErrCount)
  {
    printk( KERN_INFO " CSL:ISIF:LSC:%d\n", hndl->ldcPrefecthErrCount);
  }
  #endif
  
  return status;
}

CSL_Status CSL_ccdcLscEnable(CSL_CcdcHandle hndl, Bool32 enable)
{
  if(hndl==NULL)
    return CSL_EFAIL;

  if(enable==FALSE) {
      
  } else {
    CSL_FINS(hndl->regs->LSCHVAL, ISIF_LSCHVAL_HVAL, hndl->lscHval);
    CSL_FINS(hndl->regs->LSCVVAL, ISIF_LSCHVAL_HVAL, hndl->lscVval);   
  }
    
  CSL_FINS(hndl->regs->DLSCCFG, ISIF_DLSCCFG_ENABLE, enable);        
      
  return CSL_SOK;  
}

CSL_Status CSL_ccdcLscSetGainTableAddr(CSL_CcdcHandle hndl, Uint8 *inAddr)
{
  CSL_Status status = CSL_SOK;
  Uint16  addrL, addrH;
  Uint32  addr;
  
  if(hndl==NULL||inAddr==NULL)
    return CSL_EFAIL;
    
  addr = (Uint32) inAddr;

  if ((addr & 0x3) != 0)
    status = CSL_EFAIL;


  addrL = (Uint16) addr;
  addrH = (Uint16) (addr >> 16);

  hndl->regs->DLSCGRBU = addrH;
  hndl->regs->DLSCGRBL = addrL;

  return status;    
}

CSL_Status CSL_ccdcLscSetOffsetTableAddr(CSL_CcdcHandle hndl, Uint8 *inAddr)
{
  CSL_Status status = CSL_SOK;
  Uint16  addrL, addrH;
  Uint32  addr;
  
  if(hndl==NULL||inAddr==NULL)
    return CSL_EFAIL;

  addr = (Uint32) inAddr;

  if ((addr & 0x3) != 0)
    status = CSL_EFAIL;

  addrL = (Uint16) addr;
  addrH = (Uint16) (addr >> 16);

  hndl->regs->DLSCORBU = addrH;
  hndl->regs->DLSCORBL = addrL;

  return status;    
}

CSL_Status CSL_ccdcLscPrefetchErrReset(CSL_CcdcHandle hndl)
{
  volatile Uint16 val;

  val = hndl->regs->DLSCIRQST;
  if(val & 0x2)
  {
    hndl->regs->DLSCIRQST = 0x2;  
    CSL_FINS(hndl->regs->DLSCCFG, ISIF_DLSCCFG_ENABLE, 0);    
    CSL_FINS(hndl->regs->DLSCCFG, ISIF_DLSCCFG_ENABLE, 1);        
    hndl->ldcPrefecthErrCount++;
  }

  return CSL_SOK;
}

CSL_Status CSL_ccdcGetLscStatus(CSL_CcdcHandle hndl, Bool32 * intSof, Bool32 * prefetchDone, Bool32 * prefetchError, Bool32 * lscDone)
{
  Uint16  val;
  Uint16  wrVal;

  if (hndl == NULL)
    return CSL_EFAIL;

  wrVal = 0;
  val = hndl->regs->DLSCIRQST;

  if (intSof != NULL) {
    *intSof = FALSE;

    if (val & 0x8) {
      wrVal |= 0x8;
      *intSof = TRUE;
    }
  }

  if (prefetchDone != NULL) {
    *prefetchDone = FALSE;

    if (val & 0x4) {
      wrVal |= 0x4;
      *prefetchDone = TRUE;
    }
  }

  if (prefetchError != NULL) {
    *prefetchError = FALSE;

    if (val & 0x2) {
      wrVal |= 0x2;
      *prefetchError = TRUE;
    }
  }

  if (lscDone != NULL) {
    *lscDone = FALSE;

    if (val & 0x1) {
      wrVal |= 0x1;
      *lscDone = TRUE;
    }
  }
  // clear pending status
  hndl->regs->DLSCIRQST = wrVal;

  return CSL_SOK;
}


CSL_Status CSL_ccdcSetGainOffsetConfig(CSL_CcdcHandle hndl, CSL_CcdcGainOffsetConfig * data)
{
  if (hndl == NULL || data == NULL)
    return CSL_EFAIL;


  CSL_FINS(hndl->regs->CRGAIN, ISIF_CRGAIN_CGR, data->gainR);
  CSL_FINS(hndl->regs->CGRGAIN, ISIF_CGRGAIN_CGGR, data->gainGr);
  CSL_FINS(hndl->regs->CGBGAIN, ISIF_CGBGAIN_CGGB, data->gainGb);
  CSL_FINS(hndl->regs->CBGAIN, ISIF_CBGAIN_CGB, data->gainB);

  CSL_FINS(hndl->regs->COFSTA, ISIF_COFSTA_COFT, data->offset);

  CSL_FINS(hndl->regs->CGAMMAWD, ISIF_CGAMMAWD_WBEN1, data->ipipeGainEnable);
  CSL_FINS(hndl->regs->CGAMMAWD, ISIF_CGAMMAWD_OFSTEN1, data->ipipeOffsetEnable);


  CSL_FINS(hndl->regs->CGAMMAWD, ISIF_CGAMMAWD_WBEN2, data->h3aGainEnable);
  CSL_FINS(hndl->regs->CGAMMAWD, ISIF_CGAMMAWD_OFSTEN2, data->h3aOffsetEnable);

  CSL_FINS(hndl->regs->CGAMMAWD, ISIF_CGAMMAWD_WBEN0, data->sdramOutGainEnable);
  CSL_FINS(hndl->regs->CGAMMAWD, ISIF_CGAMMAWD_OFSTEN0, data->sdramOutOffsetEnable);

  return CSL_SOK;
}

CSL_Status CSL_ccdcSetSdramOutSizeConfig(CSL_CcdcHandle hndl, CSL_CcdcSdramOutSizeConfig * data)
{
  if (hndl == NULL || data == NULL)
    return CSL_EFAIL;

  CSL_FINS(hndl->regs->SPH, ISIF_SPH_SPH, data->outStartH);
  CSL_FINS(hndl->regs->SLV0, ISIF_SLV0_SLV0, data->outStartV0);
  CSL_FINS(hndl->regs->SLV1, ISIF_SLV1_SLV1, data->outStartV1);
  CSL_FINS(hndl->regs->LNH, ISIF_LNH_LNH, data->outWidth - 1);
  CSL_FINS(hndl->regs->LNV, ISIF_LNV_LNV, data->outHeight - 1);

  return CSL_SOK;
}

CSL_Status CSL_ccdcSetSdramOutConfig(CSL_CcdcHandle hndl, CSL_CcdcSdramOutConfig * data)
{
  CSL_Status status = CSL_SOK;

  if (hndl == NULL || data == NULL)
    return CSL_EFAIL;

  CSL_FINS(hndl->regs->MODESET, ISIF_MODESET_CCDW, data->outDataShift);

  CSL_FINS(hndl->regs->SPH, ISIF_SPH_SPH, data->outStartH);
  CSL_FINS(hndl->regs->SLV0, ISIF_SLV0_SLV0, data->outStartV0);
  CSL_FINS(hndl->regs->SLV1, ISIF_SLV1_SLV1, data->outStartV1);
  CSL_FINS(hndl->regs->LNH, ISIF_LNH_LNH, data->outWidth - 1);
  CSL_FINS(hndl->regs->LNV, ISIF_LNV_LNV, data->outHeight - 1);

  CSL_FINS(hndl->regs->CULH, ISIF_CULH_CLHE, data->culHEven);
  CSL_FINS(hndl->regs->CULH, ISIF_CULH_CLHO, data->culHOdd);
  CSL_FINS(hndl->regs->CULV, ISIF_CULV_CULV, data->culV);

  if (data->outLineOffset % 32 != 0)
    status = CSL_EFAIL;

  CSL_FINS(hndl->regs->HSIZE, ISIF_HSIZE_HSIZE, data->outLineOffset / 32);
  CSL_FINS(hndl->regs->HSIZE, ISIF_HSIZE_ADCR, data->outAddrDecrement);

  CSL_FINS(hndl->regs->SDOFST, ISIF_SDOFST_FIINV, data->sdramOffsetConfig.fidInv);
  CSL_FINS(hndl->regs->SDOFST, ISIF_SDOFST_FOFST, data->sdramOffsetConfig.fidOffsetOdd);
  CSL_FINS(hndl->regs->SDOFST, ISIF_SDOFST_LOFTS0, data->sdramOffsetConfig.lineOffsetEvenEven);
  CSL_FINS(hndl->regs->SDOFST, ISIF_SDOFST_LOFTS1, data->sdramOffsetConfig.lineOffsetOddEven);
  CSL_FINS(hndl->regs->SDOFST, ISIF_SDOFST_LOFTS2, data->sdramOffsetConfig.lineOffsetEvenOdd);
  CSL_FINS(hndl->regs->SDOFST, ISIF_SDOFST_LOFTS3, data->sdramOffsetConfig.lineOffsetOddOdd);

  status = CSL_ccdcSetSdramOutAddr(hndl, data->outAddr);

  CSL_FINS(hndl->regs->CGAMMAWD, ISIF_CGAMMAWD_CCDTBL, data->alawEnable);
  CSL_FINS(hndl->regs->CCDCFG, ISIF_CCDCFG_BSWD, data->byteSwapEnable);

  CSL_FINS(hndl->regs->CCDCFG, ISIF_CCDCFG_SDRPACK, data->packMode);

  return status;
}

CSL_Status CSL_ccdcGetSdramOutSizeConfig(CSL_CcdcHandle hndl, CSL_CcdcSdramOutSizeConfig * data)
{
  if (hndl == NULL || data == NULL)
    return CSL_EFAIL;

  data->outStartH = CSL_FEXT(hndl->regs->SPH, ISIF_SPH_SPH);
  data->outStartV0 = CSL_FEXT(hndl->regs->SLV0, ISIF_SLV0_SLV0);
  data->outStartV1 = CSL_FEXT(hndl->regs->SLV1, ISIF_SLV1_SLV1);
  data->outWidth = CSL_FEXT(hndl->regs->LNH, ISIF_LNH_LNH) + 1;
  data->outHeight = CSL_FEXT(hndl->regs->LNV, ISIF_LNV_LNV) + 1;
  data->outFid = CSL_FEXT(hndl->regs->MODESET, ISIF_MODESET_MDFS);

  return CSL_SOK;
}

CSL_Status CSL_ccdcSetDpcmConfig(CSL_CcdcHandle hndl, CSL_CcdcDpcmConfig * data)
{
  if (hndl == NULL || data == NULL)
    return CSL_EFAIL;

  CSL_FINS(hndl->regs->MISC, ISIF_MISC_DPCMPRE, data->predictor);
  CSL_FINS(hndl->regs->MISC, ISIF_MISC_DPCMEN, data->enable);

  return CSL_SOK;
}

CSL_Status CSL_ccdcSetRec656Config(CSL_CcdcHandle hndl, CSL_CcdcRec656Config * data)
{
  if (hndl == NULL || data == NULL)
    return CSL_EFAIL;

  CSL_FINS(hndl->regs->REC656IF, ISIF_REC656IF_ECCFVH, data->errorCorrectEnable);
  CSL_FINS(hndl->regs->CCDCFG, ISIF_CCDCFG_BW656, data->dataWidth);
  CSL_FINS(hndl->regs->REC656IF, ISIF_REC656IF_R656ON, data->enable);

  return CSL_SOK;
}

CSL_Status CSL_ccdcSetFlashConfig(CSL_CcdcHandle hndl, CSL_CcdcFlashConfig * data)
{
  if (hndl == NULL || data == NULL)
    return CSL_EFAIL;

  CSL_FINS(hndl->regs->FLSHCFG1, ISIF_FLSHCFG1_SFLSH, data->timingStartLine);
  CSL_FINS(hndl->regs->FLSHCFG2, ISIF_FLSHCFG2_VFLSH, data->timingWidth / 2 - 1);
  CSL_FINS(hndl->regs->FLSHCFG0, ISIF_FLSHCFG0_FLSHEN, data->timingEnable);

  return CSL_SOK;
}

CSL_Status CSL_ccdcSetVdIntConfig(CSL_CcdcHandle hndl, CSL_CcdcVdIntConfig * data)
{
  if (hndl == NULL || data == NULL)
    return CSL_EFAIL;

  if(data->vdInt0!=0xFFFF)
    CSL_FINS(hndl->regs->VDINT0, ISIF_VDINT0_CVD0, data->vdInt0);

  if(data->vdInt1!=0xFFFF)
    CSL_FINS(hndl->regs->VDINT1, ISIF_VDINT1_CVD1, data->vdInt1);

  if(data->vdInt2!=0xFFFF)
    CSL_FINS(hndl->regs->VDINT2, ISIF_VDINT2_CVD2, data->vdInt2);

  return CSL_SOK;
}

CSL_Status CSL_ccdcSetSdramOutAddr(CSL_CcdcHandle hndl, Uint8 * outAddr)
{
  CSL_Status status = CSL_SOK;
  Uint32  addr = (Uint32)outAddr;
  Uint16  addrU, addrL;

  if (hndl == NULL || outAddr == NULL)
    return CSL_EFAIL;

  if (addr % 32 != 0)
    status = CSL_EFAIL;

  addr = addr / 32;

  addrL = (Uint16) (addr);
  addrU = (Uint16) (addr >> 16);

  CSL_FINS(hndl->regs->CADU, ISIF_CADU_CADU, addrU);
  CSL_FINS(hndl->regs->CADL, ISIF_CADL_CADL, addrL);

  return status;
}
