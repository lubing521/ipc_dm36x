
#include <avserver_ui.h>

//#define DEBUG_OPTIONS

char gUI_mainMenu[] = {

  " ==============\n"
  " AV Server Main\n"
  " ==============\n"
  " \n"
  " 1: Toggle ON/OFF: Face Detect\n"
  " 2: Toggle ON/OFF: LDC\n"
  " 3: Toggle ON/OFF: Video Stabilization\n"
  " 4: Toggle ON/OFF: Spatial  NF\n"
  " 5: Toggle ON/OFF: Temporal NF\n"
  " 6: Toggle ON/OFF: Save to file\n"
  " 7: Toggle ON/OFF: AE/AWB\n"
  " \n"
  " r: Save RAW data to file\n"
  " y: Save YUV data to file\n"
  " p: Print Profile Info\n"
  " \n"
  " 0: Stop and Exit\n"
  " \n"
};

int UI_menuRun()
{
  Bool done = FALSE;
  char ch;

  while(!done) {

    OSA_printf( " \n");
    OSA_printf( " Current Settings,\n");

    OSA_printf( " Face Detect                     : ");
    if(gAVSERVER_UI_ctrl.fdEnable) {
      OSA_printf("ON");
    } else {
      OSA_printf("OFF");
    }
    OSA_printf( "\n");

    OSA_printf( " Lens Distortion Correction (LDC): ");
    if(gAVSERVER_UI_ctrl.ldcEnable) {
      OSA_printf("ON");
    } else {
      OSA_printf("OFF");
    }
    OSA_printf( "\n");

    OSA_printf( " Video Stabilization             : ");
    if(gAVSERVER_UI_ctrl.vsEnable) {
      OSA_printf("ON");
    } else {
      OSA_printf("OFF");
    }
    OSA_printf( "\n");

    OSA_printf( " Spatial NF                      : ");
    if(gAVSERVER_UI_ctrl.snfEnable) {
      OSA_printf("ON");
    } else {
      OSA_printf("OFF");
    }
    OSA_printf( "\n");

    OSA_printf( " Temporal NF                     : ");
    if(gAVSERVER_UI_ctrl.tnfEnable) {
      OSA_printf("ON");
    } else {
      OSA_printf("OFF");
    }
    OSA_printf( "\n");

    OSA_printf( " File Save                       : ");
    if(gAVSERVER_UI_ctrl.fileSaveEnable) {
      OSA_printf("ON");
    } else {
      OSA_printf("OFF");
    }
    OSA_printf( "\n");

    OSA_printf( " AE/AWB                          : ");
    if(gAVSERVER_UI_ctrl.aewbEnable) {
      OSA_printf("ON");
    } else {
      OSA_printf("OFF");
    }
    OSA_printf( "\n");

    OSA_printf( " Host-name / IP address          : %s\n", gAVSERVER_UI_ctrl.hostName);


    OSA_printf( "\n");

    OSA_printf( gUI_mainMenu);

    do {

      OSA_printf(" Enter Choice : ");

      ch = getchar();

      OSA_printf( " \n");

    } while(ch=='\n');

    switch(ch) {

      case '1':

        gAVSERVER_UI_ctrl.fdEnable = !gAVSERVER_UI_ctrl.fdEnable;

        AVSERVER_faceDetectEnable(gAVSERVER_UI_ctrl.fdEnable);

        break;

      case '2':

        if(gAVSERVER_UI_config.ldcEnable) {

          gAVSERVER_UI_ctrl.ldcEnable = !gAVSERVER_UI_ctrl.ldcEnable;

          AVSERVER_ldcEnable(0, gAVSERVER_UI_ctrl.ldcEnable);
          AVSERVER_ldcEnable(1, gAVSERVER_UI_ctrl.ldcEnable);
          AVSERVER_ldcEnable(2, gAVSERVER_UI_ctrl.ldcEnable);

        } else {
          OSA_printf(" AVSERVER: LDC is NOT ENABLED while starting AV SERVER !!!\n");
        }

        break;

      case '3':

        if(gAVSERVER_UI_config.vsEnable) {
          gAVSERVER_UI_ctrl.vsEnable = !gAVSERVER_UI_ctrl.vsEnable;

          AVSERVER_vsEnable(0, gAVSERVER_UI_ctrl.vsEnable);
          AVSERVER_vsEnable(1, gAVSERVER_UI_ctrl.vsEnable);
          AVSERVER_vsEnable(2, gAVSERVER_UI_ctrl.vsEnable);
        } else {
          OSA_printf(" AVSERVER: VS is NOT ENABLED while starting AV SERVER !!!\n");
        }
        break;

      case '4':

        if(gAVSERVER_UI_config.snfEnable) {
          gAVSERVER_UI_ctrl.snfEnable = !gAVSERVER_UI_ctrl.snfEnable;

          AVSERVER_snfEnable(0, gAVSERVER_UI_ctrl.snfEnable);
        } else {
          OSA_printf(" AVSERVER: SNF is NOT ENABLED while starting AV SERVER !!!\n");
        }
        break;

      case '5':

        if(gAVSERVER_UI_config.tnfEnable) {
          gAVSERVER_UI_ctrl.tnfEnable = !gAVSERVER_UI_ctrl.tnfEnable;

          AVSERVER_tnfEnable(0, gAVSERVER_UI_ctrl.tnfEnable);
        } else {
          OSA_printf(" AVSERVER: TNF is NOT ENABLED while starting AV SERVER !!!\n");
        }
        break;

      case '6':

        gAVSERVER_UI_ctrl.fileSaveEnable = !gAVSERVER_UI_ctrl.fileSaveEnable;

        AVSERVER_fileSaveEnable(0, gAVSERVER_UI_ctrl.fileSaveEnable);
        AVSERVER_fileSaveEnable(1, gAVSERVER_UI_ctrl.fileSaveEnable);

        break;

      case '7':
        gAVSERVER_UI_ctrl.aewbEnable = !gAVSERVER_UI_ctrl.aewbEnable;
        AVSERVER_aewbEnable(gAVSERVER_UI_ctrl.aewbEnable);
        break;

      case 'r':

        DRV_imageTuneSaveDataToFile(DRV_DATA_FORMAT_RAW, gAVSERVER_UI_ctrl.fileSaveIndex);
        gAVSERVER_UI_ctrl.fileSaveIndex++;
        break;

      case 'y':

        DRV_imageTuneSaveDataToFile(DRV_DATA_FORMAT_YUV420, gAVSERVER_UI_ctrl.fileSaveIndex);
        gAVSERVER_UI_ctrl.fileSaveIndex++;
        break;

      case 'p':
        AVSERVER_profileInfoShow();
        break;

      case '0':
        done = TRUE;
        break;
    }
  }

  AVSERVER_profileInfoShow();

  return OSA_SOK;
}
