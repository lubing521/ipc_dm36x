/* ===========================================================================
* @file jpg_mng.c
*
* @path $(IPNCPATH)\sys_server\src\
*
* @desc
* .
* Copyright (c) Appro Photoelectron Inc.  2008
*
* Use of this software is controlled by the terms and conditions found
* in the license agreement under which this software has been supplied
*
* =========================================================================== */
 #define _XOPEN_SOURCE 600
#include <jpg_mng.h>
#include <file_msg_drv.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys_env_type.h>
#include <pthread.h>
#include <unistd.h>
#include <file_dispatcher.h>
#include <Appro_interface.h>
#include <sem_util.h>
#include <system_default.h>
#include <fcntl.h>

#define JPG_INT_STATE	0 ///< JPEG manager initial state.
#define JPG_RUN	1 ///< JPEG running flag.
#define JPG_THR_EXIT (1<<1) ///< JPEG manager exit flag.
#define JPG_DST_MASK (JPG_TO_SD | JPG_TO_SMTP | JPG_TO_FTP) ///< JPEG file destination flag
#define JPEG_COUNT	20 ///< Max JPEG files saved in SD card in one run command.
#define JPEG_CONTINUOUS_MODE (1<<5)
#define JPG_SERIAL_UPDATE (1 << 6)
#define JPG_STOP (1 << 7)
#define JPG_PATH_LEN 40
#define JPG_SERIAL_GAP	10

#ifdef JPG_DEBUG
#define JPG_DBG(fmt, args...)	fprintf(stderr, fmt, ##args)
#else
#define JPG_DBG(fmt, args...)
#endif
#define JPG_ERR(fmt, args...)	fprintf(stderr, fmt, ##args)

extern  int posix_fadvise(int fd, off_t  offset, off_t len, int advice);
extern SemHandl_t hAlarmStatusSem;
extern int	gFileDispCount;
extern const int frame_rate[5];

static pthread_mutex_t gJpgMutex, gJpgConMutex;
static pthread_cond_t gJpgRunCond = PTHREAD_COND_INITIALIZER;
static pthread_t gJpgMngThr;
static int gJpgState = 0;
static SysInfo* pSysinfo;
static int gJpgRunNum = 0, gJpgStartSerial = -1;


/**
* @brief Generate a config file for JPEG dispatch.
*
* @param JpegFiles [I ] Jpeg file names.
* @param pTarFileName [I ] File name of config file.
* @param nJpgCount [I ] How many JPEG files will be dispatched.
* @return 0 on success.
*/
int GenerateJpgConfigFile(char JpegFiles[JPEG_COUNT][JPG_PATH_LEN],
							char *pConfigFileName, int nJpgCount)
{
	int ret, i,fd,result;
	FILE *fp;
	char strFile[JPG_PATH_LEN];
	sprintf(strFile, "%s/%s", TEMP_FOLDER, pConfigFileName);
	if((fp = fopen(strFile, "wt")) != NULL){
		fprintf(fp, "%d\n", nJpgCount);
		for(i = 0; i < nJpgCount; i++)
			fprintf(fp, "%s\n", JpegFiles[i]);
		fclose(fp);
		fd = open(strFile,  O_RDONLY, 0666);
		if(fd == -1) {
			printf("Open file failed.\n");
			return 0;
		}
		fdatasync(fd);
	       result = posix_fadvise(fd, 0,0,POSIX_FADV_DONTNEED);
		close(fd);
		printf("The result of posix_fadvise is %d.\n",result);
		ret = 0;
	} else
		ret = -1;
	return ret;
}
/**
* @brief Get current JPEG manager status.
* This function will get JPEG status and clear flag if needed.
* @param fClrFlg [I ] Bits to clear.
* @return JPEG status.
*/
int GetJpgState(int fClrFlg)
{
	int ret;
	pthread_mutex_lock(&gJpgMutex);
	ret = gJpgState;
	if(fClrFlg){
		JPG_DBG("gJpgState change: 0x%x", gJpgState);
		gJpgState &= (~fClrFlg);
		JPG_DBG("-> 0x%x\n", gJpgState);
	}
	pthread_mutex_unlock(&gJpgMutex);
	return ret;
}
/**
* @brief Generate a JPEG file name
* @param strFileName [O ] JPEG file name.
* @return 0 on success.
*/
int GenerateJpegFileName(char *strFileName)
{
	time_t tCurrentTime;
	struct tm *tmnow;
	struct timeval tv;
	time(&tCurrentTime);
	gettimeofday(&tv,NULL);
	tmnow = localtime(&tCurrentTime);
	sprintf(strFileName, "%04d%02d%02d%02d%02d%02d%03ld.jpg",
		tmnow->tm_year+1900, tmnow->tm_mon+1, tmnow->tm_mday,
		tmnow->tm_hour, tmnow->tm_min, tmnow->tm_sec, tv.tv_usec / 1000);
	return 0;
}
/**
* @brief Generate a tar file name
* @param strFileName [O ] Tar file name.
* @return 0 on success.
*/
int GenerateTarFileName(char *strFileName)
{
	time_t tCurrentTime;
	struct tm *tmnow;
	struct timeval tv;
	time(&tCurrentTime);
	gettimeofday(&tv,NULL);
	tmnow = localtime(&tCurrentTime);
	sprintf(strFileName, "/%04d%02d%02d%02d%02d%02d%03ld.tar",
		tmnow->tm_year+1900, tmnow->tm_mon+1, tmnow->tm_mday,
		tmnow->tm_hour, tmnow->tm_min, tmnow->tm_sec, tv.tv_usec / 1000);
	return 0;
}
/**
* @brief Set JPEG manager start serial number.
*
* @param nSerial [I ] Start serial number in Appro interface.
*/
void SetJpegStartSerial(int nSerial)
{
	pthread_mutex_lock(&gJpgMutex);
	gJpgStartSerial = nSerial;
	gJpgState |= JPG_SERIAL_UPDATE;
	pthread_mutex_unlock(&gJpgMutex);
}
/**
* @brief Get JPEG manager start serial number.
*
* @return JPEG start serial number.
*/
int GetJpegStartSerial()
{
	int ret;
	pthread_mutex_lock(&gJpgMutex);
	ret = gJpgStartSerial;
	gJpgState &= (~JPG_SERIAL_UPDATE);
	pthread_mutex_unlock(&gJpgMutex);
	return ret;
}
/**
* @brief Save specific JPEG file to JPG_PATH.
*
* @param iSerial [I ] Serial number in Appro interface.
* @param pFilePath [O ] File name.
* @return Error code from Appro interface.
* @retval -1028 File create fail.
*/
int SaveJpegBySerial(int iSerial, char *pFilePath)
{
	int ret = 0;
	AV_DATA tAVData;
	int fd;
	char strJpegFile[JPG_PATH_LEN];
	if((ret = GetAVData(AV_OP_LOCK_MJPEG, iSerial, &tAVData)) != RET_SUCCESS)
		return ret;
	GenerateJpegFileName(pFilePath);
	sprintf(strJpegFile, "%s/%s", TEMP_FOLDER, pFilePath);
	 fd = open(strJpegFile, O_CREAT | O_TRUNC | O_WRONLY, 0666);
	if(fd){
		if(write(fd, tAVData.ptr, tAVData.size) != tAVData.size)
			JPG_ERR("File write fail\n");
		fsync(fd);
             posix_fadvise(fd, 0,0, POSIX_FADV_DONTNEED  );
		close(fd);
	} else {
		JPG_ERR("File open fail\n");
		ret = -1028;
	}
	GetAVData(AV_OP_UNLOCK_MJPEG, iSerial, &tAVData);
	return ret;
}
/**
* @brief Save some JPEG files.
*
* @retval 0 Success.
* @param nDstFlg [I ] Where JPEG will save.
* @retval -1 Fail.
*/
int SaveJpeg(int nDstFlg)
{
	int ret = 0, i, iSerial, nCount, nState;
	int j, nSleepCount;
	unsigned int nTime, nDiffTime;
	char strFileName[80];
	char JpegFiles[JPEG_COUNT][JPG_PATH_LEN];
	AV_DATA tAVData;
	JPG_DBG("Enter %s\n", __func__);
	if((ret = GetAVData(AV_OP_GET_MJPEG_SERIAL, -1, &tAVData)) != 0){
		JPG_ERR("Can't get JPEG image\n");
		return ret;
	}
	/* If JpegStartSerial < 0 or JpegStartSerial is very old. */
	if((iSerial = GetJpegStartSerial()) < 0 || tAVData.serial - iSerial > 40){
		iSerial = tAVData.serial;
	} else {
		GetAVData(AV_OP_GET_MJPEG_SERIAL, iSerial, &tAVData);
	}
	nTime = tAVData.timestamp;
	nCount = pSysinfo->smtp_config.attachments;
	JPG_DBG("Count:%d\n", nCount);
	nSleepCount = 200 / nCount;
	nDiffTime = 10000 / nCount;
	for(i = 0;i < nCount;){
		JPG_DBG("Serial = %d\n", iSerial);
		if(SaveJpegBySerial(iSerial, JpegFiles[i]) == 0){
			i++;
		} else
			JPG_DBG("Not save\n");
		nState = GetJpgState(JPG_STOP);
		if(nState & JPG_STOP){
			JPG_DBG("JPG_STOP flag is set\n");
			break;
		}
		/* Get new JPEG */
		if(nState & JPG_SERIAL_UPDATE){
			iSerial = GetJpegStartSerial();
			GetAVData(AV_OP_GET_MJPEG_SERIAL, iSerial, &tAVData);
			nTime = tAVData.timestamp;
		} else
			nTime += nDiffTime;
		j = 0;
		while(nTime > tAVData.timestamp && j++ < nSleepCount){
			usleep(100000);
			GetAVData(AV_OP_GET_MJPEG_SERIAL, -1, &tAVData);
		}
		if(j > nSleepCount){
			JPG_DBG("Time out!!!\n");
			ret = -1;
			break;
		}
		iSerial = tAVData.serial;
	}
	if(i > 0){
		strcpy(strFileName, JpegFiles[0]);
		strcpy(strFileName + strlen(strFileName) - 3, "cfg");
		GenerateJpgConfigFile(JpegFiles, strFileName, i);
		DoFileDispatch(strFileName, nDstFlg);
	}
	/* Update JPEG start serial */
	pthread_mutex_lock(&gJpgMutex);
	if(!(gJpgState & JPG_SERIAL_UPDATE))
		gJpgStartSerial = iSerial;
	pthread_mutex_unlock(&gJpgMutex);
	return ret;
}
/**
* @brief JPEG manager thread
* @param arg [I ] Not used.
* @return 0 on normal end.
*/
void *JpgMngThr(void *arg)
{
	int nState, nSetFlg;
	unsigned short nFlg;
	while(!(GetJpgState(0) & JPG_THR_EXIT)){
		JPG_DBG("Ready\n");
		while(!((nState = GetJpgState(JPG_RUN | JPG_DST_MASK | JPG_STOP)) & (JPG_RUN | JPG_THR_EXIT))){
			if(SemWait(hAlarmStatusSem) == 0){
				nFlg = pSysinfo->lan_config.alarmstatus & (~FLG_UI_JPG);
				if(!(nFlg & (FLG_UI_AVI | FLG_UI_JPG)))
					nFlg &= (~FLG_UI_RECORD);
				fSetAlarmStatus(nFlg);
				SemRelease(hAlarmStatusSem);
			}
			JPG_DBG("JpgMngThr sleep\n");
			pthread_cond_wait(&gJpgRunCond, &gJpgConMutex);
			JPG_DBG("JpgMngThr Wake up\n");
		}
		if(JPG_THR_EXIT & nState)
			break;
		nSetFlg = 0;
		if(!pSysinfo->lan_config.Supportstream1){
			JPG_DBG("Jpeg stream not support\n");
			continue;
		}
		if(SemWait(hAlarmStatusSem) == 0){
			nFlg = pSysinfo->lan_config.alarmstatus | FLG_UI_JPG |	FLG_UI_RECORD;
			fSetAlarmStatus(nFlg);
			SemRelease(hAlarmStatusSem);
		}
		JPG_DBG("Jpeg Run\n");
		if((nState & JPG_DST_MASK) == 0){
			JPG_ERR("Unknown jpeg file destination\n");
			continue;
		}
#if 0
		if(nState & JPG_TO_SMTP)
			DoJpegDispatch(NULL, JPG_DISPATCH_TO_SMTP);
		if(nState & JPG_TO_FTP)
			DoJpegDispatch(NULL, JPG_DISPATCH_TO_FTP);
		if(nState & JPG_TO_SD)
			SaveJpegToSD();
		else
			sleep(5);
#else
		if(nState & JPG_TO_SMTP)
			nSetFlg |= FILE_DISPATCH_TO_SMTP;
		if(nState & JPG_TO_FTP)
			nSetFlg |= FILE_DISPATCH_TO_FTP;
		if(nState & JPG_TO_SD)
			nSetFlg |= FILE_DISPATCH_TO_SD;
		if(gFileDispCount < FILE_DISP_THR_LIMIT){
			JPG_DBG("nSetFlg is 0x%x\n", nSetFlg);
			SaveJpeg(nSetFlg);
		} else
			printf("Jpeg save limit \n");
#endif
	}
	JPG_DBG("Thread exit\n");
	return (void *)0;
}
/**
* @brief Initialize JPEG manager.
* @retval 0 Success.
* @retval -1 Fail.
*/
int JpgMngInit()
{
	if((pSysinfo = GetSysInfo()) == NULL){
		JPG_ERR("Can't get system info!!\n");
		return -1;
	}
	if( pthread_mutex_init(&gJpgMutex, NULL) != 0 ){
		pSysinfo = NULL;
		JPG_ERR("Mutex create faill!!\n");
		return -1;
	}
	if( pthread_mutex_init(&gJpgConMutex, NULL) != 0 ){
		pSysinfo = NULL;
		pthread_mutex_destroy(&gJpgMutex);
		JPG_ERR("Mutex create faill!!\n");
		return -1;
	}
	if(pthread_create(&gJpgMngThr, NULL, JpgMngThr, NULL)){
		pSysinfo = NULL;
		pthread_mutex_destroy(&gJpgMutex);
		pthread_mutex_destroy(&gJpgConMutex);
		JPG_ERR("JpgMngThr create faill!!\n");
		return -1;
	}
	gJpgState = JPG_INT_STATE;
	return 0;
}
/**
* @brief JPEG manager exit.
* @return 0 on success.
*/
int JpgMngExit()
{
	int ret;
	pSysinfo = NULL;
	pthread_mutex_lock(&gJpgMutex);
	gJpgState |= JPG_THR_EXIT;
	pthread_mutex_unlock(&gJpgMutex);
	pthread_cond_signal(&gJpgRunCond);
	pthread_join(gJpgMngThr, (void **)&ret);
	pthread_detach(gJpgMngThr);
	ret |= pthread_mutex_destroy(&gJpgConMutex);
	return ret |= pthread_mutex_destroy(&gJpgMutex);
}
/**
* @brief Send command to JPEG manager itself thread.

* This thread will be generated in JPEG manager continuous mode, which will
* send JpgRun() command to JPEG manager every 1 second.
* @param arg [I ] Pointer to interger, which have value of JPEG manager destination.
* @return 0 on normal end.
*/
void *JpgSelfCmdThr(void *arg)
{
	int *pDstFlg = (int *)arg;
	void *ret = (void*) 0;
	if(pDstFlg == NULL){
		JPG_ERR("Thread %s should not be created with NULL parameter\n",
			__func__);
		ret = (void *) -1;
	} else {
		while (1){
			sleep(1);
			if(GetJpgState(0) & JPEG_CONTINUOUS_MODE)
				JpgRun(*pDstFlg, 0);
			else
				break;
		}
		free(pDstFlg);
	}
	return ret;
}
/**
* @brief Ask JPEG manager to run.
* @param nDstFlg [I ] JPEG file destination flag. (Can be JPG_TO_SD | JPG_TO_SMTP | JPG_TO_FTP)
* @param isJpgContinue [I ] If running JPEG continuous mode.
* @return 0 on success.
*/
int JpgRun(int nDstFlg, int isJpgContinue)
{
	int ret = 0, nJpgRunNum;
	JPG_DBG("Enter %s\n", __func__);
	if(isJpgContinue){
		pthread_mutex_lock(&gJpgMutex);
		nJpgRunNum = gJpgRunNum++;
		pthread_mutex_unlock(&gJpgMutex);
		if(nJpgRunNum == 0){
			int *pDstFlg = malloc(sizeof(int));
			if(pDstFlg == NULL){
				JPG_ERR("Not enough memory to run JPEG continuous mode\n");
				ret = -1;
			} else {
				pthread_t pthread;
				*pDstFlg = nDstFlg;
				if(pthread_create(&pthread, NULL, JpgSelfCmdThr, pDstFlg)){
					JPG_ERR("JpgSelfCmdThr create failed\n");
					free(pDstFlg);
					ret = -1;
				} else {
					pthread_mutex_lock(&gJpgMutex);
					gJpgState &= (~JPG_STOP);
					gJpgState |= (JPG_RUN | JPEG_CONTINUOUS_MODE |
						(nDstFlg & JPG_DST_MASK));
					pthread_mutex_unlock(&gJpgMutex);
					ret = pthread_cond_signal(&gJpgRunCond);
					pthread_detach(pthread);
				}
			}
		}
	} else {
		pthread_mutex_lock(&gJpgMutex);
		gJpgState |= (JPG_RUN | (nDstFlg & JPG_DST_MASK));
		pthread_mutex_unlock(&gJpgMutex);
		ret = pthread_cond_signal(&gJpgRunCond);
	}
	return ret;
}
/**
* @brief Ask JPEG manager to stop.
* @param isJpgContinue [I ] If running JPEG continuous mode.
* @return 0 on success.
*/
int JpgStop(int isJpgContinue)
{
	int ret = 0, nJpgRunNum;
	JPG_DBG("Enter %s mode:%d\n", __func__, isJpgContinue);
	if(isJpgContinue){
		pthread_mutex_lock(&gJpgMutex);
		nJpgRunNum = --gJpgRunNum;
		pthread_mutex_unlock(&gJpgMutex);
		JPG_DBG("nJpgRunNum = %d\n",nJpgRunNum);
		if(nJpgRunNum == 0){
			pthread_mutex_lock(&gJpgMutex);
			gJpgState |= JPG_STOP;
			gJpgState &= (~JPEG_CONTINUOUS_MODE);
			pthread_mutex_unlock(&gJpgMutex);
		} else if(nJpgRunNum < 0){
			JPG_DBG("JpgStop is called more than JpgRun is\n");
			/* Restore to default */
			pthread_mutex_lock(&gJpgMutex);
			gJpgRunNum = 0;
			pthread_mutex_unlock(&gJpgMutex);
			/* Still be an error */
			ret = -1;
		}
	} else {
		pthread_mutex_lock(&gJpgMutex);
		gJpgState |= JPG_STOP;
		pthread_mutex_unlock(&gJpgMutex);
	}
	return ret;
}

