#ifndef _IMAGE_TUNE_PARAMS_H_
#define _IMAGE_TUNE_PARAMS_H_

#include <osa.h>
#include <csl_hndl.h>
#include <alg_vnf.h>
#include <alg_ti_aewb_priv.h>

#ifdef YUV_MODE_INTERLACED
#define IMAGE_TUNE_LSC_BUFFER_MAX   ((720/16)*(480/8)*4)
#else
#define IMAGE_TUNE_LSC_BUFFER_MAX   ((2048/16)*(1536/8)*4)
#endif

#define IMAGE_TUNE_DEFAULT_PARAMSET  0 
#define IMAGE_TUNE_MAX_PARAMSET         16 // 0 to 15 

#define IMAGE_TUNE_MULTI_PARAM     //Multi param support

#ifdef IMAGE_TUNE_MULTI_PARAM

#define IMAGE_TUNE_PARAMSET_DIR          "/mnt/nand/IMG_Paramset.bin"  /* ImageTune paramset Dir */

/* For Testing */
//#define IMAGE_TUNE_GET_NEW_PRM   //To get latest param sent through ITT(PC) to IPNC.
//#define IMAGE_TUNE_TEST_GET_PRM   //To get param 0 to 15 through ITT(PC) to IPNC.

#endif

typedef struct {

  Bool32                        hLpfEnable;
  Uint32                      inDataMsbPosition;
  CSL_CcdcLinerizationConfig  lin;
  Uint32                      linTable[CSL_ISIF_LIN_TBL_MAX_ENTRIES];
  CSL_CcdcColPatConfig        colPat;
  CSL_CcdcGainOffsetConfig    gainOffset;
  CSL_CcdcDfcConfig           dfc;
  CSL_CcdcVdfcTable           vdfcTable;
  CSL_CcdcClampConfig         clamp;
  CSL_CcdcLscConfig           lsc;
  Uint32                      lscGainTable[IMAGE_TUNE_LSC_BUFFER_MAX/4];
  Uint32                      lscOffsetTable[IMAGE_TUNE_LSC_BUFFER_MAX/4];

} IMAGE_TUNE_CcdcParams;

typedef struct {

  Bool32    vpiIsifInDpcEnable;
  Uint32  vpiIsifInDpcThreshold;
  Bool32    ddrInDpcEnable;
  Uint32  ddrInDpcThreshold;
  Uint32  gain;
  Uint32  outClip;
  Uint32  avgFilterEnable;

} IMAGE_TUNE_IpipeifParams;

typedef struct {

  CSL_IpipeColPatConfig               colPat;
  CSL_IpipeLscConfig                  lsc;
  CSL_IpipeDpcConfig                  dpc;
  Uint32                              dpcTable[CSL_IPIPE_DPC_LUT_MAX_ENTRIES];
  CSL_IpipeNfConfig                   nf1;
  CSL_IpipeNfConfig                   nf2;
  CSL_IpipeGicConfig                  gic;
  CSL_IpipeWbConfig                   wb;
  CSL_IpipeCfaConfig                  cfa;
  CSL_IpipeRgb2RgbConfig              rgb2rgb1;
  CSL_IpipeGammaConfig                gamma;
  Uint32                              gammaTableR[CSL_IPIPE_GAMMA_TABLE_MAX_ENTRIES];
  Uint32                              gammaTableG[CSL_IPIPE_GAMMA_TABLE_MAX_ENTRIES];
  Uint32                              gammaTableB[CSL_IPIPE_GAMMA_TABLE_MAX_ENTRIES];
  CSL_IpipeRgb2RgbConfig              rgb2rgb2;
  CSL_Ipipe3dLutConfig                lut3d;
  Uint32                              lut3dTable[CSL_IPIPE_3D_LUT_MAX_ENTRIES];
  CSL_IpipeRgb2YuvConfig              rgb2yuv;
  CSL_IpipeCntBrtConfig               cntBrt;
  CSL_IpipeGbceConfig                 gbce;
  Uint32                              gbceTable[CSL_IPIPE_GBCE_GAIN_TABLE_MAX_ENTRIES];
  CSL_IpipeEdgeEnhanceConfig          yee;
  Uint32                              yeeTable[CSL_IPIPE_EDGE_ENHANCE_TABLE_MAX_ENTRIES];
  CSL_IpipeChromaArtifactReduceConfig car;
  CSL_IpipeChromaGainSupressConfig    cgs;

} IMAGE_TUNE_IpipeParams;

typedef struct {

  CSL_LdcHwSetup              ldcSetup;
  Uint32                      ldcTable[CSL_LDC_TABLE_MAX_ENTRIES];

} IMAGE_TUNE_LdcParams;

typedef struct {

  Uint32 headerMagicNum;      ///< header magic number to identify as image tuning tool parameter set
  Uint32 checkSum;            ///< check sum for parameter validation
  Uint32 paramFlags;          ///< set to 0

  IMAGE_TUNE_CcdcParams     ccdcPrm;
  IMAGE_TUNE_IpipeifParams  ipipeifPrm;
  IMAGE_TUNE_IpipeParams    ipipePrm;
  IMAGE_TUNE_LdcParams      ldcPrm;
  ALG_vnfParams             vnfPrm;
  awb_calc_data_t awbPrm;
  
} IMAGE_TUNE_ParamSet;

#ifdef  IMAGE_TUNE_MULTI_PARAM  

/* Copy of IMAGE_TUNE_CcdcParams - Table data removed to reduce the multi param set list size  */

typedef struct {

  Bool32                        hLpfEnable;
  Uint32                      inDataMsbPosition;
  CSL_CcdcLinerizationConfig  lin;
  Uint32                      linTable[CSL_ISIF_LIN_TBL_MAX_ENTRIES];
  CSL_CcdcColPatConfig        colPat;
  CSL_CcdcGainOffsetConfig    gainOffset;
  CSL_CcdcDfcConfig           dfc;
  CSL_CcdcVdfcTable           vdfcTable;
  CSL_CcdcClampConfig         clamp;
  CSL_CcdcLscConfig           lsc;
} IMAGE_TUNE_CcdcParams_m;


/* Copy of IMAGE_TUNE_CcdcParams Table - removed to reduce the multi param set list size  */

typedef struct {
	
  Uint32                      lscGainTable[IMAGE_TUNE_LSC_BUFFER_MAX/4];
  Uint32                      lscOffsetTable[IMAGE_TUNE_LSC_BUFFER_MAX/4];

} IMAGE_TUNE_CCDCTable;


/* Copy of IMAGE_TUNE_ParamSet - Ccdc st modified to reduce the multi param set list size  */
typedef struct {

  Uint32 headerMagicNum;      ///< header magic number to identify as image tuning tool parameter set
  Uint32 checkSum;            ///< check sum for parameter validation
  Uint32 paramFlags;          ///< set to 0
  IMAGE_TUNE_CcdcParams_m     ccdcPrm;
  IMAGE_TUNE_IpipeifParams  ipipeifPrm;
  IMAGE_TUNE_IpipeParams    ipipePrm;
  IMAGE_TUNE_LdcParams      ldcPrm;
  ALG_vnfParams             vnfPrm;
  awb_calc_data_t awbPrm;

} IMAGE_TUNE_ParamSet_m;


/*Paramset Lisit */
typedef struct {

  Uint32 curParamset;                                     //if paramFlag == paramsetID, thenonly Paramet (ID) will set for boot up params.
  Uint32 paramFlag[IMAGE_TUNE_MAX_PARAMSET];              //Check paramset status -if "0" paramset not at all set
  IMAGE_TUNE_ParamSet_m paramSet[IMAGE_TUNE_MAX_PARAMSET];
  IMAGE_TUNE_CCDCTable ccdcTable;  
 
} IMAGE_TUNE_ParamSetList;  


/* Copy of IMAGE_TUNE_ParamSet - Awb st removed to for sending it to IIT(PC)  */
/* Revisit */
typedef struct {

  Uint32 headerMagicNum;      ///< header magic number to identify as image tuning tool parameter set
  Uint32 checkSum;            ///< check sum for parameter validation
  Uint32 paramFlags;          ///< set to 0

  IMAGE_TUNE_CcdcParams     ccdcPrm;
  IMAGE_TUNE_IpipeifParams  ipipeifPrm;
  IMAGE_TUNE_IpipeParams    ipipePrm;
  IMAGE_TUNE_LdcParams      ldcPrm;
  ALG_vnfParams             vnfPrm;
  
} IMAGE_TUNE_Pkt_ParamSet;

#endif

#endif /* _IMAGE_TUNE_PARAMS_H_ */

