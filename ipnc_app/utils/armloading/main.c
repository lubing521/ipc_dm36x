/*
 * ctrl.c
 *
 * ============================================================================
 * Copyright (c) Texas Instruments Inc 2007
 *
 * Use of this software is controlled by the terms and conditions found in the
 * license agreement under which this software has been supplied or provided.
 * ============================================================================
 */

/* Standard Linux headers */
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/* Function error codes */
#define SUCCESS         0
#define FAILURE         -1

/* True of false enumeration */
#define TRUE            1
#define FALSE           0

#ifdef __DEBUG
#define DBG(fmt, args...) fprintf(stderr, "Encode Debug: " fmt, ## args)
#else
#define DBG(fmt, args...)
#endif

#define ERR(fmt, args...) fprintf(stderr, "Encode Error: " fmt, ## args)

#define ARMLOAD_NUM_FRAMERATE		(100)

static int Quit_flag = FALSE;
static void signalHandler(int signum)
{
	fprintf(stderr, "Quitting!!!\n");
    Quit_flag = TRUE;
}

/******************************************************************************
 * getArmCpuLoad
 ******************************************************************************/
int getArmCpuLoad(int *procLoad, int *cpuLoad)
{
    static unsigned long prevIdle     = 0;
    static unsigned long prevTotal    = 0;
    static unsigned long prevProc     = 0;
    int                  cpuLoadFound = FALSE;
    unsigned long        user, nice, sys, idle, total, proc;
    unsigned long        uTime, sTime, cuTime, csTime;
    unsigned long        deltaTotal, deltaIdle, deltaProc;
    char                 textBuf[4];
    FILE                *fptr;

    /* Read the overall system information */
    fptr = fopen("/proc/stat", "r");

    if (fptr == NULL) {
        ERR("/proc/stat not found. Is the /proc filesystem mounted?\n");
        return FAILURE;
    }

    /* Scan the file line by line */
    while (fscanf(fptr, "%4s %lu %lu %lu %lu %*[^\n]", textBuf,
                  &user, &nice, &sys, &idle) != EOF) {
        if (strcmp(textBuf, "cpu") == 0) {
            cpuLoadFound = TRUE;
            break;
        }
    }

    if (fclose(fptr) != 0) {
        return FAILURE;
    }

    if (!cpuLoadFound) {
        return FAILURE;
    }

    /* Read the current process information */
    fptr = fopen("/proc/self/stat", "r");

    if (fptr == NULL) {
        ERR("/proc/self/stat not found. Is the /proc filesystem mounted?\n");
        return FAILURE;
    }

    if (fscanf(fptr, "%*d %*s %*s %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %lu "
                     "%lu %lu %lu", &uTime, &sTime, &cuTime, &csTime) != 4) {
        ERR("Failed to get process load information.\n");
        fclose(fptr);
        return FAILURE;
    }

    if (fclose(fptr) != 0) {
        return FAILURE;
    }

    total = user + nice + sys + idle;
    proc = uTime + sTime + cuTime + csTime;

    /* Check if this is the first time, if so init the prev values */
    if (prevIdle == 0 && prevTotal == 0 && prevProc == 0) {
        prevIdle = idle;
        prevTotal = total;
        prevProc = proc;
        return SUCCESS;
    }

    deltaIdle = idle - prevIdle;
    deltaTotal = total - prevTotal;
    deltaProc = proc - prevProc;

    prevIdle = idle;
    prevTotal = total;
    prevProc = proc;

    *cpuLoad = 100 - deltaIdle * 100 / deltaTotal;
    *procLoad = deltaProc * 100 / deltaTotal;

    return SUCCESS;
}

/******************************************************************************
 * ctrlThrFxn
 ******************************************************************************/
int main(int argc, char **argv)
{
/*  int                    osdTransparency      = MIN_TRANSPARENCY<<1;//OSD_TRANSPARENCY;
    OsdData                osdData              = OSD_DATA;
    int                    uiCreated            = FALSE;
    char                  *osdDisplays[NUM_OSD_BUFS];
    UIParams               uiParams;
	char				  TempBuff[100];*/
	struct sigaction 	   sigAction;
	double total = 0.0;
	int proc=0,cpu=0,max = 0;
	int cnt = 0, i = 0;
	int armLoadCnt = ARMLOAD_NUM_FRAMERATE;
	FILE *fp;
	//int timeCount[301]={0};

    /* insure a clean shutdown if user types ctrl-c */
    sigAction.sa_handler = signalHandler;
    sigemptyset(&sigAction.sa_mask);
    sigAction.sa_flags = 0;
    sigaction(SIGINT, &sigAction, NULL);

	if(argc==1) {
		armLoadCnt = ARMLOAD_NUM_FRAMERATE;
	}
	else {
		armLoadCnt = atoi(argv[1]);
	}

	printf("ARM LOAD RUNNING FOR COUNT: %d \n", armLoadCnt);

	if ((fp = fopen("/mnt/ramdisk/timelog.txt", "wb")) == NULL) {
		printf("Can't create log file\n");
		Quit_flag = TRUE;
	}

    while (!Quit_flag) {
		/*GetIPAddr(TempBuff);
        uiDrawText(TempBuff, osdData.imageWidth-300, osdData.imageHeight-30, osdData.displayIdx);*/
		//uiDrawText("test", osdData.imageWidth-600, osdData.imageHeight-30, osdData.workingIdx);

		/*osdData.displayIdx = (osdData.displayIdx + 1) % NUM_OSD_BUFS;
		osdData.workingIdx = (osdData.workingIdx + 1) % NUM_OSD_BUFS;
		waitForVsync(osdData.osdFd);*/

		getArmCpuLoad(&proc, &cpu);
		if ((cnt > 0) && (cnt <= armLoadCnt)) {
			if (cpu > max)
				max = cpu;
			total += cpu;i ++;
			fprintf(fp,"%4d\t%5d\n",cnt,cpu);
			printf("%4d\t%5d\n",cnt,cpu);
		} else if (cnt > armLoadCnt){
			Quit_flag = TRUE;
		}
		cnt++;
        sleep(1);
    }

	total /= i;
	fprintf(fp,"Average:%4.3f   Max:%d\n",total,max);
	printf("Average:%4.3f   Max:%d\n",total,max);

	fclose(fp);
    return 1;
}
