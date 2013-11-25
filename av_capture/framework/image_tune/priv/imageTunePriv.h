#ifndef _IMAGE_TUNE_PRIV_H_
#define _IMAGE_TUNE_PRIV_H_

#include <imageTune.h>
#include <imageTuneParams.h>

#define IMAGE_TUNE_CMD_HANDLER_DEBUG 
//#define IMAGE_TUNE_CLIENT_DEBUG 
#define IMAGE_TUNE_SERVER_DEBUG 
#define IMAGE_TUNE_PACKET_DEBUG 
#define IMAGE_TUNE_COMM_DEBUG

#define IMAGE_TUNE_AWB_RGB_SIZE    1024
#define IMAGE_TUNE_AWB_YUV_SIZE    1024
#define IMAGE_TUNE_AWB_PRMDATA_SIZE    1024

/*
  Image Tune control structure
*/
typedef struct {
  
 int serverSocketId;   // Server socket id
 int clientSocketId;   // Client socket id
 int connectedSocketId;    // connection client socket id
 int serverPort;       // communication port 
 
 Uint32 serverDataBuf[IMAGE_TUNE_PACKET_SIZE_MAX/4];
 Uint32 statusDataBuf[IMAGE_TUNE_PACKET_SIZE_MAX/4]; 
 
 IMAGE_TUNE_CmdHandler cmdHandlerList[IMAGE_TUNE_CMD_MAX];
 
 IMAGE_TUNE_ParamSet  curPrm;
  
} IMAGE_TUNE_Ctrl;

/**
  \brief  Message packet header structure
*/
typedef struct {

  Uint32 headerMagicNum;      ///< header magic number to identify as image tuning tool message
  Uint32 checkSum;            ///< check sum for message validation 
  Uint32 commandId;           ///< command id
  Uint32 commandFlags;        ///< command flags to identify the type the command
  Uint32 prmSize;             ///< size of parameters

} IMAGE_TUNE_PacketHeader;

/**
  \brief YUV/RAW data file header format
*/
typedef struct {

  Uint32 validDataStartOffset;///< offset in bytes from start of file where actual data is stored in file
  Uint32 magicNum;            ///< to indicate if this is a valid header
  Uint32 version;             ///< header format version number, 0x00010000, is v1.0
  Uint32 dataFormat;          ///< 0: raw data, 1: YUV422 data, 2: YUV420 data
  Uint32 dataWidth;           ///< in pixels
  Uint32 dataHeight;          ///< in lines
  Uint32 dataBytesPerLines;   ///< in bytes 
  Uint32 yuv420ChormaOffset;  ///< in bytes from start of valid data, only valid for YUV420 data, file offset is (validDataStartOffset+dataChormaOffset)
  Uint32 rawDataStartPhase;   ///< 0: R, 1: Gr, 2: Gb, 3: B  
  Uint32 rawDataBitsPerPixel; ///< 8..14 bits
  Uint32 rawDataFormat;       ///< 0: Normal 1pixel in 16-bits, no compression, 1: Alaw compressed, 2: Dpcm compressed
  Uint32 H3aRegs[32];            ///< H3A Register Dump  - AWB
  Uint32 AwbNumWinH;          ///< Number of AWB windows in H direction
  Uint32 AwbNumWinV;          ///< Number of AWB windows in V direction
  Uint32 AwbRgbDataOffset;  ///< Offset in bytes from start of file where RGB H3A data is present
  Uint32 AwbYuvDataOffset;  ///< Offset in bytes from start of file where YUV H3A data is present
  Uint32 AwbMiscData[16];     ///< AWB algorithm specific data
  Uint32 AwbRgbData[3*IMAGE_TUNE_AWB_RGB_SIZE];    ///< Valid data is 4x3xN bytes where N = AwbNumWinH x AwbNumWinV
  Uint32 AwbYuvData[3*IMAGE_TUNE_AWB_YUV_SIZE];    ///< Valid data is 4x3xN bytes where N = AwbNumWinH x AwbNumWinV

} IMAGE_TUNE_SaveDataFileHeader;

/**
  \brief  Message packet footer structure
*/
typedef struct {

  Uint32 footerMagicNum;       ///< footer magic number to confirm end of parameters

} IMAGE_TUNE_PacketFooter;  


extern IMAGE_TUNE_Ctrl gIMAGE_TUNE_ctrl;

#define IMAGE_TUNE_PACKET_HEADER_SIZE    sizeof(IMAGE_TUNE_PacketHeader)
#define IMAGE_TUNE_PACKET_FOOTER_SIZE    sizeof(IMAGE_TUNE_PacketFooter)

#define IMAGE_TUNE_HEADER_MAGIC_NUM   (0xABCD1234)
#define IMAGE_TUNE_FOOTER_MAGIC_NUM   (0x5678EF09)

#define IMAGE_TUNE_HEADER_OFFSET      (0)  
#define IMAGE_TUNE_CHECK_SUM_OFFSET   (1)  
#define IMAGE_TUNE_COMMAND_ID_OFFSET  (2)  
#define IMAGE_TUNE_PRM_SIZE_OFFSET    (4)

#define IMAGE_TUNE_PARAM_STORAGE_FILE_PATH    "/opt/ipnc"

/* Image Tune Message APIs */
int IMAGE_TUNE_MakePacket(IMAGE_TUNE_CmdInfo *cmdInfo, Uint32 *packet, Uint32 *packetSize); 
int IMAGE_TUNE_ParsePacket(Uint32 *packet, Uint32 packetSize, IMAGE_TUNE_CmdInfo *cmdInfo );

/* Command Handler API */
int IMAGE_TUNE_CmdExecute(IMAGE_TUNE_CmdInfo *cmdInfo, IMAGE_TUNE_CmdInfo *statusInfo);

/* Communication Link APIs */
int IMAGE_TUNE_ClientOpen(char *ip_addr, Uint32 port);
int IMAGE_TUNE_ClientSendData(Uint8 *dataBuf, Uint32 dataSize);
int IMAGE_TUNE_ClientRecvData(Uint8 *dataBuf, Uint32 *dataSize);
int IMAGE_TUNE_ClientClose();

int IMAGE_TUNE_ServerWaitConnect();
int IMAGE_TUNE_ServerRecvData(Uint8 *dataBuf, Uint32 *dataSize);
int IMAGE_TUNE_ServerSendData(Uint8 *dataBuf, Uint32 dataSize);

int IMAGE_TUNE_CalcParamSetChecksum(IMAGE_TUNE_ParamSet *prm);
#ifdef  IMAGE_TUNE_MULTI_PARAM  
int IMAGE_TUNE_CalcParamSetChecksum2(IMAGE_TUNE_Pkt_ParamSet *prm);
#endif

#endif /* _IMAGE_TUNE_PRIV_H_ */
