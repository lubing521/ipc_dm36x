#ifndef _SALDRE_H_
#define _SALDRE_H_

#define SALDRE_MAX_IMGWIDTH 	(1920)
#define SALDRE_MAX_IMGHEIGHT 	(1088)

Int32 SALDRE_create(Uint16 width, Uint16 height);
Uint32 SALDRE_getOutputBufferSize(Uint16 width, Uint16 height, Uint16 dstStride);
Int32 SALDRE_runFunc0(Uint8* srcAddr, Uint8* dstAddr, Uint16 imgWidth, Uint16 imgHeight, Uint16 stride);
Int32 SALDRE_runFunc1(Uint8 *g_saldre_boxcarVirtAddr, Uint16 index, Uint16 imgWidth, Uint16 imgHeight, Uint32 config, Int16 GBEStrength, Int16 GCEStrength, Int16 LBEStrength, Int16 LCEStrength);
void SALDRE_resetFunc2();
Int32 SALDRE_setupFunc2(Uint8 *srcAddr, Uint8*dstAddr, Uint8* weight, Uint8 *toneCurves,  Uint16 imgWidth, Uint16 imgHeight, Uint16 strideSrc, Uint16 strideDst);
void SALDRE_startFunc2();
void SALDRE_waitFunc2();
void SALDRE_endFunc2();

#endif
