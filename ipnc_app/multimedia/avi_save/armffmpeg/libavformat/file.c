/*
 * Buffered file io for ffmpeg system
 * Copyright (c) 2001 Fabrice Bellard
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */
 #define _XOPEN_SOURCE 600

#include "libavutil/avstring.h"
#include "avformat.h"
#include <fcntl.h>
#if HAVE_SETMODE
#include <io.h>
#endif
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>
#include "os_support.h"


#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h> 
#include <fcntl.h>
#include <sys/mman.h>
#include <linux/types.h>
#include <sys/ioctl.h> 

#define ERR(fmt, args...) fprintf(stderr, "Error: " fmt, ## args)
extern  int posix_fadvise(int fd, off_t  offset, off_t len, int advice);

#define MAXFILE_HANDLE (10)

typedef struct _HANDLE_FILE{
	URLContext *h[MAXFILE_HANDLE];
	int filesize[MAXFILE_HANDLE];
	int cnt;
}HANDLE_FILE;

static HANDLE_FILE file_hdl = {
	.cnt = 0
};

int InsertFile_hdl(URLContext *h)
{
	int cnt = 0;
	for( cnt = 0; cnt < MAXFILE_HANDLE; cnt++ )
	{
		if( file_hdl.h[cnt] == NULL )
		{
			file_hdl.h[cnt] = h;
			file_hdl.filesize[cnt] = 0;
			file_hdl.cnt++;
			break;
		}
	}
	
	return 0;
}

void RemoveFile_hdl(URLContext *h)
{
	int cnt = 0;
	for( cnt = 0; cnt < MAXFILE_HANDLE; cnt++ )
	{
		if( file_hdl.h[cnt] == h )
		{
			file_hdl.h[cnt] = NULL;
			file_hdl.filesize[cnt] = 0;
			file_hdl.cnt--;
			break;
		}
	}
}
int EachFileSave = 2*1024*1024;
void CheckFile_hdl(URLContext *h, int size)
{
	int cnt = 0;
	int fd = (size_t)h->priv_data;
   	
	for( cnt = 0; cnt < MAXFILE_HANDLE; cnt++ )
	{
		if( file_hdl.h[cnt] == h )
		{
			file_hdl.filesize[cnt] += size;
			
			if( file_hdl.filesize[cnt] >= EachFileSave )
			{
				//printf("avi file flush!!\n");
				file_hdl.filesize[cnt] = 0;
				fdatasync(fd);
				posix_fadvise(fd, 0,0, POSIX_FADV_DONTNEED  );
	
			}
			break;
		}
	}
}
int freememchksize = 1600;
static int GetfreememInfo( void )
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
		ERR("CheckCpuSpeed open file fail \n");
		ret = -1;
		goto CHECK_CPU_END;
		
	}

	ret = read(fd_mem,  StrBuffer, sizeof(StrBuffer)-1);
	if( ret <= 0 )
	{
		ERR("read device error !!");
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
			if(freeInKb <freememchksize){
				//fprintf(stderr,"Write file check free Memory = %ld Kbyte is not enough \n", (long)freeInKb);
				ret = -1;
			}else{
				ret = 0;
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

/* standard file protocol */

static int file_open(URLContext *h, const char *filename, int flags)
{
    int access;
    int fd;

    av_strstart(filename, "file:", &filename);

    if (flags & URL_RDWR) {
        access = O_CREAT | O_TRUNC | O_RDWR;
    } else if (flags & URL_WRONLY) {
        access = O_CREAT | O_TRUNC | O_WRONLY;
    } else {
        access = O_RDONLY;
    }
#ifdef O_BINARY
    access |= O_BINARY;
#endif
    fd = open(filename, access, 0666);
    if (fd == -1)
        return AVERROR(ENOENT);
    h->priv_data = (void *) (intptr_t) fd;
	InsertFile_hdl(h);
    return 0;
}

static int file_read(URLContext *h, unsigned char *buf, int size)
{
    int fd = (intptr_t) h->priv_data;
    return read(fd, buf, size);
}

static int file_write(URLContext *h, unsigned char *buf, int size)
{
    int fd = (intptr_t) h->priv_data;
   if (GetfreememInfo() < 0 )
   {
		fdatasync(fd);
		posix_fadvise(fd, 0,0, POSIX_FADV_DONTNEED );
   }	
   
   CheckFile_hdl(h, size);
   
    return write(fd, buf, size);
}

/* XXX: use llseek */
static int64_t file_seek(URLContext *h, int64_t pos, int whence)
{
    int fd = (intptr_t) h->priv_data;
    return lseek(fd, pos, whence);
}

static int file_close(URLContext *h)
{
    int fd = (intptr_t) h->priv_data;
   	fdatasync(fd);
    posix_fadvise(fd, 0,0, POSIX_FADV_DONTNEED  );
	
	RemoveFile_hdl(h);
	
    return close(fd);
}

static int file_get_handle(URLContext *h)
{
    return (intptr_t) h->priv_data;
}

URLProtocol file_protocol = {
    "file",
    file_open,
    file_read,
    file_write,
    file_seek,
    file_close,
    .url_get_file_handle = file_get_handle,
};

/* pipe protocol */

static int pipe_open(URLContext *h, const char *filename, int flags)
{
    int fd;
    char *final;
    av_strstart(filename, "pipe:", &filename);

    fd = strtol(filename, &final, 10);
    if((filename == final) || *final ) {/* No digits found, or something like 10ab */
        if (flags & URL_WRONLY) {
            fd = 1;
        } else {
            fd = 0;
        }
    }
#if HAVE_SETMODE
    setmode(fd, O_BINARY);
#endif
    h->priv_data = (void *) (intptr_t) fd;
    h->is_streamed = 1;
    return 0;
}

URLProtocol pipe_protocol = {
    "pipe",
    pipe_open,
    file_read,
    file_write,
    .url_get_file_handle = file_get_handle,
};
