

#include <osa.h>

#ifdef OSA_PRF_ENABLE

int OSA_prfBegin(OSA_PrfHndl *hndl)
{
  hndl->curTime = OSA_getCurTimeInMsec();

  return OSA_SOK;
}

int OSA_prfEnd(OSA_PrfHndl *hndl, Uint32 value)
{
  hndl->curTime = OSA_getCurTimeInMsec() - hndl->curTime;

  hndl->count++;

  hndl->totalTime += hndl->curTime;
  if(hndl->curTime > hndl->maxTime)
    hndl->maxTime = hndl->curTime;
  if(hndl->curTime < hndl->minTime)
    hndl->minTime = hndl->curTime;

  hndl->totalValue += value;
  if(value > hndl->maxValue)
    hndl->maxValue = value;
  if(value < hndl->minTime)
    hndl->minValue = hndl->curValue;

  return OSA_SOK;
}

int OSA_prfReset(OSA_PrfHndl *hndl)
{
  memset(hndl, 0, sizeof(*hndl) );

  return OSA_SOK;
}

int OSA_prfPrint(OSA_PrfHndl *hndl, char *name, Uint32 flags)
{
  OSA_printf(" \n");
  OSA_printf(" Profile Info  : %s \n", name);
  OSA_printf(" ======================\n");
  
  if(flags==0)
    flags = OSA_PRF_PRINT_DEFAULT;

  if(flags & OSA_PRF_PRINT_COUNT ) {
    OSA_printf(" Iterations    : %d \n", hndl->count);
  }

  if(flags & OSA_PRF_PRINT_TIME ) {

    OSA_printf(" Avg Time (ms) : %9.2f \n", (float)hndl->totalTime/hndl->count);

    if(flags & OSA_PRF_PRINT_MIN_MAX ) {
      OSA_printf(" Min Time (ms) : %d \n", hndl->minTime);
    }

    if(flags & OSA_PRF_PRINT_MIN_MAX ) {
      OSA_printf(" Max Time (ms) : %d \n", hndl->maxTime);
    }
  }

  if(flags & OSA_PRF_PRINT_VALUE ) {

    OSA_printf(" Avg Value     : %9.2f \n", (float)hndl->totalValue/hndl->count);
    OSA_printf(" Avg Value/sec : %9.2f \n", (float)(hndl->totalValue*1000)/hndl->totalTime);

    if(flags & OSA_PRF_PRINT_MIN_MAX ) {
      OSA_printf(" Min Value     : %d \n", hndl->minValue);
    }

    if(flags & OSA_PRF_PRINT_MIN_MAX ) {
      OSA_printf(" Max Value     : %d \n", hndl->maxValue);
    }
  }
  OSA_printf(" \n");  

  return OSA_SOK;
}

#endif
