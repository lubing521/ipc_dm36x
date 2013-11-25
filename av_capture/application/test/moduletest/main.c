
#include <osa.h>

int CSL_sysTestMain(int argc, char **argv);

int OSA_ipcTestMain(int argc, char **argv);
int OSA_cmemTestMain(int argc, char **argv);
int OSA_tskTestMain(int argc, char **argv);

int DRV_drvTestMain(int argc, char **argv);
int DRV_reszTestMain(int argc, char **argv);
int DRV_dmaTestMain(int argc, char **argv);
int DRV_captureTestMain(int argc, char **argv);
int DRV_displayTestMain(int argc, char **argv);
int DRV_faceDetectTestMain(int argc, char **argv);
int DRV_ldcTestMain(int argc, char **argv);

int DRV_imgsTestMain(int argc, char **argv);
int DRV_isifTestMain(int argc, char **argv);

int IMAGE_TUNE_sysTestMain(int argc, char **argv);

int ALG_sysTestMain(int argc, char **argv);
int ALG_vidEncTestMain(int argc, char **argv);
int ALG_vidDecTestMain(int argc, char **argv);

int ALG_faceRecogTestMain(int argc, char **argv); /* ANR - FR*/

int main(int argc, char **argv)
{
  int status=0;

  if(argc < 2) {
    OSA_printf("\n");
    OSA_printf(" %s <test module name>\n", argv[0]);
    OSA_printf("\n");
    OSA_printf(" Supported modules,\n");
    OSA_printf(" csl\n");
    OSA_printf(" osa_cmem osa_tsk osa_ipc\n");
    OSA_printf(" drv drv_dma drv_display drv_fd drv_resz drv_ldc drv_imgs drv_isif drv_capture \n");
    OSA_printf(" alg alg_vidEnc \n");
    OSA_printf(" image_tune\n");
    OSA_printf("\n");
    return 0;
  }

  if(strcmp(argv[1], "csl")==0)
    status = CSL_sysTestMain(argc, argv);
  else
  if(strcmp(argv[1], "osa_tsk")==0)
    status = OSA_tskTestMain(argc, argv);
  else
  if(strcmp(argv[1], "osa_ipc")==0)
    status = OSA_ipcTestMain(argc, argv);
  else
  if(strcmp(argv[1], "osa_cmem")==0)
    status = OSA_cmemTestMain(argc, argv);
  else
  if(strcmp(argv[1], "drv")==0)
    status = DRV_drvTestMain(argc, argv);
  else
  if(strcmp(argv[1], "drv_dma")==0)
    status = DRV_dmaTestMain(argc, argv);
  else
  if(strcmp(argv[1], "drv_display")==0)
    status = DRV_displayTestMain(argc, argv);
  else
  if(strcmp(argv[1], "drv_fd")==0)
    status = DRV_faceDetectTestMain(argc, argv);
  else
  if(strcmp(argv[1], "drv_resz")==0)
    status = DRV_reszTestMain(argc, argv);
  else
  if(strcmp(argv[1], "drv_ldc")==0)
    status = DRV_ldcTestMain(argc, argv);
  else
  if(strcmp(argv[1], "drv_imgs")==0)
    status = DRV_imgsTestMain(argc, argv);
  else
  if(strcmp(argv[1], "drv_isif")==0)
    status = DRV_isifTestMain(argc, argv);
  else
  if(strcmp(argv[1], "drv_capture")==0)
    status = DRV_captureTestMain(argc, argv);
  else
  if(strcmp(argv[1], "image_tune")==0)
    status = IMAGE_TUNE_sysTestMain(argc, argv);
  else
  if(strcmp(argv[1], "alg")==0)
    status = ALG_sysTestMain(argc, argv);
  else
  if(strcmp(argv[1], "alg_vidEnc")==0)
    status = ALG_vidEncTestMain(argc, argv);
  else
  if(strcmp(argv[1], "alg_vidDec")==0)
    status = ALG_vidDecTestMain(argc, argv);
  else
  if(strcmp(argv[1], "alg_faceRecog")==0)
    status = ALG_faceRecogTestMain(argc, argv);  /* ANR */

  return status;
}

