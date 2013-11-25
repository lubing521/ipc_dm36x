/*
    DM360 Evaluation Software

    (c)Texas Instruments 2007
*/


#include <csl_ldc.h>



CSL_Status CSL_ldcFrameConfig(CSL_LdcHandle hndl, CSL_LdcFrameConfig * frameConfig)
{
  CSL_Status status = CSL_SOK;

  if (hndl == NULL || frameConfig == NULL)
    return CSL_EFAIL;

  if ((frameConfig->outBlkV % 2) || (frameConfig->outBlkV > frameConfig->frameHeight))
    status = CSL_EFAIL;
  CSL_FINS(hndl->regs->BLOCK, LDC_BLOCK_OBH, frameConfig->outBlkV);

  if ((frameConfig->outBlkH > frameConfig->frameWidth))
    status = CSL_EFAIL;
  CSL_FINS(hndl->regs->BLOCK, LDC_BLOCK_OBW, frameConfig->outBlkH);

  CSL_FINS(hndl->regs->BLOCK, LDC_BLOCK_PIXPAD, frameConfig->pixelPad);

  if (frameConfig->frameWidth % frameConfig->outBlkH)
    status = CSL_EFAIL;
  CSL_FINS(hndl->regs->FRAME_SIZE, LDC_FRAME_SIZE_W, frameConfig->frameWidth);

  if (frameConfig->frameHeight % frameConfig->outBlkV)
    status = CSL_EFAIL;
  CSL_FINS(hndl->regs->FRAME_SIZE, LDC_FRAME_SIZE_H, frameConfig->frameHeight);

  if (frameConfig->initX % 2)
    status = CSL_EFAIL;
  CSL_FINS(hndl->regs->INITXY, LDC_INITXY_INITX, frameConfig->initX);

  if (frameConfig->initY % 2)
    status = CSL_EFAIL;
  CSL_FINS(hndl->regs->INITXY, LDC_INITXY_INITY, frameConfig->initY);

  if (((Uint32) frameConfig->inputAddr) % 32)
    status = CSL_EFAIL;
  hndl->regs->RD_BASE = (Uint32) frameConfig->inputAddr;

  if (((Uint32) frameConfig->inputLineOffset) % 32)
    status = CSL_EFAIL;
  hndl->regs->RD_OFST = frameConfig->inputLineOffset;

  if (((Uint32) frameConfig->inputAddr420C) % 32)
    status = CSL_EFAIL;
  hndl->regs->C420_RD_BASE = (Uint32) frameConfig->inputAddr420C;

  if (((Uint32) frameConfig->outputAddr) % 32)
    status = CSL_EFAIL;
  hndl->regs->WR_BASE = (Uint32) frameConfig->outputAddr;

  if (((Uint32) frameConfig->outputLineOffset) % 32)
    status = CSL_EFAIL;
  hndl->regs->WR_OFST = frameConfig->outputLineOffset;

  if (((Uint32) frameConfig->outputAddr420C) % 32)
    status = CSL_EFAIL;
  hndl->regs->C420_WR_BASE = (Uint32) frameConfig->outputAddr420C;

  return status;
}

CSL_Status CSL_ldcEnable(CSL_LdcHandle hndl, Bool32 enable)
{
  if (hndl == NULL)
    return CSL_EFAIL;

  CSL_FINS(hndl->regs->PCR, LDC_PCR_EN, enable);

  return CSL_SOK;
}

CSL_Status CSL_ldcIsBusy(CSL_LdcHandle hndl, Bool32 * isBusy)
{
  if (hndl == NULL || isBusy == NULL)
    return CSL_EFAIL;
    
  *isBusy = TRUE;  

  if (CSL_FEXT(hndl->regs->PCR, LDC_PCR_BUSY) == CSL_LDC_STATE_IDLE)
    *isBusy = FALSE;
  else if (CSL_FEXT(hndl->regs->PCR, LDC_PCR_BUSY) == CSL_LDC_STATE_BUSY)
    *isBusy = TRUE;

  return CSL_SOK;
}

CSL_Status CSL_ldcWriteTable(CSL_LdcHandle hndl, Uint32 * table)  
{
  Uint32  count;

  if (hndl == NULL || table == NULL)
    return CSL_EFAIL;

  hndl->regs->LUT_ADDR = 0;

  for (count = 0; count < CSL_LDC_TABLE_MAX_ENTRIES; count++)
    hndl->regs->LUT_WDATA = table[count];

  return CSL_SOK;
}

CSL_Status CSL_ldcReadTable(CSL_LdcHandle hndl, Uint32 * table) 
{
  Uint32  count;

  if (hndl == NULL || table == NULL)
    return CSL_EFAIL;

  hndl->regs->LUT_ADDR = 0;

  for (count = 0; count < CSL_LDC_TABLE_MAX_ENTRIES; count++)
    table[count] = hndl->regs->LUT_RDATA;

  return CSL_SOK;
}

