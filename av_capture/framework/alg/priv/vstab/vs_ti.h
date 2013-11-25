
/*
//============================================================================
//    FILE NAME : vs_ti.h
//    ALGORITHM : VS
//    VENDOR    : TI
//    TARGET DSP: C64x+
//    PURPOSE   : This is the top level driver file that exercises the VS code
//============================================================================
*/
/* ------------------------------------------------------------------------ */
/*            Copyright (c) 2006 Texas Instruments, Incorporated.           */
/*                           All Rights Reserved.                           */
/* ======================================================================== */

#ifndef VS_TI_
#define VS_TI_

#include <ivs1.h>

#define VS_MEMRECS 	(2)

#define VS_INT_SCRATCH_MEMORY_SIZE    (8 * 1024)
#define VS_INT_PERSISTENT_MEMORY_SIZE (512)

/*
 *  ======== VS_TI_IALG ========
 *  TI's implementation of the IALG interface for VS
 */
extern IALG_Fxns VS_TI_IALG;

//extern IDMA3_Fxns JPEGENC_TI_IDMA3;
/*
 *  ======== VS_TI_IVS ========
 *  TI's implementation of the IJPEGENC interface
 */
extern IVS1_Fxns VS_TI_IVS;

extern XDAS_Int32 VS_TI_initObj(IALG_Handle handle,
                         const IALG_MemRec memTab[],
                         IALG_Handle p,
                         const IALG_Params * vsParams);

extern XDAS_Int32 VS_TI_numAlloc();

extern XDAS_Int32 VS_TI_alloc(const IALG_Params *vsParams,
                       IALG_Fxns **fxns,IALG_MemRec memTab[]);

extern XDAS_Void VS_TI_activate(IALG_Handle handle);

extern XDAS_Void VS_TI_deactivate(IALG_Handle handle);

extern XDAS_Int32 VS_TI_apply(IVS1_Handle handle,
                       IVS1_InArgs *Inargs,
                       IVS1_OutArgs *Outargs);

extern XDAS_Int32 VS_TI_free(IALG_Handle handle, IALG_MemRec memTab[]);

#endif	/* VS_TI_ */

/* ======================================================================== */
/* End of file : vs_ti.h                                             */
/* ------------------------------------------------------------------------ */
/*            Copyright (c) 2006 Texas Instruments, Incorporated.           */
/*                           All Rights Reserved.                           */
/* ======================================================================== */

