#include <stdlib.h>
#include <string.h>

#include <avserver.h>
#include <osa_cmem.h>
#include <osa_sem.h>

//#define SALDRE_BENCHMARK
//#define SALDRE_DEBUG
//#define SALDRE_LOAD_TEST_FRAME

OSA_SemHndl g_saldreSemStart;
OSA_SemHndl g_saldreSemDone;

static OSA_BufInfo **g_pOutBufInfo;
static int *g_rszBufId;
static int g_numStreams;

static OSA_ThrHndl g_saldreThrHndl;

static Uint8 *g_saldre_inbufVirtAddr[2];
static int g_saldre_rawBufId[2];
static Uint8 *g_saldre_outbufVirtAddr[2];
static Uint8 *g_saldre_weightBufVirtAddr;
static Uint8 *g_saldre_boxcarVirtAddr;

static Int16 saldreInBufferIndex;
static Int16 saldreOutBufferIndex;
static Int16 saldreFirstTime= 1;

static Uint16 inputImageWidth;
static Uint16 inputImageHeight;

#ifdef SALDRE_LOAD_TEST_FRAME
static Uint8 *g_saldre_testFrame;
static void load_frame(char * filename, void * data, size_t len);

static void load_frame(char * filename, void * data, size_t len)
{
  FILE * _fi;

  _fi = fopen(filename, "rb");
  if(0 == _fi)
  {
    OSA_ERROR("Failed to create file\n");
    return;
  }

  if(1 !=  fread(data, len, 1,_fi))
  {
    OSA_ERROR("Failed with read\n");
  }

  fclose(_fi);
}
#endif

int VIDEO_saldreAllocBuffers(Uint16 width, Uint16 height)
{
  int bufSize, outputBufSize;
   bufSize= width*height;

  /* Call function that calculates the output buffer size, which can contains more lines than input image of width*height pixels
     Note that width input is in number of pixels, not bytes. Output of this function is in number of bytes.
  */
  outputBufSize= SALDRE_getOutputBufferSize(width/2, height, width/2);

#ifdef SALDRE_DEBUG
  OSA_printf("VIDEO_saldreAllocBuffers: allocation saldre buffers of size %d byte \n", bufSize);
#endif

  g_saldre_outbufVirtAddr[0] = OSA_cmemAlloc(outputBufSize, 32);
  g_saldre_outbufVirtAddr[1] = OSA_cmemAlloc(outputBufSize, 32);
  g_saldre_weightBufVirtAddr = OSA_cmemAlloc(bufSize, 32);
  g_saldre_boxcarVirtAddr = OSA_cmemAlloc((bufSize/256)*5*2, 32);
  if (!(g_saldre_outbufVirtAddr[0] && g_saldre_outbufVirtAddr[1] && g_saldre_weightBufVirtAddr && g_saldre_boxcarVirtAddr))
    OSA_ERROR("OSA_cmemAlloc()\n");

  return OSA_SOK;
}

int VIDEO_saldreSetBufInfo(int rawBufId, Uint8* inputAddr)
{
  g_saldre_rawBufId[saldreInBufferIndex]= rawBufId;
  g_saldre_inbufVirtAddr[saldreInBufferIndex]= inputAddr;

  return OSA_SOK;
}

Uint8* VIDEO_saldreGetOutputBufAddr()
{
 return (g_saldre_outbufVirtAddr[saldreOutBufferIndex^1]);
}

int VIDEO_saldreIsifPutOutBuf()
{
 if (g_saldre_rawBufId[saldreInBufferIndex]!= -1){
    DRV_isifPutOutBuf(g_saldre_rawBufId[saldreInBufferIndex]);
  }
  return OSA_SOK;
}

int VIDEO_saldreStop()
{
    VIDEO_saldreIsifPutOutBuf();
    saldreFirstTime= 1;
    saldreInBufferIndex= 0;
    saldreOutBufferIndex= 0;
    g_saldre_rawBufId[0]= -1;
    g_saldre_rawBufId[1]= -1;
    SALDRE_resetFunc2();

    return 0;
}

int VIDEO_saldreDelete()
{
  VIDEO_saldreStop();
  OSA_cmemFree(g_saldre_outbufVirtAddr[0]);
  OSA_cmemFree(g_saldre_outbufVirtAddr[1]);
  OSA_cmemFree(g_saldre_weightBufVirtAddr);
  OSA_cmemFree(g_saldre_boxcarVirtAddr);
  OSA_thrDelete(&g_saldreThrHndl);
  OSA_semDelete(&g_saldreSemStart);
  OSA_semDelete(&g_saldreSemDone);

  return OSA_SOK;
}

int VIDEO_saldreRun(OSA_BufInfo **pOutBufInfo, int *rszBufId, int numStreams)
{
#ifdef SALDRE_BENCHMARK
  static struct timeval tv1, tv2, tv3, tv4, tv5;
  static Uint32 numframes= 0;
  int time_func0, time_func1, time_func2_setup, time_func2;
#endif

  Uint32 GBEStrength, GCEStrength, LBEStrengh, LCEStrength;

  Uint16 imgWidth, imgHeight, stride;
  Uint32 offset;
  Uint8* srcAddr;

  g_pOutBufInfo= pOutBufInfo;
  g_rszBufId= rszBufId;
  g_numStreams= numStreams;

#ifdef SALDRE_BENCHMARK
  gettimeofday(&tv1, NULL);
#ifdef SALDRE_DEBUG
  OSA_printf("Start func0\n");fflush(stdout);
#endif
#endif

  imgWidth= inputImageWidth;
  imgHeight= inputImageHeight;
  stride= gVIDEO_ctrl.captureInfo.isifInfo.ddrOutDataWidth;
  offset= (gVIDEO_ctrl.captureInfo.isifInfo.ddrOutDataWidth - imgWidth) + stride*(gVIDEO_ctrl.captureInfo.isifInfo.ddrOutDataHeight - imgHeight);

#ifdef PROC_TOPHALF
  imgHeight= (imgHeight/2 + 31) & 0xFFE0;
#endif

#ifdef SALDRE_LOAD_TEST_FRAME
  srcAddr= g_saldre_testFrame;
#else
  srcAddr= (Uint8*)g_saldre_inbufVirtAddr[saldreInBufferIndex] + offset;
#endif

  SALDRE_runFunc0(srcAddr,  (Uint8*)g_saldre_boxcarVirtAddr, imgWidth, imgHeight, stride);

#ifdef SALDRE_BENCHMARK
  gettimeofday(&tv2, NULL);
  time_func0 = (tv2.tv_sec-tv1.tv_sec)*1000000 + (tv2.tv_usec-tv1.tv_usec);
#ifdef SALDRE_DEBUG
  OSA_printf("End func0, start func2_setup\n");fflush(stdout);
#endif
#endif

  /* If not the first time saldre called, then we execute the block of code below */
  if (!saldreFirstTime) {

#ifdef SALDRE_LOAD_TEST_FRAME
    srcAddr= g_saldre_testFrame;
#else
    srcAddr= (Uint8*)g_saldre_inbufVirtAddr[saldreInBufferIndex^1] + offset;
#endif

    SALDRE_setupFunc2(srcAddr, (Uint8*)g_saldre_outbufVirtAddr[saldreOutBufferIndex], g_saldre_weightBufVirtAddr, NULL, imgWidth, imgHeight, stride, stride);

#ifdef SALDRE_BENCHMARK
  gettimeofday(&tv3, NULL);
  time_func2_setup = (tv3.tv_sec-tv2.tv_sec)*1000000 + (tv3.tv_usec-tv2.tv_usec);
#ifdef SALDRE_DEBUG
  OSA_printf("End func2_setup, Start func2\n");fflush(stdout);
#endif
#endif

  SALDRE_startFunc2();

#ifdef PROC_TOPHALF
    {
		Uint32 col, row;
		Uint32 *dst= (Uint32*)(base.dstBuf[0].ptr + 2*stride*imgHeight);
		Uint32 *src= (Uint32*)base.srcBuf[0].ptr;

		for (row=0; row<imgHeight; row++)
		  for(col= 0; col<imgWidth/2; col++)
			dst[col + row*(stride>>1)]= src[col + row*(stride>>1)];
    }
#endif

    //Toggle output buffer
    saldreOutBufferIndex^= 1;

#ifdef SALDRE_BENCHMARK

    SALDRE_waitFunc2();
#ifdef SALDRE_DEBUG
  OSA_printf("End func 2, Start func 1\n");fflush(stdout);
#endif

    gettimeofday(&tv4, NULL);
    time_func2 = (tv4.tv_sec-tv3.tv_sec)*1000000 + (tv4.tv_usec-tv3.tv_usec);
#endif

  }

   if(gAVSERVER_config.aewb_config.saldreLevel==SALDRE_HIGH){
    GBEStrength= 220; GCEStrength= 96;
    LBEStrengh= 8192; LCEStrength= 4096;
   }
   else if(gAVSERVER_config.aewb_config.saldreLevel==SALDRE_MED) {
    GBEStrength= 128; GCEStrength= 64;
    LBEStrengh= 6144; LCEStrength= 4096;
   }
   else { /* SALDRE_LOW */
    GBEStrength= 64; GCEStrength= 64;
    LBEStrengh= 4096; LCEStrength= 4096;
   }

  if(gAVSERVER_config.aewb_config.saldreMode!=SALDRE_LOCAL)  {
    LBEStrengh= 0; LCEStrength=0;
  }
   else {
    GBEStrength= 64; GCEStrength= 64;
   }
  //countframes = countframes+1;
  SALDRE_runFunc1(g_saldre_boxcarVirtAddr, saldreInBufferIndex, imgWidth, imgHeight, 0,GBEStrength, GCEStrength, LBEStrengh, LCEStrength);

  //Toggle input buffer
  saldreInBufferIndex^= 1;

  /*
  If first time, we don't need to execute the accelerator part of the saldre algorithm
  since the pipeline is not yet full
  */
  if (saldreFirstTime)
    saldreFirstTime= 0;
  else {
#ifndef SALDRE_BENCHMARK
    SALDRE_waitFunc2();
#else
    gettimeofday(&tv5, NULL);
    time_func1 = (tv5.tv_sec-tv4.tv_sec)*1000000 + (tv5.tv_usec-tv4.tv_usec);
#ifdef SALDRE_DEBUG
  OSA_printf("End func1\n");fflush(stdout);
#endif
#endif
    SALDRE_endFunc2();
    //OSA_semWait(&g_saldreSemDone, OSA_TIMEOUT_FOREVER);
  }

#ifdef SALDRE_BENCHMARK
  if (numframes++ % 300 == 0) {
    OSA_printf("func0 time_frm = %d\n", time_func0);
    OSA_printf("func1 time_frm = %d\n", time_func1);
    OSA_printf("func2 setup time_frm = %d\n", time_func2_setup);
    OSA_printf("func2 time_frm = %d\n", time_func2);
    OSA_printf("Total frame processing time (func0 + setup func2 + max(func1, func2)= %d\n\n", time_func0+time_func2_setup+(time_func1 > time_func2 ? time_func1 : time_func2));
  }
#endif

  return OSA_SOK;
}

void *VIDEO_saldreThrMain(void *prm)
{
  OSA_setTskName("saldre");
  while (1) {
    OSA_semWait(&g_saldreSemStart, OSA_TIMEOUT_FOREVER);
   	OSA_prfBegin(&gAVSERVER_ctrl.saldrePrf);
    VIDEO_saldreRun(NULL, NULL, 1);
   	OSA_prfEnd(&gAVSERVER_ctrl.saldrePrf, 1);
    OSA_semSignal(&g_saldreSemDone);
  }
}

int VIDEO_saldreCreate(void)
{
  int status;
  Uint16 stride;

  status= OSA_semCreate(&g_saldreSemStart, 1, 0);
  if(status!=OSA_SOK) {
    OSA_ERROR("OSA_semCreate()\n");
    return status;
  }

  status= OSA_semCreate(&g_saldreSemDone, 1, 0);
  if(status!=OSA_SOK) {
    OSA_ERROR("OSA_semCreate()\n");
    return status;
  }

  status= OSA_thrCreate(&g_saldreThrHndl, VIDEO_saldreThrMain, VIDEO_SALDRE_THR_PRI, VIDEO_CAPTURE_STACK_SIZE, NULL);
  if(status!=OSA_SOK) {
    OSA_ERROR("OSA_thrCreate()\n");
    return status;
  }

  inputImageWidth= (SALDRE_MAX_IMGWIDTH < gVIDEO_ctrl.captureInfo.isifInfo.ddrOutDataWidth ? SALDRE_MAX_IMGWIDTH : gVIDEO_ctrl.captureInfo.isifInfo.ddrOutDataWidth);
  inputImageHeight= (SALDRE_MAX_IMGHEIGHT < gVIDEO_ctrl.captureInfo.isifInfo.ddrOutDataHeight ? SALDRE_MAX_IMGHEIGHT : gVIDEO_ctrl.captureInfo.isifInfo.ddrOutDataHeight);

  SALDRE_create(inputImageWidth, inputImageHeight);

  stride= gVIDEO_ctrl.captureInfo.isifInfo.ddrOutDataOffsetH;

  VIDEO_saldreAllocBuffers(stride, gVIDEO_ctrl.captureInfo.isifInfo.ddrOutDataHeight);

  saldreInBufferIndex= 0;
  saldreOutBufferIndex= 0;
  saldreFirstTime= 1;
  g_saldre_rawBufId[0]= -1;
  g_saldre_rawBufId[1]= -1;

#ifdef SALDRE_LOAD_TEST_FRAME
{
  g_saldre_testFrame= OSA_cmemAlloc(inputImageWidth*inputImageHeight*2, 32);
  if (g_saldre_testFrame== NULL) {
    OSA_ERROR("saldre: Unable to allocate memory for test frame\n");
  }
  else {
    OSA_printf("saldre: Loading test frame ...\n");
    load_frame("ipnc_raw_test.raw", g_saldre_testFrame, inputImageWidth*inputImageHeight*2);
    OSA_printf("saldre: Test frame finished loading\n");
    }
}
#endif
  return OSA_SOK;
}
