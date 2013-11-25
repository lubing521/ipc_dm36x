

#ifndef _ALG_PRIV_H_
#define _ALG_PRIV_H_

#include <xdc/std.h>
#include <ti/sdo/ce/trace/gt.h>
#include <ti/sdo/ce/CERuntime.h>
#include <ti/sdo/ce/utils/trace/TraceUtil.h>
#include <ti/sdo/ce/Engine.h>
#include <ti/sdo/ce/osal/Memory.h>
#include <ti/sdo/ce/speech1/sphenc1.h>
#include <ti/sdo/ce/audio1/audenc1.h>
#include <ti/sdo/ce/video1/videnc1.h>
#include <ti/sdo/ce/video2/viddec2.h>
#include <ti/sdo/ce/image1/imgenc1.h>
#include <ti/sdo/ce/image1/imgdec1.h>

#include <ti/sdo/fc/rman/rman.h>
#include <ti/sdo/fc/ires/iresman.h>
#include <ti/sdo/fc/ires/vicp/iresman_vicp2.h>
#include <ti/sdo/fc/ires/hdvicp/iresman_hdvicp.h>
#include <ti/sdo/fc/ires/hdvicp/ires_hdvicp.h>
#include <ti/sdo/fc/ires/edma3chan/iresman_edma3Chan.h>
#include <ti/sdo/fc/ires/edma3chan/ires_edma3Chan.h>
#include <ti/sdo/fc/edma3/edma3_config.h>
#include <ti/sdo/edma3/rm/edma3_rm.h>
#include <ti/sdo/fc/ires/addrspace/ires_addrspace.h>
#include <ti/sdo/fc/ires/addrspace/iresman_addrspace.h>

#include <alg.h>

#define ALG_CODEC_ENGINE_NAME         "alg_server"

#define ALG_VID_CODEC_H264_ENC_NAME   "h264enc"
#define ALG_VID_CODEC_MPEG4_ENC_NAME  "mpeg4enc"
#define ALG_VID_CODEC_JPEG_ENC_NAME   "jpegenc"

#define ALG_VID_CODEC_H264_DEC_NAME   "h264dec"
#define ALG_VID_CODEC_MPEG4_DEC_NAME  "mpeg4dec"
#define ALG_VID_CODEC_JPEG_DEC_NAME   "jpegdec"

#define ALG_AUD_CODEC_G711_ENC_NAME   "g711enc"
#define ALG_AUD_CODEC_G711_DEC_NAME   "g711dec"

#define ALG_AUD_CODEC_AAC_ENC_NAME   "aaclc_enc"
#define ALG_AUD_CODEC_AAC_DEC_NAME   "aac_dec"

#define ALG_VID_ADV_FEATURE_KTNF_NAME  "ktnf"

extern Engine_Handle gALG_hEngine;

extern OSA_MutexHndl vnf_imcop_lock;
extern OSA_MutexHndl vicp_imcop_lock;

#endif
