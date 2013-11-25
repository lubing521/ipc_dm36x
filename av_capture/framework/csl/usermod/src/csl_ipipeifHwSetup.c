/*
    DM360 Evaluation Software

    (c)Texas Instruments 2007
*/

#include <csl_ipipeif.h>


CSL_Status CSL_ipipeifHwSetup(CSL_IpipeifHandle hndl, CSL_IpipeifHwSetup * data)
{
  CSL_Status status = CSL_SOK;

  if (hndl == NULL || data == NULL)
    return CSL_EFAIL;


  CSL_ipipeifSetInputSource1(hndl, data->inputSource1);

  CSL_FINS(hndl->regs->CFG2, IPIPEIF_CFG2_YUV8P, data->chromaPos);
  CSL_FINS(hndl->regs->CFG2, IPIPEIF_CFG2_INTSRC, data->interuptSource);

  if (data->sdramInConfig != NULL) {

    if (CSL_ipipeifSetSdramInConfig(hndl, data->sdramInConfig) != CSL_SOK)
      status = CSL_EFAIL;

  } else if (data->bypassModuleIfNullConfig == TRUE) {

    CSL_FINS(hndl->regs->ENABLE, IPIPEIF_ENABLE_ENABLE, CSL_IPIPEIF_ENABLE_ENABLE_RESETVAL);
    CSL_FINS(hndl->regs->CFG1, IPIPEIF_CFG1_ONESHOT, CSL_IPIPEIF_CFG1_ONESHOT_RESETVAL);
    CSL_FINS(hndl->regs->CFG1, IPIPEIF_CFG1_DATASFT, CSL_IPIPEIF_CFG1_DATASFT_RESETVAL);
    CSL_FINS(hndl->regs->CFG1, IPIPEIF_CFG1_PACK8IN, CSL_IPIPEIF_CFG1_PACK8IN_RESETVAL);

    hndl->regs->HNUM = CSL_IPIPEIF_HNUM_RESETVAL;
    hndl->regs->VNUM = CSL_IPIPEIF_VNUM_RESETVAL;

    hndl->regs->ADDRL = CSL_IPIPEIF_ADDRL_RESETVAL;
    hndl->regs->ADDRU = CSL_IPIPEIF_ADDRU_RESETVAL;

    hndl->regs->ADOFS = CSL_IPIPEIF_ADOFS_RESETVAL;

    CSL_FINS(hndl->regs->DPC1, IPIPEIF_DPC1_DPC1ENA, CSL_IPIPEIF_DPC1_DPC1ENA_RESETVAL);
    CSL_FINS(hndl->regs->DPC1, IPIPEIF_DPC1_DPC1TH, CSL_IPIPEIF_DPC1_DPC1TH_RESETVAL);

    CSL_FINS(hndl->regs->DPCM, IPIPEIF_DPCM_ENA, CSL_IPIPEIF_DPCM_ENA_RESETVAL);

    CSL_FINS(hndl->regs->CFG1, IPIPEIF_CFG1_CLKSEL, CSL_IPIPEIF_CFG1_CLKSEL_RESETVAL);

    hndl->regs->CLKDIV = CSL_IPIPEIF_CLKDIV_RESETVAL;

    hndl->regs->PPLN = CSL_IPIPEIF_PPLN_RESETVAL;
    hndl->regs->LPFR = CSL_IPIPEIF_LPFR_RESETVAL;
  }

  if (CSL_ipipeifSetInSource2Config(hndl, data->inSource2Config) != CSL_SOK && data->bypassModuleIfNullConfig == TRUE) {

    CSL_FINS(hndl->regs->CFG1, IPIPEIF_CFG1_INPSRC2, CSL_IPIPEIF_CFG1_INPSRC2_RESETVAL);
    CSL_FINS(hndl->regs->CFG1, IPIPEIF_CFG1_DECM, CSL_IPIPEIF_CFG1_DECM_RESETVAL);

    hndl->regs->RSZ = CSL_IPIPEIF_RSZ_RSZ_RESETVAL;
    CSL_FINS(hndl->regs->CFG1, IPIPEIF_CFG1_AVGFILT, CSL_IPIPEIF_CFG1_AVGFILT_RESETVAL);

    CSL_FINS(hndl->regs->INIRSZ, IPIPEIF_INIRSZ_SYNCAL, CSL_IPIPEIF_INIRSZ_SYNCAL_RESETVAL);
    CSL_FINS(hndl->regs->INIRSZ, IPIPEIF_INIRSZ_INIRSZ, CSL_IPIPEIF_INIRSZ_INIRSZ_RESETVAL);

    hndl->regs->OCLIP = CSL_IPIPEIF_OCLIP_OCLIP_RESETVAL;

    hndl->regs->GAIN = CSL_IPIPEIF_GAIN_GAIN_RESETVAL;

    CSL_FINS(hndl->regs->CFG2, IPIPEIF_CFG2_INPTYP, CSL_IPIPEIF_CFG2_INPTYP_RESETVAL);
    CSL_FINS(hndl->regs->CFG2, IPIPEIF_CFG2_YUV8, CSL_IPIPEIF_CFG2_YUV8_RESETVAL);

    CSL_FINS(hndl->regs->ENABLE, IPIPEIF_ENABLE_SYNCOFF, CSL_IPIPEIF_ENABLE_SYNCOFF_RESETVAL);
  }

  if (CSL_ipipeifSetVpifIsifInConfig(hndl, data->vpifIsifInConfig) != CSL_SOK && data->bypassModuleIfNullConfig == TRUE) {
    CSL_FINS(hndl->regs->DPC1, IPIPEIF_DPC1_DPC1ENA, CSL_IPIPEIF_DPC1_DPC1ENA_RESETVAL);
  }

  if (CSL_ipipeifSetDarkFrameConfig(hndl, data->darkFrameConfig) != CSL_SOK && data->bypassModuleIfNullConfig == TRUE) {
    CSL_FINS(hndl->regs->DFSGVL, IPIPEIF_DFSGVL_DFSGEN, CSL_IPIPEIF_DFSGVL_DFSGEN_RESETVAL);
  }

  if (CSL_ipipeifSetH3aConfig(hndl, data->h3aConfig) != CSL_SOK && data->bypassModuleIfNullConfig == TRUE) {
    CSL_FINS(hndl->regs->RSZ2, IPIPEIF_RSZ2_DECM2, CSL_IPIPEIF_RSZ2_DECM2_RESETVAL);
  }

  return status;
}
