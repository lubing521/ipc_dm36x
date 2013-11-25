/* ===========================================================================
* @file file_dispatcher.c
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
#include <file_dispatcher.h>
#include <pthread.h>
#include <sem_util.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <avi_mng.h>
#include <sys/vfs.h>
#include <system_default.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <linux/types.h>
#include <sys/ioctl.h>

#define SD_MNT_PATH	"/mnt/mmc"
#define CMD_LIVE_TIME	10
#define FILE_PATH_LEN	40
#ifndef FD_DEBUG
#define FD_DBG(fmt, args...)
#else
#define FD_DBG(fmt, args...)	fprintf(stderr, fmt, ##args)
#endif

#define FD_ERR(fmt, args...)	fprintf(stderr, fmt, ##args)

typedef struct FileDispatchPrm_t{
	FileType	tFileType;
	char*	pFilePath;
	int		nDestination;
	pthread_t*	tThisThr;
}FileDispatchPrm_t;

static SemHandl_t gFileDispatcherSem = NULL;
static SemHandl_t gFTPSem = NULL;
static SemHandl_t gSMTPSem = NULL;
//static int gFileDispatcherState;
int	gFileDispCount = 0;
//static time_t	gFileDispSetTime[sizeof(gFileDispatcherState)];

int GetfreememInfo( void )
{
	int fd_mem = -1;
	int ret = -1;
	char StrBuffer[200];
	char TmpBuf[50];
	char *pStr = NULL;
	int freeInKb = 0;
	char delima_buf[] = ":\r\n ";

	fd_mem = open("/proc/meminfo", O_RDONLY);
	if( fd_mem < 0 )
	{
		FD_ERR("CheckCpuSpeed open file fail \n");
		ret = -1;
		goto CHECK_CPU_END;

	}

	ret = read(fd_mem,  StrBuffer, sizeof(StrBuffer)-1);
	if( ret <= 0 )
	{
		FD_ERR("read device error !!");
		ret = -1;
		goto CHECK_CPU_END;

	}

	ret = -1;
	StrBuffer[sizeof(StrBuffer)-1] = '\0';
	pStr = strtok(StrBuffer,delima_buf );

	while( pStr != NULL )
	{
		sscanf( pStr,"%s",TmpBuf);

		if( strncmp(TmpBuf, "MemFree", sizeof("MemFree")) == 0 )
		{

			pStr = strtok(NULL, delima_buf);

			//fprintf(stderr, " %s \n", pStr);

			sscanf( pStr,"%d",&freeInKb);

			if(freeInKb <2048){
				FD_ERR("Free Memory = %ld Kbyte is not enough \n", (long)freeInKb);
				ret = -1;
			}else{
				ret = freeInKb;
			}
			goto CHECK_CPU_END;

		}

		pStr = strtok(NULL, delima_buf);

	}

CHECK_CPU_END:

	if( fd_mem >= 0 )
		close(fd_mem);

	return ret;


}
#if 0
/**
* @brief Get current file disoatch status.
* This function will get file disoatch status and clear flag if needed.
* @param fClrFlg [I ] Bits to clear.
* @return file disoatch status.
*/
int GetFileDispState(int nClrFlg)
{
	int ret, i, nRealClrFlg = 0;
	time_t rawtime;
	SemWait(gFileDispatcherSem);
	time(&rawtime);
	ret = gFileDispatcherState;
	for(i = 0;nClrFlg && i < sizeof(gFileDispatcherState);i++, nClrFlg >>= 1){
		if((nClrFlg & 1) &&
				difftime(rawtime, gFileDispSetTime[i]) > CMD_LIVE_TIME)
			nRealClrFlg |= (1<<i);
	}
	if(nRealClrFlg){
		gFileDispatcherState &= (~nRealClrFlg);
	}
	FD_DBG("gFileDispatcherState:%x\n", gFileDispatcherState);
	SemRelease(gFileDispatcherSem);
	return ret;
}
/**
* @brief Tell file dispatcher wher to send the file.
* @param nSetFlg [I ] File destination.
*/
void SetFileDispState(int nSetFlg)
{
	int i;
	time_t rawtime;
	if(SemWait(gFileDispatcherSem) == 0){
		time(&rawtime);
		gFileDispatcherState |= nSetFlg;
		for(i = 0;i < sizeof(gFileDispatcherState) && nSetFlg;i++, nSetFlg >>= 1){
			if(nSetFlg & 1)
				gFileDispSetTime[i] = rawtime;
		}
		SemRelease(gFileDispatcherSem);
	}
}
#endif
/**
 * @brief	Record to SD card
 * @param	strFileName [I ] Config file name.
 * @retval	0 : success
 * @retval	-1 : fail
 */
int RecordToSD(char *strFileName)
{
	struct stat tFileStatus;
	long long int nSpaceLeft = 0;
	char cmd[80]= "";
	int nFileCount, i, ret = 0, offset;
	char buffer[FILE_PATH_LEN];
	struct statfs disk_statfs;
	FILE *fp;

	offset = strlen(TEMP_FOLDER);
	if( statfs(SD_MNT_PATH, &disk_statfs) >= 0 )
	{
		nSpaceLeft = (((long long int)disk_statfs.f_bsize  * (long long int)disk_statfs.f_bfree)/(long long int)1024);
	}

    FD_DBG("SD card free space:%lld Kbytes\n", nSpaceLeft);
	sprintf(buffer, "%s/%s", TEMP_FOLDER, strFileName);
	if((fp = fopen(buffer, "rt")) == NULL){
		FD_ERR("%s open fail\n", buffer);
		return -1;
	}
	fgets(buffer, FILE_PATH_LEN, fp);
	nFileCount = atoi(buffer);
	sprintf(buffer, "%s/", TEMP_FOLDER);
	for(i = 0; i < nFileCount; i++){
		fgets(buffer + offset + 1, FILE_PATH_LEN - offset - 1, fp);
		/* remove '\n' */
		buffer[strlen(buffer) - 1] = '\0';
		if(stat(buffer, &tFileStatus) != 0){
			FD_ERR("Error on get file <%s> status\n", buffer);
			ret = -1;
			break;
		}
		if(nSpaceLeft < tFileStatus.st_size){
			FD_ERR("Card space not enough\n");
			ret = -1;
			break;
		}
		sprintf(cmd, "cp %s %s\n", buffer, SD_PATH);
		if(system(cmd)){
			ret = -1;
			break;
		}
	}
	fclose(fp);
	return 0;
}
/**
* @brief Sent file(s) to FTP server.
* @param strFileName [I ] File name (with path). If this value is NULL, JPEG files will be send.
* @return 0 on success.
*/
int RecordToFTP(char *strFileName)
{
	char cmd[80];
	int ret;
	FD_DBG("Enter %s\n", __func__);
	ret = SemWait(gFTPSem);
	if(ret == 0){
		if(strFileName){
			sprintf(cmd, "/opt/ipnc/quftp -f %s\n", strFileName);
		}else
			sprintf(cmd, "/opt/ipnc/quftp\n");/* special use for JPEG */
		ret = system(cmd);
		if(ret)
			FD_ERR("error return in %s: %d\n", __func__, ret);
		SemRelease(gFTPSem);
	}
	FD_DBG("Leave %s\n", __func__);
	return ret;
}
/**
* @brief Sent file(s) to SMTP server.
* @param strFileName [I ] File name (with path). If this value is NULL, JPEG files will be send.
* @return 0 on success.
*/
int SentFileViaSMTP(char *strFileName)
{
	char cmd[80];
	int ret;
	if(strFileName)
		sprintf(cmd, "./appro-mail %s\n", strFileName);
	else
		sprintf(cmd, "./appro-mail\n");/* special use for JPEG */
	ret = SemWait(gSMTPSem);
	if(ret == 0){
		FD_DBG("%s\n", cmd);
		ret = system(cmd);
		SemRelease(gSMTPSem);
	}
	return ret;
}
/**
* @brief Remove a file.
* @param strFileName [I ] Config file name (with path).
* @return 0 on success.
*/
int RemoveFile(char *strFileName)
{
	char cmd[80]= "\n";
	FILE *fp;
	int nFileCount, i, ret = 0, offset;
	char buffer[FILE_PATH_LEN];

	offset = strlen(TEMP_FOLDER);
	sprintf(buffer, "%s/%s", TEMP_FOLDER, strFileName);
	if((fp = fopen(buffer, "rt")) == NULL){
		FD_ERR("Error on open config file <%s>\n", strFileName);
		return -1;
	}
	fgets(buffer, FILE_PATH_LEN, fp);
	nFileCount = atoi(buffer);
	sprintf(buffer, "%s/", TEMP_FOLDER);
	for(i = 0; i < nFileCount; i++){
		fgets(buffer + offset + 1, FILE_PATH_LEN - offset - 1, fp);
		/* remove '\n' */
		buffer[strlen(buffer) - 1] = '\0';
		sprintf(cmd, "rm -f %s\n", buffer);
		if(system(cmd)){
			ret = -1;
		}
	}
	fclose(fp);
	sprintf(cmd, "rm -f %s/%s\n", TEMP_FOLDER, strFileName);
	system(cmd);
	return ret;
}
/**
* @brief File dispatcher thread
* @param arg [I ] Not used.
* @return 0 on normal end.
*/
void *FileDispatcherThr(void *arg)
{
	void * ret = NULL;
	int nFree;
	char* strFilePath;
        struct statfs disk_statfs;

	FileDispatchPrm_t* pPrm = (FileDispatchPrm_t*) arg;
	strFilePath = pPrm->pFilePath;
	if(pPrm->tFileType == ALL_TYPE){
		FD_DBG("FilePath:%s\n", strFilePath);

		do{
			if (pPrm->nDestination & FILE_DISPATCH_TO_SD) {
				if( statfs(SD_MNT_PATH, &disk_statfs) >= 0 )
				{
					nFree = (((long long int)disk_statfs.f_bsize  * (long long int)disk_statfs.f_bfree)/(long long int)1024);
				}
			}
			else {
				nFree = GetfreememInfo();
			}
			FD_DBG("Wait free memory (%s).\n", strFilePath);
			sleep(1);
		} while(nFree < 0);
		fprintf(stdout,"FileDispatcher check free Memory = %ld Kbyte  \n", (long)nFree);
		if(pPrm->nDestination & FILE_DISPATCH_TO_FTP){
			/* send file to ftp */
			FD_DBG("FILE_DISPATCH_TO_FTP\n");
			RecordToFTP(strFilePath);
		}
		if(pPrm->nDestination & FILE_DISPATCH_TO_SD){
			/* send file to sd */
			FD_DBG("FILE_DISPATCH_TO_SD\n");
			RecordToSD(strFilePath);
		}
		/* SMTP will remove files when it read them */
		if(pPrm->nDestination & FILE_DISPATCH_TO_SMTP){
			/* send file to smtp */
			FD_DBG("FILE_DISPATCH_TO_SMTP\n");
			SentFileViaSMTP(strFilePath);
		} //else
		RemoveFile(strFilePath);
	} else if(pPrm->tFileType == JPG_TYPE){
		switch(pPrm->nDestination){
			case JPG_DISPATCH_TO_SD:
				/* send file to sd */
				FD_DBG("JPG_DISPATCH_TO_SD\n");
				RecordToSD(strFilePath);
				RemoveFile(strFilePath);
				break;
			case JPG_DISPATCH_TO_FTP:
				/* send file to ftp */
				FD_DBG("JPG_DISPATCH_TO_FTP\n");
				RecordToFTP(NULL);
				break;
			case JPG_DISPATCH_TO_SMTP:
				/* send file to smtp */
				FD_DBG("JPG_DISPATCH_TO_SMTP\n");
				SentFileViaSMTP(NULL);
				break;
			default:
				FD_ERR("Unknow destination for JPEG\n");
				break;
		}
	} else {
		FD_ERR("Error file type\n");
		ret = (void *)-1;
	}
	if(SemWait(gFileDispatcherSem) == 0){
		FD_DBG("%s: gFileDispCount = %d\n", __func__, gFileDispCount);
		gFileDispCount --;
		SemRelease(gFileDispatcherSem);
	} else {
		FD_ERR("gFileDispatcherSem lock error\n");
		ret = (void *)-1;
	}
	nFree = pthread_detach(*pPrm->tThisThr);
	if(nFree)
		FD_ERR("Error code return in from pthread_detach : %d\n", nFree);
	if(strFilePath)
		free(strFilePath);
	free(pPrm->tThisThr);
	free(pPrm);
	pthread_exit(ret);
	return ret;
}
/**
* @brief File dispatcher initialize.
* @retval 0 Success
* @retval -1 Fail
*/
int FileDispatcherInit()
{
	if(gFileDispatcherSem == NULL)
		gFileDispatcherSem = MakeSem();
	if(gFileDispatcherSem == NULL)
		return -1;
	if(gFTPSem == NULL)
		gFTPSem = MakeSem();
	if(gFTPSem == NULL){
		DestroySem(gFileDispatcherSem);
		return -1;
	}
	if(gSMTPSem == NULL)
		gSMTPSem = MakeSem();
	if(gSMTPSem == NULL){
		DestroySem(gFileDispatcherSem);
		DestroySem(gFTPSem);
		return -1;
	}
	FD_DBG("File dispatcher initial success\n");
	return 0;
}
/**
* @brief Make file dispatcher leave.
* @return 0 on Success
*/
int FileDispatcherExit()
{
	int ret = 0;
	while(1)
		if(SemWait(gFileDispatcherSem) == 0){
			if(gFileDispCount == 0){
				gFileDispCount = FILE_DISP_THR_LIMIT;
				break;
			}
			SemRelease(gFileDispatcherSem);
			sleep(1);
		}
	SemRelease(gFileDispatcherSem);
	DestroySem(gSMTPSem);
	DestroySem(gFTPSem);
	DestroySem(gFileDispatcherSem);
	gFileDispatcherSem = gFTPSem = gSMTPSem = NULL;
	return ret;
}
#if 0
/**
* @brief Send a JPEG file.
* @param strFilePath [I ] JPEG file path.
* @param nDestination [I ] JPEG file's destination.
* @retval 0 Success
* @retval -1 Fail
*/
int DoJpegDispatch(char *strFilePath, int nDestination)
{
	pthread_t tTmpThr;
	FileDispatchPrm_t* pPrm;
	void *ret =NULL;
	char *pFilePath = NULL;
	if(strFilePath){
		pFilePath = malloc(strlen(strFilePath) + 1);
		if(pFilePath == NULL){
			FD_ERR("Not enough memory\n");
			return -1;
		}
		strcpy(pFilePath, strFilePath);
		FD_DBG("pFilePath: %s\n", pFilePath);
	}
	pPrm = malloc(sizeof(FileDispatchPrm_t));
	if(pPrm == NULL){
		if(pFilePath)
			free(pFilePath);
		FD_ERR("Not enough memory\n");
		return -1;
	}
	pPrm->pFilePath = pFilePath;
	pPrm->tFileType = JPG_TYPE;
	pPrm->nDestination = nDestination;
	if(pthread_create(&tTmpThr, NULL, FileDispatcherThr, pPrm)){
		if(pFilePath)
			free(pFilePath);
		free(pPrm);
		FD_ERR("Can not create FileDispatcherThr\n");
		return -1;
	}
	if(nDestination == JPG_DISPATCH_TO_SD){
		if(pthread_join(tTmpThr, &ret) < 0){
			FD_ERR("Can't join FileDispatcherThr\n");
			return -1;
		}
	}
	pthread_detach(tTmpThr);
	return (int) ret;
}
#endif
/**
* @brief Send a file.
* @param strFilePath [I ] File path.
* @param nDestination [I ] File destination.
* @retval 0 Success
* @retval -1 Fail
*/
int DoFileDispatch(char *strFilePath, int nDestination)
{
	pthread_t* pTmpThr;
	FileDispatchPrm_t* pPrm;
	char *pFilePath = NULL;
	int ret = 0;
	if(SemWait(gFileDispatcherSem) == 0){
		FD_DBG("%s: gFileDispCount = %d\n", __func__, gFileDispCount);
		if(gFileDispCount < FILE_DISP_THR_LIMIT){
			/* request memory */
			if(strFilePath){
				pFilePath = malloc(strlen(strFilePath) + 1);
				if(pFilePath == NULL){
					SemRelease(gFileDispatcherSem);
					RemoveFile(strFilePath);
					FD_ERR("Not enough memory\n");
					return -1;
				}
				strcpy(pFilePath, strFilePath);
			}
			pTmpThr = malloc(sizeof(pthread_t));
			if(pTmpThr == NULL){
				SemRelease(gFileDispatcherSem);
				if(pFilePath){
					RemoveFile(pFilePath);
					free(pFilePath);
				}
				FD_ERR("Not enough memory\n");
				return -1;
			}
			pPrm = malloc(sizeof(FileDispatchPrm_t));
			if(pPrm == NULL){
				SemRelease(gFileDispatcherSem);
				if(pFilePath){
					RemoveFile(pFilePath);
					free(pFilePath);
				}
				free(pTmpThr);
				FD_ERR("Not enough memory\n");
				return -1;
			}
			pPrm->pFilePath = pFilePath;
			pPrm->tFileType = ALL_TYPE;
			pPrm->nDestination = nDestination;
			pPrm->tThisThr = pTmpThr;
			if(pthread_create(pTmpThr, NULL, FileDispatcherThr, pPrm)){
				SemRelease(gFileDispatcherSem);
				if(pFilePath){
					RemoveFile(pFilePath);
					free(pFilePath);
				}
				free(pTmpThr);
				free(pPrm);
				FD_ERR("Can not create FileDispatcherThr\n");
				ret = -1;
			} else {
				gFileDispCount++;
				SemRelease(gFileDispatcherSem);
			}
		} else {
			SemRelease(gFileDispatcherSem);
			FD_ERR("File dispatcher threads over limit %d\n", FILE_DISP_THR_LIMIT);
			ret = -1;
		}
	}else{
		FD_ERR("Semaphore lock error\n");
		ret = -1;
	}
	return ret;
}
