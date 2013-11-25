/* ===========================================================================
* @file avi_mng.c
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
#include <avi_mng.h>
#include <file_msg_drv.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys_env_type.h>
#include <pthread.h>
#include <unistd.h>
#include <file_dispatcher.h>
#include <sem_util.h>
#include <system_default.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/dir.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/statfs.h>
#include <strings.h>
#include <dirent.h>
#include <av_server_ctrl.h>

#define AVI_INT_STATE	0
#define AVI_RUN	1
#define AVI_THR_EXIT (1<<1)
#define AVI_NO_STOP (1<<2)
#define AVI_DST_MASK (AVI_TO_SD | AVI_TO_SMTP | AVI_TO_FTP)
#define AVI_EXT_SEC 10
#define AVI_RUN_AGAIN	0x6300

#ifdef AVI_DEBUG
#define AVI_DBG(fmt, args...)	fprintf(stderr, fmt, ##args)
#else
#define AVI_DBG(fmt, args...)
#endif
#define AVI_ERR(fmt, args...)	fprintf(stderr, fmt, ##args)

extern volatile SemHandl_t hAlarmStatusSem;
extern int gFileDispCount;
extern int GetfreememInfo( void );

/* extern from av_server_ctrl.c */
extern const int audio_bitrate_aac[2][3];
extern const int audio_bitrate_g711[2][1];

static pthread_mutex_t gAviMutex, gAviConMutex;
static pthread_cond_t gAviRunCond = PTHREAD_COND_INITIALIZER;
static pthread_t gAviMngThr;
static int gAviState = 0;
static SysInfo* pSysinfo;
static volatile char pAviRun = 0;
char gSdFileName[30] = "";
SemHandl_t hSdFileSem = NULL;
/**
* @brief Number of calling AviRun().

* This number only be used in no stop mode.
*/
static int gAviRunNum = 0;
/**
* @brief Get current AVI manager status.
* This function will get AVI status and clear flag if needed.
* @param fClrFlg [I ] Bits to clear.
* @return AVI status.
*/
int GetAviState(int fClrFlg)
{
	int ret;
	pthread_mutex_lock(&gAviMutex);
	ret = gAviState;
	if(fClrFlg)
		gAviState &= (~fClrFlg);
	pthread_mutex_unlock(&gAviMutex);
	return ret;
}
/**
* @brief Save an AVI file.
* @param sAviName [I ] AVI file name(with path).
* @param nInterval [I ] How long a file is.
* @param nFrameRate [I ] AVI frame rate.
* @param nAudioCodec [I ] Select audio codec(0: no audio. 1: u-law. 2. AAC.).
* @param nMsgId [I ] Message ID to communicate with encode stream.
* @param bIsChkCard[I ] Tell Appro_avi_save to check card left space or not.
* @param nStreamId[I ] Which stream to save. 0: stream2. 1: stream3.
* @param nAudioBitrate[I] Audio bitrate which AV server used.
* @return 0 on success. AVI_RUN_AGAIN means File size is reached 1GB.
			Otherwise meets fail.
*/
static int Appro_avi_save(char *sAviName, int nInterval, int nFrameRate,
						int nAudioCodec, int nMsgId, int bIsChkCard,
						int nStreamId, int nAudioBitrate)
{
	char cmd[80];
	int ret;
	struct timeval first,  second,  lapsed;
	sprintf(cmd, "./Appro_avi_save %s %d %d %d %d %d %d %d\n",
			sAviName, nInterval, nMsgId, nFrameRate, nAudioCodec, bIsChkCard,
			nStreamId, nAudioBitrate);
	AVI_DBG("%s\n",cmd);
	gettimeofday (&first, NULL);
	ret = system(cmd);
	gettimeofday (&second, NULL);
	if (first.tv_usec > second.tv_usec) {
		second.tv_usec += 1000000;
		second.tv_sec--;
	}
	lapsed.tv_usec = second.tv_usec - first.tv_usec;
	lapsed.tv_sec = second.tv_sec - first.tv_sec;
	AVI_DBG("lapsed time %d.%d (second)\n", lapsed.tv_sec, lapsed.tv_usec);
	if(ret != AVI_RUN_AGAIN){
		if(ret){
			/* remove error file*/
			sprintf(cmd, "rm -f %s\n", sAviName);
			AVI_DBG("%s\n",cmd);
			system(cmd);
		} else if(lapsed.tv_sec < 3){
			/* remove small file */
			sprintf(cmd, "rm -f %s\n", sAviName);
			AVI_DBG("%s\n",cmd);
			system(cmd);
			ret = -1;
		}
	}
	AVI_DBG("Return %d\n", ret);
	return ret;
}
/**
* @brief Generate a MPEG4 file name
* @param strFileName [O ] MPEG4 file name.
* @return Length of file name(without path).
*/
int GenerateMpeg4FileName(char *strFileName)
{
	time_t tCurrentTime;
	struct tm *tmnow;
	time(&tCurrentTime);
	tmnow = localtime(&tCurrentTime);
	return sprintf(strFileName, "/%04d%02d%02d%02d%02d%02d.avi",
		tmnow->tm_year+1900, tmnow->tm_mon+1, tmnow->tm_mday, tmnow->tm_hour,
		tmnow->tm_min, tmnow->tm_sec);
}
/**
* @brief Generate AVI config file for file dispatch.
*
* @param pAviFile [I ] AVI file name.
* @param pConfigFileName [O ] Config file name.
* @return 0 on success.
*/
int GenerateAviConfigFile(char *pAviFile, char *pConfigFileName)
{
	int ret, len;
	FILE *fp;
	len = strlen(pAviFile);
	strcpy(pConfigFileName, pAviFile);
	strcpy(pConfigFileName + len - 3, "cfg");
	if((fp = fopen(pConfigFileName, "wt")) == NULL)
		ret = -1;
	else {
		fprintf(fp, "%d\n", 1); // no fo files to attach
		fprintf(fp, "%s\n", pAviFile + strlen(AVI_PATH) + 1);
		fclose(fp);
		ret = 0;
	}
	strcpy(pConfigFileName, pConfigFileName + strlen(AVI_PATH) + 1);
	return ret;
}

long long GetDiskfreeSpace(char *pDisk)
{
	long long freespace = 0;
	struct statfs disk_statfs;

	if( statfs(pDisk, &disk_statfs) >= 0 )
	{
		freespace = (((long long)disk_statfs.f_bsize  * (long long)disk_statfs.f_bfree)/(long long)1024);
	}
	/**
	fprintf(stderr,"GetDiskfreeSpace %lli \n",freespace);
	fprintf(stderr,"f_type: 0x%X\n", disk_statfs.f_type);
	fprintf(stderr,"f_bsize: %d\n", disk_statfs.f_bsize);
	fprintf(stderr,"f_blocks: %li\n", disk_statfs.f_blocks);
	fprintf(stderr,"f_bfree: %li\n", disk_statfs.f_bfree);
	fprintf(stderr,"f_bavail: %li\n", disk_statfs.f_bavail);
	fprintf(stderr,"f_files: %li\n", disk_statfs.f_files);
	fprintf(stderr,"f_ffree: %li\n", disk_statfs.f_ffree);
	*/
	return freespace;
}

/**
* @brief Create an AVI file and call file dispatcher.
* @param nDstFlg [I ] Where AVI will save.
* @return 0 on success.
*/
int AviSave(int nDstFlg)
{
	char strFilename[80], strConfigFile[80];
	int nFrameRate,AVI_INTERVAL, nSetFlg = 0;
	int AVIType, i, j, nAudioCodec, nAudioBitrate;
	SysInfo* pSysInfo = GetSysInfo();
	if(pSysInfo == NULL)
		return -1;
	AVI_DBG("%s\n", __func__);
	if (GetfreememInfo() < 0)
			return 0;

	GenerateMpeg4FileName(strFilename + sprintf(strFilename, "%s", AVI_PATH));

	switch (pSysInfo->lan_config.aviduration){
		case 0:
			AVI_INTERVAL=5;
			break;
		case 1:
			AVI_INTERVAL=10;
			break;
		default:
			AVI_INTERVAL=5;
			break;
	}

	AVIType=0;
	i = pSysInfo->lan_config.nVideocodecmode;
	j = pSysInfo->lan_config.nVideocodecres;

	if (pSysInfo->lan_config.aviformat == 0) {
		if(pSysInfo->lan_config.Supportstream5) {
		/* supportH264 */
			AVIType=3;
		}else if (pSysInfo->lan_config.Supportstream2) {
		/* supportMpeg4 */
			AVIType=0;
		}
		if(pSysInfo->lan_config.nDayNight == 0)
			nFrameRate = pSysInfo->lan_config.chipConfig ?
				enc_framerate_night_1_dm368[i][j][pSysInfo->lan_config.nFrameRate1]/1000:enc_framerate_night_1_dm365[i][j][pSysInfo->lan_config.nFrameRate1]/1000;
		else
			nFrameRate = pSysInfo->lan_config.chipConfig ?
				enc_framerate_day_1_dm368[i][j][pSysInfo->lan_config.nFrameRate1]/1000:enc_framerate_day_1_dm365[i][j][pSysInfo->lan_config.nFrameRate1]/1000;
	}else {
		if(pSysInfo->lan_config.Supportstream6) {
		/* supportH264cif */
			AVIType=4;
		}else if(pSysInfo->lan_config.Supportstream3) {
		/* supportMpeg4cif */
			AVIType=1;
		}
		if(pSysInfo->lan_config.nVideocodecmode>7) {
			if(pSysInfo->lan_config.nDayNight == 0)
				nFrameRate = pSysInfo->lan_config.chipConfig ?
					enc_framerate_night_3_dm368[i][j][pSysInfo->lan_config.nFrameRate3]/1000:enc_framerate_night_3_dm365[i][j][pSysInfo->lan_config.nFrameRate3]/1000;
			else
				nFrameRate = pSysInfo->lan_config.chipConfig ?
					enc_framerate_day_3_dm368[i][j][pSysInfo->lan_config.nFrameRate3]/1000:enc_framerate_day_3_dm365[i][j][pSysInfo->lan_config.nFrameRate3]/1000;
		}
		else {
			if(pSysInfo->lan_config.nDayNight == 0)
				nFrameRate = pSysInfo->lan_config.chipConfig ?
					enc_framerate_night_2_dm368[i][j][pSysInfo->lan_config.nFrameRate2]/1000:enc_framerate_night_2_dm365[i][j][pSysInfo->lan_config.nFrameRate2]/1000;
			else
				nFrameRate = pSysInfo->lan_config.chipConfig ?
					enc_framerate_day_2_dm368[i][j][pSysInfo->lan_config.nFrameRate2]/1000:enc_framerate_day_2_dm365[i][j][pSysInfo->lan_config.nFrameRate2]/1000;
		}
	}
	/* Audio config */
	if(pSysInfo->audio_config.audioenable == 0){
		nAudioCodec = 0;
		nAudioBitrate = 0;
	}else if(pSysInfo->audio_config.codectype == 0){
		nAudioCodec = 1;
		nAudioBitrate = audio_bitrate_g711[pSysInfo->audio_config.samplerate][pSysInfo->audio_config.bitrate];
	}else{
		nAudioCodec = 2;
		nAudioBitrate = audio_bitrate_aac[pSysInfo->audio_config.samplerate][pSysInfo->audio_config.bitrate];
	}

	if(GetAviState(0) & AVI_NO_STOP){
		/* No stop case */
		/* Because we can't send a non stop AVI to anywhere besides
		SD card, we save the AVI file to SD card directly. */
		SemWait(hSdFileSem);
		strcpy(gSdFileName, strFilename + strlen(AVI_PATH) + 1);
		SemRelease(hSdFileSem);
		sprintf(strFilename + sprintf(strFilename, "%s", SD_PATH), "/%s", gSdFileName);
		while(Appro_avi_save(strFilename, 999999999, nFrameRate,
						nAudioCodec, AVI_MSG_ID, 1,
						AVIType, nAudioBitrate) == AVI_RUN_AGAIN){
			/* AVI file is bigger then 1GB, and there is space left on SD card.
			We should create a new file name to save another AVI file.
			*/
			GenerateMpeg4FileName(strFilename + sprintf(strFilename, SD_PATH));
			SemWait(hSdFileSem);
			strcpy(gSdFileName, strFilename + strlen(SD_PATH) + 1);
			SemRelease(hSdFileSem);
		}
		SemWait(hSdFileSem);
		gSdFileName[0] = '\0';
		SemRelease(hSdFileSem);
	} else {
		if(gFileDispCount < FILE_DISP_THR_LIMIT){
			/* The AVI file shoud not bigger than 1GB in this case */
			if(0 == Appro_avi_save(strFilename, AVI_INTERVAL, nFrameRate,
									nAudioCodec, AVI_MSG_ID,
									0, AVIType, nAudioBitrate)){
				GenerateAviConfigFile(strFilename, strConfigFile);
				if(nDstFlg & AVI_TO_SD)
					nSetFlg |= FILE_DISPATCH_TO_SD;
				if(nDstFlg & AVI_TO_SMTP)
					nSetFlg |= FILE_DISPATCH_TO_SMTP;
				if(nDstFlg & AVI_TO_FTP)
					nSetFlg |= FILE_DISPATCH_TO_FTP;
				DoFileDispatch(strConfigFile, nSetFlg);
			}
		}else{
			AVI_ERR("Avi File dispatcher threads over limit %d\n", FILE_DISP_THR_LIMIT);
		}
	}
	return 0;
}
/**
* @brief AVI manager thread
* @param arg [I ] Not used.
* @return 0 on normal end.
*/
void *AviMngThr(void *arg)
{
	unsigned short nFlg;
	int nState;
	while(!(GetAviState(0) & AVI_THR_EXIT)){
		AVI_DBG("Ready\n");
		while(!((nState = GetAviState(AVI_RUN | AVI_DST_MASK)) & (AVI_RUN | AVI_THR_EXIT))){
			/* Update alarm status to AVI stop in UI */
			if(SemWait(hAlarmStatusSem) == 0){
				nFlg = pSysinfo->lan_config.alarmstatus & (~FLG_UI_AVI);
				if(!(nFlg & (FLG_UI_AVI | FLG_UI_JPG)))
					nFlg &= (~FLG_UI_RECORD);
				fSetAlarmStatus(nFlg);
				SemRelease(hAlarmStatusSem);
			}
			AVI_DBG("Go to sleep\n");
			pthread_cond_wait(&gAviRunCond, &gAviConMutex);
			AVI_DBG("I Wake up\n");
		}
		if(AVI_THR_EXIT & nState)
			break;
		/* Update alarm status to AVI start in UI */
		if(SemWait(hAlarmStatusSem) == 0){
			nFlg = pSysinfo->lan_config.alarmstatus | FLG_UI_AVI |	FLG_UI_RECORD;
			fSetAlarmStatus(nFlg);
			SemRelease(hAlarmStatusSem);
		}
		AVI_DBG("Run\n");
		AviSave(nState & AVI_DST_MASK);
	}
	AVI_DBG("Thread exit\n");
	return (void *)0;
}
/**
* @brief Initialize AVI manager.
* @retval 0 Success
* @retval -1 Fail
*/
int AviMngInit()
{
	if((pSysinfo = GetSysInfo()) == NULL){
		AVI_ERR("Can't get system info!!\n");
		return -1;
	}
	if( pthread_mutex_init(&gAviMutex, NULL) != 0 ){
		pSysinfo = NULL;
		AVI_ERR("Mutex create faill!!\n");
		return -1;
	}
	if( pthread_mutex_init(&gAviConMutex, NULL) != 0 ){
		pSysinfo = NULL;
		pthread_mutex_destroy(&gAviMutex);
		AVI_ERR("Mutex create faill!!\n");
		return -1;
	}
	if(hSdFileSem == NULL)
		hSdFileSem = MakeSem();
	if(hSdFileSem == NULL){
		pSysinfo = NULL;
		pthread_mutex_destroy(&gAviMutex);
		pthread_mutex_destroy(&gAviConMutex);
		AVI_ERR("hSdFileSem create fail!!\n");
		return -1;
	}
	if(pthread_create(&gAviMngThr, NULL, AviMngThr, NULL)){
		pSysinfo = NULL;
		pthread_mutex_destroy(&gAviMutex);
		pthread_mutex_destroy(&gAviConMutex);
		DestroySem(hSdFileSem);
		hSdFileSem = NULL;
		AVI_ERR("AviMngThr create faill!!\n");
		return -1;
	}
	gAviState = AVI_INT_STATE;
	return 0;
}
/**
* @brief Make AVI manager leave.
* @retval 0 Success
* @retval -1 Fail
*/
int AviMngExit()
{
	int ret = 0;
	AVI_DBG("Enter %s\n", __func__);
	pSysinfo = NULL;
	pthread_mutex_lock(&gAviMutex);
	gAviState |= AVI_THR_EXIT;
	pthread_mutex_unlock(&gAviMutex);
	pthread_cond_signal(&gAviRunCond);
	AVI_DBG("Wait AVI manager done\n");
	pthread_join(gAviMngThr, (void **)&ret);
	pthread_detach(gAviMngThr);
	ret |= pthread_mutex_destroy(&gAviConMutex);
	ret |= pthread_mutex_destroy(&gAviMutex);
	if(DestroySem(hSdFileSem))
		ret |= -1;
	else
		hSdFileSem = NULL;
	AVI_DBG("Leave %s\n", __func__);
	return ret;
}
/**
* @brief Tell AVI manager to start record a AVI.

* This function will make AVI manager to start recording if there is no AVI
* recording. If there is one AVI recording, AVI manager will start another AVI
* recording when it have done current recording.
* @param isNoStop [I] Run no stop mode or not.
* @param nDstFlg [I ] AVI file destination flag.
* @retval 0 Command sent successfully.
* @retval -1 Failed to send command.
*/
int AviRun(int isNoStop, int nDstFlg)
{
	int ret = 0, nRunNum;
	nDstFlg &= AVI_DST_MASK;
	AVI_DBG("Enter %s: mode %d, Flag : 0x%x\n", __func__, isNoStop, nDstFlg);
	if(!isNoStop){
		pthread_mutex_lock(&gAviMutex);
		gAviState |= (AVI_RUN | nDstFlg);
		pthread_mutex_unlock(&gAviMutex);
		ret = pthread_cond_signal(&gAviRunCond);
	} else {
		/* AVI no stop case */
		pthread_mutex_lock(&gAviMutex);
		nRunNum = gAviRunNum++;
		if(nRunNum == 0){
			gAviState |= (AVI_RUN | AVI_NO_STOP);
			ret = pthread_cond_signal(&gAviRunCond);
		}
		pthread_mutex_unlock(&gAviMutex);
	}
	return ret;
}
/**
* @brief Send a stop command to AVI manager

* AVI recording will stop immediately in normal mode. In no stop mode, AVI
* recording will stop when gAviRunNum is zero.
* @retval 0 Command sent without error.
* @retval -1 Error occurs in AviRun and AviStop.
*/
int AviStop()
{
	int ret = 0, nRunNum, nState;
	nState = GetAviState(0);
	AVI_DBG("Enter %s\n", __func__);
	if(nState & AVI_NO_STOP){
		/* AVI no stop case */
		pthread_mutex_lock(&gAviMutex);
		nRunNum = (--gAviRunNum);
		if(nRunNum < 0){
			AVI_DBG("Error usage in AviRun and AviStop\n");
			/* Restore to default */
			gAviRunNum = 0;
			ret = -1;
		} else if(nRunNum == 0){
			ret = system("killall -2 Appro_avi_save");
			gAviState &= (~AVI_NO_STOP);
			if(ret)
				AVI_ERR("Failed to stop AVI recording\n");
		}
		pthread_mutex_unlock(&gAviMutex);
	} else {
		if(pSysinfo->lan_config.alarmstatus & FLG_UI_AVI){
			ret = system("killall -2 Appro_avi_save");
			if(ret)
				AVI_ERR("Failed to stop AVI recording\n");
		} else {
			ret = -1;
			AVI_DBG("No AVI recording now\n");
		}
		GetAviState(AVI_RUN);
	}
	return ret;
}

