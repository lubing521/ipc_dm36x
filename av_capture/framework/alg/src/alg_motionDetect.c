#include <alg_motionDetect.h>

void *ALG_motionDetectCreate(ALG_MotionDetectCreate *create, ALG_MotionDetectCreateStatus *status)
{
		ALG_MotionObj  *pObj = NULL;

		pObj = OSA_memAlloc(sizeof(ALG_MotionObj));

		if(pObj==NULL)
			return NULL;

		memset(pObj, 0, sizeof(*pObj));
		if( create )
			pObj->createPrm = *create;

		if( status )
			pObj->createStatus = *status;

		/*Start motion detection function after delay start_cnt frames*/
		pObj->frame_count 	= 0;
		pObj->start_cnt 	= 200;

		return pObj;
}


int ALG_motionDetectGetThres(ALG_MotionObj  *pObj)
{
	int block_size;
	if( pObj == NULL )
		return ALG_MOTION_S_FAIL;

	pObj->frame_width   = (pObj->runPrm.ImageWidth  >> 4); // Number of macroblocks in frame width
	pObj->frame_height  = (pObj->runPrm.ImageHeight >> 4); // Number of macroblocks in frame height

	/* Set the motion block size base on image size */
	pObj->win_width 	= pObj->runPrm.windowWidth >> 4; //Window width in macroblocks
	pObj->win_height 	= pObj->runPrm.windowHeight >> 4; //Window height in macroblocks

	block_size = pObj->win_width * pObj->win_height;

	if(block_size >= 600) /* for 1080P */
	{
		pObj->SAD_THRESHOLD = 2500;
	}
	else if(block_size < 600 && block_size >= 300 ) /* for 720P */
	{
		pObj->SAD_THRESHOLD = 3000;
	}
	else if(block_size < 300 && block_size >= 100 ) /* for D1 */
	{
		pObj->SAD_THRESHOLD = 3500;
	}
	else if(block_size < 100 && block_size >= 20 ) /* for CIF */
	{
		pObj->SAD_THRESHOLD = 4000;
	}
	else /* less than CIF */
	{
		pObj->SAD_THRESHOLD = 4500;
	}

	return ALG_MOTION_S_OK;
}

int ALG_motionDetectGetSensitive(ALG_MotionObj  *pObj)
{
	if( pObj == NULL )
		return ALG_MOTION_S_FAIL;

    pObj->runPrm.motionsens = 1; /* Intialize to avoid non-zero value */

	/* Get motion sensitivity from webpage according to user input*/
	if(pObj->runPrm.motioncenable == 0)
	{
			if(pObj->frame_width >= 120) /* 1080P = 675MB/zone */
			{
				if(pObj->runPrm.motionlevel == ALG_MOTION_DETECT_SENSITIVITY_LOW)			//low level
				{
						pObj->runPrm.motionsens = 35;
				}
				else if(pObj->runPrm.motionlevel == ALG_MOTION_DETECT_SENSITIVITY_MEDIUM)			//medium level
				{
						pObj->runPrm.motionsens = 55;
				}
				else if(pObj->runPrm.motionlevel == ALG_MOTION_DETECT_SENSITIVITY_HIGH)			//high level
				{
						pObj->runPrm.motionsens = 95;
				}
			}
			else if(pObj->frame_width < 120 && pObj->frame_width >= 80) /* 720P = 300MB/zone */
			{
				if(pObj->runPrm.motionlevel == ALG_MOTION_DETECT_SENSITIVITY_LOW)			//low level
				{
						pObj->runPrm.motionsens = 15;
				}
				else if(pObj->runPrm.motionlevel == ALG_MOTION_DETECT_SENSITIVITY_MEDIUM)			//medium level
				{
						pObj->runPrm.motionsens = 25;
				}
				else if(pObj->runPrm.motionlevel == ALG_MOTION_DETECT_SENSITIVITY_HIGH)			//high level
				{
						pObj->runPrm.motionsens = 50;
				}
			}
			else if(pObj->frame_width < 80 && pObj->frame_width >= 40) /* D1 = 112.5MB/zone */
			{
				if(pObj->runPrm.motionlevel == ALG_MOTION_DETECT_SENSITIVITY_LOW)			//low level
				{
						pObj->runPrm.motionsens = 7;
				}
				else if(pObj->runPrm.motionlevel == ALG_MOTION_DETECT_SENSITIVITY_MEDIUM)			//medium level
				{
						pObj->runPrm.motionsens = 11;
				}
				else if(pObj->runPrm.motionlevel == ALG_MOTION_DETECT_SENSITIVITY_HIGH)			//high level
				{
						pObj->runPrm.motionsens = 20;
				}
			}
			else /* CIF = 20MB/zone */
			{
				if(pObj->runPrm.motionlevel == ALG_MOTION_DETECT_SENSITIVITY_LOW)			//low level
				{
						pObj->runPrm.motionsens = 3;

				}else if(pObj->runPrm.motionlevel == ALG_MOTION_DETECT_SENSITIVITY_MEDIUM)			//medium level
				{
						pObj->runPrm.motionsens = 5;

				}else if(pObj->runPrm.motionlevel == ALG_MOTION_DETECT_SENSITIVITY_HIGH)			//high level
				{
						pObj->runPrm.motionsens = 10;

				}
			}
	}
	else
	{

		if(pObj->runPrm.motioncvalue == 0)
		{
			pObj->runPrm.motionsens = 20;

		}
		else
		{
			if(pObj->frame_width > 45)
			{
				if(pObj->runPrm.motioncvalue >= 20)
				{
					pObj->runPrm.motionsens = pObj->runPrm.motioncvalue;
				}else{
					pObj->runPrm.motionsens = 20;
				}

			}
			else
			{
				pObj->runPrm.motionsens = (pObj->runPrm.motioncvalue / 5);

				if(pObj->runPrm.motionsens < 10)
				{
					pObj->runPrm.motionsens = 10;
				}

			}
		}
	}

	if( pObj->runPrm.motionsens == 0)
		pObj->runPrm.motionsens = 1;

	/* Calculate the threshold value base on the motion sensitivity */
	pObj->threshold = (pObj->win_width * pObj->win_height)/ pObj->runPrm.motionsens;

	return ALG_MOTION_S_OK;
}

int ALG_motionDetectCalc(ALG_MotionObj  *pObj)
{
	ALG_MotionDetectMbMvInfo *mbMV_data;
	int i, j, status;

	mbMV_data = pObj->runPrm.mbMvInfo + pObj->MvDataOffset;

	pObj->warning_count = 0;

	for (i = 0; i < pObj->win_height; i++)
	{
		for(j = 0; j < pObj->win_width; j++)
		{
			if(mbMV_data->SAD > pObj->SAD_THRESHOLD)
			{
				pObj->warning_count++;
			}
			mbMV_data ++;
		}
		mbMV_data = mbMV_data + (pObj->frame_width - pObj->win_width);
	 }

	/* If the pObj->warning_count is bigger than pObj->threshold,
	the function will return a alarm signal*/

	status = ( pObj->warning_count >= pObj->threshold ) ? ALG_MOTION_S_DETECT : ALG_MOTION_S_NO_DETECT;

	return status;
}

int ALG_motionDetectStart(ALG_MotionObj  *pObj)
{
	int detect_cnt = 0;
	int i=0,j=0;

	pObj->runPrm.windowEnableMap[0][0] = (pObj->runPrm.blockNum & 0x100)?1:0;
	pObj->runPrm.windowEnableMap[1][0] = (pObj->runPrm.blockNum & 0x200)?1:0;
	pObj->runPrm.windowEnableMap[2][0] = (pObj->runPrm.blockNum & 0x400)?1:0;
	pObj->runPrm.windowEnableMap[3][0] = (pObj->runPrm.blockNum & 0x800)?1:0;
	pObj->runPrm.windowEnableMap[0][1] = (pObj->runPrm.blockNum & 0x010)?1:0;
	pObj->runPrm.windowEnableMap[1][1] = (pObj->runPrm.blockNum & 0x020)?1:0;
	pObj->runPrm.windowEnableMap[2][1] = (pObj->runPrm.blockNum & 0x040)?1:0;
	pObj->runPrm.windowEnableMap[3][1] = (pObj->runPrm.blockNum & 0x080)?1:0;
	pObj->runPrm.windowEnableMap[0][2] = (pObj->runPrm.blockNum & 0x001)?1:0;
	pObj->runPrm.windowEnableMap[1][2] = (pObj->runPrm.blockNum & 0x002)?1:0;
	pObj->runPrm.windowEnableMap[2][2] = (pObj->runPrm.blockNum & 0x004)?1:0;
	pObj->runPrm.windowEnableMap[3][2] = (pObj->runPrm.blockNum & 0x008)?1:0;

	pObj->MvDataOffset = 0;

	for(i=0;((pObj->win_height*i)<pObj->frame_height);i++)
	{
		if(i >= ALG_MOTION_DETECT_VERT_REGIONS)
		    continue;

		for(j=0;((pObj->win_width*j)<pObj->frame_width);j++)
		{
			if(j >= ALG_MOTION_DETECT_HORZ_REGIONS)
			    continue;

			if(pObj->runPrm.windowEnableMap[j][i] == 1) //1- True , 0 - False
			{
				pObj->MvDataOffset = pObj->win_width*j + pObj->frame_width * pObj->win_height * i ;

				if(ALG_motionDetectCalc(pObj) == ALG_MOTION_S_DETECT )
			    {
					pObj->runStatus.windowMotionDetected[j][i] = 1; //Motion Detected
					detect_cnt++;
				}
				else
					 pObj->runStatus.windowMotionDetected[j][i] = 0; //Motion Not Detected
			}
		}
	}

	return ((detect_cnt>0) ? ALG_MOTION_S_DETECT:ALG_MOTION_S_NO_DETECT);
}

int ALG_motionDetectRun(void *hndl, ALG_MotionDetectRunPrm *prm, ALG_MotionDetectRunStatus *status)
{
	ALG_MotionObj  *pObj = (ALG_MotionObj*)hndl;

	if( pObj == NULL )
		return ALG_MOTION_S_FAIL;

	/*Parm tranfer*/
	pObj->runPrm 	= *prm;
	pObj->runStatus = *status;
	pObj->frame_count++;

	if((pObj->runPrm.isKeyFrame == TRUE) || (pObj->frame_count < pObj->start_cnt))
	{
		return ALG_MOTION_S_NO_DETECT;
	}

	ALG_motionDetectGetThres(pObj);
	ALG_motionDetectGetSensitive(pObj);

	return ALG_motionDetectStart(pObj);
}

int ALG_motionDetectDelete(void *hndl)
{
	ALG_MotionObj *pObj=(ALG_MotionObj *)hndl;

	if(pObj==NULL)
		return ALG_MOTION_S_FAIL;

	OSA_memFree(pObj);

	return ALG_MOTION_S_OK;
}
