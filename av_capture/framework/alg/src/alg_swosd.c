
#include <alg_priv.h>
#include <alg_swosd.h>
#include <osa_cmem.h>
#include <osa_file.h>

#define D1_WIDTH (720)
#define HIST_LINE_THICKNESS	(4)

void *ALG_swosdCreate(ALG_SwosdCreate *create)
{

  return NULL;
}

int ALG_swosdDelete(void *hndl)
{

  return 0;
}

int ALG_swosdRun(void *hndl, ALG_SwosdRunPrm *prm)
{
  int i, centerX, centerY, size, xbound=0, ybound=0;
  ALG_SwosdRectInfo rectInfo;

  rectInfo.colorY    = 0xFF;
  rectInfo.colorC    = 0x80;
  rectInfo.thickness = 4;

  for(i=0; i<prm->faceInfo.numFaces; i++) {


    centerX = ((prm->faceInfo.info[i].centerX) * prm->videoWidth ) / (prm->faceInfoInWidth);
    centerY = ((prm->faceInfo.info[i].centerY) * prm->videoHeight ) / (prm->faceInfoInHeight);
    size    = (CSL_FACE_DETECT_GET_SIZE(prm->faceInfo.info[i].sizeConfidence)*prm->videoWidth) / prm->faceInfoInWidth;

    rectInfo.startX  = centerX - size/2;
    rectInfo.startY  = centerY - size/2;
    rectInfo.width   = size;
    rectInfo.height  = size;

    if(rectInfo.startX<1)rectInfo.startX=1;
    if(rectInfo.startX>=prm->videoWidth)rectInfo.startX=prm->videoWidth-1;

    if(rectInfo.startY<1)rectInfo.startY=1;
    if(rectInfo.startY>=prm->videoHeight)rectInfo.startY=prm->videoHeight-1;

    rectInfo.privacyMask = prm->privacyMask;

	xbound=0; ybound=0;

	if((rectInfo.startX>prm->faceInfoROIx)&&((rectInfo.startX+rectInfo.width)<(prm->faceInfoROIx+prm->faceInfoROIw)))
		xbound = 1;
	if((rectInfo.startY>prm->faceInfoROIy)&&((rectInfo.startY+rectInfo.height)<(prm->faceInfoROIy+prm->faceInfoROIh)))
		ybound = 1;

    if(xbound&ybound) {
    	ALG_swosdDrawRect(prm, &rectInfo);
	}
  }

  if(prm->histStatus && prm->histEnable) {
	if(prm->videoWidth >= D1_WIDTH) {
		rectInfo.startX  		= 16+HIST_LINE_THICKNESS;
		rectInfo.startY  		= prm->videoHeight - 16 - (prm->videoHeight>>2);
		rectInfo.width   		= 256;
		rectInfo.height  		= prm->videoHeight>>2;
  		rectInfo.thickness 		= 1;
		rectInfo.privacyMask 	= FALSE;

		ALG_swosdDrawRect(prm, &rectInfo);
		ALG_swosdDrawHist(prm);
	}
  }

  if(prm->fxdROIflag) {
	rectInfo.startX  = prm->videoWidth>>2;
    rectInfo.startY  = prm->videoHeight>>2;
    rectInfo.width   = rectInfo.startX*2;
    rectInfo.height  = rectInfo.startY*2;
	rectInfo.thickness = 2;
    rectInfo.privacyMask = FALSE;

    ALG_swosdDrawRect(prm, &rectInfo);
  }

  return OSA_SOK;
}

int ALG_swosdDrawLine(ALG_SwosdRunPrm *bufInfo, ALG_SwosdLineInfo *lineInfo)
{
  Uint8   *startAddr;
  Uint16  hlen, vlen, incAddr;
  Uint32  color, i;
  Uint32  *curAddr;

  if(lineInfo->vertLine) {
    hlen = lineInfo->thickness;
    vlen = lineInfo->length;
  } else {
    hlen = lineInfo->length;
    vlen = lineInfo->thickness;
  }

  if(lineInfo->startY >= (bufInfo->videoHeight) )
    return OSA_SOK;

  if(lineInfo->startX >= (bufInfo->videoWidth) )
    return OSA_SOK;

  lineInfo->startX = OSA_floor(lineInfo->startX, 4);

  if(lineInfo->startX + hlen > (bufInfo->videoWidth) ) {
    hlen = bufInfo->videoWidth - lineInfo->startX;
  }

  if(lineInfo->startY + vlen > (bufInfo->videoHeight)) {
    vlen = bufInfo->videoHeight - lineInfo->startY;
  }

  if(bufInfo->videoDataFormat==DRV_DATA_FORMAT_YUV422) {
    incAddr    = bufInfo->videoOffsetH*2;
    startAddr  = bufInfo->videoInOutAddr + lineInfo->startY * incAddr + lineInfo->startX*2;
    hlen      *= 2;
    color      = (lineInfo->colorY << 24) + (lineInfo->colorC << 16) + (lineInfo->colorY << 8) + lineInfo->colorC;
  } else {
    incAddr   = bufInfo->videoOffsetH;
    startAddr = bufInfo->videoInOutAddr + lineInfo->startY * incAddr + lineInfo->startX;
    color     = (lineInfo->colorY << 24) + (lineInfo->colorY << 16) + (lineInfo->colorY << 8 )+ lineInfo->colorY;
  }

  hlen = OSA_floor(hlen, 4);

  while(vlen) {
    curAddr = (Uint32*)startAddr;

    for(i=0; i<hlen/4; i++)
      *curAddr++ = color;

    startAddr+=incAddr;
    vlen--;
  }

  if(bufInfo->videoDataFormat==DRV_DATA_FORMAT_YUV420) {

    startAddr  = bufInfo->videoInOutAddr + bufInfo->videoOffsetH*bufInfo->videoOffsetV;
    startAddr += lineInfo->startY/2 * incAddr + lineInfo->startX;
    color      = (lineInfo->colorC << 24) + (lineInfo->colorC << 16) + (lineInfo->colorC << 8) + lineInfo->colorC;
    vlen      /= 2;

    while(vlen) {
      curAddr = (Uint32*)startAddr;

      for(i=0; i<hlen/4; i++)
        *curAddr++ = color;

      startAddr+=incAddr;
      vlen--;
    }
  }

  return OSA_SOK;
}

int ALG_swosdDrawRect(ALG_SwosdRunPrm *bufInfo, ALG_SwosdRectInfo *rectInfo)
{
  ALG_SwosdLineInfo lineInfo;

  rectInfo->startX    = OSA_floor(rectInfo->startX, 4);
  rectInfo->startY    = OSA_floor(rectInfo->startY, 2);

  rectInfo->width     = OSA_align(rectInfo->width, 4);
  rectInfo->height    = OSA_align(rectInfo->height, 2);
  rectInfo->thickness = OSA_align(rectInfo->thickness, 4);

  lineInfo.thickness  = rectInfo->thickness;
  lineInfo.colorY     = rectInfo->colorY;
  lineInfo.colorC     = rectInfo->colorC;

  if(rectInfo->privacyMask == TRUE)
  {
	DRV_FrameCopyPrm frameCopy;
	int status;

	frameCopy.srcPhysAddr = NULL;
	frameCopy.srcVirtAddr = NULL;
	frameCopy.dstPhysAddr = NULL;
	frameCopy.dstVirtAddr = bufInfo->videoInOutAddr;
	frameCopy.srcOffsetH  = rectInfo->width;
	frameCopy.srcOffsetV  = rectInfo->height;
	frameCopy.dstOffsetH  = bufInfo->videoOffsetH;
	frameCopy.dstOffsetV  = bufInfo->videoOffsetV;
	frameCopy.copyWidth   = (rectInfo->width*12)/10;
	frameCopy.copyHeight  = (rectInfo->height*12)/10;
	frameCopy.dataFormat  = bufInfo->videoDataFormat;
	frameCopy.srcStartX   = 0;
	frameCopy.srcStartY   = 0;
	frameCopy.dstStartX   = rectInfo->startX;
	frameCopy.dstStartY   = rectInfo->startY;

	if( (rectInfo->startX + frameCopy.copyWidth) > bufInfo->videoWidth )
	{
		frameCopy.copyWidth = bufInfo->videoWidth - rectInfo->startX;
	}
	if( (rectInfo->startY + frameCopy.copyHeight) > bufInfo->videoHeight)
	{
		frameCopy.copyHeight = bufInfo->videoHeight - rectInfo->startY;
	}

	  status = DRV_frameCopy(NULL, &frameCopy);
	  if(status!=OSA_SOK) {
		OSA_ERROR("DRV_frameCopy(srcOffset:%dx%d dstOffset:%dx%d wxh:%dx%d)\n",
		  frameCopy.srcOffsetH, frameCopy.srcOffsetV,
		  frameCopy.dstOffsetH, frameCopy.dstOffsetV,
		  frameCopy.copyWidth, frameCopy.copyHeight
			);
	  }
	  return status;
  }

  lineInfo.startX     = rectInfo->startX;
  lineInfo.startY     = rectInfo->startY;
  lineInfo.length     = rectInfo->width;
  lineInfo.vertLine   = FALSE;

  ALG_swosdDrawLine(bufInfo, &lineInfo);

  lineInfo.startX     = rectInfo->startX;
  lineInfo.startY     = rectInfo->startY;
  lineInfo.length     = rectInfo->height;
  lineInfo.vertLine   = TRUE;

  ALG_swosdDrawLine(bufInfo, &lineInfo);

  lineInfo.startX     = (rectInfo->startX+rectInfo->width-rectInfo->thickness);
  lineInfo.startY     = rectInfo->startY;
  lineInfo.length     = rectInfo->height;
  lineInfo.vertLine   = TRUE;

  ALG_swosdDrawLine(bufInfo, &lineInfo);

  lineInfo.startX     = rectInfo->startX;
  lineInfo.startY     = (rectInfo->startY+rectInfo->height-rectInfo->thickness);
  lineInfo.length     = rectInfo->width;
  lineInfo.vertLine   = FALSE;

  ALG_swosdDrawLine(bufInfo, &lineInfo);

  return OSA_SOK;
}



/* Draw distogrma data in SWOSD */
int ALG_swosdDrawHist( ALG_SwosdRunPrm *prm)
{
  ALG_SwosdLineInfo lineInfo;
  Uint32 i, histHeight = 0;
  Uint32 clip_value=prm->videoHeight>>2;

  lineInfo.colorY     = 0xFF;  // White color - 0xFF80FF80;
  lineInfo.colorC     = 0x80;
  lineInfo.vertLine   = TRUE;
  lineInfo.thickness  = HIST_LINE_THICKNESS;
  lineInfo.startX     = 16;

  for(i=0; i<256; i=i+lineInfo.thickness){  //255

    histHeight = (prm->histData[i]>>4);
    if(histHeight > clip_value)
      histHeight = clip_value;

    lineInfo.startX     += lineInfo.thickness;
    lineInfo.startY     = prm->videoHeight - 16 - histHeight;
    lineInfo.length     = histHeight;
    ALG_swosdDrawLine(prm, &lineInfo);

  }

  return OSA_SOK;
}



