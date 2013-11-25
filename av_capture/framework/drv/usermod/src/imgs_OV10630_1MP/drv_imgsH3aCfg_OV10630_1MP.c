
#include <drv_imgs.h>

DRV_ImgsH3aConfig gDRV_imgsH3aConfig_Appro = {

  .medFiltThreshold  = 0,

  .aewbMedFiltEnable = FALSE,
  .aewbSatLimit      = 960,
  .aewbWinStartX     = 32,
  .aewbWinStartY     = 32,
  .aewbWinNumH       = 12,
  .aewbWinNumV       = 16,
  .aewbOutFormat     = CSL_H3A_OUTFMT_SUM_OF_SQUARES,
  .aewbShift         = 0,

  .afPaxNumH = 0,
  .afPaxNumV = 0,

};

DRV_ImgsH3aConfig gDRV_imgsH3aConfig_TI = {

  .medFiltThreshold = 0,

  .aewbMedFiltEnable = FALSE,
  .aewbSatLimit      = 1023,
  .aewbWinStartX     = 0,
  .aewbWinStartY     = 0,
  .aewbWinNumH       = 32,
  .aewbWinNumV       = 12,
  .aewbOutFormat     = CSL_H3A_OUTFMT_SUM_OF_SQUARES,
  .aewbShift         = 0,

  .afPaxNumH = 0,
  .afPaxNumV = 0,

};

DRV_ImgsH3aConfig gDRV_imgsH3aConfig_Common = {

  .medFiltThreshold  = 0,

  .aewbMedFiltEnable = FALSE,
  .aewbSatLimit      = 960,
  .aewbWinStartX     = 32,
  .aewbWinStartY     = 32,
  .aewbWinNumH       = 12,
  .aewbWinNumV       = 16,
  .aewbOutFormat     = CSL_H3A_OUTFMT_SUM_OF_SQUARES,
  .aewbShift         = 0,

  .afPaxNumH = 0,
  .afPaxNumV = 0,

};
