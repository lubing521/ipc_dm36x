
#include <csl_rsz.h>

CSL_Status CSL_rszBufInit(CSL_RszHandle hndl, Uint8 rszMod, CSL_BufInit * bufInit, CSL_RszBufConfig *bufConfig)
{
  CSL_Status status;
  Uint8 *pAddrY, *pAddrC;
  
  if (rszMod >= CSL_RSZ_CH_MAX)
    return CSL_EFAIL;

  status = CSL_bufInit(&hndl->outBuf[rszMod], bufInit);
  
  hndl->yuv420BufCoffset[rszMod] = bufConfig->offsetH*bufConfig->offsetV;

  if(bufConfig->format==CSL_RSZ_CHROMA_FORMAT_422) {
    hndl->flipVOffsetY[rszMod] = bufConfig->offsetH*2*(bufConfig->height-1);
    hndl->flipHOffsetYC[rszMod] = bufConfig->width*2-1;    
  } else {
    hndl->flipVOffsetY[rszMod] = bufConfig->offsetH*(bufConfig->height-1);  
    hndl->flipHOffsetYC[rszMod] = bufConfig->width-1;
  }

  hndl->flipVOffsetC[rszMod] = bufConfig->offsetH * (bufConfig->height/2-1);    

  hndl->curBufInfo[rszMod].id = CSL_BUF_ID_INVALID;
  
  pAddrY = (Uint8 *) bufInit->bufAddr[0];
  pAddrC = (Uint8 *)( bufInit->bufAddr[0] + hndl->yuv420BufCoffset[rszMod]);
  
  if(hndl->flipV[rszMod]) {
    pAddrY += hndl->flipVOffsetY[rszMod];
    pAddrC += hndl->flipVOffsetC[rszMod];   
  }
  
  if(hndl->flipH[rszMod]) {
    pAddrY += hndl->flipHOffsetYC[rszMod];
    pAddrC += hndl->flipHOffsetYC[rszMod];   
  }  

  CSL_rszSetOutAddr(hndl, rszMod, pAddrY, pAddrC);

  return status;
}

CSL_Status CSL_rszBufSwitchEnable(CSL_RszHandle hndl, Uint8 rszMod, Bool32 enable)
{
  if (rszMod >= CSL_RSZ_CH_MAX)
    return CSL_EFAIL;

  hndl->outBufSwitchEnable[rszMod] = enable;

  return CSL_SOK;
}

CSL_Status CSL_rszBufGetFull(CSL_RszHandle hndl, Uint8 rszMod, CSL_BufInfo * buf, Uint32 minBuf, Uint32 timeout)
{
  CSL_Status status;

  if (rszMod >= CSL_RSZ_CH_MAX)
    return CSL_EFAIL;

  status = CSL_bufGetFull(&hndl->outBuf[rszMod], buf, minBuf, timeout);

  return status;
}

CSL_Status CSL_rszBufPutEmpty(CSL_RszHandle hndl, Uint8 rszMod, CSL_BufInfo * buf)
{
  CSL_Status status;

  if (rszMod >= CSL_RSZ_CH_MAX)
    return CSL_EFAIL;

  status = CSL_bufPutEmpty(&hndl->outBuf[rszMod], buf);

  return status;
}

CSL_Status CSL_rszBufSwitch(CSL_RszHandle hndl, Uint8 rszMod, Uint32 timestamp, Uint32 count)
{
  CSL_Status status = CSL_SOK;
  Uint8 *pAddrY, *pAddrC;
  
  if (rszMod >= CSL_RSZ_CH_MAX)
    return CSL_EFAIL;

  if (hndl->outBufSwitchEnable[rszMod]) {
  	/* update buffer height and width from hardware regs */
	if (hndl->curBufInfo[rszMod].id != CSL_BUF_ID_INVALID) {
    	if (rszMod == CSL_RSZ_A) {
		  hndl->curBufInfo[rszMod].width  = (hndl->regs->RZA_O_HSZ +1);
  		  hndl->curBufInfo[rszMod].height = (hndl->regs->RZA_O_VSZ +1);
		} else if (rszMod == CSL_RSZ_B) {
          hndl->curBufInfo[rszMod].width  = (hndl->regs->RZB_O_HSZ +1);
    	  hndl->curBufInfo[rszMod].height = (hndl->regs->RZB_O_VSZ +1);
		 }
	}
    /* do buffer switch*/
	status = CSL_bufSwitchFull(&hndl->outBuf[rszMod], &hndl->curBufInfo[rszMod], 1, timestamp, count);

    if (hndl->curBufInfo[rszMod].id != CSL_BUF_ID_INVALID) {
 
      pAddrY = (Uint8 *) hndl->curBufInfo[rszMod].addr;
      pAddrC = (Uint8 *)( pAddrY + hndl->yuv420BufCoffset[rszMod]);
  
      if(hndl->flipV[rszMod]) {
        pAddrY += hndl->flipVOffsetY[rszMod];
        pAddrC += hndl->flipVOffsetC[rszMod];   
      }
  
      if(hndl->flipH[rszMod]) {
        pAddrY += hndl->flipHOffsetYC[rszMod];
        pAddrC += hndl->flipHOffsetYC[rszMod];   
      }  

      if(rszMod==CSL_RSZ_A) {
        CSL_FINS(hndl->regs->SEQ, RSZ_SEQ_HRVA, hndl->flipH[rszMod]);
        CSL_FINS(hndl->regs->SEQ, RSZ_SEQ_VRVA, hndl->flipV[rszMod]);
      } else {
        CSL_FINS(hndl->regs->SEQ, RSZ_SEQ_HRVB, hndl->flipH[rszMod]);
        CSL_FINS(hndl->regs->SEQ, RSZ_SEQ_VRVB, hndl->flipV[rszMod]);
      }

      CSL_rszSetOutAddr(hndl, rszMod, pAddrY, pAddrC);
    }
  }
  
  return status;
}
