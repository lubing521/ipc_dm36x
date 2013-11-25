#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include <image.h>
#include <asm/types.h>
#include <asm/byteorder.h>
#include <system_default.h>
#include <ctype.h>
#include <fcntl.h>
#include <time.h>
#include <sys/ioctl.h>
#include <sys/types.h>

#include <asm/types.h>
#include "mtd/mtd-user.h"

#define SUCCESS			0
#define FAIL				-1
#define ENVALID_PRM		-2
#define UNKOWN			-3
#define FILE_NAME_SIZE	80
#define KERNELBLOCK		"/dev/mtd2"
#define FILESYSBLOCK	"/dev/mtd3"
#define TEMP_PATH		"/tmp"
#define TEMP_FILE		"tmpfile.txt"
#define FILESYS_PRE		"cramfs"

#ifdef U_DEBUG
#define DBG(fmt, args...)		write(STDOUT_FILENO, out_buf, sprintf(out_buf, fmt, ##args) + 1)
#else
#define DBG(fmt, args...)
#endif

#define OUTPUT(fmt, args...)	write(STDOUT_FILENO, out_buf, sprintf(out_buf, fmt, ##args) + 1)

typedef struct _FileInfo{
	long nStartOffset;
	long nEndOffset;
	int fd;
	long nLength;
	char strFileName[FILE_NAME_SIZE + 1];
}FileInfo;

const char update_html_head[] =
	"HTTP/1.1 200 OK\r\nContent-type: text/html\r\nPragma: no-cache\r\n"
	"Cache-Control: no-store\r\n\r\n"
	"<HTML><HEAD><TITLE>Firmware Update</TITLE></HEAD>\n"
	"<style>\n BODY { PADDING-RIGHT: 0px; MARGIN-TOP: 10px; PADDING-LEFT: 10px; MARGIN-LEFT: auto; WIDTH: auto; MARGIN-RIGHT: auto; background-color:#182439; FONT-FAMILY: Tahoma, Geneva, Arial, \"Arial Narrow\", Verdana, sans-serif; font-size:14px; color:#FFFFFF;overflow-x: hidden;overflow-y: hidden} </style>";

const char update_html_end[] = "";

static int gLastI = -1, gLastRead = 0;
static char out_buf[80];

/******************************************************************************
	Start of nandwrite global
******************************************************************************/
#define MAX_OOB_SIZE	64
#define MAX_PAGE_SIZE	2048

unsigned char oobbuf[MAX_OOB_SIZE];
unsigned char oobreadbuf[MAX_OOB_SIZE];
unsigned char writebuf[MAX_PAGE_SIZE];
int	blockalign = 1; /*default to using 16K block size */
int	forcejffs2 = 1;
int	forceyaffs = 0;
int	autoplace = 0;
int	forcelegacy = 1;
int	noecc = 0;
int	writeoob = 0;
int	pad = 1;

struct nand_oobinfo jffs2_oobinfo = {
	.useecc = MTD_NANDECC_PLACE,
	.eccbytes = 6,
	.eccpos = { 0, 1, 2, 3, 6, 7 }
};

struct nand_oobinfo yaffs_oobinfo = {
	.useecc = MTD_NANDECC_PLACE,
	.eccbytes = 6,
	.eccpos = { 8, 9, 10, 13, 14, 15}
};
/******************************************************************************
	End of nandwrite global
******************************************************************************/

extern unsigned long crc32 (unsigned long, const unsigned char *, unsigned int);
/**
*@brief Reverse a string
*@param[in,out] pStr String to reverse.
*/
void Reverse(char *pStr)
{
	int i,j;
	char temp;
	for(i = 0,j = strlen(pStr) - 1;i < j; i++, j--){
		temp = pStr[i];
		pStr[i] = pStr[j];
		pStr[j] = temp;
	}
}
/**
*@brief Take the file name from string
*@param[in] pbuf String include file name in it.
*@param[in] nBufSize String size.
*@param[out] pFileName File name get from string.
*/
void TakeFileName(char *pbuf, int nBufSize, char *pFileName)
{
	int i,count = 0;
	for(i = 0;i < nBufSize;i++){
		if(pbuf[i] != '"')
			continue;
		gLastI = i;
		while(--i >= 0 ){
			if(pbuf[i] == '/' || pbuf[i] == '\\')
				break;
			pFileName[count++] = pbuf[i];
		}
		break;
	}
	pFileName[count] = '\0';
	Reverse(pFileName);
	return ;
}
/**
*@brief Get file name from stdin
*@param[in] fd File fd.
*@param[in] pBuf Buffer to store temp date.
*@param[in] nBufSize Buffer size.
*@return File name.
*/
char* GetFileName(int fd, char *pBuf, int nBufSize)
{
	static char strFileName[FILE_NAME_SIZE + 1];
	const char target[]="filename=\"";
	int nReadBytes = 0, count = 0, i,tempI = gLastI;
	while((nReadBytes = read(fd, pBuf, nBufSize)) > 0){
		for(i = 0;i < nBufSize;i++){
			if(count == strlen(target)){
				gLastI = i;
				TakeFileName(&pBuf[i], nReadBytes - i, strFileName);
				if(strFileName[0] == '\0'){
					count = 0;
					gLastI = tempI;
				}else{
					gLastRead = nReadBytes;
					return strFileName;
				}
			} else if(pBuf[i] == target[count])
				count++;
			else
				count = 0;
		}
	}
	return NULL;
}
/**
*@brief Skip package header
*@param[in] fd File fd.
*@param[in] pBuf Buffer to store temp date.
*@param[in] nBufSize Buffer size.
*@return Value of buffer index
*/
int SkipHeader(int fd, char *pBuf, int nBufSize){
	int i,count = 0;
	const char target[]="\r\n\r\n";
	if(gLastRead > 0){
		i = (gLastI > 0) ? gLastI : 0;
		do {
			for(; i < gLastRead;i++){
				if(count == strlen(target)){
					return i;
				} else if(pBuf[i] == target[count])
					count++;
				else
					count = 0;
			}
			i = 0;
		}while((gLastRead = read(fd, pBuf, nBufSize)) > 0);
	} else {
		while((gLastRead = read(fd, pBuf, nBufSize)) > 0){
			for(i = 0; i < gLastRead;i++){
				if(count == strlen(target)){
					return i;
				} else if(pBuf[i] == target[count])
					count++;
				else
					count = 0;
			}
		}
	}
	return -1;
}
/**
*@brief Create a file
*@param[in] pFileName File name.
*@param[in] fd File fd.
*@param[in] pBuf Buffer to store temp date.
*@param[in] nBufSize Buffer size.
*@param[out] pFile File information to new file.
*@retval ENVALID_PRM gLastRead is not set correctly.
*@retval FAIL File create fail.
*@retval SUCCESS File create success.
*/
int CreateFile(char *pFileName, int fd, char *pBuf, int nBufSize, FileInfo* pFile)
{
	char path[100];
	const char target[]="\r\n-----------------------------";
	int i,count = 0;
	FILE *db;
	db = fopen("/tmp/createfile.txt", "wt");
	sprintf(path, "%s/%s", TEMP_PATH, pFileName);
	fprintf(db,"path=<%s>\n", path);
	fprintf(db,"gLastRead=%d\n", gLastRead);
	fclose(db);
	if(gLastRead <= 0)
		return ENVALID_PRM;
	i = (gLastI > 0) ? gLastI : 0;
	DBG("i = %d\n", i);
	strcpy(pFile->strFileName, pFileName);
	pFile->fd = fd;
	pFile->nStartOffset = lseek(fd, 0, SEEK_CUR) - gLastRead + i;
	do{
		DBG("gLastRead : %d\n", gLastRead);
		for(; i < gLastRead;i++){
			if(count == strlen(target)){
				gLastI = i;
				pFile->nEndOffset = lseek(fd, 0, SEEK_CUR) - gLastRead + i - count;
				pFile->nLength = pFile->nEndOffset - pFile->nStartOffset;
				DBG("File length = %ld\n", pFile->nLength);
				return SUCCESS;
			} else if(pBuf[i] == target[count]){
				count++;
			} else if(count > 0){
				count = 0;
				i--;
			} else {
//				putc(pBuf[i], fp);
			}
		}
		i = 0;
	}while((gLastRead = read(fd, pBuf, nBufSize)) > 0);
	gLastI = -1;
	return FAIL;
}
/**
*@brief Print image type
*@param[in] hdr uImage header.
*/
static void print_type (image_header_t *hdr)
{
	char *os, *arch, *type, *comp;

	switch (hdr->ih_os) {
		case IH_OS_INVALID:	os = "Invalid OS";		break;
		case IH_OS_NETBSD:	os = "NetBSD";			break;
		case IH_OS_LINUX:	os = "Linux";			break;
		case IH_OS_VXWORKS:	os = "VxWorks";			break;
		case IH_OS_QNX:		os = "QNX";			break;
		case IH_OS_U_BOOT:	os = "U-Boot";			break;
		case IH_OS_RTEMS:	os = "RTEMS";			break;
		default:		os = "Unknown OS";		break;
	}

	switch (hdr->ih_arch) {
		case IH_CPU_INVALID:	arch = "Invalid CPU";		break;
		case IH_CPU_ALPHA:	arch = "Alpha";			break;
		case IH_CPU_ARM:	arch = "ARM";			break;
		case IH_CPU_AVR32:	arch = "AVR32";			break;
		case IH_CPU_I386:	arch = "Intel x86";		break;
		case IH_CPU_IA64:	arch = "IA64";			break;
		case IH_CPU_MIPS:	arch = "MIPS";			break;
		case IH_CPU_MIPS64:	arch = "MIPS 64 Bit";		break;
		case IH_CPU_PPC:	arch = "PowerPC";		break;
		case IH_CPU_S390:	arch = "IBM S390";		break;
		case IH_CPU_SH:		arch = "SuperH";		break;
		case IH_CPU_SPARC:	arch = "SPARC";			break;
		case IH_CPU_SPARC64:	arch = "SPARC 64 Bit";		break;
		case IH_CPU_M68K:	arch = "M68K"; 			break;
		case IH_CPU_MICROBLAZE:	arch = "Microblaze"; 		break;
		case IH_CPU_NIOS:	arch = "Nios";			break;
		case IH_CPU_NIOS2:	arch = "Nios-II";		break;
		default:		arch = "Unknown Architecture";	break;
	}

	switch (hdr->ih_type) {
		case IH_TYPE_INVALID:	type = "Invalid Image";		break;
		case IH_TYPE_STANDALONE:type = "Standalone Program";	break;
		case IH_TYPE_KERNEL:	type = "Kernel Image";		break;
		case IH_TYPE_RAMDISK:	type = "RAMDisk Image";		break;
		case IH_TYPE_MULTI:	type = "Multi-File Image";	break;
		case IH_TYPE_FIRMWARE:	type = "Firmware";		break;
		case IH_TYPE_SCRIPT:	type = "Script";		break;
		case IH_TYPE_FLATDT:	type = "Flat Device Tree";	break;
		default:		type = "Unknown Image";		break;
	}

	switch (hdr->ih_comp) {
		case IH_COMP_NONE:	comp = "uncompressed";		break;
		case IH_COMP_GZIP:	comp = "gzip compressed";	break;
		case IH_COMP_BZIP2:	comp = "bzip2 compressed";	break;
		default:		comp = "unknown compression";	break;
	}

	OUTPUT("%s %s %s (%s)", arch, os, type, comp);
}
/**
 *@brief Print size

 * print sizes as "xxx kB", "xxx.y kB", "xxx MB" or "xxx.y MB" as needed;
 * allow for optional trailing string (like "\n")
 *@param[in] size Size to print.
 *@param[in] s Tailing string.
 */
void print_size (unsigned long size, const char *s)
{
	unsigned long m, n;
	unsigned long d = 1 << 20;		/* 1 MB */
	char  c = 'M';

	if (size < d) {			/* print in kB */
		c = 'k';
		d = 1 << 10;
	}

	n = size / d;

	m = (10 * (size - (n * d)) + (d / 2) ) / d;

	if (m >= 10) {
		m -= 10;
		n += 1;
	}

	OUTPUT ("%2ld", n);
	if (m) {
		OUTPUT (".%ld", m);
	}
	OUTPUT (" %cB%s", c, s);
}
/**
*@brief Print image header
*@param[in] hdr uImage header.
*/
void print_image_hdr (image_header_t *hdr)
{
	OUTPUT("<H6>   Image Name:   %.*s\n", IH_NMLEN, hdr->ih_name);
	OUTPUT("<H6>   Image Type:   ");print_type(hdr);
	OUTPUT ("<H6>\n   Data Size:    %d Bytes = ", ntohl(hdr->ih_size));
	print_size (ntohl(hdr->ih_size), "\n");
	OUTPUT ("<H6>   Load Address: %08x\n"
		"<H6>   Entry Point:  %08x\n",
		 ntohl(hdr->ih_load), ntohl(hdr->ih_ep));

	if (hdr->ih_type == IH_TYPE_MULTI) {
		int i;
		unsigned long len;
		unsigned long *len_ptr;
		len_ptr = (unsigned long *)
				((unsigned long)hdr + sizeof(image_header_t));

		OUTPUT("<H6>   Contents:\n");
		for (i=0; (len = ntohl(*len_ptr)); ++i, ++len_ptr) {
			OUTPUT ("<H6>   Image %d: %8ld Bytes = ", i, len);
			print_size (len, "\n");
		}
	}
}
/**
*@brief Write file to nand
*@param[in] pFile File to write.
*@param[in] pBlockName Block device name.
*@param[in] offset Offset to block start.
*@retval SUCCESS Nand write complete.
*@retval FAIL Error occures.
*/
int FlashNand(FileInfo* pFile, char *pBlockName, unsigned long offset)
{
	char cmd[100];
	int cnt, fd, imglen = 0, pagelen, baderaseblock, blockstart = -1;
	struct mtd_info_user meminfo;
	struct mtd_oob_buf oob;
	loff_t offs;
	int ret, readlen;
	int oobinfochanged = 0;
	struct nand_oobinfo old_oobinfo;
	ret = SUCCESS;
	sprintf(cmd, "/usr/sbin/flash_eraseall -j %s > %s/%s\n", pBlockName,
			TEMP_PATH, TEMP_FILE);
	if(system(cmd)){
		OUTPUT("<H6>Fail on erase block\r\n");
		ret = FAIL;
		goto EXIT_FLASHNAND;
	}
#if 0
	sprintf(cmd, "/usr/sbin/nandwrite -s %ld -f -p -j %s %s/%s > %s/%s\n", offset, pBlockName,
			TEMP_PATH, pFile->strFileName, TEMP_PATH, TEMP_FILE);
	DBG(cmd);
	if(system(cmd)){
		OUTPUT("<H6>Fail on update new firmware\r\n");
		sprintf(cmd, "rm -f %s/%s\n", TEMP_PATH, TEMP_FILE);
		system(cmd);
		return FAIL;
	}
#else
	/* mtdoffset = offset, forcelegacy = 1, pad = 1, forcejffs2 = 1,
	mtd_device = pBlockName, img = pFile */
	memset(oobbuf, 0xff, sizeof(oobbuf));
	/* Open the device */
	if ((fd = open(pBlockName, O_RDWR)) == -1) {
		OUTPUT("<H6>open flash error\r\n");
		ret = FAIL;
		goto EXIT_FLASHNAND;
	}
	/* Fill in MTD device capability structure */
	if (ioctl(fd, MEMGETINFO, &meminfo) != 0) {
		OUTPUT("<H6>MEMGETINFO error\r\n");
		close(fd);
		ret = FAIL;
		goto EXIT_FLASHNAND;
	}
        /* Set erasesize to specified number of blocks - to match jffs2 (virtual) block size */
        meminfo.erasesize *= blockalign;
	/* Make sure device page sizes are valid */
	if (!(meminfo.oobsize == 16 && meminfo.oobblock == 512) &&
	    !(meminfo.oobsize == 8 && meminfo.oobblock == 256) &&
	    !(meminfo.oobsize == 64 && meminfo.oobblock == 2048)) {
		OUTPUT("<H6>Unknown flash (not normal NAND)\r\n");
		close(fd);
		ret = FAIL;
		goto EXIT_FLASHNAND;
	}
	/* Read the current oob info */
	if (ioctl (fd, MEMGETOOBSEL, &old_oobinfo) != 0) {
		OUTPUT("<H6>MEMGETOOBSEL error\r\n");
		close (fd);
		ret = FAIL;
		goto EXIT_FLASHNAND;
	}
	/*
	 * force oob layout for jffs2 or yaffs ?
	 * Legacy support
	 */
	if (forcejffs2 || forceyaffs) {
		struct nand_oobinfo *oobsel = forcejffs2 ? &jffs2_oobinfo : &yaffs_oobinfo;

		if (autoplace) {
			OUTPUT("<H6>Autoplacement is not possible for legacy -j/-y options\n");
			ret = FAIL;
			goto restoreoob;
		}
		if ((old_oobinfo.useecc == MTD_NANDECC_AUTOPLACE) && !forcelegacy) {
			OUTPUT("<H6>Use -f option to enforce legacy placement on autoplacement enabled mtd device\n");
			ret = FAIL;
			goto restoreoob;
		}
		if (meminfo.oobsize == 8) {
    			if (forceyaffs) {
				OUTPUT("<H6>YAFSS cannot operate on 256 Byte page size");
				ret = FAIL;
				goto restoreoob;
			}
			/* Adjust number of ecc bytes */
			jffs2_oobinfo.eccbytes = 3;
		}

		if (ioctl (fd, MEMSETOOBSEL, oobsel) != 0) {
			OUTPUT("<H6>MEMSETOOBSEL error\r\n");
			ret = FAIL;
			goto restoreoob;
		}
	}
	oob.length = meminfo.oobsize;
	oob.ptr = noecc ? oobreadbuf : oobbuf;

	/* Set file to start */
	lseek(pFile->fd, pFile->nStartOffset, SEEK_SET);
	// get image length
   	imglen = pFile->nLength;
	pagelen = meminfo.oobblock + ((writeoob == 1) ? meminfo.oobsize : 0);
	// Check, if file is pagealigned
	if ((!pad) && ((imglen % pagelen) != 0)) {
		OUTPUT("<H6>Input file is not page aligned\n");
		ret = FAIL;
		goto restoreoob;
	}
	// Check, if length fits into device
	if ( ((imglen / pagelen) * meminfo.oobblock) > (meminfo.size - offset)) {
		OUTPUT("<H6>Image %d bytes, NAND page %d bytes, OOB area %u bytes, device size %u bytes\n",
				imglen, pagelen, meminfo.oobblock, meminfo.size);
		OUTPUT("<H6>Input file does not fit into device error\r\n");
		ret = FAIL;
		goto restoreoob;
	}
	/* Get data from input and write to the device */
	while (imglen && (offset < meminfo.size)) {
		// new eraseblock , check for bad block(s)
		// Stay in the loop to be sure if the mtdoffset changes because
		// of a bad block, that the next block that will be written to
		// is also checked. Thus avoiding errors if the block(s) after the
		// skipped block(s) is also bad (number of blocks depending on
		// the blockalign
		while (blockstart != (offset & (~meminfo.erasesize + 1))) {
			blockstart = offset & (~meminfo.erasesize + 1);
			offs = blockstart;
		        baderaseblock = 0;
			DBG("Writing data to block %x\n", blockstart);

		        /* Check all the blocks in an erase block for bad blocks */
			do {
			   	if ((ret = ioctl(fd, MEMGETBADBLOCK, &offs)) < 0) {
					OUTPUT("<H6>ioctl(MEMGETBADBLOCK) error\r\n");
					ret = FAIL;
					goto restoreoob;
				}
				if (ret == 1) {
						baderaseblock = 1;
						DBG("Bad block at %x, %u block(s) from %x will be skipped\n", (int) offs, blockalign, blockstart);
					}

				if (baderaseblock) {
					offset = blockstart + meminfo.erasesize;
				}
			        offs +=  meminfo.erasesize / blockalign ;
			} while ( offs < blockstart + meminfo.erasesize );

		}

		readlen = meminfo.oobblock;
		if (pad && (imglen < readlen))
		{
			readlen = imglen;
			memset(writebuf + readlen, 0xff, meminfo.oobblock - readlen);
		}

		/* Read Page Data from input file */
		if ((cnt = read(pFile->fd, writebuf, readlen)) != readlen) {
			if (cnt == 0)	// EOF
				break;
			OUTPUT("<H6>File I/O error on input file\r\n");
			ret =FAIL;
			goto restoreoob;
		}

		if (writeoob) {
			/* Read OOB data from input file, exit on failure */
			if ((cnt = read(pFile->fd, oobreadbuf, meminfo.oobsize)) != meminfo.oobsize) {
				OUTPUT("<H6>File I/O error on input file");
				ret = FAIL;
				goto restoreoob;
			}
			if (!noecc) {
				int i, start, len;
				/*
				 *  We use autoplacement and have the oobinfo with the autoplacement
				 * information from the kernel available
				 *
				 * Modified to support out of order oobfree segments,
				 * such as the layout used by diskonchip.c
				 */
				if (!oobinfochanged && (old_oobinfo.useecc == MTD_NANDECC_AUTOPLACE)) {
					for (i = 0;old_oobinfo.oobfree[i][1]; i++) {
						/* Set the reserved bytes to 0xff */
						start = old_oobinfo.oobfree[i][0];
						len = old_oobinfo.oobfree[i][1];
						memcpy(oobbuf + start,
							oobreadbuf + start,
							len);
					}
				} else {
					/* Set at least the ecc byte positions to 0xff */
					start = old_oobinfo.eccbytes;
					len = meminfo.oobsize - start;
					memcpy(oobbuf + start,
						oobreadbuf + start,
						len);
				}
			}
			/* Write OOB data first, as ecc will be placed in there*/
			oob.start = offset;
			if (ioctl(fd, MEMWRITEOOB, &oob) != 0) {
				OUTPUT("<H6>ioctl(MEMWRITEOOB) error\r\n");
				ret = FAIL;
				goto restoreoob;
			}
			imglen -= meminfo.oobsize;
		}

		/* Write out the Page data */
		if (pwrite(fd, writebuf, meminfo.oobblock, offset) != meminfo.oobblock) {
			OUTPUT("<H6>pwrite error\r\n");
			ret = FAIL;
			goto restoreoob;
		}
		imglen -= readlen;
		offset += meminfo.oobblock;
	}
restoreoob:
	if (oobinfochanged) {
		if (ioctl (fd, MEMSETOOBSEL, &old_oobinfo) != 0) {
			OUTPUT ("<H6>MEMSETOOBSEL error\r\n");
			close (fd);
			ret = FAIL;
			goto EXIT_FLASHNAND;
		}
	}

	close(fd);

	if (imglen > 0) {
		OUTPUT ("<H6>Data did not fit into device, due to bad blocks\n");
		ret = FAIL;
	}
#endif
EXIT_FLASHNAND:
	return ret;
}
/**
*@brief Write kernel to flash
*@param[in] pFileName Kernel file name.
*@retval SUCCESS Kernel write success.
*@retval FAIL Error occures.
*/
int FlashKernel(char *pFileName)
{
	char cmd[80], path[100];
//	FILE* fp;
	sprintf(path, "%s/update.sh", TEMP_PATH);
#if 0
	fp = fopen(path, "wt");
	if(fp){
		fprintf(fp, "/usr/sbin/flash_eraseall -j %s\n", KERNELBLOCK);
		fprintf(fp ,"/usr/sbin/nandwrite -p -j %s %s/%s\n", KERNELBLOCK, TEMP_PATH,
				pFileName);
		fclose(fp);
	} else
		return FAIL;
	sprintf(cmd, "chmod 700 %s/update.sh", TEMP_PATH);
	if(system(cmd)){
		OUTPUT("<H6>Can't change mode\r\n");
		return FAIL;
	}
	sprintf(cmd, "%s/update.sh\n", TEMP_PATH);
	return system(cmd);
#else
	sprintf(cmd, "/usr/sbin/flash_eraseall -j %s > %s/%s\n", KERNELBLOCK,
			TEMP_PATH, TEMP_FILE);
	if(system(cmd)){
		OUTPUT("<H6>Fail on erase kernel block\r\n");
		sprintf(cmd, "rm -f %s/%s\n", TEMP_PATH, TEMP_FILE);
		system(cmd);
		return FAIL;
	}
	sprintf(cmd, "/usr/sbin/nandwrite -p -j %s %s/%s > %s/%s\n", KERNELBLOCK,
			TEMP_PATH, pFileName, TEMP_PATH, TEMP_FILE);
	if(system(cmd)){
		OUTPUT("<H6>Fail on update kernel\r\n");
		sprintf(cmd, "rm -f %s/%s\n", TEMP_PATH, TEMP_FILE);
		system(cmd);
		return FAIL;
	}
	sprintf(cmd, "rm -f %s/%s\n", TEMP_PATH, TEMP_FILE);
	system(cmd);
	return SUCCESS;
#endif
}
/**
*@brief Do kernel update
*@param[in] pFile Kernel file information.
*@retval SUCCESS Update success.
*@retval FAIL Fail to update.
*@retval UNKOWN Unkown file format.
*/
int DoKernelUpdate(FileInfo* pFile)
{
	image_header_t hdr;
	unsigned long checksum,len, data;
	long nFileSize;
	void *pImage = NULL;
	int ret = SUCCESS;
	lseek(pFile->fd, pFile->nStartOffset, SEEK_SET);
	if(read(pFile->fd, &hdr, sizeof(hdr)) != sizeof(hdr)){
		ret = UNKOWN;
		goto EXIT_UPDATE_K;
	}
	if(ntohl(hdr.ih_magic) != IH_MAGIC){
		ret = UNKOWN;
		goto EXIT_UPDATE_K;
	}
	DBG("Valid magic number\r\n");
	data = (unsigned long)&hdr;
	len = sizeof(hdr);
	checksum = ntohl(hdr.ih_hcrc);
	hdr.ih_hcrc = 0;
	if (crc32 (0, (unsigned char *)data, len) != checksum) {
		DBG ("Bad Header Checksum\n");
		ret = UNKOWN;
		goto EXIT_UPDATE_K;
	}
	print_image_hdr(&hdr);
	len  = ntohl(hdr.ih_size);
	pImage = malloc(len);
	if(pImage == NULL){
		OUTPUT("<H6>No enough memory to cache uImage\r\n");
		ret = FAIL;
		goto EXIT_UPDATE_K;
	}
	data = (unsigned long)pImage;
	if(read(pFile->fd, pImage, len) != len){
		DBG("<H6>File size error\r\n");
		ret = UNKOWN;
		goto EXIT_UPDATE_K;
	}
	DBG("   Verifying Checksum ... \r\n");
	if (crc32 (0, (unsigned char *)data, len) != ntohl(hdr.ih_dcrc)) {
		DBG("Bad Data CRC\n");
		ret = UNKOWN;
		goto EXIT_UPDATE_K;
	}
	DBG("OK\n");
	if((nFileSize = pFile->nLength) < 0){
		OUTPUT("<H6>Error on get file size\r\n");
		ret = FAIL;
		goto EXIT_UPDATE_K;
	}
	data = nFileSize;
	if(data != (len + sizeof(hdr))){
		OUTPUT("<H6>Unkown data found at tail\r\n");
		ret = FAIL;
		goto EXIT_UPDATE_K;
	}
	if(FlashNand(pFile, KERNELBLOCK, 0) != SUCCESS){
		ret = FAIL;
		goto EXIT_UPDATE_K;
	}
EXIT_UPDATE_K:
	if(pImage)
		free(pImage);
	return ret;
}
/**
*@brief Root file system update
*@param[in] pFile File information.
*@retval SUCCESS Update success.
*@retval FAIL Update fail.
*@retval UNKOWN Unkown file format.
*/
int DoRootFileSysUpdate(FileInfo* pFile)
{
//	char cmd[80];
	/* We only check filename */
	DBG("Enter %s\r\n", __func__);
	if(strncmp(FILESYS_PRE, pFile->strFileName, strlen(FILESYS_PRE))){
		DBG("Not root file\r\n");
		return UNKOWN;
	}
	DBG("File name ok\r\n");
#if 0
	/* CRC check */
	sprintf(cmd, "gzip -t %s/%s\n", TEMP_PATH, pFileName);
	if(system(cmd)){
		DBG("Can't check\r\n");
		return FAIL;
	}
#endif
	return FlashNand(pFile, FILESYSBLOCK, 0);
}
/**
*@brief Firmware update
*@param[in] pFile File information.
*@retval SUCCESS Firmware update successfully.
*@retval FAIL Firmware update failed.
*@retval UNKOWN Unkown file format.
*/
int DoFirmwareUpdate(FileInfo* pFile)
{
	int ret;
	char cmd[80];
	ret = DoKernelUpdate(pFile);
	if(ret == SUCCESS){
		OUTPUT("<H4>\nKernel update success,\r\n");
	} else if(ret == UNKOWN){
		if((ret = DoRootFileSysUpdate(pFile)) == SUCCESS){
			OUTPUT("<H4>File system update success,\r\n");
			sprintf(cmd, "rm -f %s\n", SYS_FILE);
			system(cmd);
			sprintf(cmd, "rm -f %s\n", LOG_FILE);
			system(cmd);
		}
	}
	return ret;
}
/**
*
*/
int main(int argc, char **argv)
{
	char buffer[512],*pFileName;
	struct stat st;
	FileInfo tFile;
#ifdef U_DEBUG
	FILE *fp;
#endif

	fstat(STDIN_FILENO, &st);
	write(STDOUT_FILENO, update_html_head, sizeof(update_html_head));
	if((pFileName = GetFileName(STDIN_FILENO, buffer, sizeof(buffer))) != NULL){
#ifdef U_DEBUG
		fp = fopen("/tmp/main.dbg", "wt");
		fprintf(fp, "pFileName=<%s>\n",pFileName);
#endif
		gLastI = SkipHeader(STDIN_FILENO, buffer, sizeof(buffer));
#ifdef U_DEBUG
		fprintf(fp, "gLastI=%d\n",gLastI);
#endif
		if(CreateFile(pFileName, STDIN_FILENO, buffer, sizeof(buffer), &tFile)==SUCCESS){
			OUTPUT("<H4>File create success\r\n");
			OUTPUT("<H4>\r\n");
			switch(DoFirmwareUpdate(&tFile)){
				case SUCCESS:
					OUTPUT("Firmware update success\r\n");
					OUTPUT("<H4> Please Restart IPNC by Clicking on \"Restart Camera\" Button\r\n");
					break;
				case UNKOWN:
					OUTPUT("<H4>Unknown file format\r\n");
					break;
				default:
					OUTPUT("<H4>Unknown error\r\n");
					break;
			}

		}else
			OUTPUT("<H4>Fail to create file\r\n");
#ifdef U_DEBUG
		fclose(fp);
#endif
	} else {
		OUTPUT("<H3>Unknown file.</H3>\r\n");
	}
	write(STDOUT_FILENO, update_html_end, sizeof(update_html_end));

	return SUCCESS;
}

