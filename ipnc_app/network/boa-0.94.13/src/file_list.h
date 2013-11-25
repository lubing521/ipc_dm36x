#ifndef _FILE_LIST_H_
#define _FILE_LIST_H_

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/dir.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/statfs.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <dirent.h>
#include <time.h>
#include <ifaddrs.h>

typedef struct _FILE_INFO{
	char name[50];
	char date[15];
	char time[10];
	char size[20];
} FILE_INFO;


int file_select(const struct dirent   *entry);
int MEM_List_files_To_html(char *pInputDir,char *pOutDisk, void *pOutMem, int MaxSize);

int OuputMem_html_empty(void *pMem, int maxsize);

int DeleteFile(char *pDir,char *pFilename);
int CheckFileExist(char *pDir,char *pFilename);
long long GetDiskfreeSpace(char *pDisk);
long long GetDiskusedSpace(char *pDisk);
void GetIP_Addr(char *pMem );
#endif
