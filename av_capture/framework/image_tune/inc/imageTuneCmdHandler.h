#ifndef _IMAGE_TUNE_CMD_HANDLER_H_
#define _IMAGE_TUNE_CMD_HANDLER_H_

#include <imageTune.h>

// Sys handlers
int IMAGE_TUNE_CmdExecuteEchoString(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo);
int IMAGE_TUNE_CmdExecuteGetVersion(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo);
int IMAGE_TUNE_CmdExecuteGetParamSet(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo);
int IMAGE_TUNE_CmdExecuteSaveParamSet(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo);
int IMAGE_TUNE_CmdExecuteLoadParamSet(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo);

// Ipipe handlers
int IMAGE_TUNE_CmdExecuteIpipeColPat(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo);
int IMAGE_TUNE_CmdExecuteIpipeLsc(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo);
int IMAGE_TUNE_CmdExecuteIpipeDpc(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo);
int IMAGE_TUNE_CmdExecuteIpipeNf1(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo);
int IMAGE_TUNE_CmdExecuteIpipeNf2(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo);
int IMAGE_TUNE_CmdExecuteIpipeGic(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo);
int IMAGE_TUNE_CmdExecuteIpipeWb(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo);
int IMAGE_TUNE_CmdExecuteIpipeCfa(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo);
int IMAGE_TUNE_CmdExecuteIpipeRgb2Rgb1(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo);
int IMAGE_TUNE_CmdExecuteIpipeGamma(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo);
int IMAGE_TUNE_CmdExecuteIpipeRgb2Rgb2(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo);
int IMAGE_TUNE_CmdExecuteIpipeLut3d(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo);
int IMAGE_TUNE_CmdExecuteIpipeRgb2Yuv(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo);
int IMAGE_TUNE_CmdExecuteIpipeCntBrt(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo);
int IMAGE_TUNE_CmdExecuteIpipeGbce(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo);
int IMAGE_TUNE_CmdExecuteIpipeEdgeEnhance(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo);
int IMAGE_TUNE_CmdExecuteIpipeCromaArtifactReduce(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo);
int IMAGE_TUNE_CmdExecuteIpipeChromaGainSupress(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo);

// Ccdc handlers
int IMAGE_TUNE_CmdExecuteCcdcHLpf(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo);
int IMAGE_TUNE_CmdExecuteCcdcInDataMsbPosition(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo);
int IMAGE_TUNE_CmdExecuteCcdcLinerization(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo);
int IMAGE_TUNE_CmdExecuteCcdcColPat(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo);
int IMAGE_TUNE_CmdExecuteCcdcGainOffset(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo);
int IMAGE_TUNE_CmdExecuteCcdcDfc(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo);
int IMAGE_TUNE_CmdExecuteCcdcClamp(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo);
int IMAGE_TUNE_CmdExecuteCcdcLsc(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo);

// Ipipeif handlers
int IMAGE_TUNE_CmdExecuteIpipeifVpiIsifInDpc(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo);
int IMAGE_TUNE_CmdExecuteIpipeifDdrInDpc(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo);
int IMAGE_TUNE_CmdExecuteIpipeifGain(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo);
int IMAGE_TUNE_CmdExecuteIpipeifOutClip(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo);
int IMAGE_TUNE_CmdExecuteIpipeifAvgFilter(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo);

// LDC handlers
int IMAGE_TUNE_CmdExecuteLdcPrm(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo);

// VNF handler's
int IMAGE_TUNE_CmdExecuteVnfPrm(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo);
// AWB handler's 
int IMAGE_TUNE_CmdExecuteAwbPrm(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo);
int IMAGE_TUNE_CmdSetAwbPrmStatus(int status);
int IMAGE_TUNE_CmdGetAwbPrmStatus(int* status);
// Multi Params 
int IMAGE_TUNE_MultiParams_Set(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo);


int IMAGE_TUNE_CmdExecuteIpipeRgb2Yuv_new(Uint32 id);


#endif /* _IMAGE_TUNE_CMD_HANDLER_H_ */
