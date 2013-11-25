/* ======================================================================== */
/*  TEXAS INSTRUMENTS, INC.                                                 */
/*                                                                          */
/*  VICP Signal Processing Library                                          */
/*                                                                          */
/*  This library contains proprietary intellectual property of Texas        */
/*  Instruments, Inc.  The library and its source code are protected by     */
/*  various copyrights, and portions may also be protected by patents or    */
/*  other legal protections.                                                */
/*                                                                          */
/*  This software is licensed for use with Texas Instruments TMS320         */
/*  family DSPs.  This license was provided to you prior to installing      */
/*  the software.  You may review this license by consulting the file       */
/*  TI_license.PDF which accompanies the files in this library.             */
/*                                                                          */
/* ------------------------------------------------------------------------ */
/*                                                                          */
/*     NAME                                                                 */
/*        vicplib.h -- Signal Processing Library Interface header file      */
/*                                                                          */
/*     DESCRIPTION                                                          */
/*        This file includes the definitions and the interfaces supported   */
/*        by the Library                                                    */
/*                                                                          */
/*     REV                                                                  */
/*        version 0.2.1:  4 Nov , 2009                                      */
/*        Updates to extend VICP Lib API support for DM365                  */
/*                                                                          */
/*        version 0.2.0:  19 Oct , 2009                                     */
/*        Added sad                                                         */
/*                                                                          */
/*        version 0.1.0:  12 June, 2009                                     */
/*        Added cfa interpolation                                           */
/*                                                                          */
/*        version 0.0.9:  29 May, 2009                                      */
/*        Added affine transform                                            */
/*                                                                          */
/*        version 0.0.8:  29 May, 2009                                      */
/*        Added affine transform                                            */
/*                                                                          */
/*        version 0.0.7:  10 April, 2009                                    */
/*        Added integral image, sobel, image pyramid                        */
/*                                                                          */
/*        version 0.0.6:  3rd March, 2009                                   */
/*        Added CPIS_updateSrcDstPtr                                        */
/*                                                                          */
/*        version 0.0.5:  5th Jan, 2009                                     */
/*        Added 2-D median filtering                                        */
/*                                                                          */ 
/*        version 0.0.4:  10th Dec                                          */
/*        Added recursive filtering                                         */
/*                                                                          */ 
/*        version 0.0.3:  19th Nov                                          */
/*        Updated CPIS_init to include staticDMAAlloc option                */
/*                                                                          */
/*        version 0.0.2:  27th Oct                                          */
/*        Adding kernels for release 2.0                                    */
/*                                                                          */
/*        version 0.0.1:  22nd Sep                                          */
/*        Initial version                                                   */
/*                                                                          */
/* ------------------------------------------------------------------------ */
/*            Copyright (c) 2008 Texas Instruments, Incorporated.           */
/*                           All Rights Reserved.                           */
/* ======================================================================== */

#ifndef _CPISLIB_H
#define _CPISLIB_H

#ifdef __cplusplus
    extern "C" {
#endif

#include <tistdtypes.h>

#define CPIS_MAX_SRC_CHANNELS 8
#define CPIS_MAX_DST_CHANNELS 6

/* 
CPIS_MAX_CHANNELS does not have to be necessarily equal to 
CPIS_MAX_SRC_CHANNELS + CPIS_MAX_DST_CHANNELS
It represents the number of total channels allocatable
by the vicp library, including source an destination channels
hence you can have a combination of: 8 SRC channels + 2 DST channels
or 2 SRC channels + 8 DST channels
*/

#define CPIS_MAX_CHANNELS     14

/* Error symbols used by the library */
#define CPIS_INIT_ERROR                 1
#define CPIS_NOTINIT_ERROR              2
#define CPIS_UNPACK_ERROR               3
#define CPIS_NOSUPPORTFORMAT_ERROR      4
#define CPIS_NOSUPPORTDIM_ERROR         5
#define CPIS_PACK_ERROR                 6
#define CPIS_MAXNUMFUNCREACHED          7
#define CPIS_OUTOFMEM                   8
#define CPIS_NOSUPPORTANGLE_ERROR       9
#define CPIS_NOSUPPORTOP_ERROR          10
#define CPIS_NOT_ENOUGH_EDMACHAN_ERROR  11
#define CPIS_ALGO_REGISTRATION_ERROR    12


extern Uint32 CPIS_errno;

/* Various data formats supported by the library */
typedef enum {
    CPIS_YUV_420P=0, /* Planar symbols must be listed first */
    CPIS_YUV_422P,
    CPIS_YUV_444P,
    CPIS_YUV_411P,
    CPIS_YUV_422VP,  /* Vertical subsampling */
    CPIS_RGB_P,
    CPIS_BAYER_P,
    CPIS_YUV_422IBE,
    CPIS_YUV_422ILE,
    CPIS_RGB_555,
    CPIS_RGB_565,
    CPIS_BAYER,
    CPIS_YUV_444IBE,
    CPIS_YUV_444ILE,
    CPIS_RGB_888,
    CPIS_YUV_GRAY,
    CPIS_8BIT,
    CPIS_16BIT,
    CPIS_32BIT,
    CPIS_64BIT,
    CPIS_U8BIT,
    CPIS_U16BIT,
    CPIS_U32BIT,
    CPIS_U64BIT,
    CPIS_1BIT,
    CPIS_YUV_420SP    
} CPIS_Format;

typedef enum {
    CPIS_TOP2BOTTOM,
    CPIS_BOTTOM2TOP,
    CPIS_LEFT2RIGHT,
    CPIS_RIGHT2LEFT
} CPIS_FilterDir;

typedef enum {
    CPIS_USE_BOUNDARY,
    CPIS_USE_PASSED_VALUES
} CPIS_FilterInitialMode;

#define CPIS_ALPHA 0x8000
#define CPIS_ALPHA_SEPARATE_PLANE 0x4000
#define CPIS_FOREGROUND_ALPHA_ARGB888_PLANE	0x2000
#define CPIS_BACKGROUND_ALPHA_ARGB888_PLANE	0x1000

/* Enum that controls the synchronous or async operation of the library APIs */
typedef enum {
    CPIS_SYNC,
    CPIS_ASYNC
} CPIS_ExecType;

/* Initialization structure for the library */
typedef void (*Cache_wbInv)(void *, Uint32, Bool);
typedef void (*VicpLock)(void*);
typedef void (*VicpUnlock)(void*);

#define CPIS_INIT_CE                1
#define CPIS_INIT_RMAN              (1<<1)
#define CPIS_INIT_VICP_IRES         (1<<2)
#define CPIS_INIT_EDMA3_IRES        (1<<3)
#define CPIS_INIT_ADDRSPACE_IRES    (1<<4)
#define CPIS_INIT_FC_ALL            (CPIS_INIT_CE | CPIS_INIT_RMAN | CPIS_INIT_VICP_IRES |CPIS_INIT_EDMA3_IRES | CPIS_INIT_ADDRSPACE_IRES)
#define CPIS_INIT_FC_NONE           0

typedef struct {
    Uint16 maxNumProcFunc;
    void *mem;
    Uint32 memSize;
    Cache_wbInv cacheWbInv;
    Uint16 staticDmaAlloc;
    Uint16 initFC;          /* Initialize framework components according to bitmask combination:
                                   CPIS_INIT_CE: initialize CE, need to set member engineName as well
                                   CPIS_INIT_RMAN: initialize RMAN
                                   CPIS_INIT_VICP_IRES: register VICP res manager 
                                   CPIS_INIT_EDMA3_IRES: register EDMA3 res manager
                                   CPIS_INIT_ADDRSPACE_IRES: register ADDRSPACE res manager
                                All these previous symbols can be ORed to form a bitmask.
                                In addition the symbol CPIS_INIT_FC_ALL can be used to initialize
                                all the Framework components.
                                CPIS_INIT_FC_NONE can be used to disable all FC initialization by
                                the VICP library.   
                             */
    char *engineName;        /* Codec engine name, must match name used in vicpLib365.cfg
                                normally, it is "alg_server"  
                                This member is irrelevant if initFC's bit #0 is set to 0   
                             */
    void *codecEngineHandle; /* Codec engine handle */
    VicpLock lock;
    void *lockArg;
    VicpUnlock unlock;
    void *unlockArg;
    Uint16 maxNumDma;       /* Maximum number of EDMA channels that vicplib can use.
                               This allow the application integrator to optimize
                               the EDMA channel usage with the rest of the system */
} CPIS_Init;


/* Structures used to convey information regarding the various blocks */
typedef struct {
    Uint32 width;   /* In number of pixels */
    Uint32 height;  /* In number of lines */
} CPIS_Size;

typedef struct {
    Uint8 *ptr;
    Uint32 stride;  /* In number of pixels */
} CPIS_Buffer;


/* Base paramters common to all APIs */

typedef struct {
    CPIS_Format srcFormat[CPIS_MAX_SRC_CHANNELS];
    CPIS_Buffer srcBuf[CPIS_MAX_SRC_CHANNELS];
    CPIS_Format dstFormat[CPIS_MAX_DST_CHANNELS];
    CPIS_Buffer dstBuf[CPIS_MAX_DST_CHANNELS];
    CPIS_Size roiSize;
    CPIS_Size procBlockSize;
    Uint16 numInput;    /* Number of input buffers */
    Uint16 numOutput;   /* Number of ouput buffers */
} CPIS_BaseParms;

typedef void* CPIS_Handle;

/* AlphaBlendig API params */
typedef struct {
    /* 
        if set to 1 then alpha value specified next is used for entire image
        otherwise if 0, use alpha plane passed as src[1]
    */
    Uint16 useGlobalAlpha; 
    Uint16 alphaValue;  /* global alpha value, 0-255, 255 let see foreground, 0 let see background */
    CPIS_Buffer background;
    CPIS_Buffer backgroundUV;
    CPIS_Format backgroundFormat;
} CPIS_AlphaBlendParms;

/* APIs supported by the library */
Int32 CPIS_getMemSize(Uint16 maxNumProc);
Int32 CPIS_init(CPIS_Init *init);
Int32 CPIS_deInit();

Int32 CPIS_alphaBlend(
    CPIS_Handle *handle,
    CPIS_BaseParms *base,
    CPIS_AlphaBlendParms *params,
    CPIS_ExecType execType
);

Int32 CPIS_start(CPIS_Handle handle);

Int32 CPIS_wait(CPIS_Handle handle);

Int32 CPIS_reset(CPIS_Handle handle);

Int32 CPIS_updateSrcDstPtr(CPIS_Handle handle, CPIS_BaseParms *base);

Int32 CPIS_isBusy();

Int32 CPIS_setWaitCB(Int32 (*waitCB)(void*arg), void* waitCBarg);

Int32 CPIS_setWaitCBArg(void* waitCBarg);

Int32 CPIS_delete(CPIS_Handle handle);

void CPIS_lock(void);

void CPIS_unlock(void);

#ifdef __cplusplus
 }
#endif

#endif /* #define _CPISLIB_H */

/* ======================================================================== */
/*                       End of file                                        */
/* ------------------------------------------------------------------------ */
/*            Copyright (c) 2008 Texas Instruments, Incorporated.           */
/*                           All Rights Reserved.                           */
/* ======================================================================== */
