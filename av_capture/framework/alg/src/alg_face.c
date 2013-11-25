/*
    DM365 Evaluation Software

    (c)Texas Instruments 2009

    Note:  Dont change any values /Modified values will get effect only in the
              new library (alg_fr_ti.a) built after the changes.
*/

#include "alg_priv.h"
#include "alg_face.h"
#include "alg_ti_facetrack.h"

#include <osa_cmem.h>
#include <osa_file.h>

//#define FACE_CMEM_ALLOC OSA_cmemAlloc
#define FACE_CMEM_ALLOC OSA_cmemAllocCached

//#define FACERECOG_DEBUG

/* Facial feature memory allocation */
int ALG_facialFeatureGetMem(void)
{
    int size[3] = {0,0,0};
	void *tempbuff;
	int val = 0;

	/* Facial Pointer */
    szMemSize.nPtRec = ALG_ffpt_GetMemRec( &val);
    if(ffMemBuff.ptRecBuff != NULL)
		OSA_cmemFree(ffMemBuff.ptRecBuff);

	ffMemBuff.ptRecBuff = (void *)FACE_CMEM_ALLOC(szMemSize.nPtRec,32);

    tempbuff = ffMemBuff.ptRecBuff;
    ALG_ffpt_GetMemSize(szMemSize.nPtRec, tempbuff, size);
	szMemSize.facePTRecSize[0] = size[0];
	szMemSize.facePTRecSize[1] = size[1];
	szMemSize.facePTRecSize[2] = size[2];

    if(ffMemBuff.ptAlgBuff[0] != NULL)
		OSA_cmemFree(ffMemBuff.ptAlgBuff[0]);
    if(ffMemBuff.ptAlgBuff[1] != NULL)
		OSA_cmemFree(ffMemBuff.ptAlgBuff[1]);
    if(ffMemBuff.ptAlgBuff[2] != NULL)
		OSA_cmemFree(ffMemBuff.ptAlgBuff[2]);

	ffMemBuff.ptAlgBuff[0]= (void *)FACE_CMEM_ALLOC(szMemSize.facePTRecSize[0], 32);
	ffMemBuff.ptAlgBuff[1]= (void *)FACE_CMEM_ALLOC(szMemSize.facePTRecSize[1], 32);
	ffMemBuff.ptAlgBuff[2]= (void *)FACE_CMEM_ALLOC(szMemSize.facePTRecSize[2], 32);

	/* Facial Feature */
    szMemSize.nFrRec = ALG_fffr_GetMemRec( &val);
    if(ffMemBuff.frRecBuff != NULL)
		OSA_cmemFree(ffMemBuff.frRecBuff);
	ffMemBuff.frRecBuff = (void *)FACE_CMEM_ALLOC(szMemSize.nFrRec,32);

    tempbuff = ffMemBuff.frRecBuff;
    ALG_fffr_GetMemSize(szMemSize.nFrRec, tempbuff, size);
	szMemSize.faceFRRecSize[0] = size[0];
	szMemSize.faceFRRecSize[1] = size[1];
	szMemSize.faceFRRecSize[2] = size[2];

    if(ffMemBuff.frAlgBuff[0] != NULL)
		OSA_cmemFree(ffMemBuff.frAlgBuff[0]);
    if(ffMemBuff.frAlgBuff[1] != NULL)
		OSA_cmemFree(ffMemBuff.frAlgBuff[1]);
    if(ffMemBuff.frAlgBuff[2] != NULL)
		OSA_cmemFree(ffMemBuff.frAlgBuff[2]);

	ffMemBuff.frAlgBuff[0]= (void *)FACE_CMEM_ALLOC(szMemSize.faceFRRecSize[0], 32);
	ffMemBuff.frAlgBuff[1]= (void *)FACE_CMEM_ALLOC(szMemSize.faceFRRecSize[1], 32);
	ffMemBuff.frAlgBuff[2]= (void *)FACE_CMEM_ALLOC(szMemSize.faceFRRecSize[2], 32);

    /* For Face Album data */
    if(ffMemBuff.albumBuff != NULL)
		OSA_cmemFree(ffMemBuff.albumBuff);
	ffMemBuff.albumBuff= (unsigned int*)FACE_CMEM_ALLOC(FR_ALBUM_SIZE, 32);

    /* For image data */
    if(ffMemBuff.imageDataBuff!= NULL)
		OSA_cmemFree(ffMemBuff.imageDataBuff);
	ffMemBuff.imageDataBuff = (void *)FACE_CMEM_ALLOC(FR_IMAGE_SIZE, 32);

    if(ffMemBuff.imageDataBuff!= NULL)
    	ffMemBuff.phy_imageDataBuff = OSA_cmemGetPhysAddr(ffMemBuff.imageDataBuff);

#ifdef FACERECOG_DEBUG
	/* Debug */
    printf("\n\n AF: szMemSize.nPtRec: Size %d, Ptr %p", szMemSize.nPtRec,ffMemBuff.ptRecBuff);
    printf("\n AF: ffMemBuff.ptAlgBuff[0]: Size %d, Ptr %p", szMemSize.facePTRecSize[0],ffMemBuff.ptAlgBuff[0] );
    printf("\n AF: ffMemBuff.ptAlgBuff[1]: Size %d, Ptr %p", szMemSize.facePTRecSize[1],ffMemBuff.ptAlgBuff[1] );
    printf("\n AF: ffMemBuff.ptAlgBuff[2]: Size %d, Ptr %p", szMemSize.facePTRecSize[2],ffMemBuff.ptAlgBuff[2] );

    printf("\n\n AF: szMemSize.nFrRec: Size %d, Ptr %p", szMemSize.nFrRec,ffMemBuff.frRecBuff);
    printf("\n AF: ffMemBuff.frAlgBuff[0]: Size %d, Ptr %p", szMemSize.faceFRRecSize[0],ffMemBuff.frAlgBuff[0] );
    printf("\n AF: ffMemBuff.frAlgBuff[1]: Size %d, Ptr %p", szMemSize.faceFRRecSize[1],ffMemBuff.frAlgBuff[1] );
    printf("\n AF: ffMemBuff.frAlgBuff[2]: Size %d, Ptr %p", szMemSize.faceFRRecSize[2],ffMemBuff.frAlgBuff[2] );

    printf("\n AF: ffMemBuff.albumBuff: Size %d, Ptr %p", FR_ALBUM_SIZE,ffMemBuff.albumBuff);
    printf("\n AF: ffMemBuff.imageDataBuff: Size %d, Ptr %p", FR_IMAGE_SIZE,ffMemBuff.imageDataBuff);
#endif

    return 0;
}

/* Facial feature memory allocation */
int ALG_facialFeatureRelMem(void)
{
	/* Facial Pointer */
  	if(ffMemBuff.ptAlgBuff[0] != NULL){
		OSA_cmemFree(ffMemBuff.ptAlgBuff[0]);
		ffMemBuff.ptAlgBuff[0] = NULL;
 	}
 	if(ffMemBuff.ptAlgBuff[1] != NULL){
		OSA_cmemFree(ffMemBuff.ptAlgBuff[1]);
		ffMemBuff.ptAlgBuff[1] = NULL;
 	}
 	if(ffMemBuff.ptAlgBuff[2] != NULL){
		OSA_cmemFree(ffMemBuff.ptAlgBuff[2]);
		ffMemBuff.ptAlgBuff[2] = NULL;
 	}
 	if(ffMemBuff.ptRecBuff != NULL){
		OSA_cmemFree(ffMemBuff.ptRecBuff);
		ffMemBuff.ptRecBuff = NULL;
 	}

	/* Facial Feature */
 	if(ffMemBuff.frAlgBuff[0] != NULL){
		OSA_cmemFree(ffMemBuff.frAlgBuff[0]);
		ffMemBuff.frAlgBuff[0] = NULL;
 	}
 	if(ffMemBuff.frAlgBuff[1] != NULL){
		OSA_cmemFree(ffMemBuff.frAlgBuff[1]);
		ffMemBuff.frAlgBuff[1] = NULL;
 	}
 	if(ffMemBuff.frAlgBuff[2] != NULL){
		OSA_cmemFree(ffMemBuff.frAlgBuff[2]);
		ffMemBuff.frAlgBuff[2] = NULL;
 	}
 	if(ffMemBuff.frRecBuff != NULL){
		OSA_cmemFree(ffMemBuff.frRecBuff);
		ffMemBuff.frRecBuff = NULL;
 	}


    /* For Face Album data */
    if(ffMemBuff.albumBuff != NULL){
		OSA_cmemFree(ffMemBuff.albumBuff);
		ffMemBuff.albumBuff = NULL;
    }

    /* For image data */
    if(ffMemBuff.imageDataBuff != NULL){
		OSA_cmemFree(ffMemBuff.imageDataBuff);
		ffMemBuff.imageDataBuff = NULL;
		ffMemBuff.phy_imageDataBuff = NULL;
    }


    return 0;
}

//#define FACETRACKER_DEBUG
Facetrackerstruct *gFacetracker;
Uint8 *Face_track_buffer;

int ALG_faceTrackerOpen(void)
{
	int i,j;

	Face_track_buffer = FACE_CMEM_ALLOC(MAX_FACETRACKER_FRAMEBUF, 32);
	gFacetracker = FACE_CMEM_ALLOC(sizeof(Facetrackerstruct), 32);

	/* Face Tracker Intialization */
	gFacetracker->frmBuffer_1 = Face_track_buffer;
	gFacetracker->frmBuffer_2 = Face_track_buffer;
	gFacetracker->face_number = 0;
	gFacetracker->face_number_previous = 0;

	for (i = 0; i < MAX_FACE_NUMBER; i++)
	{
		for (j = 0; j < 4; j++)
		{
			gFacetracker->face_pos[i][j] = 0;
			gFacetracker->face_pos_previous[i][j] = 0;
            gFacetracker->roi_pos[i][j] = 0 ;
            gFacetracker->face_pos_lastdetect[i][j] = 0;
		}
		for (j = 0; j < 2; j++) // VC
		{
		  gFacetracker->mv[i][j] = 0;
		  gFacetracker->mv[i][j] = 0;
		}

		gFacetracker->tracking_counter[i] = 0;
		gFacetracker->tracking_counter_previous[i] = 0;
	}

    return 0;
}

int ALG_faceTrackerClose(void)
{
	OSA_cmemFree(Face_track_buffer);
	OSA_cmemFree(gFacetracker);

    return 0;
}

int ALG_faceTrackerRun(Uint8* bufptr, int width, int height, DRV_FaceDetectRunStatus *status)
{
	int i = 0, size, trackerFlag[MAX_FACE_NUMBER];

	memset(trackerFlag, 0, sizeof(int)*MAX_FACE_NUMBER);

	gFacetracker->frmBuffer_1 = bufptr;

	gFacetracker->inpImgSizeH = width;
	gFacetracker->inpImgSizeV = height;

// Max number of frames during which a face can be tracked
// without any positive detection by the face detector.
// This number usually depends on the frame rate
// For 30 fps, setting it to 90 means the tracker will
// be active for up to 3 seconds if no face is detected
    gFacetracker->maxNumOfTrackingFrames= 30;

    gFacetracker->searchRange= 5; //VC
// Amount of expansion applied to the ROI box
// in the left, right, top, bottom sides .
// For expandRoiLeft and expandRoiRight, the unit is 1/8 of the face detector bounding box's width.
// For instance, if expandRoiLeft= 2, it means the amount of expansion to the left is 2/8 of the face detector box's width
// For expandRoiTop and expandRoiBottom, the unit is 1/8 of the face detector bounding box's height.
// For instance, if expandRoiTop= 3, it means the amount of expansion to the left is 3/8 of the face detector box's height
    gFacetracker->expandRoiLeft= 1;
    gFacetracker->expandRoiRight= 1;
    gFacetracker->expandRoiTop= 1;
    gFacetracker->expandRoiBottom= 1;

	gFacetracker->face_number = status->numFaces;

	for(i=0;i<status->numFaces;i++) {
		size = CSL_FACE_DETECT_GET_SIZE(status->info[i].sizeConfidence);
		gFacetracker->face_pos[i][0] = status->info[i].centerY - size/2;
		gFacetracker->face_pos[i][1] = status->info[i].centerX - size/2;
		gFacetracker->face_pos[i][2] = status->info[i].centerY + size/2 - 1;
		gFacetracker->face_pos[i][3] = status->info[i].centerX + size/2 - 1;


#ifdef FACETRACKER_DEBUG
		OSA_printf("FACE DETECT DEBUG VALUE BEFORE: %d %d %d %d\n", status->numFaces, status->info[i].centerX,
			status->info[i].centerY, status->info[i].sizeConfidence);
		//OSA_printf("FACE TRACKER DEBUG VALUE BEFORE: %d %d %d %d %d\n", gFacetracker->face_number, gFacetracker->face_pos[i][0],
		//	gFacetracker->face_pos[i][1], gFacetracker->face_pos[i][2], gFacetracker->face_pos[i][3]);
#endif
		trackerFlag[i]=1;
	}

    //OSA_mutexLock(&vicp_imcop_lock);
    //OSA_mutexLock(&vnf_imcop_lock);

	TI_track_face_run(gFacetracker);

	/* Need to optimize this with DMA later */
	memcpy(gFacetracker->frmBuffer_2, gFacetracker->frmBuffer_1, width*height);

    //OSA_mutexUnlock(&vnf_imcop_lock);
    //OSA_mutexUnlock(&vicp_imcop_lock);

	status->numFaces = gFacetracker->face_number;
	for(i=0;i<gFacetracker->face_number;i++) {
		status->info[i].sizeConfidence &= 0x0ffffff00;
#if 0
		status->info[i].sizeConfidence |= (gFacetracker->face_pos[i][2]-gFacetracker->face_pos[i][0]);
        status->info[i].centerY = gFacetracker->face_pos[i][0] + (gFacetracker->face_pos[i][2]-gFacetracker->face_pos[i][0])/2;
		status->info[i].centerX = gFacetracker->face_pos[i][1] + (gFacetracker->face_pos[i][3]-gFacetracker->face_pos[i][1])/2;
#else
        status->info[i].sizeConfidence |= (gFacetracker->roi_pos[i][2]-gFacetracker->roi_pos[i][0]);
        status->info[i].centerY = (gFacetracker->roi_pos[i][0] + gFacetracker->roi_pos[i][2])/2;
		status->info[i].centerX = (gFacetracker->roi_pos[i][1] + gFacetracker->roi_pos[i][3])/2;
#endif

#ifdef FACETRACKER_DEBUG
		OSA_printf("FACE DETECT DEBUG VALUE AFTER: %d %d %d %d\n", status->numFaces, status->info[i].centerX ,
			status->info[i].centerY, status->info[i].sizeConfidence);
		//OSA_printf("FACE TRACKER DEBUG VALUE AFTER: %d %d %d %d %d\n", gFacetracker->face_number, gFacetracker->face_pos[i][0],
		//	gFacetracker->face_pos[i][1], gFacetracker->face_pos[i][2], gFacetracker->face_pos[i][3]);
    OSA_printf("mvx= %d, mvy= %d\n", gFacetracker->mv[i][1], gFacetracker->mv[i][0]);
    OSA_printf("\n");
#endif
		//if(trackerFlag[i]==0)
		//	OSA_printf("TRACKING IS SWITCHED ON: %d\n", gFacetracker->tracking_counter[i]);

		trackerFlag[i]=0;
	}

    return 0;
}
