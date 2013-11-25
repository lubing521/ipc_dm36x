
#include <drv_imgs.h>

DRV_ImgsH3aConfig gDRV_imgsH3aConfig_Appro = {

  .medFiltThreshold  = 0,

  .aewbMedFiltEnable = FALSE,
  .aewbSatLimit      = 960,
  .aewbWinStartX     = 21,//72,//32,
  .aewbWinStartY     = 20,//32,
  .aewbWinNumH       = 32,//12,
  .aewbWinNumV       = 18,//16,
  .aewbOutFormat     = CSL_H3A_OUTFMT_SUM_OF_SQUARES,
  .aewbShift         = 0,

  .afPaxNumH = 0,
  .afPaxNumV = 0,

};

DRV_ImgsH3aConfig gDRV_imgsH3aConfig_TI = {

  .medFiltThreshold = 0,

  .aewbMedFiltEnable = FALSE,
  .aewbSatLimit      = 1023,
  .aewbWinStartX     = 0,  //changed by Richard 0 for AWB//changed by windsor 1
  .aewbWinStartY     = 0,
  .aewbWinNumH       = 12, //changed by windsor //12, //32,
  .aewbWinNumV       = 32, //changed by windsor //32, //12,
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
