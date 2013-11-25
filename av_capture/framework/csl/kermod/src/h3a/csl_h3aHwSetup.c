/*
    DM360 Evaluation Software

    (c)Texas Instruments 2007
*/

#include <csl_h3a.h>


CSL_Status CSL_h3aHwSetup(CSL_H3aHandle hndl, CSL_H3aHwSetup * data)
{
  CSL_Status status = CSL_SOK;

  if (hndl == NULL || data == NULL)
    return CSL_EFAIL;

  if (data->inFrameConfig != NULL) {
    if (CSL_h3aInFrameConfig(hndl, data->inFrameConfig) != CSL_SOK)
      status = CSL_EFAIL;
  }

  if (data->afConfig != NULL) {
    if (CSL_h3aAfSetConfig(hndl, data->afConfig) != CSL_SOK)
      status = CSL_EFAIL;
  }

  if (data->aewbConfig != NULL) {
    if (CSL_h3aAewbSetConfig(hndl, data->aewbConfig) != CSL_SOK)
      status = CSL_EFAIL;
  }
  return status;
}
