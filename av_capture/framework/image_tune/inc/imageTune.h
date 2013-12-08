#ifndef _IMAGE_TUNE_H_
#define _IMAGE_TUNE_H_

#include <osa.h>
#include <imageTuneParams.h>
#include <alg_aewb.h>
#include <alg_vnf.h>

#define IMAGE_TUNE_SERVER_DEFAULT_IP_ADDR "127.0.0.1"
#define IMAGE_TUNE_SERVER_DEFAULT_PORT    (5000)

#define IMAGE_TUNE_VERSION                (0x00010002)

/*
If IMAGE_TUNE_USE_FACTORY_PARAMSET enabled, uses the paramset in the code (hard coded) as
default paramset during boot else IMAGE_TUNE_USE_DEFAULT_PARAMSET as boot paramset if it
is available and may change based on the boot paramset ID - now in vnf - Revisit
*/
#define IMAGE_TUNE_USE_FACTORY_PARAMSET
#define IMAGE_TUNE_USE_DEFAULT_PARAMSET       (0x01)

#define IMAGE_TUNE_SYS_GET_VERSION            (0x01)
#define IMAGE_TUNE_SYS_ECHO_STRING            (0x02)
#define IMAGE_TUNE_SYS_RESET                  (0x03)
#define IMAGE_TUNE_SYS_SET_HW_REG             (0x04)
#define IMAGE_TUNE_SYS_GET_HW_REG             (0x05)
#define IMAGE_TUNE_SYS_SAVE_PARAM_SET         (0x06)
#define IMAGE_TUNE_SYS_LOAD_PARAM_SET         (0x07)
#define IMAGE_TUNE_SYS_GET_PARAMS             (0x08)
#define IMAGE_TUNE_SYS_SAVE_RAW_DATA          (0x09)
#define IMAGE_TUNE_SYS_SAVE_YUV_DATA          (0x0A)
#define IMAGE_TUNE_SYS_AEWB_ENABLE            (0x0B)
#define IMAGE_TUNE_SYS_SEND_RAW_DATA          (0x0C)
#define IMAGE_TUNE_SYS_SEND_YUV_DATA          (0x0D)

#define IMAGE_TUNE_CCDC_HLPF_ENABLE           (0x10)
#define IMAGE_TUNE_CCDC_IN_DATA_MSB_POSITION  (0x11)
#define IMAGE_TUNE_CCDC_LINEARIZATION         (0x12)
#define IMAGE_TUNE_CCDC_COL_PAT               (0x13)
#define IMAGE_TUNE_CCDC_GAIN_OFFSET           (0x14)
#define IMAGE_TUNE_CCDC_DFC                   (0x15)
#define IMAGE_TUNE_CCDC_CLAMP                 (0x16)
#define IMAGE_TUNE_CCDC_LSC                   (0x17)

#define IMAGE_TUNE_IPIPEIF_VPI_ISIF_IN_DPC    (0x20)
#define IMAGE_TUNE_IPIPEIF_DDR_IN_DPC         (0x21)
#define IMAGE_TUNE_IPIPEIF_GAIN               (0x22)
#define IMAGE_TUNE_IPIPEIF_OUTCLIP            (0x23)
#define IMAGE_TUNE_IPIPEIF_AVG_FILTER_ENABLE  (0x24)

#define IMAGE_TUNE_IPIPE_COL_PAT              (0x30)
#define IMAGE_TUNE_IPIPE_LSC                  (0x31)
#define IMAGE_TUNE_IPIPE_DPC                  (0x32)
#define IMAGE_TUNE_IPIPE_NF1                  (0x33)
#define IMAGE_TUNE_IPIPE_NF2                  (0x34)
#define IMAGE_TUNE_IPIPE_GIC                  (0x35)
#define IMAGE_TUNE_IPIPE_WB                   (0x36)
#define IMAGE_TUNE_IPIPE_CFA                  (0x37)
#define IMAGE_TUNE_IPIPE_RGB2RGB1             (0x38)
#define IMAGE_TUNE_IPIPE_GAMMA                (0x39)
#define IMAGE_TUNE_IPIPE_RGB2RGB2             (0x3A)
#define IMAGE_TUNE_IPIPE_LUT3D                (0x3B)
#define IMAGE_TUNE_IPIPE_RGB2YUV              (0x3C)
#define IMAGE_TUNE_IPIPE_CNT_BRT              (0x3D)
#define IMAGE_TUNE_IPIPE_GBCE                 (0x3E)
#define IMAGE_TUNE_IPIPE_EDGE_ENHANCE         (0x3F)
#define IMAGE_TUNE_IPIPE_CAR                  (0x40)
#define IMAGE_TUNE_IPIPE_CGS                  (0x41)

#define IMAGE_TUNE_LDC_PRM                    (0x50)
#define IMAGE_TUNE_VNF_PRM                    (0x51)

#define IMAGE_TUNE_AWB_PRM                    (0x52)

#define IMAGE_TUNE_CMD_MAX                    (0x60)

#define IMAGE_TUNE_PACKET_SIZE_MAX            (142 * 2 * KB)

#define IMAGE_TUNE_CMD_FLAG_SEND_REPLY        (0x00000001)
#define IMAGE_TUNE_CMD_FLAG_STATUS_ERROR      (0x00000010)

#define IMAGE_TUNE_CMD_PRMSET_ID_ST           5      // bits (5- 12)  ANR - IT
#define IMAGE_TUNE_CMD_PRMSET_ID_MUL          0xFF   // need 8bits

// Data Structures

/**
  \brief  Command info structure
*/
typedef struct
{
    Uint32 commandId;            ///< command id
    Uint32 commandFlags;         ///< command flags
    Uint32 *prm;                 ///< command parameters
    Uint32 prmSize;              ///< size of command parameters
} IMAGE_TUNE_CmdInfo;


typedef struct
{
    IMAGE_TUNE_CcdcParams     *pCcdcPrm;
    IMAGE_TUNE_IpipeifParams  *pIpipeifPrm;
    IMAGE_TUNE_IpipeParams    *pIpipePrm;
    IMAGE_TUNE_LdcParams      *pLdcPrm;
    ALG_vnfParams             *pVnfPrm;
    awb_calc_data_t           *pAwbPrm;
} IMAGE_TUNE_Params;

typedef struct
{
    Uint16 dataFormat;      // 0: RAW, 1: YUV422, 2: YUV420
    Uint16 frameWidth;      // in pixels
    Uint16 frameHeight;     // in lines
    Uint16 frameOffsetH;    // in pixels for YUV422, YUV420, in bytes for RAW
    Uint16 frameOffsetV;    // in lines for YUV422, YUV420, NOT USED for RAW
    Uint8  *frameVirtAddr;  // virt address of frame data
    Uint16 rawDataStartPhase;   ///< 0: R, 1: Gr, 2: Gb, 3: B
    Uint16 rawDataBitsPerPixel; ///< 8..14 bits
    Uint16 rawDataFormat;       ///< 0: Normal 1pixel in 16-bits, no compression, 1: Alaw compressed, 2: Dpcm compressed
    ALG_AewbData_ITTAwb awbData;    // IT
} IMAGE_TUNE_SaveDataInfo;

typedef int (*IMAGE_TUNE_CmdHandler)(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo);


int IMAGE_TUNE_Init();
int IMAGE_TUNE_Exit();

int IMAGE_TUNE_AttachCmdHandler(Uint32 cmd, IMAGE_TUNE_CmdHandler handler);

int IMAGE_TUNE_ServerOpen(Uint32 port);
int IMAGE_TUNE_ServerRun();
int IMAGE_TUNE_ServerClose();

int IMAGE_TUNE_ClientSendCmd(char *serverIpAddr, int port, IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo);

int IMAGE_TUNE_GetParams(IMAGE_TUNE_Params *prm);
int IMAGE_TUNE_SetParams(IMAGE_TUNE_Params *prm);

int IMAGE_TUNE_GetIsifLscParams(CSL_CcdcLscConfig *lscPrm);
int IMAGE_TUNE_SetIsifGainOffset(CSL_CcdcGainOffsetConfig *prm);
int IMAGE_TUNE_SetIsifDcsub(Int16 dcSub);

int IMAGE_TUNE_SetIpipeRgb2Rgb1(CSL_IpipeRgb2RgbConfig *prm);
int IMAGE_TUNE_SetIpipeRgb2Rgb2(CSL_IpipeRgb2RgbConfig *prm);

int IMAGE_TUNE_SetIpipeRgb2Yuv(CSL_IpipeRgb2YuvConfig *prm);
int IMAGE_TUNE_SetIpipeWb(Uint16 gainR, Uint16 gainGr, Uint16 gainGb, Uint16 gainB);
int IMAGE_TUNE_SetIpipeCntBrt(CSL_IpipeCntBrtConfig *prm);
int IMAGE_TUNE_SetIpipeEdgeEnhance(CSL_IpipeEdgeEnhanceConfig *prm);
int IMAGE_TUNE_SetIpipeNf2(CSL_IpipeNfConfig *prm);

int IMAGE_TUNE_SaveData(char *filename, IMAGE_TUNE_SaveDataInfo *info);

int IMAGE_TUNE_SaveParams(int paramSetId);
int IMAGE_TUNE_LoadParams(int paramSetId);

int IMAGE_TUNE_GetVnfParams(ALG_vnfParams *vnfParams);
int IMAGE_TUNE_SetVnfParams(ALG_vnfParams *vnfParams);

/* IT */
int IMAGE_TUNE_GetAwbParams(awb_calc_data_t *awbParams);
int IMAGE_TUNE_SetAwbParams(awb_calc_data_t *awbParams);

int IMAGE_TUNE_SetSendRawData(int val);
int IMAGE_TUNE_SetSendYuvData(int val);
int IMAGE_TUNE_CmdDataSendSetUp(int headerSize, int dSize, Uint8 *headerData, Uint8 *ImgData);
int IMAGE_TUNE_CmdDataSend(IMAGE_TUNE_CmdInfo *statusInfo);
int IMAGE_TUNE_CopyImageData(int hSize, int dSize, Uint8 *headerData, Uint8 *ImgData);
int IMAGE_TUNE_SaveParamSetList(IMAGE_TUNE_ParamSet *prm, int pID);
int IMAGE_TUNE_ReadParamset(int value);

#endif /* _IMAGE_TUNE_H_ */
