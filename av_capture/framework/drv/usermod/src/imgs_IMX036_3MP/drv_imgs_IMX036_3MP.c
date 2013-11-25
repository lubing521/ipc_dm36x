
#include "drv_imgs_IMX036_3MP.h"
#include <drv_gpio.h>
#include <math.h>

#define __DRV_IMGS_IMX036_DEBUG__
#ifdef __DRV_IMGS_IMX036_DEBUG__
#define DB(fmt, args...) fprintf(stderr,fmt, ## args)
#else
#define DB(fmt, args...)
#endif
#define DBErr(fmt, args...) fprintf(stderr,fmt, ## args)

#define ID00_CNT	(0xFF)
#define ID01_CNT	(0xED)
const Uint8 IMX036_ID00_MEM[ID00_CNT] = {
0x55,
0x70,
0x00,
0x08,
0x0D,
0x00,
0x00,
0x98,
0x08,
0x65,
0x04,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x0D,
0x00,
0xE0,
0x00,
0x00,
0x0F,
0x00,
0x00,
0xF0,
0x00,
0x13,
0x5C,
0x00,
0x00,
0x47,
0x00,
0xD6,
0x00,
0x00,
0x22,
0x10,
0x00,
0x88,
0x06,
0x06,
0x24,
0x04,
0x86,
0x01,
0x00,
0x00,
0x00,
0x00,
0x40,
0x00,
0x00,
0xC0,
0x00,
0x00,
0xB8,
0x08,
0x40,
0x00,
0xC0,
0x16,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x20,
0x80,
0x00,
0xFF,
0x02,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x01,
0x00,
0x01,
0x09,
0xA0,
0x20,
0x00,
0x00,
0x80,
0x00,
0x00,
0xB1,
0x01,
0x70,
0x60,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x82,
0x00,
0x02,
0x00,
0x2C,
0x2C,
0x00,
0x00,
0xF2,
0x19,
0x00,
0x05,
0x20,
0x00,
0x00,
0x40,
0x00,
0x00,
0x30,
0x00,
0x00,
0x00,
0x18,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x02,
0x04,
0x0F,
0x00,
0x01,
0x48,
0x05,
0x00,
0x00,
0x00,
0xFF,
0x01,
0xFF,
0x02,
0x7F,
0x03,
0xBF,
0x03,
0x00,
0x00,
0x0A,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x10,
0x00,
0x10,
0x40,
0x20,
0x66,
0x0E,
0x01,
0x06,
0x06,
0x0C,
0x10,
0x00,
0x7C,
0x00,
0x00,
0xFF,
0xFF,
0xFF,
0xFF,
0xFF,
0xFF,
0x00,
0xC9,
0x00,
0x00,
0x00,
0x12,
0x01,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00
};


const Uint8 IMX036_ID01_MEM[ID01_CNT] = {
0x01,
0xA0,
0x3F,
0x01,
0xF0,
0x00,
0xF0,
0xA0,
0x10,
0x10,
0x24,
0x42,
0xFA,
0x43,
0x43,
0x46,
0x04,
0x41,
0x35,
0x41,
0x35,
0x51,
0x49,
0x41,
0x35,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x09,
0xC1,
0x10,
0x01,
0x60,
0x44,
0xFA,
0x43,
0x43,
0x01,
0x40,
0x43,
0xFA,
0x43,
0x43,
0x4A,
0xB4,
0x3F,
0x01,
0xD0,
0x00,
0x00,
0x10,
0x01,
0xFA,
0x13,
0x00,
0xFA,
0x13,
0x00,
0x01,
0xC0,
0x02,
0x02,
0x10,
0x00,
0xEE,
0xF0,
0x0E,
0x21,
0x10,
0x00,
0x01,
0x00,
0x28,
0x00,
0x31,
0x00,
0x31,
0x00,
0x16,
0x01,
0x16,
0x01,
0x01,
0x00,
0x36,
0x00,
0x0F,
0x00,
0xFA,
0x03,
0x0E,
0x00,
0xF1,
0x00,
0x02,
0x00,
0xF9,
0x03,
0x02,
0x00,
0x01,
0x00,
0x34,
0x00,
0x2D,
0x00,
0xFB,
0x00,
0xF6,
0x00,
0x01,
0x00,
0x2D,
0x00,
0x03,
0x00,
0x01,
0x00,
0x2F,
0x00,
0x2D,
0x00,
0xF8,
0x00,
0xF6,
0x00,
0x01,
0x00,
0xF6,
0x00,
0x0B,
0x00,
0x16,
0x00,
0x21,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0xF7,
0x00,
0x0B,
0x01,
0x06,
0x00,
0x0E,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x16,
0x00,
0x07,
0x00,
0x16,
0x00,
0x07,
0x00,
0x0E,
0x00,
0x0B,
0x01,
0x0B,
0x01,
0x0E,
0x00,
0x0E,
0x00,
0x0B,
0x01,
0x0F,
0x01,
0x13,
0x01,
0x12,
0x00,
0x16,
0x00,
0xF7,
0x30,
0x11,
0x06,
0x60,
0x01,
0x36,
0x00,
0xEF,
0x60,
0x13,
0xFB,
0x03,
0x24,
0x04,
0x28,
0x04,
0x00,
0x00,
0x00,
0x00,
0x3F,
0x00,
0xEE,
0x00,
0x3F,
0x01,
0xFA,
0x03,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00
};
#define IMX036_AGAIN_IDXMAX	(81)
unsigned short IMX036_AGainTbl[IMX036_AGAIN_IDXMAX]={
0x0,
0x23,
0x44,
0x65,
0x84,
0xA2,
0xC0,
0xDC,
0xF7,
0x112,
0x12B,
0x144,
0x15B,
0x172,
0x189,
0x19E,
0x1B3,
0x1C7,
0x1DA,
0x1ED,
0x1FF,
0x210,
0x221,
0x231,
0x241,
0x250,
0x25F,
0x26D,
0x27B,
0x288,
0x295,
0x2A1,
0x2AD,
0x2B8,
0x2C4,
0x2CE,
0x2D9,
0x2E3,
0x2EC,
0x2F6,
0x2FF,
0x308,
0x310,
0x318,
0x320,
0x328,
0x32F,
0x336,
0x33D,
0x344,
0x34A,
0x350,
0x356,
0x35C,
0x361,
0x367,
0x36C,
0x371,
0x376,
0x37B,
0x37F,
0x383,
0x388,
0x38C,
0x390,
0x394,
0x397,
0x39B,
0x39E,
0x3A2,
0x3A5,
0x3A8,
0x3AB,
0x3AE,
0x3B1,
0x3B3,
0x3B6,
0x3B8,
0x3BB,
0x3BD,
0x3BF
};

DRV_ImgsObj gDRV_imgsObj;
Bool gbIsDDRIn = FALSE;
Bool gbIsAlawEnalbe = FALSE;
int gCurrsensorMode = DRV_IMGS_SENSOR_MODE_1920x1080;

int DRV_imgsGetDdrInMode(void){
	return (int)gbIsDDRIn;
}

int DRV_imgsOpen(DRV_ImgsConfig *config)
{
	int status=0;
	Uint16 width, height;
	DB("############OPEN###########\n");
	memset(&gDRV_imgsObj, 0, sizeof(gDRV_imgsObj));
	gCurrsensorMode = config->sensorMode;
	DRV_imgGetWidthHeight(config->sensorMode, &width, &height);
	if(gCurrsensorMode==DRV_IMGS_SENSOR_MODE_2048x1536){
		width+=IMGS_H_3M_PAD;
		height+=IMGS_V_3M_PAD;
	}else{
	width+=IMGS_H_PAD;
	height+=IMGS_V_PAD;
	}
	#if (1)  
	DRV_imgsCalcFrameTime(config->fps, width, height, config->binEnable);
	#else
	DRV_imgsCalcFrameTime(config->fps, width, height, config->binEnable);
	gDRV_imgsObj.curModeConfig.sensorDataWidth   = width;
	gDRV_imgsObj.curModeConfig.sensorDataHeight  = height;
	gDRV_imgsObj.curModeConfig.validStartX       = IMGS_H_PAD/2;
	gDRV_imgsObj.curModeConfig.validStartY       = IMGS_V_PAD/2;    
	gDRV_imgsObj.curModeConfig.validWidth        = width-IMGS_H_PAD;      
	gDRV_imgsObj.curModeConfig.validHeight       = height-IMGS_V_PAD;        
	gDRV_imgsObj.curModeConfig.binEnable         = 0;
	#endif
	DB("############reset###########\n");
	/* Sensor reset */
	DRV_gpioSetMode(IMGS_RESET_GPIO, DRV_GPIO_DIR_OUT);
	DRV_gpioSet(IMGS_RESET_GPIO);  
	DRV_gpioClr(IMGS_RESET_GPIO);
	OSA_waitMsecs(50);
	DRV_gpioSet(IMGS_RESET_GPIO);  
	OSA_waitMsecs(50);  
	DB("############SPI OPEN###########\n");	
	status = DRV_SPIOpen(&gDRV_imgsObj.spiHndl, IMGS_SPI_ADDR);
	if(status!=OSA_SOK) {
		OSA_ERROR("DRV_spiOpen()\n");
		return status;
	}
	DB("############SET REG###########\n");	
	status = DRV_imgsSetRegs(width,  height, config->fps);
	if(status!=OSA_SOK) {
		OSA_ERROR("DRV_imgsCheckId()\n");
		DRV_SPIClose(&gDRV_imgsObj.spiHndl);    
		return status;
	}
	DB("############OPEN END###########\n");	
	gbIsDDRIn = config->bIsDdrIn;
	gbIsAlawEnalbe = config->bIsALawEnable;

	return 0;
}

int DRV_imgsClose()
{
	int status;
	//DRV_imgs_DarkframeRelease();
	status = DRV_imgsEnable(FALSE);
	status |= DRV_SPIClose(&gDRV_imgsObj.spiHndl);

	return status;
}

char* DRV_imgsGetImagerName()
{
	return "IMX036_3MP";
}

int DRV_imgsSpecificSetting(void)
{
#if 0
	Uint8 regAddr[30];
  Uint16 regValue[30];
  int i, status;

  i=0;
  regAddr[i]  = 0x29;  regValue[i] = 0x481;  	i++;
	regAddr[i]  = 0x3E;  regValue[i] = 0x87;  	i++;
	regAddr[i]  = 0x3F;  regValue[i] = 0x07;  	i++;
	regAddr[i]  = 0x41;  regValue[i] = 0x0003;  i++;
	regAddr[i]  = 0x42;  regValue[i] = 0x0003;  i++;
	regAddr[i]  = 0x43;  regValue[i] = 0x0003;  i++;
	regAddr[i]  = 0x4f;  regValue[i] = 0x0014;  i++;
	regAddr[i]  = 0x48;  regValue[i] = 0x18;  	i++;
	regAddr[i]  = 0x5F;  regValue[i] = 0x1C16;  i++;
	regAddr[i]  = 0x57;  regValue[i] = 0x7;  		i++;
	regAddr[i]  = 0x70;  regValue[i] = 0x005C;  i++;
	regAddr[i]  = 0x71;  regValue[i] = 0x5B00;  i++;
	regAddr[i]  = 0x72;  regValue[i] = 0x5900;  i++;
	regAddr[i]  = 0x73;  regValue[i] = 0x200;  	i++;
	regAddr[i]  = 0x74;  regValue[i] = 0x200;  	i++;
	regAddr[i]  = 0x75;  regValue[i] = 0x2800;  i++;
	regAddr[i]  = 0x76;  regValue[i] = 0x3E29;  i++;
	regAddr[i]  = 0x77;  regValue[i] = 0x3E29;  i++;
	regAddr[i]  = 0x78;  regValue[i] = 0x583F;  i++;
	regAddr[i]  = 0x79;  regValue[i] = 0x5B00;  i++;
	regAddr[i]  = 0x7A;  regValue[i] = 0x5A00;  i++;
	regAddr[i]  = 0x7B;  regValue[i] = 0x5900;  i++;
	regAddr[i]  = 0x7C;  regValue[i] = 0x5900;  i++;
	regAddr[i]  = 0x7E;  regValue[i] = 0x5900;  i++;
	regAddr[i]  = 0x7F;  regValue[i] = 0x5900;  i++;

  status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
  if(status!=OSA_SOK) {
    OSA_ERROR("DRV_i2cWrite16()\n");
    return status;
  }

  return status;
#else
  return OSA_SOK;
#endif
}


int DRV_imgsSet50_60Hz(Bool is50Hz)
{
#if (1)
	#define IMX036_REG00_SET(X,Y) count=0;regset[count++]=IMGS_SPI_ADDR;regset[count++]=X;regset[count++]=Y;DRV_SPIWrite8(&gDRV_imgsObj.spiHndl, regset,count);
	int count=0;
	Uint8 regset[3];

	if(is50Hz==0){
		/* NTSC */
		IMX036_REG00_SET(0x07,0x98);  //H 2200
		IMX036_REG00_SET(0x08,0x08);
	}else{
		/* PAL */
		IMX036_REG00_SET(0x07,0x50);
		IMX036_REG00_SET(0x08,0x0A);
	}
#else
	int fps;

	if( gDRV_imgsObj.curFrameTime.fps==30
	||gDRV_imgsObj.curFrameTime.fps==25
	) {
		if(is50Hz)
			fps = 25;
		else
			fps = 30;
		gDRV_imgsObj.curFrameTime.Is50Hz = is50Hz;
		DRV_imgsSetFramerate(fps);
	}
#endif
	return 0;

}

int DRV_imgsSetFramerate(int fps)
{
#if (1)
	printf("XXXXXXXXXXXXX DRV_imgsSetFramerate() Not ready!!\nXXXXXXXXXXXXX");
	return OSA_SOK;
#else
		int status, i;
		Uint8 regAddr[6], regValue[6];
		Uint16 SetValue;
		DRV_ImgsFrameTime *pFrame = &gDRV_imgsObj.curFrameTime;

		i = 0;		
#if 0 /*By cahnging pixel clk*/
		regAddr[i]	= REG5C;
		switch(pFrame->pll_PREDIV){
			case 1:
			regValue[i] = 0x19;
			break;
		case 2:
			regValue[i] = 0x59;
			break;
		case 4:
			regValue[i] = 0x79;
			break;
		default:
			/* never run here */
			regValue[i] = 0x59;
			break;
		}
		i++;
	
		regAddr[i]	= REG5D;
		regValue[i] = 0x00 | ((pFrame->pll_DIV -1) << 2);
		i++;
#else /*By cahnging HSIZE */
	
		if( pFrame->pll_PREDIV == 2 &&	pFrame->pll_DIV == 1 )
		{ 
			if( fps == 30 )
			{
				regAddr[i]	= REG2A;
				regValue[i] = 0x98;
				i++;
				regAddr[i]	= REG2B;
				regValue[i] = 0x06;
				i++;
			}else{
				regAddr[i]	= REG2A;
				regValue[i] = 0xE9;
				i++;
				regAddr[i]	= REG2B;
				regValue[i] = 0x07;
				i++;
			}
		}
		else if( pFrame->pll_PREDIV == 4 && 	pFrame->pll_DIV == 1 )
		{ 
			if( fps == 30 )
			{
				regAddr[i]	= REG2A;
				regValue[i] = 0x30;
				i++;
				regAddr[i]	= REG2B;
				regValue[i] = 0x0D;
				i++;
			}else{
				regAddr[i]	= REG2A;
				regValue[i] = 0xD3;
				i++;
				regAddr[i]	= REG2B;
				regValue[i] = 0x0F;
				i++;

			}
		}
		else if( pFrame->pll_PREDIV == 4 && 	pFrame->pll_DIV == 2 )
		{
			if( fps == 30 )
			{
				regAddr[i]	= REG2A;
				regValue[i] = 0x60;
				i++;
				regAddr[i]	= REG2B;
				regValue[i] = 0x1A;
				i++;
			}else{
				regAddr[i]	= REG2A;
				regValue[i] = 0xA6;
				i++;
				regAddr[i]	= REG2B;
				regValue[i] = 0x1F;
				i++;
			}
		}
#endif
	 
		status = DRV_i2cWrite8(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
		if(status!=OSA_SOK) {
			OSA_ERROR("DRV_i2cWrite8()\n");
			return status;
		}
	
		return status;

  return OSA_SOK;
#endif
}

int DRV_imgsBinEnable(Bool enable)
{
#if 0
  Uint8 regAddr[4];
  Uint16 regValue[4];
  int i, col_bin, row_bin, status;

  if(!enable) {
    col_bin = 0;
    row_bin = 0;
  } else {
    col_bin = gDRV_imgsObj.curFrameTime.col_bin;
    row_bin = gDRV_imgsObj.curFrameTime.row_bin;
  }

  i=0;
  regAddr[i]  = ROW_ADDR_MODE;
  regValue[i] = (gDRV_imgsObj.curFrameTime.row_skip & 0x7 ) | ((row_bin & 0x3) << 4);
  i++;

  regAddr[i]  = COL_ADDR_MODE;
  regValue[i] = (gDRV_imgsObj.curFrameTime.col_skip & 0x7 ) | ((col_bin & 0x3) << 4);
  i++;

  status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
  if(status!=OSA_SOK) {
    OSA_ERROR("DRV_i2cWrite16()\n");
    return status;
  }

  return status;
#else
  return OSA_EFAIL;
#endif
}

int DRV_imgsBinMode(int binMode)
{
#if 0
  Uint8 regAddr[4];
  Uint16 regValue[4];
  int i, status;

  i=0;
  regAddr[i]  = READ_MODE_2;
  regValue[i] = binMode;
  i++;


  status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
  if(status!=OSA_SOK) {
    OSA_ERROR("DRV_i2cWrite16()\n");
    return status;
  }

  return status;
#else
  return OSA_SOK;
#endif
}

//return value is 8 times the true analog gain
int DRV_imgsGetAgain(int *again)
{
	/* Not support */
	return OSA_EFAIL;
}

int DRV_imgsGetEshutter(Uint32 *eshutterInUsec)
{
	/* Not support */
	return OSA_EFAIL;
}

int DRV_imgsSetOB(int Level)
{
	int	status = OSA_SOK;
	#define IMX036_REG00_SET(X,Y) count=0;regset[count++]=IMGS_SPI_ADDR;regset[count++]=X;regset[count++]=Y;DRV_SPIWrite8(&gDRV_imgsObj.spiHndl, regset,count);
	int count=0;
	Uint8 regset[3];

	IMX036_REG00_SET(0x20, (Level>>8)&0xFF);	//AGAIN High
	IMX036_REG00_SET(0x1F, (Level&0xFF) );		//AGAIN LOW
	return status;
}

int DRV_imgsSetDgain(int dgain)
{
    return OSA_SOK;
}

int DRV_imgsSetAgain(int again, int setRegDirect)
{
	int	status = OSA_SOK;
	#define IMX036_REG00_SET(X,Y) count=0;regset[count++]=IMGS_SPI_ADDR;regset[count++]=X;regset[count++]=Y;DRV_SPIWrite8(&gDRV_imgsObj.spiHndl, regset,count);
	#define IMX036_REGSET3(X,V1,V2) count=0;regset[count++]=IMGS_SPI_ADDR;regset[count++]=X;regset[count++]=V1;regset[count++]=V2;DRV_SPIWrite8(&gDRV_imgsObj.spiHndl, regset,count);
	int count=0;
	Uint8 regset[5];
	int SetIdx;
	//unsigned long tmp;
	unsigned short SetVal1,SetVal2;
	#define MAX(a, b)   (((a)>(b))? (a):(b))
	#define MIN(a, b)   (((a)<(b))? (a):(b))

#if (1)
	/* 0db~42db , 0.3db/step */
	//printf("AG:\t%d\n",again);	
	if(again<1000 || again>128000){
		DBErr("XXXXX DRV_imgsSetAgain() Error(%d)\n",again);
		//status = OSA_EFAIL;
		return status;
	}
//printf("AGAIN----------------\n");
	if(again>16000){
		SetIdx = IMX036_AGAIN_IDXMAX-1;
		SetVal1 = IMX036_AGainTbl[SetIdx];
		SetVal2 = (200*log((double)again/16000)/(3*log(10)));
	}else{
		SetIdx = (200*(log((double)again/1000))/(3*log(10)));
		SetIdx = MIN(SetIdx,IMX036_AGAIN_IDXMAX-1);
		SetVal1 = IMX036_AGainTbl[SetIdx];
		SetVal2 = 0;
	}
	//printf("IDX,S1,S2 = %02d,0x%04X,0x%04X\n",SetIdx,SetVal1,SetVal2);
#if 0
	IMX036_REG00_SET(0x06, (SetVal1>>8)&0xFF);	//AGAIN High
	IMX036_REG00_SET(0x05, (SetVal1&0xFF) );		//AGAIN LOW
	IMX036_REG00_SET(0x61, (SetVal2&0xFF) );		//Sensor DGain
#else
	IMX036_REGSET3(0x05,(SetVal1&0xFF),((SetVal1>>8)&0xFF));
	IMX036_REG00_SET(0x61, (SetVal2&0xFF) );		//Sensor DGain
#endif
	return status;
	
#else
	int Gain_Step = again; //1;	
	if ( Gain_Step == 0 ){		/* 0db */
		IMX036_REG00_SET(0x06,0x00);
		IMX036_REG00_SET(0x05,0x00);
		IMX036_REG00_SET(0x61,0x00);		//Sensor DGain
	}else if ( Gain_Step == 1 ){   /* 06db */
		IMX036_REG00_SET(0x06,0x01);
		IMX036_REG00_SET(0x05,0xFF);
		IMX036_REG00_SET(0x61,0x00);		//Sensor DGain
	}else if ( Gain_Step == 2 ){   /* 12db */
		IMX036_REG00_SET(0x06,0x02);
		IMX036_REG00_SET(0x05,0xFF);
		IMX036_REG00_SET(0x61,0x00);		//Sensor DGain
	}else if ( Gain_Step == 3 ){	   /* 18db */
		IMX036_REG00_SET(0x06,0x03);
		IMX036_REG00_SET(0x05,0x7F);
		IMX036_REG00_SET(0x61,0x00);		//Sensor DGain
	}else if ( Gain_Step == 4 ){   /* 24db */
		IMX036_REG00_SET(0x06,0x03);
		IMX036_REG00_SET(0x05,0xBF);
		IMX036_REG00_SET(0x61,0x00);		//Sensor DGain
	}else if ( Gain_Step == 5 ){   /* 30db */
		IMX036_REG00_SET(0x06,0x03);
		IMX036_REG00_SET(0x05,0xBF);
		IMX036_REG00_SET(0x61,0x14);		//Sensor DGain
	}else if  ( Gain_Step == 6 ){ /* 36db */
		IMX036_REG00_SET(0x06,0x03);
		IMX036_REG00_SET(0x05,0xBF);
		IMX036_REG00_SET(0x61,0x28);		//Sensor DGain
	}else if  ( Gain_Step == 7 ){ /* 42db */
		IMX036_REG00_SET(0x06,0x03);
		IMX036_REG00_SET(0x05,0xBF);
		IMX036_REG00_SET(0x61,0x3C);		//Sensor DGain
	}else{
		OSA_ERROR("Gain Error(%d)\r\n",Gain_Step);
	}

	return status;
#endif
}

int DRV_imgsSetEshutter(Uint32 eshutterInUsec, int setRegDirect)
{
	int	status = OSA_SOK;
	Uint32	SetValue,SUB_Control=0,tempdata;
	Uint8	Send_HH,Send_H,Send_L;
	DRV_ImgsFrameTime *pFrame = &gDRV_imgsObj.curFrameTime;
	#define IMX036_REG00_SET(X,Y) count=0;regset[count++]=IMGS_SPI_ADDR;regset[count++]=X;regset[count++]=Y;DRV_SPIWrite8(&gDRV_imgsObj.spiHndl, regset,count);
	#define IMX036_REGSET4(X,V1,V2,V3) count=0;regset[count++]=IMGS_SPI_ADDR;regset[count++]=X;regset[count++]=V1;regset[count++]=V2;regset[count++]=V3;DRV_SPIWrite8(&gDRV_imgsObj.spiHndl, regset,count);
	int count=0;
	Uint8 regset[5];

	if(eshutterInUsec==0){
		printf("STError:\t%d\n",eshutterInUsec);
		return status;
	}
		
	if(setRegDirect) {
		// eshutterInUsec in line
		SetValue = eshutterInUsec;
	} else  {
		// eshutterInUsec in Usec.
		DRV_imgsCalcSW(eshutterInUsec);
		SetValue = pFrame->SW;
	}
//printf("xxxST(%d):\t%d\n",setRegDirect,SetValue);
	if(gCurrsensorMode==DRV_IMGS_SENSOR_MODE_2048x1536){
	
		if(SetValue<IMGS_SENSOR_LINEWIDTH_3MP){
			SUB_Control = IMGS_SENSOR_LINEWIDTH_3MP - SetValue; 
		}else{
			SUB_Control = 0;
		}
		Send_HH=(Uint8)(SUB_Control>>16);
		Send_H=(Uint8)((SUB_Control>>8)&0x00FF);
		Send_L=(Uint8)(SUB_Control & 0x00FF);
		IMX036_REGSET4(0x0C,Send_L,Send_H,Send_HH);
		/* long exposure == > 1024*128/1152/30= 3.8 sec !!*/
		if(SetValue<IMGS_SENSOR_LINEWIDTH_3MP){
			tempdata = IMGS_SENSOR_LINEWIDTH_3MP;
		}else{
			tempdata = SetValue;
		}
		Send_HH=(Uint8)(tempdata>>16);
		Send_H=(Uint8)((tempdata>>8)&0x00FF);
		Send_L=(Uint8)(tempdata & 0x00FF);
		IMX036_REGSET4(0x09,Send_L,Send_H,Send_HH);	

	}else{
	
	if(SetValue<IMGS_SENSOR_LINEWIDTH){
		SUB_Control = IMGS_SENSOR_LINEWIDTH - SetValue; 
	}else{
		SUB_Control = 0;
	}
	Send_HH=(Uint8)(SUB_Control>>16);
	Send_H=(Uint8)((SUB_Control>>8)&0x00FF);
	Send_L=(Uint8)(SUB_Control & 0x00FF);
	IMX036_REGSET4(0x0C,Send_L,Send_H,Send_HH);
	/* long exposure == > 1024*128/1152/30= 3.8 sec !!*/
	if(SetValue<IMGS_SENSOR_LINEWIDTH){
		tempdata = IMGS_SENSOR_LINEWIDTH;
	}else{
		tempdata = SetValue;
	}
	Send_HH=(Uint8)(tempdata>>16);
	Send_H=(Uint8)((tempdata>>8)&0x00FF);
	Send_L=(Uint8)(tempdata & 0x00FF);
	IMX036_REGSET4(0x09,Send_L,Send_H,Send_HH);
		
	}

	#if (0)
	if( gDRV_imgsObj.curFrameTime.Is50Hz == 0 ){
		DRV_imgsSetFramerate(30);
	}else{
		DRV_imgsSetFramerate(25);
	}  
	#endif

	return status;
}

int DRV_imgsSetDcSub(Uint32 dcSub, int setRegDirect)
{
	return OSA_SOK;
}

int DRV_imgsEnable(Bool enable)
{
	return OSA_SOK;
}

int DRV_imgsGetLightSensorMode(void){
	/* return Low light:1, Normal:0 */
	/* LS:Normal 1, low light 0 */
	int Val;
	static int is1stTime=1;
	static int PreVal = -1;
	if(is1stTime){
		DRV_gpioSetMode(IMGS_LIGHT_SENSOR, DRV_GPIO_DIR_IN);
		DRV_gpioSetMode(IMGS_LIGHT_SENSOR_LINK, DRV_GPIO_DIR_IN);		
		is1stTime = 0;
	}

	Val = !DRV_gpioGet(IMGS_LIGHT_SENSOR);
	if(Val!=PreVal){
		PreVal = Val;
		//DBErr("----------------------------->LS:%d\n",Val);
	}
	return Val;
}

int DRV_imgs_IsDDRIn(void){
	return gbIsDDRIn;
}

int DRV_imgs_IsAlawEnable(void){
	return gbIsAlawEnalbe;
}

int DRV_imgsNDShutter(int bIsNight)
{
	int status=0;
	static int FirstTimeIn=1;
	static int Pre_Mode=-1;

	if(Pre_Mode==bIsNight){
		return (0);
	}
	Pre_Mode = bIsNight;
	
	if(FirstTimeIn){
		DRV_gpioSetMode(IMGS_ND_CTRL1, DRV_GPIO_DIR_OUT);
		DRV_gpioSetMode(IMGS_ND_CTRL2, DRV_GPIO_DIR_OUT);
		/* Day mode. (default) */
		DRV_gpioSet(IMGS_ND_CTRL2); 
		OSA_waitMsecs(30); // 20ms   
		DRV_gpioClr(IMGS_ND_CTRL2);	
		OSA_waitMsecs(30); // 20ms   

		DRV_gpioClr(IMGS_ND_CTRL1);
		DRV_gpioClr(IMGS_ND_CTRL2);
		FirstTimeIn = 0;
		DBErr("ND Init\n");
	}else{
		DRV_gpioSetMode(IMGS_ND_CTRL1, DRV_GPIO_DIR_OUT);
		DRV_gpioSetMode(IMGS_ND_CTRL2, DRV_GPIO_DIR_OUT);
	}
	
	if(bIsNight){
		/* Night mode. */
		DBErr("ND:Night\n");
		DRV_gpioSet(IMGS_ND_CTRL1);
		OSA_waitMsecs(30); // 20ms
		DRV_gpioClr(IMGS_ND_CTRL1);
	}else{
		/* Day mode. */
		DBErr("ND:Day\n");
		DRV_gpioSet(IMGS_ND_CTRL2); 
		OSA_waitMsecs(30); // 20ms   
		DRV_gpioClr(IMGS_ND_CTRL2);	
	}	
	return status; 	
}

DRV_ImgsModeConfig      *DRV_imgsGetModeConfig(int sensorMode)
{
  return &gDRV_imgsObj.curModeConfig;
}

DRV_ImgsIsifConfig      *DRV_imgsGetIsifConfig(int sensorMode)
{
  return &gDRV_imgsIsifConfig_Common;
}

DRV_ImgsIpipeConfig     *DRV_imgsGetIpipeConfig(int sensorMode, int vnfDemoCfg)
{
  if(vnfDemoCfg)
  	return &gDRV_imgsIpipeConfig_Vnfdemo;
  else
  	return &gDRV_imgsIpipeConfig_Common;
}

DRV_ImgsH3aConfig       *DRV_imgsGetH3aConfig(int sensorMode, int aewbVendor)
{
  if(aewbVendor==1) {
  	printf("APPRO Setting Applied\n");
  	return &gDRV_imgsH3aConfig_Appro;
  }
  else if(aewbVendor==2) {
  	printf("TI Setting Applied\n");
  	return &gDRV_imgsH3aConfig_TI;
  }
  else {
  	printf("Common Setting Applied\n");
  	return &gDRV_imgsH3aConfig_Common;
  }
}

DRV_ImgsLdcConfig       *DRV_imgsGetLdcConfig(int sensorMode, Uint16 ldcInFrameWidth, Uint16 ldcInFrameHeight)
{
  OSA_printf(" DRV LDC: GetConfig %04x %dx%d\n", sensorMode, ldcInFrameWidth, ldcInFrameHeight);

  sensorMode &= 0xFF;

  switch(sensorMode) {

    case DRV_IMGS_SENSOR_MODE_720x480:
    case DRV_IMGS_SENSOR_MODE_1620x1080:

      if(ldcInFrameWidth==864)
        return &gDRV_imgsLdcConfig_736x480_0_VS;

      if(ldcInFrameWidth==352)
        return &gDRV_imgsLdcConfig_736x480_1_VS;

      if(ldcInFrameWidth==736)
        return &gDRV_imgsLdcConfig_736x480_0;

      if(ldcInFrameWidth==288)
        return &gDRV_imgsLdcConfig_736x480_1;

      if(ldcInFrameWidth==768)
        return &gDRV_imgsLdcConfig_768x512_0;

      if(ldcInFrameWidth==320)
        return &gDRV_imgsLdcConfig_768x512_1;

      if(ldcInFrameWidth==928)
        return &gDRV_imgsLdcConfig_768x512_0_VS;

      if(ldcInFrameWidth==384)
        return &gDRV_imgsLdcConfig_768x512_1_VS;

      break;

    case DRV_IMGS_SENSOR_MODE_1280x720:
    case DRV_IMGS_SENSOR_MODE_1920x1080:

      if(ldcInFrameWidth==1280)
        return &gDRV_imgsLdcConfig_1280x736_0;

      if(ldcInFrameWidth==320)
        return &gDRV_imgsLdcConfig_1280x736_1;

      if(ldcInFrameWidth==640)
        return &gDRV_imgsLdcConfig_1280x736_2;

      if(ldcInFrameWidth==1536)
        return &gDRV_imgsLdcConfig_1280x736_0_VS;

      if(ldcInFrameWidth==384)
        return &gDRV_imgsLdcConfig_1280x736_1_VS;

      if(ldcInFrameWidth==768)
        return &gDRV_imgsLdcConfig_1280x736_2_VS;

      if(ldcInFrameWidth==1312)
        return &gDRV_imgsLdcConfig_1312x768_0;

      if(ldcInFrameWidth==352)
        return &gDRV_imgsLdcConfig_1312x768_1;

      if(ldcInFrameWidth==672)
        return &gDRV_imgsLdcConfig_1312x768_2;

      if(ldcInFrameWidth==1600)
        return &gDRV_imgsLdcConfig_1312x768_0_VS;

      if(ldcInFrameWidth==448)
        return &gDRV_imgsLdcConfig_1312x768_1_VS;

      if(ldcInFrameWidth==832)
        return &gDRV_imgsLdcConfig_1312x768_2_VS;

      break;

  }

  return NULL;
}

int DRV_imgsReset()
{
	return OSA_SOK;
}

int DRV_imgsCheckId()
{
	return OSA_SOK;
}

int DRV_imgsSetRegs(Uint16 width, Uint16 height, int fps)
{
	int i;
	#define IMX036_REG00_SET(X,Y) count=0;regset[count++]=IMGS_SPI_ADDR;regset[count++]=X;regset[count++]=Y;DRV_SPIWrite8(&gDRV_imgsObj.spiHndl, regset,count);
	#define IMX036_REG01_SET(X,Y) count=0;regset[count++]=IMGS_SPI_ADDR01;regset[count++]=X;regset[count++]=Y;DRV_SPIWrite8(&gDRV_imgsObj.spiHndl, regset,count);
	int count=0;
	int status=0;
	Uint8 regset[3];

	/* Send by SONY suggestion */
	/* initial Table ==> O/P 12bits,Master Mode ;HD 2200 X 1125 ( 1920X1080 ) * 30 Frame/sec,  Bit rate ==> 74.25Mhz .*/
	for ( i=0;i< ID00_CNT; i++ ){
		IMX036_REG00_SET(i,IMX036_ID00_MEM[i]);
	}
	for ( i=0;i< ID01_CNT; i++ ){
		IMX036_REG01_SET(i,IMX036_ID01_MEM[i]);
	}
	
	if(gCurrsensorMode==DRV_IMGS_SENSOR_MODE_2048x1536){
		/* 3MP 20 fps */
		IMX036_REG00_SET(0x01,0x00);// WINDOW MODE change ( full size mode ) 
		IMX036_REG00_SET(0x0b,0x00);// VMAX setting 1608 line ==> keep the frame rate 20.9 frame/sec
		IMX036_REG00_SET(0x0a,0x06);
		IMX036_REG00_SET(0x09,0x48); 

	IMX036_REG00_SET(0x2D,0x08);	// master O/P mode 
	IMX036_REG00_SET(0x00,0x54);	// out of standby
	
	DRV_imgsSetAgain(1000, 0); // default aGain
	DRV_imgsSetEshutter(IMGS_SENSOR_LINEWIDTH, 1);
	DRV_imgsSetOB(240);	// 240(12bits )/4=60 ( 10bits )
	}else{
		/* 1080p 30fps */
		IMX036_REG00_SET(0x01,0x70);// WINDOW MODE change ( full HD size mode ) 
		IMX036_REG00_SET(0x0b,0x00);// VMAX setting 1125 line ==> keep the frame rate 30 frame/sec
		IMX036_REG00_SET(0x0a,0x04);
		IMX036_REG00_SET(0x09,0x65); 

		IMX036_REG00_SET(0x2D,0x08);	// master O/P mode 
		IMX036_REG00_SET(0x00,0x54);	// out of standby

		DRV_imgsSetAgain(1000, 0); // default aGain
		DRV_imgsSetEshutter(IMGS_SENSOR_LINEWIDTH, 1);
		DRV_imgsSetOB(240);	// 240(12bits )/4=60 ( 10bits )
	}

	
	return status;
}

int DRV_imgs_APChipInit(int *pGioC1, int *pGioD1){
	unsigned int *pPINMUXC1 = NULL;
	unsigned int *pPINMUXD1 = NULL;
	unsigned int val1, val2;
	unsigned char *pRegStart; 
	int ret = 0;

	if(gCSL_vpssHndl.regBaseVirtAddr==0 || pGioC1==0 || pGioD1==0){
		return -1;
	}
	pRegStart = (unsigned char *)gCSL_vpssHndl.regBaseVirtAddr;
	
	pPINMUXC1	= (unsigned int *)( pRegStart+ (AP_CHIP_C1_PINMUX-RegBaseVirtStart));
	pPINMUXD1	= (unsigned int *)( pRegStart+ (AP_CHIP_D1_PINMUX-RegBaseVirtStart));
	/* Enable C1 */
	*pPINMUXC1 = ((*pPINMUXC1 & (~AP_CHIP_C1_MASK)) | AP_CHIP_C1_VAL);
	val1 = (*pPINMUXC1 & AP_CHIP_C1_MASK);
	if(AP_CHIP_C1_VAL!= val1){
		DBErr("PINMUXC1 setting error(%X)!!!\n",val1);
		ret = -1;
		goto __DRV_imgs_APChipInit_End__;
	}
	/* Enable D1 */
	*pPINMUXD1 = (*pPINMUXD1 & (~AP_CHIP_D1_MASK)) | (AP_CHIP_D1_VAL);
	val2 = (*pPINMUXD1 & AP_CHIP_D1_MASK);
	if(AP_CHIP_D1_VAL != val2){
		DBErr("PINMUXD1 setting error(%X)!!!\n",val2);
		ret = -1;
		goto __DRV_imgs_APChipInit_End__;
	}

	*pGioC1 = IMGS_APCHIP_C1_GIO;
	*pGioD1 = IMGS_APCHIP_D1_GIO;

__DRV_imgs_APChipInit_End__:

	return ret;
}
