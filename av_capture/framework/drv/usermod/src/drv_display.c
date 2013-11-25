
#include <drv_display.h>
#include <osa_cmem.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>
#include <media/davinci/davinci_display.h>
#include <osa_file.h>
#include <drv_frameCopy.h>
#include <drv_csl.h>

//#define DRV_DISPLAY_DEBUG

typedef struct {

  DRV_DisplayWinConfig  config;
  DRV_DisplayBufInfo    bufInfo;
  int fd;
  int putCount;

  int cropWidth;
  int cropHeight;

  Uint32 *regs;

} DRV_DisplayObj;


int DRV_displayChangeSysFsAttrib(char *attribute, char *value)
{
	int sysfd = -1;
	char init_val[32];
	char attrib_tag[128];

	bzero(init_val, sizeof(init_val));
	strcpy(attrib_tag, "/sys/class/davinci_display/");
	strcat(attrib_tag, attribute);

	sysfd = open(attrib_tag, O_RDWR);
	if (!sysfd) {
		OSA_ERROR("cannot open %s\n", attrib_tag);
		return OSA_EFAIL;
	}

	#ifdef DRV_DISPLAY_DEBUG
	OSA_printf(" DRV DISPLAY: %s was opened successfully\n", attrib_tag);
	read(sysfd, init_val, 32);
	lseek(sysfd, 0, SEEK_SET);
	OSA_printf(" DRV DISPLAY: Current %s = %s\n", attribute, init_val);
  #endif

	write(sysfd, value, 1 + strlen(value));

	#ifdef DRV_DISPLAY_DEBUG
	lseek(sysfd, 0, SEEK_SET);
	memset(init_val, '\0', 32);
	read(sysfd, init_val, 32);
	lseek(sysfd, 0, SEEK_SET);
	OSA_printf(" DRV DISPLAY: New    %s = %s\n", attribute, init_val);
  #endif

	close(sysfd);
	return OSA_SOK;
}

int DRV_displaySetMode(int mode)
{
  int status=OSA_EFAIL;

  switch(mode)
  {
    case DRV_DISPLAY_MODE_NTSC:
    case DRV_DISPLAY_MODE_PAL:
      status = DRV_displayChangeSysFsAttrib("ch0/output", "COMPOSITE");
      if(status==OSA_SOK) {
        if(mode==DRV_DISPLAY_MODE_NTSC)
          status = DRV_displayChangeSysFsAttrib("ch0/mode", "NTSC");
        else
          status = DRV_displayChangeSysFsAttrib("ch0/mode", "PAL");
      }
      break;
    case DRV_DISPLAY_MODE_1080I:
    case DRV_DISPLAY_MODE_720P:
      status = DRV_displayChangeSysFsAttrib("ch0/output", "COMPONENT");
      if(status==OSA_SOK) {
        if(mode==DRV_DISPLAY_MODE_720P)
          status = DRV_displayChangeSysFsAttrib("ch0/mode", "720P-60");
        else
          status = DRV_displayChangeSysFsAttrib("ch0/mode", "1080I-30");
      }
      break;
  }

  if(status!=OSA_SOK) {
    OSA_ERROR("display mode = %d\n", mode);
  }

  return status;
}

int DRV_displayOpen(DRV_DisplayWinHndl *hndl, DRV_DisplayWinConfig *config)
{
  char deviceName[20];
  struct v4l2_capability cap;
  int                      fd;
  int                      i = 0,ret = 0;
  struct v4l2_requestbuffers reqbuf;
  struct v4l2_format setfmt;
  int offset, bufSize;
  DRV_DisplayObj *pObj;
  DRV_FrameCopyPrm frameCopy;

  hndl->hndl = NULL;

  if(config->numBuf>DRV_DISPLAY_BUF_MAX) {
    OSA_ERROR("exceeded max buffer limit of %d", DRV_DISPLAY_BUF_MAX);
    return OSA_EFAIL;
  }

  switch(config->winId)
  {
    case DRV_DISPLAY_VID_WIN_0:
#ifdef BOARD_UD_DVR
      strcpy(deviceName, "/dev/video3");
#else
      strcpy(deviceName, "/dev/video2");
#endif
      break;
    default:
      OSA_ERROR("Invalid window ID %d\n", config->winId);
      // other windows not supported so far
      return OSA_EFAIL;
      break;
  }

  /* Open video display device */
  fd = open(deviceName, O_RDWR);

  if(fd < 0) {
    OSA_ERROR("open device %s\n", deviceName);
    return OSA_EFAIL;
  }

  hndl->hndl = OSA_memAlloc(sizeof(DRV_DisplayObj));

  pObj = (DRV_DisplayObj*)hndl->hndl;

  memset(pObj, 0, sizeof(*pObj));

  pObj->regs = (Uint32*)((Uint32)gCSL_vpssHndl.regBaseVirtAddr + 0x1c00);

  pObj->fd = fd;
  memcpy(&pObj->config, config, sizeof(config));

  pObj->putCount = 0;

  if(config->offsetH > config->width)
      bufSize = config->offsetH * config->height;
  else
      bufSize = config->width * config->height;

  if(config->dataFormat==DRV_DATA_FORMAT_YUV420)
    bufSize = (bufSize*3)/2;
  else
    bufSize = bufSize*2;

  pObj->bufInfo.numBuf = config->numBuf;
  pObj->bufInfo.bufSize = bufSize;
  pObj->bufInfo.dataFormat = config->dataFormat;
  pObj->bufInfo.width = config->width;
  pObj->bufInfo.height = config->height;
  pObj->bufInfo.offsetH = config->offsetH;
  pObj->bufInfo.offsetV = config->offsetV;

  pObj->config.startX = config->startX;
  pObj->config.startY = config->startY;
  pObj->cropWidth     = config->cropWidth;
  pObj->cropHeight    = config->cropHeight;

	ret = ioctl(fd, VIDIOC_QUERYCAP, &cap);
	if(ret<0) {
		OSA_ERROR("VIDIOC_QUERYCAP on %s\n", deviceName);
		goto error_exit;
	}

	if (!(cap.capabilities & V4L2_CAP_VIDEO_OUTPUT)){
		OSA_ERROR("Does not support display capability on %s\n", deviceName);
		goto error_exit;
	}

	if (!(cap.capabilities & V4L2_CAP_STREAMING)) {
		OSA_ERROR("Does not support streaming capability on %s\n", deviceName);
		goto error_exit;
	}

	reqbuf.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
	reqbuf.count = config->numBuf;
	reqbuf.memory = V4L2_MEMORY_USERPTR;
	ret = ioctl(fd, VIDIOC_REQBUFS, &reqbuf);
	if (ret < 0) {
		OSA_ERROR("Could not allocate buffers on %s\n", deviceName);
		goto error_exit;
	}

	memset(&setfmt, 0, sizeof(setfmt));
	setfmt.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
	if(config->dataFormat==DRV_DATA_FORMAT_YUV420) {
		setfmt.fmt.pix.pixelformat = V4L2_PIX_FMT_NV12;
		setfmt.fmt.pix.bytesperline = config->offsetH;
	}
	else {
		setfmt.fmt.pix.pixelformat = V4L2_PIX_FMT_UYVY;
		setfmt.fmt.pix.bytesperline = config->offsetH*2;
	}

  setfmt.fmt.pix.height = config->height;
#ifdef BOARD_UD_DVR
	setfmt.fmt.pix.width = config->offsetH;
#else
	setfmt.fmt.pix.width = config->width;
#endif
  setfmt.fmt.pix.sizeimage =  bufSize;

	ret = ioctl(fd, VIDIOC_S_FMT, &setfmt);
  if(ret<0) {
    OSA_ERROR("VIDIOC_S_FMT on %s\n", deviceName);
    goto error_exit;
  }

  if(config->dataFormat==DRV_DATA_FORMAT_YUV420) {
    offset = config->offsetH*config->offsetV;
    ret = ioctl(fd, VIDIOC_S_COFST, &offset);
    if(ret<0) {
      OSA_ERROR("VIDIOC_S_COFST on %s\n", deviceName);
      goto error_exit;
    }
  }

  if(config->bufFlag == 0)
  {
	  frameCopy.srcPhysAddr = NULL;
	  frameCopy.srcVirtAddr = NULL;
	  frameCopy.dstPhysAddr = NULL;
	  frameCopy.dstVirtAddr = NULL;
	  frameCopy.srcOffsetH  = 0;
	  frameCopy.srcOffsetV  = 0;
	  frameCopy.dstOffsetH  = config->offsetH;
	  frameCopy.dstOffsetV  = config->offsetV;
	  frameCopy.copyWidth   = config->width;
	  frameCopy.copyHeight  = config->height;
	  frameCopy.dataFormat  = config->dataFormat;
	  frameCopy.srcStartX   = 0;
	  frameCopy.srcStartY   = 0;
	  frameCopy.dstStartX   = 0;
	  frameCopy.dstStartY   = 0;

	  for(i=0; i<pObj->bufInfo.numBuf; i++) {

		pObj->bufInfo.virtAddr[i] = OSA_cmemAlloc(bufSize, 32);
		if(pObj->bufInfo.virtAddr[i]==NULL) {
		  OSA_ERROR("Memory allocation\n");
		  goto error_exit;
		}
		pObj->bufInfo.physAddr[i] = OSA_cmemGetPhysAddr(pObj->bufInfo.virtAddr[i]);
		if(pObj->bufInfo.physAddr[i]==NULL) {
		  OSA_ERROR("Address translation\n");
		  goto error_exit;
		}

		frameCopy.dstPhysAddr = pObj->bufInfo.physAddr[i];
		frameCopy.dstVirtAddr = pObj->bufInfo.virtAddr[i];

		ret = DRV_frameCopy(NULL, &frameCopy);
		if(ret!=OSA_SOK) {
		  OSA_ERROR("DRV_frameCopy(%d)\n", i);
		}
	  }
  }
  else
  {
	  bufSize = config->offsetH * config->offsetV;

	  if(config->dataFormat == DRV_DATA_FORMAT_YUV420)
	  {
	      bufSize = (bufSize * 3)/2;
	  }
	  else
	  {
	      bufSize *= 2;
	  }

	  pObj->bufInfo.virtAddr[0] = OSA_cmemAlloc(bufSize, 32);
	  if(pObj->bufInfo.virtAddr[0]==NULL) {
	    OSA_ERROR("Memory allocation\n");
	    goto error_exit;
	  }
	  pObj->bufInfo.physAddr[0] = OSA_cmemGetPhysAddr(pObj->bufInfo.virtAddr[0]);
	  if(pObj->bufInfo.physAddr[0]==NULL) {
	    OSA_ERROR("Address translation\n");
	    goto error_exit;
	  }

	  frameCopy.srcPhysAddr = NULL;
	  frameCopy.srcVirtAddr = NULL;
	  frameCopy.dstPhysAddr = pObj->bufInfo.physAddr[0];
	  frameCopy.dstVirtAddr = pObj->bufInfo.virtAddr[0];
	  frameCopy.srcOffsetH  = 0;
	  frameCopy.srcOffsetV  = 0;
	  frameCopy.dstOffsetH  = config->offsetH;
	  frameCopy.dstOffsetV  = config->offsetV;
	  frameCopy.copyWidth   = config->offsetH;
	  frameCopy.copyHeight  = config->offsetV;
	  frameCopy.dataFormat  = config->dataFormat;
	  frameCopy.srcStartX   = 0;
	  frameCopy.srcStartY   = 0;
	  frameCopy.dstStartX   = 0;
	  frameCopy.dstStartY   = 0;

	  ret = DRV_frameCopy(NULL, &frameCopy);
	  if(ret!=OSA_SOK)
	  {
	      OSA_ERROR("DRV_frameCopy(%d)\n", i);
	  }
  }// if(config->bufFlag == 0)

  DRV_displaySetWinZoom(hndl, config->zoomH, config->zoomV);
  DRV_displaySetWinPos(hndl, config->startX, config->startY);
  DRV_displaySetWinSize(hndl, config->cropWidth, config->cropHeight);
  DRV_displayExpand(hndl, config->expandH, config->expandV);

  return OSA_SOK;

error_exit:

  if(pObj->fd>=0)
    close(pObj->fd);

  if(pObj) {
    for(i=0; i<pObj->bufInfo.numBuf; i++) {
      if(pObj->bufInfo.virtAddr[i])
        OSA_cmemFree(pObj->bufInfo.virtAddr[i]);
    }
    OSA_memFree(pObj);
    hndl->hndl = NULL;
  }

  return OSA_EFAIL;
}

int DRV_displayClose(DRV_DisplayWinHndl *hndl)
{
  int i;
  DRV_DisplayObj *pObj;

  if(hndl==NULL)
    return OSA_EFAIL;

  if(hndl->hndl==NULL)
    return OSA_EFAIL;

  pObj = (DRV_DisplayObj *)hndl->hndl;

  if(pObj->fd>=0) {
    DRV_displayEnable(hndl, FALSE);
    close(pObj->fd);
  }

  if(pObj) {
    for(i=0; i<pObj->bufInfo.numBuf; i++) {
      if(pObj->bufInfo.virtAddr[i])
        OSA_cmemFree(pObj->bufInfo.virtAddr[i]);
    }
    OSA_memFree(pObj);
    hndl->hndl = NULL;
  }

  return OSA_SOK;
}

int DRV_displayGetBufInfo(DRV_DisplayWinHndl *hndl, DRV_DisplayBufInfo *info)
{
  DRV_DisplayObj *pObj;

  if(hndl==NULL)
    return OSA_EFAIL;

  if(hndl->hndl==NULL)
    return OSA_EFAIL;

  pObj = (DRV_DisplayObj *)hndl->hndl;

  memcpy(info, &pObj->bufInfo, sizeof(pObj->bufInfo));

  return OSA_SOK;
}

int DRV_displayEnable(DRV_DisplayWinHndl *hndl, Bool enable)
{
  DRV_DisplayObj *pObj;
  enum v4l2_buf_type type;
  int status;

  if(hndl==NULL)
    return OSA_EFAIL;

  if(hndl->hndl==NULL)
    return OSA_EFAIL;

  pObj = (DRV_DisplayObj *)hndl->hndl;

  type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
  if(enable) {
    status = ioctl(pObj->fd, VIDIOC_STREAMON, &type);
    if(status<0) {
      OSA_ERROR("VIDIOC_STREAMON\n");
    }
  } else {
  	status = ioctl(pObj->fd, VIDIOC_STREAMOFF, &type);
    if(status<0) {
      OSA_ERROR("VIDIOC_STREAMOFF\n");
    }
  }

  return status;
}

int DRV_displayGetBuf(DRV_DisplayWinHndl *hndl, int *bufId)
{
	int ret;
	struct v4l2_buffer buf;
  DRV_DisplayObj *pObj;

  if(hndl==NULL)
    return OSA_EFAIL;

  if(hndl->hndl==NULL)
    return OSA_EFAIL;

  pObj = (DRV_DisplayObj *)hndl->hndl;

	memset(&buf, 0, sizeof(buf));
	buf.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
  ret = ioctl(pObj->fd, VIDIOC_DQBUF, &buf);
	if (ret < 0) {
	  *bufId = -1;
		//OSA_ERROR("VIDIOC_DQBUF\n");
		return ret;
	}

	*bufId = buf.index;

  return OSA_SOK;
}

int DRV_displayPutBuf(DRV_DisplayWinHndl *hndl, int bufId, int bufFlag, void *bufAddr)
{
	struct v4l2_buffer buf;
	int ret;
  DRV_DisplayObj *pObj;

  if(hndl==NULL)
    return OSA_EFAIL;

  if(hndl->hndl==NULL)
    return OSA_EFAIL;

  pObj = (DRV_DisplayObj *)hndl->hndl;

	if(bufId>pObj->bufInfo.numBuf)
	  return OSA_EFAIL;

	memset(&buf, 0, sizeof(buf));
  buf.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
	buf.memory = V4L2_MEMORY_USERPTR;
	buf.index = bufId;
	buf.length = pObj->bufInfo.bufSize;

	if(bufFlag == 1)
	{
		if(bufAddr == NULL)
		{
			OSA_ERROR("NULL BUF ADDR\n");
			return OSA_EFAIL;
		}

		buf.m.userptr =(unsigned long)bufAddr;
	}
	else if(bufFlag == 2)
	 {
		 // display blank screen
		 buf.m.userptr =(unsigned long)pObj->bufInfo.virtAddr[0];
	 }
	 else
	 {
		 buf.m.userptr =(unsigned long)pObj->bufInfo.virtAddr[bufId];
	 }

	ret = ioctl(pObj->fd, VIDIOC_QBUF, &buf);
  if (ret < 0) {
		OSA_ERROR("VIDIOC_QBUF\n");
		return ret;
	}

	pObj->putCount++;

  return OSA_SOK;
}

int DRV_displaySwitchBuf(DRV_DisplayWinHndl *hndl, int *bufId, int bufFlag, void *bufAddr)
{
  int status;
  DRV_DisplayObj *pObj;

  if(hndl==NULL)
    return OSA_EFAIL;

  if(hndl->hndl==NULL)
    return OSA_EFAIL;

  pObj = (DRV_DisplayObj *)hndl->hndl;

  status = DRV_displayPutBuf(hndl, *bufId, bufFlag, bufAddr);
  if(status==OSA_SOK) {
    if(pObj->putCount>=pObj->bufInfo.numBuf) {
      status = DRV_displayGetBuf(hndl, bufId);
    }
    else {
      *bufId = (*bufId + 1) % pObj->bufInfo.numBuf;
      status = OSA_EFAIL;
    }

    if(pObj->putCount==1) {
      DRV_displayEnable(hndl, TRUE);
    }

  }

  return status;
}

int DRV_displaySetFieldModeEnable(DRV_DisplayWinHndl *hndl, Bool enable)
{
  DRV_DisplayObj *pObj;

  if(hndl==NULL)
    return OSA_EFAIL;

  if(hndl->hndl==NULL)
    return OSA_EFAIL;

  pObj = (DRV_DisplayObj *)hndl->hndl;

  if(enable) {
    pObj->regs[0x4/4] &= ~(1<<1); // set Y win to field mode
    pObj->regs[0x4/4] &= ~(1<<9); // set C win to field mode
    pObj->regs[0x18/4] = (pObj->bufInfo.offsetH*2)/32; // set Y win line offset to original*2
    pObj->regs[0x1c/4] = (pObj->bufInfo.offsetH*2)/32; // set C win line offset to original*2
  } else {
    pObj->regs[0x4/4] |= (1<<1); // set to frame mode
    pObj->regs[0x4/4] |= (1<<9); // set to frame mode
    pObj->regs[0x18/4] = (pObj->bufInfo.offsetH)/32; // set Y win line offset to original
    pObj->regs[0x1c/4] = (pObj->bufInfo.offsetH)/32; // set C win line offset to original
  }

  return OSA_SOK;
}

int DRV_displayExpand(DRV_DisplayWinHndl *hndl, Uint16 expandH, Uint16 expandV)
{
  int ret;
  struct v4l2_crop crop;
  DRV_DisplayObj *pObj = (DRV_DisplayObj*)hndl->hndl;

  memset(&crop, 0, sizeof(crop));
  crop.type   = V4L2_BUF_TYPE_VIDEO_OUTPUT;
  crop.c.top  = pObj->config.startY;
  crop.c.left = pObj->config.startX;
  if(expandH) {
	// HACK - Only for width of 640 horz expand is applied
	if(pObj->cropWidth == 640){
        crop.c.width = pObj->cropWidth*9/8;
	}
	else{
		crop.c.width = pObj->cropWidth;
	}
  }else {
    crop.c.width = pObj->cropWidth;
  }
  if(expandV) {
    crop.c.height = pObj->cropHeight*6/5;
  }else {
    crop.c.height = pObj->cropHeight;
  }
  ret = ioctl(pObj->fd, VIDIOC_S_CROP, &crop);
  if(ret < 0) {
    OSA_ERROR("VIDIOC_S_CROP ioctl\n");
    return OSA_EFAIL;
  }
  pObj->config.expandH = expandH;
  pObj->config.expandV = expandV;
  return OSA_SOK;
}

int DRV_displayExpandEx(DRV_DisplayWinHndl *hndl,DRV_DisplayWinConfig *config)
{
  int ret;
  struct v4l2_crop crop;
  DRV_DisplayObj *pObj = (DRV_DisplayObj*)hndl->hndl;
  struct v4l2_cropcap cropcap;

  memset(&cropcap,0,sizeof (cropcap));

  ret = ioctl(pObj->fd, VIDIOC_CROPCAP, &cropcap);
  if(ret) {
      OSA_ERROR("VIDIOC_CROPCAP\n");
      return OSA_EFAIL;
  }

  memset(&crop, 0, sizeof(crop));
  crop.type   = V4L2_BUF_TYPE_VIDEO_OUTPUT;
  crop.c.top  = pObj->config.startY;  //config->startY;
  crop.c.left = pObj->config.startX;  //config->startX;
  if(config->expandH) {
	// HACK - Only for width of 640 horz expand is applied
	if(pObj->cropWidth == 640){
        crop.c.width = pObj->cropWidth*9/8;
	}
	else{
		crop.c.width = pObj->cropWidth;
	}

  }else {
    crop.c.width = pObj->cropWidth;
  }
  if(config->expandV) {
    crop.c.height = pObj->cropHeight*6/5;
  }else {
    crop.c.height = pObj->cropHeight;
  }
  ret = ioctl(pObj->fd, VIDIOC_S_CROP, &crop);
  if(ret < 0) {
    OSA_ERROR("VIDIOC_S_CROP ioctl\n");
    return OSA_EFAIL;
  }

  return OSA_SOK;
}


int DRV_displaySetWinZoom(DRV_DisplayWinHndl *hndl, Uint16 zoomH, Uint16 zoomV)
{

  return OSA_SOK;
}

int DRV_displaySetWinPos(DRV_DisplayWinHndl *hndl, Uint16 startX, Uint16 startY)
{
  int ret;
  struct v4l2_crop crop;
  DRV_DisplayObj *pObj = (DRV_DisplayObj*)hndl->hndl;

  memset(&crop, 0, sizeof(crop));
  crop.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
  crop.c.top = startY;
  crop.c.left = startX;
  if(pObj->config.expandH) {
    crop.c.width = pObj->cropWidth*9/8;
  }else {
    crop.c.width = pObj->cropWidth;
  }
  if(pObj->config.expandV) {
    crop.c.height = pObj->cropHeight*6/5;
  }else {
    crop.c.height = pObj->cropHeight;
  }
  ret = ioctl(pObj->fd, VIDIOC_S_CROP, &crop);
  if(ret < 0) {
    OSA_ERROR("VIDIOC_S_CROP ioctl\n");
    return OSA_EFAIL;
  }
  pObj->config.startX = startX;
  pObj->config.startY = startY;
  return OSA_SOK;
}

int DRV_displaySetWinSize(DRV_DisplayWinHndl *hndl, Uint16 width, Uint16 height)
{
  int ret;
  struct v4l2_crop crop;
  DRV_DisplayObj *pObj = (DRV_DisplayObj*)hndl->hndl;

  memset(&crop, 0, sizeof(crop));
  crop.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
  crop.c.top = pObj->config.startY;
  crop.c.left = pObj->config.startX;
  crop.c.width = width;
  crop.c.height = height;
  ret = ioctl(pObj->fd, VIDIOC_S_CROP, &crop);
  if(ret < 0) {
    OSA_ERROR("VIDIOC_S_CROP ioctl\n");
    return OSA_EFAIL;
  }
  return OSA_SOK;
}



static DRV_DisplayWinHndl gDRV_displayHndl;
static int                gDRV_displayBufId;
static DRV_DisplayBufInfo gDRV_displayInfo;
static int                gDRV_displayQuit=0;

static void DRV_displayTestSignalHandler(int signum)
{
  gDRV_displayQuit = 1;
}

int DRV_displayTestMain(int argc, char **argv)
{
  int status, i;
  Uint32 readSize;
  DRV_DisplayWinConfig  displayConfig;
  char filename[20];

  OSA_attachSignalHandler(SIGINT, DRV_displayTestSignalHandler);

  status = DRV_init();
  if(status!=OSA_SOK)
    return status;

  displayConfig.winId       = DRV_DISPLAY_VID_WIN_0;
  displayConfig.numBuf      = 3;
  displayConfig.dataFormat  = DRV_DATA_FORMAT_YUV420;
  displayConfig.startX      = 0;
  displayConfig.startY      = 0;
  displayConfig.width       = 640;
  displayConfig.height      = 480;
  displayConfig.offsetH     = OSA_align(displayConfig.width, 32);
  displayConfig.offsetV     = displayConfig.height;
  displayConfig.zoomH       = 0;
  displayConfig.zoomV       = 0;
  displayConfig.bufFlag     = 0;

  status = DRV_displaySetMode(DRV_DISPLAY_MODE_NTSC);
  if(status!=OSA_SOK)
    goto error_exit;

  gDRV_displayBufId=0;

  status = DRV_displayOpen(&gDRV_displayHndl, &displayConfig);
  if(status!=OSA_SOK)
    goto error_exit;

  DRV_displayGetBufInfo(&gDRV_displayHndl, &gDRV_displayInfo);

  if(displayConfig.dataFormat  == DRV_DATA_FORMAT_YUV422)
    strcpy(filename,"video.yuv422");
  else
    strcpy(filename,"video.yuv420");

  status = OSA_fileReadFile(filename, gDRV_displayInfo.virtAddr[gDRV_displayBufId], gDRV_displayInfo.bufSize, &readSize);
  if(status==OSA_SOK) {

    for(i=1; i<gDRV_displayInfo.numBuf ; i++)
      memcpy(gDRV_displayInfo.virtAddr[i], gDRV_displayInfo.virtAddr[0], gDRV_displayInfo.bufSize);

    while(!gDRV_displayQuit) {
      DRV_displaySwitchBuf( &gDRV_displayHndl, &gDRV_displayBufId, 0, NULL);
    }
  }

  DRV_displayClose(&gDRV_displayHndl);

error_exit:
  DRV_exit();

  return status;
}

