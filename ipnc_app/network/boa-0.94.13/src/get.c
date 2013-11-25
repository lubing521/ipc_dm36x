/*
 *  Boa, an http server
 *  Copyright (C) 1995 Paul Phillips <paulp@go2net.com>
 *  Some changes Copyright (C) 1996,99 Larry Doolittle <ldoolitt@boa.org>
 *  Some changes Copyright (C) 1996-2002 Jon Nelson <jnelson@boa.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 1, or (at your option)
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

/* $Id: get.c,v 1.76.2.5 2002/07/26 03:05:59 jnelson Exp $*/

#include "boa.h"
#ifdef SERVER_SSL
#include <openssl/ssl.h>
#endif
#ifdef DAVINCI_IPCAM
#include "web_translate.h"
#endif

/* local prototypes */
int get_cachedir_file(request * req, struct stat *statbuf);
int index_directory(request * req, char *dest_filename);

#ifdef DAVINCI_IPCAM
char **strrfind(char *strdata)
{
	static char *tag[2];

	while (*strdata) {
		if (strdata[0] == '<' && strdata[1] == '%') {
			tag[0] = strdata;
			strdata += 2;
			while (*strdata) {
				if (strdata[0] == '%' && strdata[1] == '>') {
					tag[1] = strdata;
					return tag;
				}
				else if (strdata[0] == '<' && strdata[1] == '%') {
					tag[0] = strdata;
					strdata += 2;
				}
				else
					strdata++;
			}
			return NULL;
		}
		else
			strdata++;
	}
	return NULL;
}


int html_argument_parse(AUTHORITY authority, char *data, char *result)
{
	int len1, len2, size, total_size=0;
	char **tag, *arg, *arg1, zero_str[1]={0x00};

	while (1) {
		tag = strrfind(data);
		if (tag) {
			len1 = tag[0]-data;
			arg = tag[0]+2;

			len2 = tag[1]-data+2;
			memcpy(result, data, len2);
			*tag[1] = '\0';

			arg1 = strchr(arg, '.');
			if (arg1)
				*arg1++ = '\0';
			else
				arg1 = zero_str;

			size = TranslateWebPara(authority, result+len1, arg, arg1);
			if (size < 0) {
				result += len2;
				total_size += len2;
			}
			else {
				result += (len1+size);
				total_size += (len1+size);
			}
			data += len2;
		}
		else {
			size = strlen(data);
			total_size += size;
			memcpy(result, data, size);
			break;
		}
	}
	return total_size;
}
#endif  // DAVINCI_IPCAM

/*
 * Name: init_get
 * Description: Initializes a non-script GET or HEAD request.
 *
 * Return values:
 *   0: finished or error, request will be freed
 *   1: successfully initialized, added to ready queue
 */

int init_get(request * req)
{
    int data_fd, saved_errno, dynamic=0;
    struct stat statbuf;
    volatile int bytes;

    data_fd = open(req->pathname, O_RDONLY);
    saved_errno = errno; /* might not get used */

#ifdef GUNZIP
    if (data_fd == -1 && errno == ENOENT) {
        /* cannot open */
        /* it's either a gunzipped file or a directory */
        char gzip_pathname[MAX_PATH_LENGTH];
        int len;

        len = strlen(req->pathname);

        memcpy(gzip_pathname, req->pathname, len);
        memcpy(gzip_pathname + len, ".gz", 3);
        gzip_pathname[len + 3] = '\0';
        data_fd = open(gzip_pathname, O_RDONLY);
        if (data_fd != -1) {
            close(data_fd);

            req->response_status = R_REQUEST_OK;
            if (req->pathname)
                free(req->pathname);
            req->pathname = strdup(gzip_pathname);
            if (!req->pathname) {
                log_error_time();
                perror("strdup");
                send_r_error(req);
                return 0;
            }
            if (!req->simple) {
                req_write(req, "HTTP/1.0 200 OK-GUNZIP\r\n");
                print_http_headers(req);
                print_content_type(req);
                print_last_modified(req);
                req_write(req, "\r\n");
                req_flush(req);
            }
            if (req->method == M_HEAD)
                return 0;

            return init_cgi(req);
        }
    }
#endif

    if (data_fd == -1) {
        log_error_doc(req);
        errno = saved_errno;
        perror("document open");

        if (saved_errno == ENOENT)
            send_r_not_found(req);
        else if (saved_errno == EACCES)
            send_r_forbidden(req);
        else
            send_r_bad_request(req);
        return 0;
    }

    fstat(data_fd, &statbuf);

    if (S_ISDIR(statbuf.st_mode)) { /* directory */
        close(data_fd);         /* close dir */

        if (req->pathname[strlen(req->pathname) - 1] != '/') {
            char buffer[3 * MAX_PATH_LENGTH + 128];

            if (server_port != 80)
                sprintf(buffer, "http://%s:%d%s/", server_name,
                        server_port, req->request_uri);
            else
                sprintf(buffer, "http://%s%s/", server_name,
                        req->request_uri);
            send_r_moved_perm(req, buffer);
            return 0;
        }
        data_fd = get_dir(req, &statbuf); /* updates statbuf */

        if (data_fd == -1)      /* couldn't do it */
            return 0;           /* errors reported by get_dir */
        else if (data_fd <= 1)
            /* data_fd == 0 -> close it down, 1 -> continue */
            return data_fd;
        /* else, data_fd contains the fd of the file... */
    }
#ifdef DAVINCI_IPCAM
    if (req->http_uri && (req->http_uri->uri_flag & URI_FLAG_NEED_PARSE))
        dynamic = 1;
#endif
    if (req->if_modified_since && !dynamic &&
        !modified_since(&(statbuf.st_mtime), req->if_modified_since)) {
        send_r_not_modified(req);
        close(data_fd);
        return 0;
    }
    req->filesize = statbuf.st_size;
    req->last_modified = statbuf.st_mtime;

    if (req->method == M_HEAD || req->filesize == 0) {
        send_r_request_ok(req);
        close(data_fd);
        return 0;
    }

    if (req->filesize > MAX_FILE_MMAP) {
        send_r_request_ok(req); /* All's well */
        req->status = PIPE_READ;
        req->cgi_status = CGI_BUFFER;
        req->data_fd = data_fd;
        req_flush(req);         /* this should *always* complete due to
                                   the size of the I/O buffers */
        req->header_line = req->header_end = req->buffer;
        return 1;
    }

    if (req->filesize == 0) {  /* done */
        send_r_request_ok(req);     /* All's well *so far* */
        close(data_fd);
        return 1;
    }

    /* NOTE: I (Jon Nelson) tried performing a read(2)
     * into the output buffer provided the file data would
     * fit, before mmapping, and if successful, writing that
     * and stopping there -- all to avoid the cost
     * of a mmap.  Oddly, it was *slower* in benchmarks.
     */
    req->mmap_entry_var = find_mmap(data_fd, &statbuf);
    if (req->mmap_entry_var == NULL) {
        req->buffer_end = 0;
        if (errno == ENOENT)
            send_r_not_found(req);
        else if (errno == EACCES)
            send_r_forbidden(req);
        else
            send_r_bad_request(req);
        close(data_fd);
        return 0;
    }
    req->data_mem = req->mmap_entry_var->mmap;
    close(data_fd);             /* close data file */

    if ((long) req->data_mem == -1) {
        boa_perror(req, "mmap");
        return 0;
    }

#ifdef DAVINCI_IPCAM
    if (dynamic) {
            char *addr = (char *)malloc(req->filesize + 1025);
            if (addr) {
                req->mem_flag |= MFLAG_IS_MEMORY;
                req->mmap_ptr = req->data_mem;
                req->mmap_size = req->filesize;
                memcpy(addr+1024, req->data_mem, req->filesize);
                addr[req->filesize+1024] = '\0';
                req->data_mem = addr;
                req->filesize = html_argument_parse(req->authority, addr+1024, req->data_mem);
                send_request_ok_no_cache(req);     /* All's well */
                return 1;
            }
    }
#endif  // DAVINCI_IPCAM

    send_r_request_ok(req);     /* All's well */

    bytes = BUFFER_SIZE - req->buffer_end;

    /* bytes is now how much the buffer can hold
     * after the headers
     */

    if (bytes > 0) {
        if (bytes > req->filesize)
            bytes = req->filesize;

        if (sigsetjmp(env, 1) == 0) {
            handle_sigbus = 1;
            memcpy(req->buffer + req->buffer_end, req->data_mem, bytes);
            handle_sigbus = 0;
            /* OK, SIGBUS **after** this point is very bad! */
        } else {
            /* sigbus! */
            log_error_doc(req);
            reset_output_buffer(req);
            send_r_error(req);
            fprintf(stderr, "%sGot SIGBUS in memcpy!\n", get_commonlog_time());
            return 0;
        }
        req->buffer_end += bytes;
        req->filepos += bytes;
        if (req->filesize == req->filepos) {
            req_flush(req);
            req->status = DONE;
        }
    }

    /* We lose statbuf here, so make sure response has been sent */
    return 1;
}

#ifdef DAVINCI_IPCAM
unsigned char AUDIO_MUTE_ARRAY[4096];
#if 1
int audio_get(request * req, int video_type)
{
	AV_DATA av_data;
	int ret;

	if (req->audio_book == 0) {
		GetAVData(AV_OP_GET_ULAW_SERIAL, -1, &av_data );
		if (av_data.serial <= req->audio_lock) {
			dbg("av_data.serial <= req->audio_lock!!!\n");
			req->busy_flag &= ~BUSY_FLAG_AUDIO;
			return 0;
		}
		req->audio_book = av_data.serial;
	}

	ret = GetAVData(AV_OP_LOCK_ULAW, req->audio_book, &av_data );
	if (ret == RET_SUCCESS) {
		extern int CheckAudioStatus(void);
		if (req->audio_lock > 0) {
			GetAVData(AV_OP_UNLOCK_ULAW, req->audio_lock, NULL);
			req->audio_lock = 0;
		}
		if(CheckAudioStatus())
		{
			req->data_mem = av_data.ptr;
		}else{
			static int IsFirst = 1;

			if(IsFirst)
			{
				IsFirst = 0;
				memset(AUDIO_MUTE_ARRAY,0xff,sizeof(AUDIO_MUTE_ARRAY));
			}

			req->data_mem = AUDIO_MUTE_ARRAY;
		}
		req->filesize = av_data.size;
		req->filepos = 0;
		req->audio_lock = av_data.serial;
		req->audio_book = av_data.serial + 1;
		req->audio_sync = av_data.ref_serial[video_type];
		reset_output_buffer(req);
		req_write(req, "\r\n");
		print_audio_headers(req);
		return 1;
	}
	else if (ret == RET_NO_VALID_DATA) {
		req->busy_flag &= ~BUSY_FLAG_AUDIO;
		return 0;
	}
	else {
		req->audio_book = 0;
		dbg("ERROR, ret=%d\n", ret);
		return -1;
	}

}
#else
int audio_get(request * req)
{
	AV_DATA av_data;
	int ret;

	if (req->audio_book == 0) {
		GetAVData(AV_OP_GET_ULAW_SERIAL, -1, &av_data );
		if (av_data.serial <= req->audio_lock) {
			dbg("av_data.serial <= req->audio_lock!!!\n");
			req->busy_flag &= ~BUSY_FLAG_AUDIO;
			return 0;
		}
		req->audio_book = av_data.serial;
	}

	ret = GetAVData(AV_OP_LOCK_ULAW, req->audio_book, &av_data );
	if (ret == RET_SUCCESS) {
		if (req->audio_lock > 0)
			GetAVData(AV_OP_UNLOCK_ULAW, req->audio_lock, NULL);

		if (req->audio_length + av_data.size > SOCKETBUF_SIZE)
			req->audio_length = 0;

		memcpy(req->audio_data+req->audio_length, av_data.ptr, av_data.size);
		req->audio_length += av_data.size;
		req->audio_lock = av_data.serial;
		req->audio_book = av_data.serial + 1;
//dbg("av_data.serial = %d, av_data.size = %d\n", av_data.serial, av_data.size);
		return 1;
	}
	else if (ret == RET_NO_VALID_DATA) {
		req->busy_flag &= ~BUSY_FLAG_AUDIO;
		return 0;
	}
	else {
		req->audio_book = 0;
		dbg("ERROR, ret=%d\n", ret);
		return -1;
	}

}

void audio_send(request * req)
{
	req->data_mem = req->audio_data;
	req->filesize = req->audio_length;
	req->filepos = 0;
	req->audio_length = 0;
	reset_output_buffer(req);
	req_write(req, "\r\n");
	print_audio_headers(req);
}
#endif
#endif

/*
 * Name: process_get
 * Description: Writes a chunk of data to the socket.
 *
 * Return values:
 *  -1: request blocked, move to blocked queue
 *   0: EOF or error, close it down
 *   1: successful write, recycle in ready queue
 */

int process_get(request * req)
{
    int bytes_written;
    volatile int bytes_to_write;

    bytes_to_write = req->filesize - req->filepos;
    if (bytes_to_write > SOCKETBUF_SIZE)
        bytes_to_write = SOCKETBUF_SIZE;


    if (sigsetjmp(env, 1) == 0) {
        handle_sigbus = 1;
#ifdef SERVER_SSL
	if(req->ssl == NULL){
#endif /*SERVER_SSL*/
        bytes_written = write(req->fd, req->data_mem + req->filepos,
                              bytes_to_write);
#ifdef SERVER_SSL
	}else{
		bytes_written = SSL_write(req->ssl, req->data_mem + req->filepos, bytes_to_write);
#if 0
		printf("SSL_write\n");
#endif /*0*/
	}
#endif /*SERVER_SSL*/
        handle_sigbus = 0;
        /* OK, SIGBUS **after** this point is very bad! */
    } else {
        /* sigbus! */
        log_error_doc(req);
        /* sending an error here is inappropriate
         * if we are here, the file is mmapped, and thus,
         * a content-length has been sent. If we send fewer bytes
         * the client knows there has been a problem.
         * We run the risk of accidentally sending the right number
         * of bytes (or a few too many) and the client
         * won't be the wiser.
         */
        req->status = DEAD;
        fprintf(stderr, "%sGot SIGBUS in write(2)!\n", get_commonlog_time());
        return 0;
    }

    if (bytes_written < 0) {
        if (errno == EWOULDBLOCK || errno == EAGAIN)
            return -1;
        /* request blocked at the pipe level, but keep going */
        else {
            if (errno != EPIPE) {
                log_error_doc(req);
                /* Can generate lots of log entries, */
                perror("write");
                /* OK to disable if your logs get too big */
            }
            req->status = DEAD;
            return 0;
        }
    }
    req->filepos += bytes_written;
	req->busy_flag = BUSY_FLAG_AUDIO|BUSY_FLAG_VIDEO;
    if (req->filepos == req->filesize) { /* EOF */

#ifdef DAVINCI_IPCAM
        if (req->http_stream == URI_STREAM_MJPEG) {
#if 1
#if 0
            while (audio_get(req) > 0);
            if (req->audio_length >= AUDIO_SEND_SIZE) {
                audio_send(req);
                return 1;
            }
#else
            if (audio_get(req, FMT_MJPEG) > 0)
			return 1;
#endif
#else
		req->busy_flag &= ~BUSY_FLAG_AUDIO;
#endif
            if (req->serial_lock) {
                GetAVData(AV_OP_UNLOCK_MJPEG, req->serial_lock, NULL);
                req->serial_lock = 0;
            }
            GetAVData(AV_OP_GET_MJPEG_SERIAL, -1, &req->av_data);
            if (req->av_data.serial < req->serial_book) {
                req->busy_flag &= ~BUSY_FLAG_VIDEO;
                return 1;
            }
            GetAVData(AV_OP_LOCK_MJPEG, req->av_data.serial, &req->av_data );
            req->data_mem = req->av_data.ptr;
            req->filesize = req->av_data.size+16;
            req->filepos = 0;
            req->serial_lock = req->av_data.serial;
            req->serial_book = req->av_data.serial+1;
            reset_output_buffer(req);
            req_write(req, "\r\n");
            print_mjpeg_headers(req);
            return 1;
        }
        if (req->http_stream == URI_STREAM_MPEG4 || req->http_stream == URI_STREAM_AVC) {
            int ret;
#if 1
#if 0
		while (audio_get(req) > 0);
		if (req->audio_length > AUDIO_SEND_SIZE) {
			req->busy_flag |= BUSY_FLAG_AUDIO;
			audio_send(req);
      		      return 1;
		}
#else
            if (audio_get(req, FMT_MPEG4) > 0)
			return 1;
#endif
#else
		req->busy_flag &= ~BUSY_FLAG_AUDIO;
#endif
            ret = GetAVData(AV_OP_LOCK_MP4, req->serial_book, &req->av_data);
            if (ret == RET_SUCCESS) {
                GetAVData(AV_OP_UNLOCK_MP4, req->serial_lock, NULL);
                req->data_mem = req->av_data.ptr;
                req->filesize = req->av_data.size+16;
                req->filepos = 0;

                req->serial_lock = req->av_data.serial;
                req->serial_book = req->av_data.serial+1;

                reset_output_buffer(req);
                req_write(req, "\r\n");
				if (req->http_stream == URI_STREAM_AVC)
                { print_avc_headers(req); }
				else
				{ print_mpeg4_headers(req); }
                return 1;
            }
            else if (ret == RET_NO_VALID_DATA) {
                req->busy_flag &= ~BUSY_FLAG_VIDEO;
                return 1;
            }
            else {
                GetAVData(AV_OP_GET_MPEG4_SERIAL, -1, &req->av_data );
                req->serial_book = req->av_data.serial;
                dbg("lock error ret=%d\n", ret);
                return 1;
            }
        }
        if (req->http_stream == URI_STREAM_MPEG4CIF || req->http_stream == URI_STREAM_AVCCIF) {
            int ret;
#if 1
#if 0
		while (audio_get(req) > 0);
		if (req->audio_length > AUDIO_SEND_SIZE) {
			audio_send(req);
      		      return 1;
		}
#else
            if (audio_get(req, FMT_MPEG4_EXT) > 0)
			return 1;
#endif
#else
		req->busy_flag &= ~BUSY_FLAG_AUDIO;
#endif
            ret = GetAVData(AV_OP_LOCK_MP4_CIF, req->serial_book, &req->av_data);
            if (ret == RET_SUCCESS) {
                GetAVData(AV_OP_UNLOCK_MP4_CIF, req->serial_lock, NULL);
                req->data_mem = req->av_data.ptr;
                req->filesize = req->av_data.size+16;
                req->filepos = 0;

                req->serial_lock = req->av_data.serial;
                req->serial_book = req->av_data.serial+1;

                reset_output_buffer(req);
                req_write(req, "\r\n");
				if (req->http_stream == URI_STREAM_AVCCIF)
                { print_avc_headers(req); }
				else
				{ print_mpeg4_headers(req); }
                return 1;
            }
            else if (ret == RET_NO_VALID_DATA) {
                req->busy_flag &= ~BUSY_FLAG_VIDEO;
                return 1;
            }
            else {
                GetAVData(AV_OP_GET_MPEG4_CIF_SERIAL, -1, &req->av_data );
                req->serial_book = req->av_data.serial;
                dbg("lock error ret=%d\n", ret);
                return 1;
            }
        }
#endif  // DAVINCI_IPCAM
        return 0;
    } else
        return 1;               /* more to do */
}

/*
 * Name: get_dir
 * Description: Called from process_get if the request is a directory.
 * statbuf must describe directory on input, since we may need its
 *   device, inode, and mtime.
 * statbuf is updated, since we may need to check mtimes of a cache.
 * returns:
 *  -1 error
 *  0  cgi (either gunzip or auto-generated)
 *  >0  file descriptor of file
 */

int get_dir(request * req, struct stat *statbuf)
{

    char pathname_with_index[MAX_PATH_LENGTH];
    int data_fd;

    if (directory_index) {      /* look for index.html first?? */
        strcpy(pathname_with_index, req->pathname);
        strcat(pathname_with_index, directory_index);
        /*
           sprintf(pathname_with_index, "%s%s", req->pathname, directory_index);
         */

        data_fd = open(pathname_with_index, O_RDONLY);

        if (data_fd != -1) {    /* user's index file */
            strcpy(req->request_uri, directory_index); /* for mimetype */
            fstat(data_fd, statbuf);
            return data_fd;
        }
        if (errno == EACCES) {
            send_r_forbidden(req);
            return -1;
        } else if (errno != ENOENT) {
            /* if there is an error *other* than EACCES or ENOENT */
            send_r_not_found(req);
            return -1;
        }

#ifdef GUNZIP
        /* if we are here, trying index.html didn't work
         * try index.html.gz
         */
        strcat(pathname_with_index, ".gz");
        data_fd = open(pathname_with_index, O_RDONLY);
        if (data_fd != -1) {    /* user's index file */
            close(data_fd);

            req->response_status = R_REQUEST_OK;
            SQUASH_KA(req);
            if (req->pathname)
                free(req->pathname);
            req->pathname = strdup(pathname_with_index);
            if (!req->pathname) {
                log_error_time();
                perror("strdup");
                send_r_error(req);
                return 0;
            }
            if (!req->simple) {
                req_write(req, "HTTP/1.0 200 OK-GUNZIP\r\n");
                print_http_headers(req);
                print_last_modified(req);
                req_write(req, "Content-Type: ");
                req_write(req, get_mime_type(directory_index));
                req_write(req, "\r\n\r\n");
                req_flush(req);
            }
            if (req->method == M_HEAD)
                return 0;
            return init_cgi(req);
        }
#endif
    }

    /* only here if index.html, index.html.gz don't exist */
    if (dirmaker != NULL) {     /* don't look for index.html... maybe automake? */
        req->response_status = R_REQUEST_OK;
        SQUASH_KA(req);

        /* the indexer should take care of all headers */
        if (!req->simple) {
            req_write(req, "HTTP/1.0 200 OK\r\n");
            print_http_headers(req);
            print_last_modified(req);
            req_write(req, "Content-Type: text/html\r\n\r\n");
            req_flush(req);
        }
        if (req->method == M_HEAD)
            return 0;

        return init_cgi(req);
        /* in this case, 0 means success */
    } else if (cachedir) {
        return get_cachedir_file(req, statbuf);
    } else {                    /* neither index.html nor autogenerate are allowed */
        send_r_forbidden(req);
        return -1;              /* nothing worked */
    }
}

int get_cachedir_file(request * req, struct stat *statbuf)
{

    char pathname_with_index[MAX_PATH_LENGTH];
    int data_fd;
    time_t real_dir_mtime;

    real_dir_mtime = statbuf->st_mtime;
    sprintf(pathname_with_index, "%s/dir.%d.%ld",
            cachedir, (int) statbuf->st_dev, statbuf->st_ino);
    data_fd = open(pathname_with_index, O_RDONLY);

    if (data_fd != -1) {        /* index cache */

        fstat(data_fd, statbuf);
        if (statbuf->st_mtime > real_dir_mtime) {
            statbuf->st_mtime = real_dir_mtime; /* lie */
            strcpy(req->request_uri, directory_index); /* for mimetype */
            return data_fd;
        }
        close(data_fd);
        unlink(pathname_with_index); /* cache is stale, delete it */
    }
    if (index_directory(req, pathname_with_index) == -1)
        return -1;

    data_fd = open(pathname_with_index, O_RDONLY); /* Last chance */
    if (data_fd != -1) {
        strcpy(req->request_uri, directory_index); /* for mimetype */
        fstat(data_fd, statbuf);
        statbuf->st_mtime = real_dir_mtime; /* lie */
        return data_fd;
    }

    boa_perror(req, "re-opening dircache");
    return -1;                  /* Nothing worked. */

}

/*
 * Name: index_directory
 * Description: Called from get_cachedir_file if a directory html
 * has to be generated on the fly
 * returns -1 for problem, else 0
 * This version is the fastest, ugliest, and most accurate yet.
 * It solves the "stale size or type" problem by not ever giving
 * the size or type.  This also speeds it up since no per-file
 * stat() is required.
 */

int index_directory(request * req, char *dest_filename)
{
    DIR *request_dir;
    FILE *fdstream;
    struct dirent *dirbuf;
    int bytes = 0;
    char *escname = NULL;

    if (chdir(req->pathname) == -1) {
        if (errno == EACCES || errno == EPERM) {
            send_r_forbidden(req);
        } else {
            log_error_doc(req);
            perror("chdir");
            send_r_bad_request(req);
        }
        return -1;
    }

    request_dir = opendir(".");
    if (request_dir == NULL) {
        int errno_save = errno;
        send_r_error(req);
        log_error_time();
        fprintf(stderr, "directory \"%s\": ", req->pathname);
        errno = errno_save;
        perror("opendir");
        return -1;
    }

    fdstream = fopen(dest_filename, "w");
    if (fdstream == NULL) {
        boa_perror(req, "dircache fopen");
        closedir(request_dir);
        return -1;
    }

    bytes += fprintf(fdstream,
                     "<HTML><HEAD>\n<TITLE>Index of %s</TITLE>\n</HEAD>\n\n",
                     req->request_uri);
    bytes += fprintf(fdstream, "<BODY>\n\n<H2>Index of %s</H2>\n\n<PRE>\n",
                     req->request_uri);

    while ((dirbuf = readdir(request_dir))) {
        if (!strcmp(dirbuf->d_name, "."))
            continue;

        if (!strcmp(dirbuf->d_name, "..")) {
            bytes += fprintf(fdstream,
                             " [DIR] <A HREF=\"../\">Parent Directory</A>\n");
            continue;
        }

        if ((escname = escape_string(dirbuf->d_name, NULL)) != NULL) {
            bytes += fprintf(fdstream, " <A HREF=\"%s\">%s</A>\n",
                             escname, dirbuf->d_name);
            free(escname);
            escname = NULL;
        }
    }
    closedir(request_dir);
    bytes += fprintf(fdstream, "</PRE>\n\n</BODY>\n</HTML>\n");

    fclose(fdstream);

    chdir(server_root);

    req->filesize = bytes;      /* for logging transfer size */
    return 0;                   /* success */
}
