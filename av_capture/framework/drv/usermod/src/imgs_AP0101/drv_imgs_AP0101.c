
#include "drv_imgs_AP0101.h"
#include <drv_gpio.h>

DRV_ImgsObj gDRV_imgsObj;

int DRV_imgsEnable_AP0101(Bool enable);
int DRV_imgsOpen_AP0101(DRV_ImgsConfig *config)
{
    int status, retry=10;
    Uint16 width, height;
    memset(&gDRV_imgsObj, 0, sizeof(gDRV_imgsObj));

    DRV_imgGetWidthHeight(config->sensorMode, &width, &height);

    width+=IMGS_H_PAD;
    height+=IMGS_V_PAD;

    DRV_imgsCalcFrameTime_AP0101(config->fps, width, height, config->binEnable);

    status = DRV_i2cOpen(&gDRV_imgsObj.i2cHndl, IMGS_I2C_ADDR);
    if (status!=OSA_SOK)
    {
        OSA_ERROR("DRV_i2cOpen()\n");
        return status;
    }
    
#ifdef BOARD_AP_IPNC
    DRV_gpioSetMode(IMGS_RESET_GPIO, DRV_GPIO_DIR_OUT);
    DRV_gpioClr(IMGS_RESET_GPIO);
    OSA_waitMsecs(100);
    DRV_gpioSet(IMGS_RESET_GPIO);
    OSA_waitMsecs(100);
#endif

    do
    {
        status = DRV_imgsCheckId_AP0101();
        if (status==OSA_SOK)
        {
            break;
        }
        OSA_waitMsecs(5);
    } while (retry--);

    if (status!=OSA_SOK)
    {
        OSA_ERROR("DRV_imgsCheckId_AP0101()\n");
        DRV_i2cClose(&gDRV_imgsObj.i2cHndl);
        return status;
    }

    return 0;
}

int DRV_imgsClose_AP0101()
{
    int status;

    status = DRV_imgsEnable_AP0101(FALSE);
    status |= DRV_i2cClose(&gDRV_imgsObj.i2cHndl);

    return status;
}

char* DRV_imgsGetImagerName_AP0101()
{
    return "AP0101";
}

#if 1
int DRV_imgsSpecificSetting_AP0101(void)
{
    Uint8  regAddr = 0;
    Uint16 regValue = 0;
    int i, status = OSA_SOK;

    // Sensor Reset
    regAddr = 0x0D; regValue = 0x0001;    //Reset
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    OSA_waitMsecs(10);
    regAddr = 0x0D; regValue = 0x0000;    //Reset done
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    OSA_waitMsecs(10);

    regAddr = 0xF0; regValue = 0x0000;    //set page0
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);

    regAddr = 0x66; regValue = 0x1402; 
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0x67; regValue = 0x0500; 
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0x65; regValue = 0xA000;
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);

    OSA_waitMsecs(1000); 

    regAddr = 0x65; regValue = 0x2000;
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);

    OSA_waitMsecs(1000); 
   
    //*** YCbCr in Context B ***
    regAddr = 0xF0; regValue = 0x0001;    //set page1
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    
    regAddr = 0xC6; regValue = 0x270B;   
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC8; regValue = 0x0030;  
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC6; regValue = 0x2774;  
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC8; regValue = 0x0501;  
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);

    OSA_waitMsecs(10); 

    //*** Set Context B Blanking to Minimum ***
    regAddr = 0xF0; regValue = 0x0000;    //set page0
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    
    regAddr = 0x05; regValue = 0x011E; 
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0x06; regValue = 0x000B; 
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    
    //*** cropping the field of view to achieve higher frame rate ***
    regAddr = 0xF0; regValue = 0x0001;  //set page1
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    
    regAddr = 0xC6; regValue = 0x2709; 	// MCU_ADDRESS [MODE_OUTPUT_HEIGHT_B]
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC8; regValue = 0x02D0; 	// MCU_DATA_0
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC6; regValue = 0x2707; 	// MCU_ADDRESS [MODE_OUTPUT_WIDTH_B]
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC8; regValue = 0x0500; 	// MCU_DATA_0
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC6; regValue = 0x2735; 	// MCU_ADDRESS [MODE_CROP_X0_B]
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC8; regValue = 0x0000; 	// MCU_DATA_0
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC6; regValue = 0x2737; 	// MCU_ADDRESS [MODE_CROP_X1_B]
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC8; regValue = 0x0500; 	// MCU_DATA_0
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC6; regValue = 0x2739; 	// MCU_ADDRESS [MODE_CROP_Y0_B]
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC8; regValue = 0x0000; 	// MCU_DATA_0
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC6; regValue = 0x273B; 	// MCU_ADDRESS [MODE_CROP_Y1_B]
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC8; regValue = 0x02D0; 	// MCU_DATA_0
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC6; regValue = 0x271B; 	// MCU_ADDRESS [MODE_SENSOR_ROW_START_B]
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC8; regValue = 0x001C; 	// MCU_DATA_0
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC6; regValue = 0x271D; 	// MCU_ADDRESS [MODE_SENSOR_COL_START_B]
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC8; regValue = 0x003C; 	// MCU_DATA_0
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC6; regValue = 0x271F; 	// MCU_ADDRESS [MODE_SENSOR_ROW_HEIGHT_B]
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC8; regValue = 0x02D0; 	// MCU_DATA_0
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC6; regValue = 0x2721; 	// MCU_ADDRESS [MODE_SENSOR_COL_WIDTH_B]
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC8; regValue = 0x0500; 	// MCU_DATA_0
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC6; regValue = 0x271B; 	// MCU_ADDRESS [MODE_SENSOR_ROW_START_B]
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC8; regValue = 0x00FE; 	// MCU_DATA_0
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC6; regValue = 0x271D; 	// MCU_ADDRESS [MODE_SENSOR_COL_START_B]
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC8; regValue = 0x00BE; 	// MCU_DATA_0
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC6; regValue = 0xA120; 	// MCU_ADDRESS [SEQ_CAP_MODE]
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC8; regValue = 0x0002; 	// MCU_DATA_0
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC6; regValue = 0xA103; 	// MCU_ADDRESS [SEQ_CMD]
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC8; regValue = 0x0002; 	// MCU_DATA_0
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC6; regValue = 0xA217; 	// MCU_ADDRESS
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC8; regValue = 0x0004; 	// MCU_DATA_0
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC6; regValue = 0xA20E; 	// MCU_ADDRESS
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC8; regValue = 0x0004; 	// MCU_DATA_0
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC6; regValue = 0xA103; 	// MCU_ADDRESS
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC8; regValue = 0x0005; 	// MCU_DATA_0

    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC6; regValue = 0xA404; 	// MCU_ADDRESS [FD_MODE]
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC8; regValue = 0x00C1; 	// MCU_DATA_0
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC6; regValue = 0xA12A; 	// MCU_ADDRESS [SEQ_PREVIEW_1_FD]
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC8; regValue = 0x0002; 	// MCU_DATA_0
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC6; regValue = 0xA103; 	// MCU_ADDRESS
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC8; regValue = 0x0005; 	// MCU_DATA_0
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);

    // I/O Slew rate test
    regAddr  =  0x0A;
    status   =  DRV_i2cRead16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regValue |= ( ((unsigned short)(0x0000)) << 0 );   //10:8 DOUT slew rate 7 fastest, 0 slowest
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);

    // Reduce Flicker
    regAddr = 0xC6; regValue = 0xA12A;
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC8; regValue = 0x0002;
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC6; regValue = 0xA404;
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC8; regValue = 0x00C0;
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC6; regValue = 0xA103;
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC8; regValue = 0x0006;
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);

    // Delay 300
    OSA_waitMsecs(300);

    regAddr = 0xC6; regValue = 0xA103;
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    regAddr = 0xC8; regValue = 0x0005;
    status = DRV_i2cWrite16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);

    // return status;
    return OSA_SOK;
}
#else
int DRV_imgsSpecificSetting_AP0101(void)
{
    Uint16 regAddr[1024] = {0};
    Uint16 regValue[1024] = {0};
    Uint8 regValue8[64] = {0};
    Uint32 regValue32[64] = {0};
    int i = 0, status = OSA_SOK;

    // Sensor Reset
    #if 0
    i = 0;
    regAddr[i] = 0x001A; regValue[i] = 0x0001; i++;    //Reset
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0x001A; regValue[i] = 0x0000; i++;   //Reset done
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    #endif
    #if 0
    i = 0;
    regAddr[i] = 0x0020; regValue[i] = 0x0020; i++;   //mcu boot option
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    #endif
    //Issue SYSMGR_GET_STATE repeatedly until it does not return EBUSY
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8101; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX Issue SYSMGR_GET_STATE = %d\n", regValue[0]);
        if(regValue[0] == 0)
        {
            break;
        }
    }

#if 0   
    //Issue SENSOR_MGR_DISCOVER_SENSOR host command and poll until it returns ENOERR
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8E00; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX Issue SENSOR_MGR_DISCOVER_SENSOR = %d\n", regValue[0]);
        if(regValue[0] == 0)
        {
            break;
        }
    }
    #if 0
    //Reserve RAM to contain a patch
    i = 0;
    regAddr[i] = 0x098E; regValue[i] = 0x7C00; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC00; regValue[i] = 0x0000; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC02; regValue[i] = 0x02D4; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8706; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX Reserve RAM to contain a patch 1 return value = %d\n", regValue[0]);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    OSA_waitMsecs(5);
    //patch
    i = 0;
    regAddr[i] = 0x0982; regValue[i] = 0x0001; i++;
    regAddr[i] = 0x098A; regValue[i] = 0x2398; i++;
    regAddr[i] = 0xA398; regValue[i] = 0xC0F1; i++;
    regAddr[i] = 0xA39A; regValue[i] = 0x09BE; i++;
    regAddr[i] = 0xA39C; regValue[i] = 0x0DA0; i++;
    regAddr[i] = 0xA39E; regValue[i] = 0xD840; i++;
    
    regAddr[i] = 0xA3A0; regValue[i] = 0x0A4A; i++;
    regAddr[i] = 0xA3A2; regValue[i] = 0x03C0; i++;
    regAddr[i] = 0xA3A4; regValue[i] = 0xE080; i++;
    regAddr[i] = 0xA3A6; regValue[i] = 0xD809; i++;
    regAddr[i] = 0xA3A8; regValue[i] = 0xF24E; i++;
    regAddr[i] = 0xA3AA; regValue[i] = 0x75CF; i++;
    regAddr[i] = 0xA3AC; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xA3AE; regValue[i] = 0x92B4; i++;
    
    regAddr[i] = 0xA3B0; regValue[i] = 0x1546; i++;
    regAddr[i] = 0xA3B2; regValue[i] = 0x1101; i++;
    regAddr[i] = 0xA3B4; regValue[i] = 0xDF20; i++;
    regAddr[i] = 0xA3B6; regValue[i] = 0x2144; i++;
    regAddr[i] = 0xA3B8; regValue[i] = 0x0C00; i++;
    regAddr[i] = 0xA3BA; regValue[i] = 0xB844; i++;
    regAddr[i] = 0xA3BC; regValue[i] = 0x2186; i++;
    regAddr[i] = 0xA3BE; regValue[i] = 0x0FF3; i++;
    
    regAddr[i] = 0xA3C0; regValue[i] = 0xE083; i++;
    regAddr[i] = 0xA3C2; regValue[i] = 0xB948; i++;
    regAddr[i] = 0xA3C4; regValue[i] = 0xF405; i++;
    regAddr[i] = 0xA3C6; regValue[i] = 0x70CF; i++;
    regAddr[i] = 0xA3C8; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xA3CA; regValue[i] = 0x951C; i++;
    regAddr[i] = 0xA3CC; regValue[i] = 0x88F4; i++;
    regAddr[i] = 0xA3CE; regValue[i] = 0x0917; i++;
    
    regAddr[i] = 0xA3D0; regValue[i] = 0x00D1; i++;
    regAddr[i] = 0xA3D2; regValue[i] = 0x70CF; i++;
    regAddr[i] = 0xA3D4; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xA3D6; regValue[i] = 0x951C; i++;
    regAddr[i] = 0xA3D8; regValue[i] = 0x8814; i++;
    regAddr[i] = 0xA3DA; regValue[i] = 0x2805; i++;
    regAddr[i] = 0xA3DC; regValue[i] = 0x03FE; i++;
    regAddr[i] = 0xA3DE; regValue[i] = 0x2941; i++;
    
    regAddr[i] = 0xA3E0; regValue[i] = 0x714F; i++;
    regAddr[i] = 0xA3E2; regValue[i] = 0x8503; i++;
    regAddr[i] = 0xA3E4; regValue[i] = 0x76CF; i++;
    regAddr[i] = 0xA3E6; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xA3E8; regValue[i] = 0x97E8; i++;
    regAddr[i] = 0xA3EA; regValue[i] = 0x1E95; i++;    
    regAddr[i] = 0xA3EC; regValue[i] = 0x13C2; i++;
    regAddr[i] = 0xA3EE; regValue[i] = 0x0D52; i++;
    
    regAddr[i] = 0xA3F0; regValue[i] = 0x06E0; i++;
    regAddr[i] = 0xA3F2; regValue[i] = 0xD908; i++;
    regAddr[i] = 0xA3F4; regValue[i] = 0x1EA4; i++;
    regAddr[i] = 0xA3F6; regValue[i] = 0x1004; i++;
    regAddr[i] = 0xA3F8; regValue[i] = 0x70E9; i++;
    regAddr[i] = 0xA3FA; regValue[i] = 0x0D46; i++;    
    regAddr[i] = 0xA3FC; regValue[i] = 0x06E0; i++;
    regAddr[i] = 0xA3FE; regValue[i] = 0xD908; i++;
    
    regAddr[i] = 0xA400; regValue[i] = 0x16A4; i++;
    regAddr[i] = 0xA402; regValue[i] = 0x1141; i++;
    regAddr[i] = 0xA404; regValue[i] = 0x2099; i++;
    regAddr[i] = 0xA406; regValue[i] = 0x0802; i++;
    regAddr[i] = 0xA408; regValue[i] = 0x7902; i++;
    regAddr[i] = 0xA40A; regValue[i] = 0x9602; i++;    
    regAddr[i] = 0xA40C; regValue[i] = 0x1EA4; i++;
    regAddr[i] = 0xA40E; regValue[i] = 0x1044; i++;
    
    regAddr[i] = 0xA410; regValue[i] = 0xB8E5; i++;
    regAddr[i] = 0xA412; regValue[i] = 0x0F68; i++;
    regAddr[i] = 0xA414; regValue[i] = 0x0942; i++;
    regAddr[i] = 0xA416; regValue[i] = 0x1544; i++;
    regAddr[i] = 0xA418; regValue[i] = 0x1100; i++;
    regAddr[i] = 0xA41A; regValue[i] = 0x2053; i++;    
    regAddr[i] = 0xA41C; regValue[i] = 0x8080; i++;
    regAddr[i] = 0xA41E; regValue[i] = 0xD801; i++;
    
    regAddr[i] = 0xA420; regValue[i] = 0xF204; i++;
    regAddr[i] = 0xA422; regValue[i] = 0x850C; i++;
    regAddr[i] = 0xA424; regValue[i] = 0xB8C0; i++;
    regAddr[i] = 0xA426; regValue[i] = 0xE002; i++;
    regAddr[i] = 0xA428; regValue[i] = 0x71CF; i++;
    regAddr[i] = 0xA42A; regValue[i] = 0xFFFF; i++;    
    regAddr[i] = 0xA42C; regValue[i] = 0x87BC; i++;
    regAddr[i] = 0xA42E; regValue[i] = 0xA910; i++;
    
    regAddr[i] = 0xA430; regValue[i] = 0x154E; i++;
    regAddr[i] = 0xA432; regValue[i] = 0x1100; i++;
    regAddr[i] = 0xA434; regValue[i] = 0xB109; i++;
    regAddr[i] = 0xA436; regValue[i] = 0x154C; i++;
    regAddr[i] = 0xA438; regValue[i] = 0x1101; i++;
    regAddr[i] = 0xA43A; regValue[i] = 0x70CF; i++;    
    regAddr[i] = 0xA43C; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xA43E; regValue[i] = 0x87BC; i++;

    regAddr[i] = 0xA440; regValue[i] = 0xB02A; i++;
    regAddr[i] = 0xA442; regValue[i] = 0xD800; i++;
    regAddr[i] = 0xA444; regValue[i] = 0x0121; i++;
    regAddr[i] = 0xA446; regValue[i] = 0x0D80; i++;
    regAddr[i] = 0xA448; regValue[i] = 0xC0F1; i++;
    regAddr[i] = 0xA44A; regValue[i] = 0x0912; i++;    
    regAddr[i] = 0xA44C; regValue[i] = 0x0D80; i++;
    regAddr[i] = 0xA44E; regValue[i] = 0x76CF; i++;

    regAddr[i] = 0xA450; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xA452; regValue[i] = 0x803C; i++;
    regAddr[i] = 0xA454; regValue[i] = 0x8E04; i++;
    regAddr[i] = 0xA456; regValue[i] = 0x75CF; i++;
    regAddr[i] = 0xA458; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xA45A; regValue[i] = 0x92B4; i++;    
    regAddr[i] = 0xA45C; regValue[i] = 0xC1A1; i++;
    regAddr[i] = 0xA45E; regValue[i] = 0x080F; i++;

    regAddr[i] = 0xA460; regValue[i] = 0x0311; i++;
    regAddr[i] = 0xA462; regValue[i] = 0x15CB; i++;
    regAddr[i] = 0xA464; regValue[i] = 0x1701; i++;
    regAddr[i] = 0xA466; regValue[i] = 0x7028; i++;
    regAddr[i] = 0xA468; regValue[i] = 0xF028; i++;
    regAddr[i] = 0xA46A; regValue[i] = 0x70CF; i++;    
    regAddr[i] = 0xA46C; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xA46E; regValue[i] = 0x972C; i++;

    regAddr[i] = 0xA470; regValue[i] = 0x9004; i++;
    regAddr[i] = 0xA472; regValue[i] = 0xD908; i++;
    regAddr[i] = 0xA474; regValue[i] = 0x0C5A; i++;
    regAddr[i] = 0xA476; regValue[i] = 0x06E0; i++;
    regAddr[i] = 0xA478; regValue[i] = 0x7228; i++;
    regAddr[i] = 0xA47A; regValue[i] = 0x7108; i++;    
    regAddr[i] = 0xA47C; regValue[i] = 0x72CF; i++;
    regAddr[i] = 0xA47E; regValue[i] = 0xFFFF; i++;

    regAddr[i] = 0xA480; regValue[i] = 0x9978; i++;
    regAddr[i] = 0xA482; regValue[i] = 0x8204; i++;
    regAddr[i] = 0xA484; regValue[i] = 0x8243; i++;
    regAddr[i] = 0xA486; regValue[i] = 0x7842; i++;
    regAddr[i] = 0xA488; regValue[i] = 0x0902; i++;
    regAddr[i] = 0xA48A; regValue[i] = 0x0DA0; i++;    
    regAddr[i] = 0xA48C; regValue[i] = 0xB80C; i++;
    regAddr[i] = 0xA48E; regValue[i] = 0xD904; i++;

    regAddr[i] = 0xA490; regValue[i] = 0x0D52; i++;
    regAddr[i] = 0xA492; regValue[i] = 0x06E0; i++;
    regAddr[i] = 0xA494; regValue[i] = 0xDA00; i++;
    regAddr[i] = 0xA496; regValue[i] = 0x15CA; i++;
    regAddr[i] = 0xA498; regValue[i] = 0x1701; i++;
    regAddr[i] = 0xA49A; regValue[i] = 0x2905; i++;    
    regAddr[i] = 0xA49C; regValue[i] = 0x003E; i++;
    regAddr[i] = 0xA49E; regValue[i] = 0x15CB; i++;

    regAddr[i] = 0xA4A0; regValue[i] = 0x1701; i++;
    regAddr[i] = 0xA4A2; regValue[i] = 0x15CC; i++;
    regAddr[i] = 0xA4A4; regValue[i] = 0x1700; i++;
    regAddr[i] = 0xA4A6; regValue[i] = 0x210C; i++;
    regAddr[i] = 0xA4A8; regValue[i] = 0xF040; i++;
    regAddr[i] = 0xA4AA; regValue[i] = 0x22CA; i++;    
    regAddr[i] = 0xA4AC; regValue[i] = 0x004E; i++;
    regAddr[i] = 0xA4AE; regValue[i] = 0x22CA; i++;

    regAddr[i] = 0xA4B0; regValue[i] = 0x0E4D; i++;
    regAddr[i] = 0xA4B2; regValue[i] = 0x7050; i++;
    regAddr[i] = 0xA4B4; regValue[i] = 0x20CA; i++;
    regAddr[i] = 0xA4B6; regValue[i] = 0x0086; i++;
    regAddr[i] = 0xA4B8; regValue[i] = 0x15E5; i++;
    regAddr[i] = 0xA4BA; regValue[i] = 0x1742; i++;    
    regAddr[i] = 0xA4BC; regValue[i] = 0x7810; i++;
    regAddr[i] = 0xA4BE; regValue[i] = 0xC240; i++;

    regAddr[i] = 0xA4C0; regValue[i] = 0x15CD; i++;
    regAddr[i] = 0xA4C2; regValue[i] = 0x1742; i++;
    regAddr[i] = 0xA4C4; regValue[i] = 0x0B4E; i++;
    regAddr[i] = 0xA4C6; regValue[i] = 0x06E0; i++;
    regAddr[i] = 0xA4C8; regValue[i] = 0x15CE; i++;
    regAddr[i] = 0xA4CA; regValue[i] = 0x1743; i++;    
    regAddr[i] = 0xA4CC; regValue[i] = 0x7A10; i++;
    regAddr[i] = 0xA4CE; regValue[i] = 0x70C9; i++;

    regAddr[i] = 0xA4D0; regValue[i] = 0x0BDA; i++;
    regAddr[i] = 0xA4D2; regValue[i] = 0x06E0; i++;
    regAddr[i] = 0xA4D4; regValue[i] = 0x15CF; i++;
    regAddr[i] = 0xA4D6; regValue[i] = 0x1701; i++;
    regAddr[i] = 0xA4D8; regValue[i] = 0x15C5; i++;
    regAddr[i] = 0xA4DA; regValue[i] = 0x1702; i++;    
    regAddr[i] = 0xA4DC; regValue[i] = 0x2A05; i++;
    regAddr[i] = 0xA4DE; regValue[i] = 0x003E; i++;

    regAddr[i] = 0xA4E0; regValue[i] = 0x7048; i++;
    regAddr[i] = 0xA4E2; regValue[i] = 0x15C8; i++;
    regAddr[i] = 0xA4E4; regValue[i] = 0x1702; i++;
    regAddr[i] = 0xA4E6; regValue[i] = 0x712F; i++;
    regAddr[i] = 0xA4E8; regValue[i] = 0x1D63; i++;
    regAddr[i] = 0xA4EA; regValue[i] = 0x1E58; i++;    
    regAddr[i] = 0xA4EC; regValue[i] = 0x08EE; i++;
    regAddr[i] = 0xA4EE; regValue[i] = 0x0B20; i++;

    regAddr[i] = 0xA4F0; regValue[i] = 0x15C9; i++;
    regAddr[i] = 0xA4F2; regValue[i] = 0x1703; i++;
    regAddr[i] = 0xA4F4; regValue[i] = 0x0079; i++;
    regAddr[i] = 0xA4F6; regValue[i] = 0x0DA0; i++;
    regAddr[i] = 0xA4F8; regValue[i] = 0xC0A1; i++;
    regAddr[i] = 0xA4FA; regValue[i] = 0x78E0; i++;    
    regAddr[i] = 0xA4FC; regValue[i] = 0xC0F1; i++;
    regAddr[i] = 0xA4FE; regValue[i] = 0x085A; i++;

    regAddr[i] = 0xA500; regValue[i] = 0x0DA0; i++;
    regAddr[i] = 0xA502; regValue[i] = 0xDA00; i++;
    regAddr[i] = 0xA504; regValue[i] = 0x70CF; i++;
    regAddr[i] = 0xA506; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xA508; regValue[i] = 0x92B4; i++;
    regAddr[i] = 0xA50A; regValue[i] = 0x10E5; i++;    
    regAddr[i] = 0xA50C; regValue[i] = 0x074F; i++;
    regAddr[i] = 0xA50E; regValue[i] = 0x70CF; i++;

    regAddr[i] = 0xA510; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xA512; regValue[i] = 0xA648; i++;
    regAddr[i] = 0xA514; regValue[i] = 0x8820; i++;
    regAddr[i] = 0xA516; regValue[i] = 0xC1A3; i++;
    regAddr[i] = 0xA518; regValue[i] = 0xE181; i++;
    regAddr[i] = 0xA51A; regValue[i] = 0x75CF; i++;    
    regAddr[i] = 0xA51C; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xA51E; regValue[i] = 0x9494; i++;

    regAddr[i] = 0xA520; regValue[i] = 0xF402; i++;
    regAddr[i] = 0xA522; regValue[i] = 0x9D5A; i++;
    regAddr[i] = 0xA524; regValue[i] = 0x9D39; i++;
    regAddr[i] = 0xA526; regValue[i] = 0x6159; i++;
    regAddr[i] = 0xA528; regValue[i] = 0x7730; i++;
    regAddr[i] = 0xA52A; regValue[i] = 0xDA01; i++;    
    regAddr[i] = 0xA52C; regValue[i] = 0x22C2; i++;
    regAddr[i] = 0xA52E; regValue[i] = 0x008A; i++;

    regAddr[i] = 0xA530; regValue[i] = 0x0F5D; i++;
    regAddr[i] = 0xA532; regValue[i] = 0x1063; i++;
    regAddr[i] = 0xA534; regValue[i] = 0xA840; i++;
    regAddr[i] = 0xA536; regValue[i] = 0xC740; i++;
    regAddr[i] = 0xA538; regValue[i] = 0x76CF; i++;
    regAddr[i] = 0xA53A; regValue[i] = 0xFFFF; i++;    
    regAddr[i] = 0xA53C; regValue[i] = 0x94AC; i++;
    regAddr[i] = 0xA53E; regValue[i] = 0x8E06; i++;

    regAddr[i] = 0xA540; regValue[i] = 0x8E27; i++;
    regAddr[i] = 0xA542; regValue[i] = 0x9D4D; i++;
    regAddr[i] = 0xA544; regValue[i] = 0x0ACE; i++;
    regAddr[i] = 0xA546; regValue[i] = 0x06E0; i++;
    regAddr[i] = 0xA548; regValue[i] = 0x9D6E; i++;
    regAddr[i] = 0xA54A; regValue[i] = 0xC061; i++;    
    regAddr[i] = 0xA54C; regValue[i] = 0xC740; i++;
    regAddr[i] = 0xA54E; regValue[i] = 0x8E0C; i++;

    regAddr[i] = 0xA550; regValue[i] = 0x8E2D; i++;
    regAddr[i] = 0xA552; regValue[i] = 0x9D50; i++;
    regAddr[i] = 0xA554; regValue[i] = 0x0ABE; i++;
    regAddr[i] = 0xA556; regValue[i] = 0x06E0; i++;
    regAddr[i] = 0xA558; regValue[i] = 0x9D71; i++;
    regAddr[i] = 0xA55A; regValue[i] = 0x1C05; i++;    
    regAddr[i] = 0xA55C; regValue[i] = 0x3002; i++;
    regAddr[i] = 0xA55E; regValue[i] = 0xC740; i++;

    regAddr[i] = 0xA560; regValue[i] = 0x8E12; i++;
    regAddr[i] = 0xA562; regValue[i] = 0x8E33; i++;
    regAddr[i] = 0xA564; regValue[i] = 0x9D53; i++;
    regAddr[i] = 0xA566; regValue[i] = 0x0AAE; i++;
    regAddr[i] = 0xA568; regValue[i] = 0x06E0; i++;
    regAddr[i] = 0xA56A; regValue[i] = 0x9D74; i++;    
    regAddr[i] = 0xA56C; regValue[i] = 0x1C06; i++;
    regAddr[i] = 0xA56E; regValue[i] = 0x3002; i++;

    regAddr[i] = 0xA570; regValue[i] = 0xC740; i++;
    regAddr[i] = 0xA572; regValue[i] = 0x8E18; i++;
    regAddr[i] = 0xA574; regValue[i] = 0x8E39; i++;
    regAddr[i] = 0xA576; regValue[i] = 0x9D56; i++;
    regAddr[i] = 0xA578; regValue[i] = 0x0A9A; i++;
    regAddr[i] = 0xA57A; regValue[i] = 0x06E0; i++;    
    regAddr[i] = 0xA57C; regValue[i] = 0x9D77; i++;
    regAddr[i] = 0xA57E; regValue[i] = 0x1C07; i++;

    regAddr[i] = 0xA580; regValue[i] = 0x3002; i++;
    regAddr[i] = 0xA582; regValue[i] = 0x080E; i++;
    regAddr[i] = 0xA584; regValue[i] = 0x0B20; i++;
    regAddr[i] = 0xA586; regValue[i] = 0xC081; i++;
    regAddr[i] = 0xA588; regValue[i] = 0xD801; i++;
    regAddr[i] = 0xA58A; regValue[i] = 0xF003; i++;    
    regAddr[i] = 0xA58C; regValue[i] = 0xD800; i++;
    regAddr[i] = 0xA58E; regValue[i] = 0x0D1E; i++;

    regAddr[i] = 0xA590; regValue[i] = 0x0AC0; i++;
    regAddr[i] = 0xA592; regValue[i] = 0x07D5; i++;
    regAddr[i] = 0xA594; regValue[i] = 0x0D60; i++;
    regAddr[i] = 0xA596; regValue[i] = 0xC0A3; i++;
    regAddr[i] = 0xA598; regValue[i] = 0xC0F1; i++;
    regAddr[i] = 0xA59A; regValue[i] = 0x0FBE; i++;    
    regAddr[i] = 0xA59C; regValue[i] = 0x0D60; i++;
    regAddr[i] = 0xA59E; regValue[i] = 0xDA00; i++;

    regAddr[i] = 0xA5A0; regValue[i] = 0x70CF; i++;
    regAddr[i] = 0xA5A2; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xA5A4; regValue[i] = 0x9458; i++;
    regAddr[i] = 0xA5A6; regValue[i] = 0x98F3; i++;
    regAddr[i] = 0xA5A8; regValue[i] = 0x70CF; i++;
    regAddr[i] = 0xA5AA; regValue[i] = 0xFFFF; i++;    
    regAddr[i] = 0xA5AC; regValue[i] = 0xA648; i++;
    regAddr[i] = 0xA5AE; regValue[i] = 0x8821; i++;

    regAddr[i] = 0xA5B0; regValue[i] = 0xC1A3; i++;
    regAddr[i] = 0xA5B2; regValue[i] = 0xE181; i++;
    regAddr[i] = 0xA5B4; regValue[i] = 0x75CF; i++;
    regAddr[i] = 0xA5B6; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xA5B8; regValue[i] = 0x96B0; i++;
    regAddr[i] = 0xA5BA; regValue[i] = 0xF404; i++;    
    regAddr[i] = 0xA5BC; regValue[i] = 0x1517; i++;
    regAddr[i] = 0xA5BE; regValue[i] = 0x9742; i++;

    regAddr[i] = 0xA5C0; regValue[i] = 0x1516; i++;
    regAddr[i] = 0xA5C2; regValue[i] = 0x9741; i++;
    regAddr[i] = 0xA5C4; regValue[i] = 0x6159; i++;
    regAddr[i] = 0xA5C6; regValue[i] = 0x7730; i++;
    regAddr[i] = 0xA5C8; regValue[i] = 0xDA01; i++;
    regAddr[i] = 0xA5CA; regValue[i] = 0x22C2; i++;    
    regAddr[i] = 0xA5CC; regValue[i] = 0x008A; i++;
    regAddr[i] = 0xA5CE; regValue[i] = 0x0F59; i++;

    regAddr[i] = 0xA5D0; regValue[i] = 0x1063; i++;
    regAddr[i] = 0xA5D2; regValue[i] = 0xA841; i++;
    regAddr[i] = 0xA5D4; regValue[i] = 0xC740; i++;
    regAddr[i] = 0xA5D6; regValue[i] = 0x76CF; i++;
    regAddr[i] = 0xA5D8; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xA5DA; regValue[i] = 0x94C8; i++;    
    regAddr[i] = 0xA5DC; regValue[i] = 0x8E06; i++;
    regAddr[i] = 0xA5DE; regValue[i] = 0x8E27; i++;

    regAddr[i] = 0xA5E0; regValue[i] = 0x150D; i++;
    regAddr[i] = 0xA5E2; regValue[i] = 0x9742; i++;
    regAddr[i] = 0xA5E4; regValue[i] = 0x0A2E; i++;
    regAddr[i] = 0xA5E6; regValue[i] = 0x06E0; i++;
    regAddr[i] = 0xA5E8; regValue[i] = 0x150E; i++;
    regAddr[i] = 0xA5EA; regValue[i] = 0x9743; i++;    
    regAddr[i] = 0xA5EC; regValue[i] = 0xC062; i++;
    regAddr[i] = 0xA5EE; regValue[i] = 0xC740; i++;

    regAddr[i] = 0xA5F0; regValue[i] = 0x8E0C; i++;
    regAddr[i] = 0xA5F2; regValue[i] = 0x8E2D; i++;
    regAddr[i] = 0xA5F4; regValue[i] = 0x1510; i++;
    regAddr[i] = 0xA5F6; regValue[i] = 0x9742; i++;
    regAddr[i] = 0xA5F8; regValue[i] = 0x0A1A; i++;
    regAddr[i] = 0xA5FA; regValue[i] = 0x06E0; i++;    
    regAddr[i] = 0xA5FC; regValue[i] = 0x1511; i++;
    regAddr[i] = 0xA5FE; regValue[i] = 0x9743; i++;

    regAddr[i] = 0xA600; regValue[i] = 0x1C09; i++;
    regAddr[i] = 0xA602; regValue[i] = 0x3002; i++;
    regAddr[i] = 0xA604; regValue[i] = 0xC740; i++;
    regAddr[i] = 0xA606; regValue[i] = 0x8E12; i++;
    regAddr[i] = 0xA608; regValue[i] = 0x8E33; i++;
    regAddr[i] = 0xA60A; regValue[i] = 0x1513; i++;    
    regAddr[i] = 0xA60C; regValue[i] = 0x9742; i++;
    regAddr[i] = 0xA60E; regValue[i] = 0x0A06; i++;

    regAddr[i] = 0xA610; regValue[i] = 0x06E0; i++;
    regAddr[i] = 0xA612; regValue[i] = 0x1514; i++;
    regAddr[i] = 0xA614; regValue[i] = 0x9743; i++;
    regAddr[i] = 0xA616; regValue[i] = 0x1C0A; i++;
    regAddr[i] = 0xA618; regValue[i] = 0x3002; i++;
    regAddr[i] = 0xA61A; regValue[i] = 0x0F8E; i++;    
    regAddr[i] = 0xA61C; regValue[i] = 0x0AE0; i++;
    regAddr[i] = 0xA61E; regValue[i] = 0xC081; i++;

    regAddr[i] = 0xA620; regValue[i] = 0xD801; i++;
    regAddr[i] = 0xA622; regValue[i] = 0xF003; i++;
    regAddr[i] = 0xA624; regValue[i] = 0xD800; i++;
    regAddr[i] = 0xA626; regValue[i] = 0x0C6E; i++;
    regAddr[i] = 0xA628; regValue[i] = 0x0AC0; i++;
    regAddr[i] = 0xA62A; regValue[i] = 0xF1B5; i++;    
    regAddr[i] = 0xA62C; regValue[i] = 0xD900; i++;
    regAddr[i] = 0xA62E; regValue[i] = 0xF00A; i++;

    regAddr[i] = 0xA630; regValue[i] = 0x70CF; i++;
    regAddr[i] = 0xA632; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xA634; regValue[i] = 0xA64C; i++;
    regAddr[i] = 0xA636; regValue[i] = 0x7835; i++;
    regAddr[i] = 0xA638; regValue[i] = 0x8041; i++;
    regAddr[i] = 0xA63A; regValue[i] = 0x8000; i++;    
    regAddr[i] = 0xA63C; regValue[i] = 0xE102; i++;
    regAddr[i] = 0xA63E; regValue[i] = 0xA040; i++;

    regAddr[i] = 0xA640; regValue[i] = 0x09F1; i++;
    regAddr[i] = 0xA642; regValue[i] = 0x8214; i++;
    regAddr[i] = 0xA644; regValue[i] = 0x7FE0; i++;
    regAddr[i] = 0xA646; regValue[i] = 0xD800; i++;
    regAddr[i] = 0xA648; regValue[i] = 0x0000; i++;
    regAddr[i] = 0xA64A; regValue[i] = 0x0000; i++;    
    regAddr[i] = 0xA64C; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xA64E; regValue[i] = 0xA1AC; i++;

    regAddr[i] = 0xA650; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xA652; regValue[i] = 0xA398; i++;
    regAddr[i] = 0xA654; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xA656; regValue[i] = 0xA120; i++;
    regAddr[i] = 0xA658; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xA65A; regValue[i] = 0xA448; i++;    
    regAddr[i] = 0xA65C; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xA65E; regValue[i] = 0xA184; i++;

    regAddr[i] = 0xA660; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xA662; regValue[i] = 0xA4FC; i++;
    regAddr[i] = 0xA664; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xA666; regValue[i] = 0xA188; i++;
    regAddr[i] = 0xA668; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xA66A; regValue[i] = 0xA598; i++;    

    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);

    //apply patch
    i = 0;
    regAddr[i] = 0x098E; regValue[i] = 0x0000; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC00; regValue[i] = 0x0294; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC02; regValue[i] = 0x0303; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC04; regValue[i] = 0x6003; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);    
    i = 0;
    regAddr[i] = 0xFC06; regValue[i] = 0x0104; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC08; regValue[i] = 0x02D4; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8702; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX apply patch 1 return value = %d\n", regValue[0]);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    OSA_waitMsecs(5);
    #endif

    // Sensor Reset
    i = 0;
    regAddr[i] = 0x001A; regValue[i] = 0x0001; i++;    //Reset
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0x001A; regValue[i] = 0x0000; i++;   //Reset done
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    //Issue SYSMGR_GET_STATE repeatedly until it does not return EBUSY
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8101; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX Issue SYSMGR_GET_STATE = %d\n", regValue[0]);
        if(regValue[0] == 0)
        {
            break;
        }
    }
    //Issue SENSOR_MGR_DISCOVER_SENSOR host command and poll until it returns ENOERR
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8E00; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX Issue SENSOR_MGR_DISCOVER_SENSOR = %d\n", regValue[0]);
        if(regValue[0] == 0)
        {
            break;
        }
    }
    //Reserve RAM to contain a patch
    i = 0;
    regAddr[i] = 0x098E; regValue[i] = 0x7C00; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC00; regValue[i] = 0x02D4; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC02; regValue[i] = 0x05FC; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8706; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX Reserve RAM to contain a patch 2 return value = %d\n", regValue[0]);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    OSA_waitMsecs(5);
    //patch
    i = 0;
    regAddr[i] = 0x0982; regValue[i] = 0x0001; i++;
    regAddr[i] = 0x098A; regValue[i] = 0x266C; i++;
    regAddr[i] = 0xA66C; regValue[i] = 0xC0F1; i++;
    regAddr[i] = 0xA66E; regValue[i] = 0x0EEA; i++;

    regAddr[i] = 0xA670; regValue[i] = 0x0D40; i++;
    regAddr[i] = 0xA672; regValue[i] = 0xC1B3; i++;
    regAddr[i] = 0xA674; regValue[i] = 0xC040; i++;
    regAddr[i] = 0xA676; regValue[i] = 0xC141; i++;
    regAddr[i] = 0xA678; regValue[i] = 0x7748; i++;
    regAddr[i] = 0xA67A; regValue[i] = 0xC342; i++;
    regAddr[i] = 0xA67C; regValue[i] = 0x2153; i++;
    regAddr[i] = 0xA67E; regValue[i] = 0x0100; i++;

    regAddr[i] = 0xA680; regValue[i] = 0xE080; i++;
    regAddr[i] = 0xA682; regValue[i] = 0xC001; i++;
    regAddr[i] = 0xA684; regValue[i] = 0xE120; i++;
    regAddr[i] = 0xA686; regValue[i] = 0x20CA; i++;
    regAddr[i] = 0xA688; regValue[i] = 0x0042; i++;
    regAddr[i] = 0xA68A; regValue[i] = 0x75CF; i++;
    regAddr[i] = 0xA68C; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xA68E; regValue[i] = 0x87BC; i++;

    regAddr[i] = 0xA690; regValue[i] = 0xE010; i++;
    regAddr[i] = 0xA692; regValue[i] = 0xB825; i++;
    regAddr[i] = 0xA694; regValue[i] = 0xC043; i++;
    regAddr[i] = 0xA696; regValue[i] = 0x7810; i++;
    regAddr[i] = 0xA698; regValue[i] = 0xC044; i++;
    regAddr[i] = 0xA69A; regValue[i] = 0x8500; i++;
    regAddr[i] = 0xA69C; regValue[i] = 0x2B05; i++;
    regAddr[i] = 0xA69E; regValue[i] = 0x03FE; i++;

    regAddr[i] = 0xA6A0; regValue[i] = 0x1C14; i++;
    regAddr[i] = 0xA6A2; regValue[i] = 0x3E40; i++;
    regAddr[i] = 0xA6A4; regValue[i] = 0xD900; i++;
    regAddr[i] = 0xA6A6; regValue[i] = 0x76CF; i++;
    regAddr[i] = 0xA6A8; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xA6AA; regValue[i] = 0x95B4; i++;
    regAddr[i] = 0xA6AC; regValue[i] = 0x0B36; i++;
    regAddr[i] = 0xA6AE; regValue[i] = 0x06E0; i++;

    regAddr[i] = 0xA6B0; regValue[i] = 0x7228; i++;
    regAddr[i] = 0xA6B2; regValue[i] = 0x8502; i++;
    regAddr[i] = 0xA6B4; regValue[i] = 0xD900; i++;
    regAddr[i] = 0xA6B6; regValue[i] = 0x0B2E; i++;
    regAddr[i] = 0xA6B8; regValue[i] = 0x06E0; i++;
    regAddr[i] = 0xA6BA; regValue[i] = 0xDA08; i++;
    regAddr[i] = 0xA6BC; regValue[i] = 0x9603; i++;
    regAddr[i] = 0xA6BE; regValue[i] = 0xE804; i++;

    regAddr[i] = 0xA6C0; regValue[i] = 0x9604; i++;
    regAddr[i] = 0xA6C2; regValue[i] = 0xE89B; i++;
    regAddr[i] = 0xA6C4; regValue[i] = 0x70CF; i++;
    regAddr[i] = 0xA6C6; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xA6C8; regValue[i] = 0x92B4; i++;
    regAddr[i] = 0xA6CA; regValue[i] = 0x802C; i++;
    regAddr[i] = 0xA6CC; regValue[i] = 0xDB00; i++;
    regAddr[i] = 0xA6CE; regValue[i] = 0x2186; i++;

    regAddr[i] = 0xA6D0; regValue[i] = 0x0C7F; i++;
    regAddr[i] = 0xA6D2; regValue[i] = 0x793D; i++;
    regAddr[i] = 0xA6D4; regValue[i] = 0xE102; i++;
    regAddr[i] = 0xA6D6; regValue[i] = 0x230F; i++;
    regAddr[i] = 0xA6D8; regValue[i] = 0x0043; i++;
    regAddr[i] = 0xA6DA; regValue[i] = 0xAD76; i++;
    regAddr[i] = 0xA6DC; regValue[i] = 0x800C; i++;
    regAddr[i] = 0xA6DE; regValue[i] = 0x7A6F; i++;

    regAddr[i] = 0xA6E0; regValue[i] = 0xD900; i++;
    regAddr[i] = 0xA6E2; regValue[i] = 0x2086; i++;
    regAddr[i] = 0xA6E4; regValue[i] = 0x03FE; i++;
    regAddr[i] = 0xA6E6; regValue[i] = 0xB824; i++;
    regAddr[i] = 0xA6E8; regValue[i] = 0xE002; i++;
    regAddr[i] = 0xA6EA; regValue[i] = 0x210F; i++;
    regAddr[i] = 0xA6EC; regValue[i] = 0x0001; i++;
    regAddr[i] = 0xA6EE; regValue[i] = 0x794C; i++;

    regAddr[i] = 0xA6F0; regValue[i] = 0x1D18; i++;
    regAddr[i] = 0xA6F2; regValue[i] = 0x1E44; i++;
    regAddr[i] = 0xA6F4; regValue[i] = 0xF00D; i++;
    regAddr[i] = 0xA6F6; regValue[i] = 0x9603; i++;
    regAddr[i] = 0xA6F8; regValue[i] = 0xB825; i++;
    regAddr[i] = 0xA6FA; regValue[i] = 0x7A0F; i++;
    regAddr[i] = 0xA6FC; regValue[i] = 0xAD16; i++;
    regAddr[i] = 0xA6FE; regValue[i] = 0x9604; i++;

    regAddr[i] = 0xA700; regValue[i] = 0x784C; i++;
    regAddr[i] = 0xA702; regValue[i] = 0x2942; i++;
    regAddr[i] = 0xA704; regValue[i] = 0x7100; i++;
    regAddr[i] = 0xA706; regValue[i] = 0xB83B; i++;
    regAddr[i] = 0xA708; regValue[i] = 0x7027; i++;
    regAddr[i] = 0xA70A; regValue[i] = 0xB845; i++;
    regAddr[i] = 0xA70C; regValue[i] = 0xB50C; i++;
    regAddr[i] = 0xA70E; regValue[i] = 0x1D00; i++;

    regAddr[i] = 0xA710; regValue[i] = 0x1F80; i++;
    regAddr[i] = 0xA712; regValue[i] = 0x000F; i++;
    regAddr[i] = 0xA714; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xA716; regValue[i] = 0x1D04; i++;
    regAddr[i] = 0xA718; regValue[i] = 0x1F80; i++;
    regAddr[i] = 0xA71A; regValue[i] = 0x000F; i++;
    regAddr[i] = 0xA71C; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xA71E; regValue[i] = 0x1D08; i++;

    regAddr[i] = 0xA720; regValue[i] = 0x1F80; i++;
    regAddr[i] = 0xA722; regValue[i] = 0x000F; i++;
    regAddr[i] = 0xA724; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xA726; regValue[i] = 0x1D0C; i++;
    regAddr[i] = 0xA728; regValue[i] = 0x1F80; i++;
    regAddr[i] = 0xA72A; regValue[i] = 0x000F; i++;
    regAddr[i] = 0xA72C; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xA72E; regValue[i] = 0x1510; i++;

    regAddr[i] = 0xA730; regValue[i] = 0x108A; i++;
    regAddr[i] = 0xA732; regValue[i] = 0x70CF; i++;
    regAddr[i] = 0xA734; regValue[i] = 0x000F; i++;
    regAddr[i] = 0xA736; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xA738; regValue[i] = 0x224C; i++;
    regAddr[i] = 0xA73A; regValue[i] = 0x9040; i++;
    regAddr[i] = 0xA73C; regValue[i] = 0x21CA; i++;
    regAddr[i] = 0xA73E; regValue[i] = 0x000C; i++;

    regAddr[i] = 0xA740; regValue[i] = 0xF6C7; i++;
    regAddr[i] = 0xA742; regValue[i] = 0x952A; i++;
    regAddr[i] = 0xA744; regValue[i] = 0x9569; i++;
    regAddr[i] = 0xA746; regValue[i] = 0xA521; i++;
    regAddr[i] = 0xA748; regValue[i] = 0x2102; i++;
    regAddr[i] = 0xA74A; regValue[i] = 0x00C0; i++;
    regAddr[i] = 0xA74C; regValue[i] = 0xA500; i++;
    regAddr[i] = 0xA74E; regValue[i] = 0x0A17; i++;

    regAddr[i] = 0xA750; regValue[i] = 0x10D2; i++;
    regAddr[i] = 0xA752; regValue[i] = 0x2805; i++;
    regAddr[i] = 0xA754; regValue[i] = 0x00BE; i++;
    regAddr[i] = 0xA756; regValue[i] = 0x1D08; i++;
    regAddr[i] = 0xA758; regValue[i] = 0x1E40; i++;
    regAddr[i] = 0xA75A; regValue[i] = 0x2905; i++;
    regAddr[i] = 0xA75C; regValue[i] = 0x00BE; i++;
    regAddr[i] = 0xA75E; regValue[i] = 0x1D0C; i++;

    regAddr[i] = 0xA760; regValue[i] = 0x1E40; i++;
    regAddr[i] = 0xA762; regValue[i] = 0xD900; i++;
    regAddr[i] = 0xA764; regValue[i] = 0x0A7E; i++;
    regAddr[i] = 0xA766; regValue[i] = 0x06E0; i++;
    regAddr[i] = 0xA768; regValue[i] = 0xDA08; i++;
    regAddr[i] = 0xA76A; regValue[i] = 0x8D16; i++;
    regAddr[i] = 0xA76C; regValue[i] = 0xD900; i++;
    regAddr[i] = 0xA76E; regValue[i] = 0x0A76; i++;

    regAddr[i] = 0xA770; regValue[i] = 0x06E0; i++;
    regAddr[i] = 0xA772; regValue[i] = 0xDA08; i++;
    regAddr[i] = 0xA774; regValue[i] = 0x2805; i++;
    regAddr[i] = 0xA776; regValue[i] = 0x03FE; i++;
    regAddr[i] = 0xA778; regValue[i] = 0x2941; i++;
    regAddr[i] = 0xA77A; regValue[i] = 0x7200; i++;
    regAddr[i] = 0xA77C; regValue[i] = 0xC052; i++;
    regAddr[i] = 0xA77E; regValue[i] = 0x8502; i++;

    regAddr[i] = 0xA780; regValue[i] = 0xD900; i++;
    regAddr[i] = 0xA782; regValue[i] = 0x0A62; i++;
    regAddr[i] = 0xA784; regValue[i] = 0x06E0; i++;
    regAddr[i] = 0xA786; regValue[i] = 0x7228; i++;
    regAddr[i] = 0xA788; regValue[i] = 0x950C; i++;
    regAddr[i] = 0xA78A; regValue[i] = 0xD900; i++;
    regAddr[i] = 0xA78C; regValue[i] = 0x0A56; i++;
    regAddr[i] = 0xA78E; regValue[i] = 0x06E0; i++;

    regAddr[i] = 0xA790; regValue[i] = 0xDA08; i++;
    regAddr[i] = 0xA792; regValue[i] = 0x2805; i++;
    regAddr[i] = 0xA794; regValue[i] = 0x03FE; i++;
    regAddr[i] = 0xA796; regValue[i] = 0xC001; i++;
    regAddr[i] = 0xA798; regValue[i] = 0x8D56; i++;
    regAddr[i] = 0xA79A; regValue[i] = 0x2941; i++;
    regAddr[i] = 0xA79C; regValue[i] = 0x7201; i++;
    regAddr[i] = 0xA79E; regValue[i] = 0x2A05; i++;

    regAddr[i] = 0xA7A0; regValue[i] = 0x003E; i++;
    regAddr[i] = 0xA7A2; regValue[i] = 0xC205; i++;
    regAddr[i] = 0xA7A4; regValue[i] = 0x702F; i++;
    regAddr[i] = 0xA7A6; regValue[i] = 0x2153; i++;
    regAddr[i] = 0xA7A8; regValue[i] = 0x7103; i++;
    regAddr[i] = 0xA7AA; regValue[i] = 0xE380; i++;
    regAddr[i] = 0xA7AC; regValue[i] = 0xBA25; i++;
    regAddr[i] = 0xA7AE; regValue[i] = 0xC245; i++;

    regAddr[i] = 0xA7B0; regValue[i] = 0x7A50; i++;
    regAddr[i] = 0xA7B2; regValue[i] = 0xC246; i++;
    regAddr[i] = 0xA7B4; regValue[i] = 0x2140; i++;
    regAddr[i] = 0xA7B6; regValue[i] = 0x7802; i++;
    regAddr[i] = 0xA7B8; regValue[i] = 0x22CA; i++;
    regAddr[i] = 0xA7BA; regValue[i] = 0x0001; i++;
    regAddr[i] = 0xA7BC; regValue[i] = 0xBA25; i++;
    regAddr[i] = 0xA7BE; regValue[i] = 0xC247; i++;

    regAddr[i] = 0xA7C0; regValue[i] = 0x7E50; i++;
    regAddr[i] = 0xA7C2; regValue[i] = 0xC212; i++;
    regAddr[i] = 0xA7C4; regValue[i] = 0xC002; i++;
    regAddr[i] = 0xA7C6; regValue[i] = 0x2A05; i++;
    regAddr[i] = 0xA7C8; regValue[i] = 0x003E; i++;
    regAddr[i] = 0xA7CA; regValue[i] = 0x2941; i++;
    regAddr[i] = 0xA7CC; regValue[i] = 0x714F; i++;
    regAddr[i] = 0xA7CE; regValue[i] = 0xC001; i++;

    regAddr[i] = 0xA7D0; regValue[i] = 0x954C; i++;
    regAddr[i] = 0xA7D2; regValue[i] = 0xC748; i++;
    regAddr[i] = 0xA7D4; regValue[i] = 0x2A05; i++;
    regAddr[i] = 0xA7D6; regValue[i] = 0x003E; i++;
    regAddr[i] = 0xA7D8; regValue[i] = 0x2153; i++;
    regAddr[i] = 0xA7DA; regValue[i] = 0x7100; i++;
    regAddr[i] = 0xA7DC; regValue[i] = 0xE080; i++;
    regAddr[i] = 0xA7DE; regValue[i] = 0x2140; i++;

    regAddr[i] = 0xA7E0; regValue[i] = 0x7802; i++;
    regAddr[i] = 0xA7E2; regValue[i] = 0x22CA; i++;
    regAddr[i] = 0xA7E4; regValue[i] = 0x0E41; i++;
    regAddr[i] = 0xA7E6; regValue[i] = 0x2A41; i++;
    regAddr[i] = 0xA7E8; regValue[i] = 0x0140; i++;
    regAddr[i] = 0xA7EA; regValue[i] = 0xC049; i++;
    regAddr[i] = 0xA7EC; regValue[i] = 0x7810; i++;
    regAddr[i] = 0xA7EE; regValue[i] = 0xC04A; i++;

    regAddr[i] = 0xA7F0; regValue[i] = 0xC002; i++;
    regAddr[i] = 0xA7F2; regValue[i] = 0x7FF0; i++;
    regAddr[i] = 0xA7F4; regValue[i] = 0x2905; i++;
    regAddr[i] = 0xA7F6; regValue[i] = 0x003E; i++;
    regAddr[i] = 0xA7F8; regValue[i] = 0x2941; i++;
    regAddr[i] = 0xA7FA; regValue[i] = 0x7140; i++;
    regAddr[i] = 0xA7FC; regValue[i] = 0xC04B; i++;
    regAddr[i] = 0xA7FE; regValue[i] = 0x7810; i++;

    regAddr[i] = 0xA800; regValue[i] = 0xC04C; i++;
    regAddr[i] = 0xA802; regValue[i] = 0x8500; i++;
    regAddr[i] = 0xA804; regValue[i] = 0xD900; i++;
    regAddr[i] = 0xA806; regValue[i] = 0x09DE; i++;
    regAddr[i] = 0xA808; regValue[i] = 0x06E0; i++;
    regAddr[i] = 0xA80A; regValue[i] = 0x7228; i++;
    regAddr[i] = 0xA80C; regValue[i] = 0xC106; i++;
    regAddr[i] = 0xA80E; regValue[i] = 0x2805; i++;

    regAddr[i] = 0xA810; regValue[i] = 0x007E; i++;
    regAddr[i] = 0xA812; regValue[i] = 0xC104; i++;
    regAddr[i] = 0xA814; regValue[i] = 0x2941; i++;
    regAddr[i] = 0xA816; regValue[i] = 0x7200; i++;
    regAddr[i] = 0xA818; regValue[i] = 0x7030; i++;
    regAddr[i] = 0xA81A; regValue[i] = 0x21CA; i++;
    regAddr[i] = 0xA81C; regValue[i] = 0x000D; i++;
    regAddr[i] = 0xA81E; regValue[i] = 0xC14D; i++;

    regAddr[i] = 0xA820; regValue[i] = 0x8502; i++;
    regAddr[i] = 0xA822; regValue[i] = 0xD900; i++;
    regAddr[i] = 0xA824; regValue[i] = 0x09BE; i++;
    regAddr[i] = 0xA826; regValue[i] = 0x06E0; i++;
    regAddr[i] = 0xA828; regValue[i] = 0xDA08; i++;
    regAddr[i] = 0xA82A; regValue[i] = 0x2805; i++;
    regAddr[i] = 0xA82C; regValue[i] = 0x03FE; i++;
    regAddr[i] = 0xA82E; regValue[i] = 0x2941; i++;

    regAddr[i] = 0xA830; regValue[i] = 0x7400; i++;
    regAddr[i] = 0xA832; regValue[i] = 0x60D8; i++;
    regAddr[i] = 0xA834; regValue[i] = 0xC04E; i++;
    regAddr[i] = 0xA836; regValue[i] = 0x8501; i++;
    regAddr[i] = 0xA838; regValue[i] = 0xD900; i++;
    regAddr[i] = 0xA83A; regValue[i] = 0x09AA; i++;
    regAddr[i] = 0xA83C; regValue[i] = 0x06E0; i++;
    regAddr[i] = 0xA83E; regValue[i] = 0xDA08; i++;

    regAddr[i] = 0xA840; regValue[i] = 0x2805; i++;
    regAddr[i] = 0xA842; regValue[i] = 0x03FE; i++;
    regAddr[i] = 0xA844; regValue[i] = 0x2941; i++;
    regAddr[i] = 0xA846; regValue[i] = 0x7400; i++;
    regAddr[i] = 0xA848; regValue[i] = 0x60D8; i++;
    regAddr[i] = 0xA84A; regValue[i] = 0xC04F; i++;
    regAddr[i] = 0xA84C; regValue[i] = 0x8500; i++;
    regAddr[i] = 0xA84E; regValue[i] = 0xD900; i++;
    
    regAddr[i] = 0xA850; regValue[i] = 0x0992; i++;
    regAddr[i] = 0xA852; regValue[i] = 0x06E0; i++;
    regAddr[i] = 0xA854; regValue[i] = 0xDA08; i++;
    regAddr[i] = 0xA856; regValue[i] = 0xC106; i++;
    regAddr[i] = 0xA858; regValue[i] = 0x8540; i++;
    regAddr[i] = 0xA85A; regValue[i] = 0x2805; i++;
    regAddr[i] = 0xA85C; regValue[i] = 0x007E; i++;
    regAddr[i] = 0xA85E; regValue[i] = 0xC104; i++;

    regAddr[i] = 0xA860; regValue[i] = 0x2941; i++;
    regAddr[i] = 0xA862; regValue[i] = 0x7400; i++;
    regAddr[i] = 0xA864; regValue[i] = 0x7030; i++;
    regAddr[i] = 0xA866; regValue[i] = 0x21CA; i++;
    regAddr[i] = 0xA868; regValue[i] = 0x000D; i++;
    regAddr[i] = 0xA86A; regValue[i] = 0xC00F; i++;
    regAddr[i] = 0xA86C; regValue[i] = 0x7822; i++;
    regAddr[i] = 0xA86E; regValue[i] = 0x8521; i++;

    regAddr[i] = 0xA870; regValue[i] = 0xB808; i++;
    regAddr[i] = 0xA872; regValue[i] = 0x0D1A; i++;
    regAddr[i] = 0xA874; regValue[i] = 0x0D60; i++;
    regAddr[i] = 0xA876; regValue[i] = 0x7942; i++;
    regAddr[i] = 0xA878; regValue[i] = 0xC050; i++;
    regAddr[i] = 0xA87A; regValue[i] = 0x8503; i++;
    regAddr[i] = 0xA87C; regValue[i] = 0xD900; i++;
    regAddr[i] = 0xA87E; regValue[i] = 0x0966; i++;

    regAddr[i] = 0xA880; regValue[i] = 0x06E0; i++;
    regAddr[i] = 0xA882; regValue[i] = 0xDA08; i++;
    regAddr[i] = 0xA884; regValue[i] = 0xC10C; i++;
    regAddr[i] = 0xA886; regValue[i] = 0x2805; i++;
    regAddr[i] = 0xA888; regValue[i] = 0x007E; i++;
    regAddr[i] = 0xA88A; regValue[i] = 0xC00A; i++;
    regAddr[i] = 0xA88C; regValue[i] = 0x2941; i++;
    regAddr[i] = 0xA88E; regValue[i] = 0x7401; i++;

    regAddr[i] = 0xA890; regValue[i] = 0x6038; i++;
    regAddr[i] = 0xA892; regValue[i] = 0x78C2; i++;
    regAddr[i] = 0xA894; regValue[i] = 0xC051; i++;
    regAddr[i] = 0xA896; regValue[i] = 0x8502; i++;
    regAddr[i] = 0xA898; regValue[i] = 0xD900; i++;
    regAddr[i] = 0xA89A; regValue[i] = 0x094A; i++;
    regAddr[i] = 0xA89C; regValue[i] = 0x06E0; i++;
    regAddr[i] = 0xA89E; regValue[i] = 0xDA08; i++;

    regAddr[i] = 0xA8A0; regValue[i] = 0x2805; i++;
    regAddr[i] = 0xA8A2; regValue[i] = 0x03FE; i++;
    regAddr[i] = 0xA8A4; regValue[i] = 0xC111; i++;
    regAddr[i] = 0xA8A6; regValue[i] = 0x2941; i++;
    regAddr[i] = 0xA8A8; regValue[i] = 0x7400; i++;
    regAddr[i] = 0xA8AA; regValue[i] = 0x85C3; i++;
    regAddr[i] = 0xA8AC; regValue[i] = 0x85E2; i++;
    regAddr[i] = 0xA8AE; regValue[i] = 0x7902; i++;

    regAddr[i] = 0xA8B0; regValue[i] = 0x2940; i++;
    regAddr[i] = 0xA8B2; regValue[i] = 0x0200; i++;
    regAddr[i] = 0xA8B4; regValue[i] = 0x0CD6; i++;
    regAddr[i] = 0xA8B6; regValue[i] = 0x0D60; i++;
    regAddr[i] = 0xA8B8; regValue[i] = 0x2602; i++;
    regAddr[i] = 0xA8BA; regValue[i] = 0x13C1; i++;
    regAddr[i] = 0xA8BC; regValue[i] = 0x145F; i++;
    regAddr[i] = 0xA8BE; regValue[i] = 0x3082; i++;

    regAddr[i] = 0xA8C0; regValue[i] = 0x1500; i++;
    regAddr[i] = 0xA8C2; regValue[i] = 0x100A; i++;
    regAddr[i] = 0xA8C4; regValue[i] = 0xC300; i++;
    regAddr[i] = 0xA8C6; regValue[i] = 0x6A34; i++;
    regAddr[i] = 0xA8C8; regValue[i] = 0x6179; i++;
    regAddr[i] = 0xA8CA; regValue[i] = 0x8561; i++;
    regAddr[i] = 0xA8CC; regValue[i] = 0x1900; i++;
    regAddr[i] = 0xA8CE; regValue[i] = 0x0280; i++;

    regAddr[i] = 0xA8D0; regValue[i] = 0xBA02; i++;
    regAddr[i] = 0xA8D2; regValue[i] = 0xA161; i++;
    regAddr[i] = 0xA8D4; regValue[i] = 0xA1E2; i++;
    regAddr[i] = 0xA8D6; regValue[i] = 0xA1C3; i++;
    regAddr[i] = 0xA8D8; regValue[i] = 0xC100; i++;
    regAddr[i] = 0xA8DA; regValue[i] = 0x7A55; i++;
    regAddr[i] = 0xA8DC; regValue[i] = 0x623A; i++;
    regAddr[i] = 0xA8DE; regValue[i] = 0xC103; i++;

    regAddr[i] = 0xA8E0; regValue[i] = 0xB238; i++;
    regAddr[i] = 0xA8E2; regValue[i] = 0xC105; i++;
    regAddr[i] = 0xA8E4; regValue[i] = 0xB239; i++;
    regAddr[i] = 0xA8E6; regValue[i] = 0xC107; i++;
    regAddr[i] = 0xA8E8; regValue[i] = 0xB23A; i++;
    regAddr[i] = 0xA8EA; regValue[i] = 0xC108; i++;
    regAddr[i] = 0xA8EC; regValue[i] = 0xB23B; i++;
    regAddr[i] = 0xA8EE; regValue[i] = 0xC109; i++;

    regAddr[i] = 0xA8F0; regValue[i] = 0xB23C; i++;
    regAddr[i] = 0xA8F2; regValue[i] = 0xC10B; i++;
    regAddr[i] = 0xA8F4; regValue[i] = 0xB23D; i++;
    regAddr[i] = 0xA8F6; regValue[i] = 0xC10D; i++;
    regAddr[i] = 0xA8F8; regValue[i] = 0xB23E; i++;
    regAddr[i] = 0xA8FA; regValue[i] = 0xC110; i++;
    regAddr[i] = 0xA8FC; regValue[i] = 0xB23F; i++;
    regAddr[i] = 0xA8FE; regValue[i] = 0xC10E; i++;

    regAddr[i] = 0xA900; regValue[i] = 0x1A40; i++;
    regAddr[i] = 0xA902; regValue[i] = 0x0044; i++;
    regAddr[i] = 0xA904; regValue[i] = 0x1A42; i++;
    regAddr[i] = 0xA906; regValue[i] = 0x0004; i++;
    regAddr[i] = 0xA908; regValue[i] = 0x045D; i++;
    regAddr[i] = 0xA90A; regValue[i] = 0x0D60; i++;
    regAddr[i] = 0xA90C; regValue[i] = 0xC0B3; i++;
    regAddr[i] = 0xA90E; regValue[i] = 0x78E0; i++;

    regAddr[i] = 0xA910; regValue[i] = 0xDB0C; i++;
    regAddr[i] = 0xA912; regValue[i] = 0x0813; i++;
    regAddr[i] = 0xA914; regValue[i] = 0x0091; i++;
    regAddr[i] = 0xA916; regValue[i] = 0xE10E; i++;
    regAddr[i] = 0xA918; regValue[i] = 0xD810; i++;
    regAddr[i] = 0xA91A; regValue[i] = 0xE190; i++;
    regAddr[i] = 0xA91C; regValue[i] = 0x7FE0; i++;
    regAddr[i] = 0xA91E; regValue[i] = 0x20CA; i++;

    regAddr[i] = 0xA920; regValue[i] = 0x0045; i++;
    regAddr[i] = 0xA922; regValue[i] = 0xE083; i++;
    regAddr[i] = 0xA924; regValue[i] = 0x20CA; i++;
    regAddr[i] = 0xA926; regValue[i] = 0x00C2; i++;
    regAddr[i] = 0xA928; regValue[i] = 0x7DE0; i++;
    regAddr[i] = 0xA92A; regValue[i] = 0x6902; i++;
    regAddr[i] = 0xA92C; regValue[i] = 0xE202; i++;
    regAddr[i] = 0xA92E; regValue[i] = 0x6058; i++;

    regAddr[i] = 0xA930; regValue[i] = 0xE00C; i++;
    regAddr[i] = 0xA932; regValue[i] = 0xD914; i++;
    regAddr[i] = 0xA934; regValue[i] = 0xE094; i++;
    regAddr[i] = 0xA936; regValue[i] = 0x7FE0; i++;
    regAddr[i] = 0xA938; regValue[i] = 0x20CA; i++;
    regAddr[i] = 0xA93A; regValue[i] = 0x0046; i++;
    regAddr[i] = 0xA93C; regValue[i] = 0x70CF; i++;
    regAddr[i] = 0xA93E; regValue[i] = 0xFFFF; i++;

    regAddr[i] = 0xA940; regValue[i] = 0x92B4; i++;
    regAddr[i] = 0xA942; regValue[i] = 0x800C; i++;
    regAddr[i] = 0xA944; regValue[i] = 0x72CF; i++;
    regAddr[i] = 0xA946; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xA948; regValue[i] = 0x95B4; i++;
    regAddr[i] = 0xA94A; regValue[i] = 0x9223; i++;
    regAddr[i] = 0xA94C; regValue[i] = 0x9244; i++;
    regAddr[i] = 0xA94E; regValue[i] = 0xB8C0; i++;

    regAddr[i] = 0xA950; regValue[i] = 0xE002; i++;
    regAddr[i] = 0xA952; regValue[i] = 0xF1E0; i++;
    regAddr[i] = 0xA954; regValue[i] = 0xC0F1; i++;
    regAddr[i] = 0xA956; regValue[i] = 0x70CF; i++;
    regAddr[i] = 0xA958; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xA95A; regValue[i] = 0x92B4; i++;
    regAddr[i] = 0xA95C; regValue[i] = 0x800C; i++;
    regAddr[i] = 0xA95E; regValue[i] = 0x71CF; i++;

    regAddr[i] = 0xA960; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xA962; regValue[i] = 0x95B4; i++;
    regAddr[i] = 0xA964; regValue[i] = 0xB8C0; i++;
    regAddr[i] = 0xA966; regValue[i] = 0xE002; i++;
    regAddr[i] = 0xA968; regValue[i] = 0x0823; i++;
    regAddr[i] = 0xA96A; regValue[i] = 0x0091; i++;
    regAddr[i] = 0xA96C; regValue[i] = 0x1106; i++;
    regAddr[i] = 0xA96E; regValue[i] = 0x010A; i++;

    regAddr[i] = 0xA970; regValue[i] = 0x2A40; i++;
    regAddr[i] = 0xA972; regValue[i] = 0x11CA; i++;
    regAddr[i] = 0xA974; regValue[i] = 0xFFF2; i++;
    regAddr[i] = 0xA976; regValue[i] = 0xD900; i++;
    regAddr[i] = 0xA978; regValue[i] = 0x210F; i++;
    regAddr[i] = 0xA97A; regValue[i] = 0x0001; i++;
    regAddr[i] = 0xA97C; regValue[i] = 0xB961; i++;
    regAddr[i] = 0xA97E; regValue[i] = 0x7151; i++;

    regAddr[i] = 0xA980; regValue[i] = 0x20CA; i++;
    regAddr[i] = 0xA982; regValue[i] = 0x0286; i++;
    regAddr[i] = 0xA984; regValue[i] = 0x20CA; i++;
    regAddr[i] = 0xA986; regValue[i] = 0x0045; i++;
    regAddr[i] = 0xA988; regValue[i] = 0xF00D; i++;
    regAddr[i] = 0xA98A; regValue[i] = 0x0815; i++;
    regAddr[i] = 0xA98C; regValue[i] = 0x00D1; i++;
    regAddr[i] = 0xA98E; regValue[i] = 0x9103; i++;

    regAddr[i] = 0xA990; regValue[i] = 0x9124; i++;
    regAddr[i] = 0xA992; regValue[i] = 0xB80C; i++;
    regAddr[i] = 0xA994; regValue[i] = 0x790C; i++;
    regAddr[i] = 0xA996; regValue[i] = 0x2941; i++;
    regAddr[i] = 0xA998; regValue[i] = 0x728A; i++;
    regAddr[i] = 0xA99A; regValue[i] = 0xF1EE; i++;
    regAddr[i] = 0xA99C; regValue[i] = 0x70CF; i++;
    regAddr[i] = 0xA99E; regValue[i] = 0x0000; i++;

    regAddr[i] = 0xA9A0; regValue[i] = 0x0FFF; i++;
    regAddr[i] = 0xA9A2; regValue[i] = 0x0C8E; i++;
    regAddr[i] = 0xA9A4; regValue[i] = 0x0AC0; i++;
    regAddr[i] = 0xA9A6; regValue[i] = 0xC0D1; i++;
    regAddr[i] = 0xA9A8; regValue[i] = 0x7EE0; i++;
    regAddr[i] = 0xA9AA; regValue[i] = 0x78E0; i++;
    regAddr[i] = 0xA9AC; regValue[i] = 0x70CF; i++;
    regAddr[i] = 0xA9AE; regValue[i] = 0xFFFF; i++;

    regAddr[i] = 0xA9B0; regValue[i] = 0x8520; i++;
    regAddr[i] = 0xA9B2; regValue[i] = 0x881A; i++;
    regAddr[i] = 0xA9B4; regValue[i] = 0xE080; i++;
    regAddr[i] = 0xA9B6; regValue[i] = 0xD801; i++;
    regAddr[i] = 0xA9B8; regValue[i] = 0x7FE0; i++;
    regAddr[i] = 0xA9BA; regValue[i] = 0x78C0; i++;
    regAddr[i] = 0xA9BC; regValue[i] = 0xC0F1; i++;
    regAddr[i] = 0xA9BE; regValue[i] = 0x0B9A; i++;

    regAddr[i] = 0xA9C0; regValue[i] = 0x0D60; i++;
    regAddr[i] = 0xA9C2; regValue[i] = 0x7208; i++;
    regAddr[i] = 0xA9C4; regValue[i] = 0x76CF; i++;
    regAddr[i] = 0xA9C6; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xA9C8; regValue[i] = 0x95B4; i++;
    regAddr[i] = 0xA9CA; regValue[i] = 0x75CF; i++;
    regAddr[i] = 0xA9CC; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xA9CE; regValue[i] = 0x8520; i++;

    regAddr[i] = 0xA9D0; regValue[i] = 0xE98D; i++;
    regAddr[i] = 0xA9D2; regValue[i] = 0x8D10; i++;
    regAddr[i] = 0xA9D4; regValue[i] = 0x080F; i++;
    regAddr[i] = 0xA9D6; regValue[i] = 0x0091; i++;
    regAddr[i] = 0xA9D8; regValue[i] = 0xFFF5; i++;
    regAddr[i] = 0xA9DA; regValue[i] = 0xE885; i++;
    regAddr[i] = 0xA9DC; regValue[i] = 0xD836; i++;
    regAddr[i] = 0xA9DE; regValue[i] = 0xA501; i++;

    regAddr[i] = 0xA9E0; regValue[i] = 0xF003; i++;
    regAddr[i] = 0xA9E2; regValue[i] = 0xD80C; i++;
    regAddr[i] = 0xA9E4; regValue[i] = 0xA501; i++;
    regAddr[i] = 0xA9E6; regValue[i] = 0xD800; i++;
    regAddr[i] = 0xA9E8; regValue[i] = 0xB504; i++;
    regAddr[i] = 0xA9EA; regValue[i] = 0xE280; i++;
    regAddr[i] = 0xA9EC; regValue[i] = 0xF4A3; i++;
    regAddr[i] = 0xA9EE; regValue[i] = 0x8501; i++;

    regAddr[i] = 0xA9F0; regValue[i] = 0x0849; i++;
    regAddr[i] = 0xA9F2; regValue[i] = 0x0311; i++;
    regAddr[i] = 0xA9F4; regValue[i] = 0x1512; i++;
    regAddr[i] = 0xA9F6; regValue[i] = 0x110B; i++;
    regAddr[i] = 0xA9F8; regValue[i] = 0xF003; i++;
    regAddr[i] = 0xA9FA; regValue[i] = 0xE301; i++;
    regAddr[i] = 0xA9FC; regValue[i] = 0xB564; i++;
    regAddr[i] = 0xA9FE; regValue[i] = 0x9564; i++;

    regAddr[i] = 0xAA00; regValue[i] = 0x0BDF; i++;
    regAddr[i] = 0xAA02; regValue[i] = 0x0413; i++;
    regAddr[i] = 0xAA04; regValue[i] = 0xD800; i++;
    regAddr[i] = 0xAA06; regValue[i] = 0x200F; i++;
    regAddr[i] = 0xAA08; regValue[i] = 0x00C0; i++;
    regAddr[i] = 0xAA0A; regValue[i] = 0x230B; i++;
    regAddr[i] = 0xAA0C; regValue[i] = 0x9000; i++;
    regAddr[i] = 0xAA0E; regValue[i] = 0xF3F7; i++;

    regAddr[i] = 0xAA10; regValue[i] = 0x71CF; i++;
    regAddr[i] = 0xAA12; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xAA14; regValue[i] = 0x9608; i++;
    regAddr[i] = 0xAA16; regValue[i] = 0x1156; i++;
    regAddr[i] = 0xAA18; regValue[i] = 0x010A; i++;
    regAddr[i] = 0xAA1A; regValue[i] = 0x76CF; i++;
    regAddr[i] = 0xAA1C; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xAA1E; regValue[i] = 0x9660; i++;

    regAddr[i] = 0xAA20; regValue[i] = 0x220B; i++;
    regAddr[i] = 0xAA22; regValue[i] = 0x9000; i++;
    regAddr[i] = 0xAA24; regValue[i] = 0xF5EB; i++;
    regAddr[i] = 0xAA26; regValue[i] = 0xD80C; i++;
    regAddr[i] = 0xAA28; regValue[i] = 0xA501; i++;
    regAddr[i] = 0xAA2A; regValue[i] = 0x26F4; i++;
    regAddr[i] = 0xAA2C; regValue[i] = 0x10C0; i++;
    regAddr[i] = 0xAA2E; regValue[i] = 0xE178; i++;

    regAddr[i] = 0xAA30; regValue[i] = 0x082E; i++;
    regAddr[i] = 0xAA32; regValue[i] = 0x0620; i++;
    regAddr[i] = 0xAA34; regValue[i] = 0x7974; i++;
    regAddr[i] = 0xAA36; regValue[i] = 0xF050; i++;
    regAddr[i] = 0xAA38; regValue[i] = 0xE0B7; i++;
    regAddr[i] = 0xAA3A; regValue[i] = 0xF231; i++;
    regAddr[i] = 0xAA3C; regValue[i] = 0xF611; i++;
    regAddr[i] = 0xAA3E; regValue[i] = 0x080D; i++;

    regAddr[i] = 0xAA40; regValue[i] = 0x0351; i++;
    regAddr[i] = 0xAA42; regValue[i] = 0x1512; i++;
    regAddr[i] = 0xAA44; regValue[i] = 0x110B; i++;
    regAddr[i] = 0xAA46; regValue[i] = 0xF06D; i++;
    regAddr[i] = 0xAA48; regValue[i] = 0xE0B6; i++;
    regAddr[i] = 0xAA4A; regValue[i] = 0xF475; i++;
    regAddr[i] = 0xAA4C; regValue[i] = 0x70CF; i++;
    regAddr[i] = 0xAA4E; regValue[i] = 0x0000; i++;

    regAddr[i] = 0xAA50; regValue[i] = 0x30AC; i++;
    regAddr[i] = 0xAA52; regValue[i] = 0x080E; i++;
    regAddr[i] = 0xAA54; regValue[i] = 0x0620; i++;
    regAddr[i] = 0xAA56; regValue[i] = 0x2540; i++;
    regAddr[i] = 0xAA58; regValue[i] = 0x1281; i++;
    regAddr[i] = 0xAA5A; regValue[i] = 0xD837; i++;
    regAddr[i] = 0xAA5C; regValue[i] = 0xF01C; i++;
    regAddr[i] = 0xAA5E; regValue[i] = 0x0865; i++;

    regAddr[i] = 0xAA60; regValue[i] = 0x0E10; i++;
    regAddr[i] = 0xAA62; regValue[i] = 0x08D3; i++;
    regAddr[i] = 0xAA64; regValue[i] = 0x0E51; i++;
    regAddr[i] = 0xAA66; regValue[i] = 0x9505; i++;
    regAddr[i] = 0xAA68; regValue[i] = 0x77CF; i++;
    regAddr[i] = 0xAA6A; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xAA6C; regValue[i] = 0x9608; i++;
    regAddr[i] = 0xAA6E; regValue[i] = 0xB602; i++;

    regAddr[i] = 0xAA70; regValue[i] = 0x9718; i++;
    regAddr[i] = 0xAA72; regValue[i] = 0x9621; i++;
    regAddr[i] = 0xAA74; regValue[i] = 0x0B16; i++;
    regAddr[i] = 0xAA76; regValue[i] = 0x0D60; i++;
    regAddr[i] = 0xAA78; regValue[i] = 0xB805; i++;
    regAddr[i] = 0xAA7A; regValue[i] = 0xB603; i++;
    regAddr[i] = 0xAA7C; regValue[i] = 0x9601; i++;
    regAddr[i] = 0xAA7E; regValue[i] = 0x973F; i++;

    regAddr[i] = 0xAA80; regValue[i] = 0x2805; i++;
    regAddr[i] = 0xAA82; regValue[i] = 0x007E; i++;
    regAddr[i] = 0xAA84; regValue[i] = 0x9622; i++;
    regAddr[i] = 0xAA86; regValue[i] = 0x0B06; i++;
    regAddr[i] = 0xAA88; regValue[i] = 0x0D60; i++;
    regAddr[i] = 0xAA8A; regValue[i] = 0x2940; i++;
    regAddr[i] = 0xAA8C; regValue[i] = 0x7140; i++;
    regAddr[i] = 0xAA8E; regValue[i] = 0xB604; i++;

    regAddr[i] = 0xAA90; regValue[i] = 0xFFB1; i++;
    regAddr[i] = 0xAA92; regValue[i] = 0xD80C; i++;
    regAddr[i] = 0xAA94; regValue[i] = 0xA501; i++;
    regAddr[i] = 0xAA96; regValue[i] = 0xDA09; i++;
    regAddr[i] = 0xAA98; regValue[i] = 0xF04D; i++;
    regAddr[i] = 0xAA9A; regValue[i] = 0x9505; i++;
    regAddr[i] = 0xAA9C; regValue[i] = 0xB600; i++;
    regAddr[i] = 0xAA9E; regValue[i] = 0x70CF; i++;

    regAddr[i] = 0xAAA0; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xAAA2; regValue[i] = 0x9680; i++;
    regAddr[i] = 0xAAA4; regValue[i] = 0x9038; i++;
    regAddr[i] = 0xAAA6; regValue[i] = 0x9600; i++;
    regAddr[i] = 0xAAA8; regValue[i] = 0x7110; i++;
    regAddr[i] = 0xAAAA; regValue[i] = 0xD800; i++;
    regAddr[i] = 0xAAAC; regValue[i] = 0xF5F4; i++;
    regAddr[i] = 0xAAAE; regValue[i] = 0x70CF; i++;

    regAddr[i] = 0xAAB0; regValue[i] = 0x0000; i++;
    regAddr[i] = 0xAAB2; regValue[i] = 0x307C; i++;
    regAddr[i] = 0xAAB4; regValue[i] = 0x0FAA; i++;
    regAddr[i] = 0xAAB6; regValue[i] = 0x05E0; i++;
    regAddr[i] = 0xAAB8; regValue[i] = 0x2540; i++;
    regAddr[i] = 0xAABA; regValue[i] = 0x1281; i++;
    regAddr[i] = 0xAABC; regValue[i] = 0xD838; i++;
    regAddr[i] = 0xAABE; regValue[i] = 0xF1EC; i++;

    regAddr[i] = 0xAAC0; regValue[i] = 0x9505; i++;
    regAddr[i] = 0xAAC2; regValue[i] = 0xB601; i++;
    regAddr[i] = 0xAAC4; regValue[i] = 0xD861; i++;
    regAddr[i] = 0xAAC6; regValue[i] = 0xB807; i++;
    regAddr[i] = 0xAAC8; regValue[i] = 0x0F96; i++;
    regAddr[i] = 0xAACA; regValue[i] = 0x05E0; i++;
    regAddr[i] = 0xAACC; regValue[i] = 0x2540; i++;
    regAddr[i] = 0xAACE; regValue[i] = 0x1281; i++;

    regAddr[i] = 0xAAD0; regValue[i] = 0xD839; i++;
    regAddr[i] = 0xAAD2; regValue[i] = 0xF1E2; i++;
    regAddr[i] = 0xAAD4; regValue[i] = 0x9504; i++;
    regAddr[i] = 0xAAD6; regValue[i] = 0xE001; i++;
    regAddr[i] = 0xAAD8; regValue[i] = 0xB504; i++;
    regAddr[i] = 0xAADA; regValue[i] = 0xDA09; i++;
    regAddr[i] = 0xAADC; regValue[i] = 0xF02B; i++;
    regAddr[i] = 0xAADE; regValue[i] = 0xD800; i++;

    regAddr[i] = 0xAAE0; regValue[i] = 0xF01E; i++;
    regAddr[i] = 0xAAE2; regValue[i] = 0xDB00; i++;
    regAddr[i] = 0xAAE4; regValue[i] = 0x230F; i++;
    regAddr[i] = 0xAAE6; regValue[i] = 0x0003; i++;
    regAddr[i] = 0xAAE8; regValue[i] = 0x230B; i++;
    regAddr[i] = 0xAAEA; regValue[i] = 0x90C0; i++;
    regAddr[i] = 0xAAEC; regValue[i] = 0xF217; i++;
    regAddr[i] = 0xAAEE; regValue[i] = 0x220A; i++;

    regAddr[i] = 0xAAF0; regValue[i] = 0x1F80; i++;
    regAddr[i] = 0xAAF2; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xAAF4; regValue[i] = 0x9644; i++;
    regAddr[i] = 0xAAF6; regValue[i] = 0x121A; i++;
    regAddr[i] = 0xAAF8; regValue[i] = 0x110A; i++;
    regAddr[i] = 0xAAFA; regValue[i] = 0x71CF; i++;
    regAddr[i] = 0xAAFC; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xAAFE; regValue[i] = 0x9608; i++;

    regAddr[i] = 0xAB00; regValue[i] = 0x220B; i++;
    regAddr[i] = 0xAB02; regValue[i] = 0x90C0; i++;
    regAddr[i] = 0xAB04; regValue[i] = 0xF20B; i++;
    regAddr[i] = 0xAB06; regValue[i] = 0xDA0D; i++;
    regAddr[i] = 0xAB08; regValue[i] = 0xA541; i++;
    regAddr[i] = 0xAB0A; regValue[i] = 0x7914; i++;
    regAddr[i] = 0xAB0C; regValue[i] = 0x1158; i++;
    regAddr[i] = 0xAB0E; regValue[i] = 0x0100; i++;

    regAddr[i] = 0xAB10; regValue[i] = 0x0F26; i++;
    regAddr[i] = 0xAB12; regValue[i] = 0x05E0; i++;
    regAddr[i] = 0xAB14; regValue[i] = 0x1178; i++;
    regAddr[i] = 0xAB16; regValue[i] = 0x0101; i++;
    regAddr[i] = 0xAB18; regValue[i] = 0xF1DE; i++;
    regAddr[i] = 0xAB1A; regValue[i] = 0xE001; i++;
    regAddr[i] = 0xAB1C; regValue[i] = 0xB504; i++;
    regAddr[i] = 0xAB1E; regValue[i] = 0x9504; i++;

    regAddr[i] = 0xAB20; regValue[i] = 0x08C3; i++;
    regAddr[i] = 0xAB22; regValue[i] = 0x8412; i++;
    regAddr[i] = 0xAB24; regValue[i] = 0x71CF; i++;
    regAddr[i] = 0xAB26; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xAB28; regValue[i] = 0x9644; i++;
    regAddr[i] = 0xAB2A; regValue[i] = 0x910C; i++;
    regAddr[i] = 0xAB2C; regValue[i] = 0x2006; i++;
    regAddr[i] = 0xAB2E; regValue[i] = 0x02C0; i++;

    regAddr[i] = 0xAB30; regValue[i] = 0xB10C; i++;
    regAddr[i] = 0xAB32; regValue[i] = 0x0235; i++;
    regAddr[i] = 0xAB34; regValue[i] = 0x0D60; i++;
    regAddr[i] = 0xAB36; regValue[i] = 0x7048; i++;
    regAddr[i] = 0xAB38; regValue[i] = 0xC0F1; i++;
    regAddr[i] = 0xAB3A; regValue[i] = 0x0A1E; i++;
    regAddr[i] = 0xAB3C; regValue[i] = 0x0D40; i++;
    regAddr[i] = 0xAB3E; regValue[i] = 0x0AE2; i++;

    regAddr[i] = 0xAB40; regValue[i] = 0x02E0; i++;
    regAddr[i] = 0xAB42; regValue[i] = 0xC1A2; i++;
    regAddr[i] = 0xAB44; regValue[i] = 0x7608; i++;
    regAddr[i] = 0xAB46; regValue[i] = 0x77CF; i++;
    regAddr[i] = 0xAB48; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xAB4A; regValue[i] = 0x92B4; i++;
    regAddr[i] = 0xAB4C; regValue[i] = 0x75CF; i++;
    regAddr[i] = 0xAB4E; regValue[i] = 0xFFFF; i++;

    regAddr[i] = 0xAB50; regValue[i] = 0x9608; i++;
    regAddr[i] = 0xAB52; regValue[i] = 0x17D1; i++;
    regAddr[i] = 0xAB54; regValue[i] = 0x1080; i++;
    regAddr[i] = 0xAB56; regValue[i] = 0x9523; i++;
    regAddr[i] = 0xAB58; regValue[i] = 0x0D2E; i++;
    regAddr[i] = 0xAB5A; regValue[i] = 0x05A0; i++;
    regAddr[i] = 0xAB5C; regValue[i] = 0x72C9; i++;
    regAddr[i] = 0xAB5E; regValue[i] = 0xB505; i++;

    regAddr[i] = 0xAB60; regValue[i] = 0x70C9; i++;
    regAddr[i] = 0xAB62; regValue[i] = 0x0A2A; i++;
    regAddr[i] = 0xAB64; regValue[i] = 0x0D60; i++;
    regAddr[i] = 0xAB66; regValue[i] = 0x9524; i++;
    regAddr[i] = 0xAB68; regValue[i] = 0xB808; i++;
    regAddr[i] = 0xAB6A; regValue[i] = 0x0A22; i++;
    regAddr[i] = 0xAB6C; regValue[i] = 0x0D60; i++;
    regAddr[i] = 0xAB6E; regValue[i] = 0x9523; i++;

    regAddr[i] = 0xAB70; regValue[i] = 0xC040; i++;
    regAddr[i] = 0xAB72; regValue[i] = 0x0D32; i++;
    regAddr[i] = 0xAB74; regValue[i] = 0x05A0; i++;
    regAddr[i] = 0xAB76; regValue[i] = 0x1FD2; i++;
    regAddr[i] = 0xAB78; regValue[i] = 0x1004; i++;
    regAddr[i] = 0xAB7A; regValue[i] = 0x1DB0; i++;
    regAddr[i] = 0xAB7C; regValue[i] = 0x1004; i++;
    regAddr[i] = 0xAB7E; regValue[i] = 0x70CF; i++;

    regAddr[i] = 0xAB80; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xAB82; regValue[i] = 0x8520; i++;
    regAddr[i] = 0xAB84; regValue[i] = 0x881A; i++;
    regAddr[i] = 0xAB86; regValue[i] = 0x95C4; i++;
    regAddr[i] = 0xAB88; regValue[i] = 0xE82B; i++;
    regAddr[i] = 0xAB8A; regValue[i] = 0x0FBA; i++;
    regAddr[i] = 0xAB8C; regValue[i] = 0x0560; i++;
    regAddr[i] = 0xAB8E; regValue[i] = 0x70C9; i++;

    regAddr[i] = 0xAB90; regValue[i] = 0x9735; i++;
    regAddr[i] = 0xAB92; regValue[i] = 0x7110; i++;
    regAddr[i] = 0xAB94; regValue[i] = 0x26CA; i++;
    regAddr[i] = 0xAB96; regValue[i] = 0x1006; i++;
    regAddr[i] = 0xAB98; regValue[i] = 0x26CA; i++;
    regAddr[i] = 0xAB9A; regValue[i] = 0x1045; i++;
    regAddr[i] = 0xAB9C; regValue[i] = 0x6E09; i++;
    regAddr[i] = 0xAB9E; regValue[i] = 0x872C; i++;

    regAddr[i] = 0xABA0; regValue[i] = 0x1DA6; i++;
    regAddr[i] = 0xABA2; regValue[i] = 0x1004; i++;
    regAddr[i] = 0xABA4; regValue[i] = 0xD800; i++;
    regAddr[i] = 0xABA6; regValue[i] = 0x2144; i++;
    regAddr[i] = 0xABA8; regValue[i] = 0x0382; i++;
    regAddr[i] = 0xABAA; regValue[i] = 0x7A5D; i++;
    regAddr[i] = 0xABAC; regValue[i] = 0xE202; i++;
    regAddr[i] = 0xABAE; regValue[i] = 0x2186; i++;

    regAddr[i] = 0xABB0; regValue[i] = 0x03FE; i++;
    regAddr[i] = 0xABB2; regValue[i] = 0x200F; i++;
    regAddr[i] = 0xABB4; regValue[i] = 0x0080; i++;
    regAddr[i] = 0xABB6; regValue[i] = 0xB924; i++;
    regAddr[i] = 0xABB8; regValue[i] = 0xE102; i++;
    regAddr[i] = 0xABBA; regValue[i] = 0xDA00; i++;
    regAddr[i] = 0xABBC; regValue[i] = 0x220F; i++;
    regAddr[i] = 0xABBE; regValue[i] = 0x0042; i++;

    regAddr[i] = 0xABC0; regValue[i] = 0x7A0C; i++;
    regAddr[i] = 0xABC2; regValue[i] = 0x0C7A; i++;
    regAddr[i] = 0xABC4; regValue[i] = 0x05A0; i++;
    regAddr[i] = 0xABC6; regValue[i] = 0x702F; i++;
    regAddr[i] = 0xABC8; regValue[i] = 0x7708; i++;
    regAddr[i] = 0xABCA; regValue[i] = 0x0C72; i++;
    regAddr[i] = 0xABCC; regValue[i] = 0x05A0; i++;
    regAddr[i] = 0xABCE; regValue[i] = 0x70C9; i++;

    regAddr[i] = 0xABD0; regValue[i] = 0x7608; i++;
    regAddr[i] = 0xABD2; regValue[i] = 0x9503; i++;
    regAddr[i] = 0xABD4; regValue[i] = 0x2805; i++;
    regAddr[i] = 0xABD6; regValue[i] = 0x03FE; i++;
    regAddr[i] = 0xABD8; regValue[i] = 0x1DA0; i++;
    regAddr[i] = 0xABDA; regValue[i] = 0x1E40; i++;
    regAddr[i] = 0xABDC; regValue[i] = 0xF007; i++;
    regAddr[i] = 0xABDE; regValue[i] = 0x6E09; i++;

    regAddr[i] = 0xABE0; regValue[i] = 0x1DA6; i++;
    regAddr[i] = 0xABE2; regValue[i] = 0x1004; i++;
    regAddr[i] = 0xABE4; regValue[i] = 0x9708; i++;
    regAddr[i] = 0xABE6; regValue[i] = 0x1DA0; i++;
    regAddr[i] = 0xABE8; regValue[i] = 0x1000; i++;
    regAddr[i] = 0xABEA; regValue[i] = 0x6E09; i++;
    regAddr[i] = 0xABEC; regValue[i] = 0x1DA4; i++;
    regAddr[i] = 0xABEE; regValue[i] = 0x1004; i++;

    regAddr[i] = 0xABF0; regValue[i] = 0x9502; i++;
    regAddr[i] = 0xABF2; regValue[i] = 0x083F; i++;
    regAddr[i] = 0xABF4; regValue[i] = 0x011E; i++;
    regAddr[i] = 0xABF6; regValue[i] = 0xC000; i++;
    regAddr[i] = 0xABF8; regValue[i] = 0x2E05; i++;
    regAddr[i] = 0xABFA; regValue[i] = 0x103E; i++;
    regAddr[i] = 0xABFC; regValue[i] = 0x702F; i++;
    regAddr[i] = 0xABFE; regValue[i] = 0x098E; i++;

    regAddr[i] = 0xAC00; regValue[i] = 0x0D60; i++;
    regAddr[i] = 0xAC02; regValue[i] = 0x15B0; i++;
    regAddr[i] = 0xAC04; regValue[i] = 0x1101; i++;
    regAddr[i] = 0xAC06; regValue[i] = 0x95E5; i++;
    regAddr[i] = 0xAC08; regValue[i] = 0x0982; i++;
    regAddr[i] = 0xAC0A; regValue[i] = 0x0D60; i++;
    regAddr[i] = 0xAC0C; regValue[i] = 0x71E9; i++;
    regAddr[i] = 0xAC0E; regValue[i] = 0xC041; i++;

    regAddr[i] = 0xAC10; regValue[i] = 0x70C9; i++;
    regAddr[i] = 0xAC12; regValue[i] = 0x097A; i++;
    regAddr[i] = 0xAC14; regValue[i] = 0x0D60; i++;
    regAddr[i] = 0xAC16; regValue[i] = 0x71E9; i++;
    regAddr[i] = 0xAC18; regValue[i] = 0xD901; i++;
    regAddr[i] = 0xAC1A; regValue[i] = 0xE081; i++;
    regAddr[i] = 0xAC1C; regValue[i] = 0x20CA; i++;
    regAddr[i] = 0xAC1E; regValue[i] = 0x004E; i++;

    regAddr[i] = 0xAC20; regValue[i] = 0x7910; i++;
    regAddr[i] = 0xAC22; regValue[i] = 0xB507; i++;
    regAddr[i] = 0xAC24; regValue[i] = 0xC001; i++;
    regAddr[i] = 0xAC26; regValue[i] = 0x7110; i++;
    regAddr[i] = 0xAC28; regValue[i] = 0x20CA; i++;
    regAddr[i] = 0xAC2A; regValue[i] = 0x004D; i++;
    regAddr[i] = 0xAC2C; regValue[i] = 0xB506; i++;
    regAddr[i] = 0xAC2E; regValue[i] = 0x0139; i++;

    regAddr[i] = 0xAC30; regValue[i] = 0x0D60; i++;
    regAddr[i] = 0xAC32; regValue[i] = 0xC0A2; i++;
    regAddr[i] = 0xAC34; regValue[i] = 0xD900; i++;
    regAddr[i] = 0xAC36; regValue[i] = 0xF00A; i++;
    regAddr[i] = 0xAC38; regValue[i] = 0x70CF; i++;
    regAddr[i] = 0xAC3A; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xAC3C; regValue[i] = 0xAC50; i++;
    regAddr[i] = 0xAC3E; regValue[i] = 0x7835; i++;

    regAddr[i] = 0xAC40; regValue[i] = 0x8041; i++;
    regAddr[i] = 0xAC42; regValue[i] = 0x8000; i++;
    regAddr[i] = 0xAC44; regValue[i] = 0xE102; i++;
    regAddr[i] = 0xAC46; regValue[i] = 0xA040; i++;
    regAddr[i] = 0xAC48; regValue[i] = 0x09F1; i++;
    regAddr[i] = 0xAC4A; regValue[i] = 0x8194; i++;
    regAddr[i] = 0xAC4C; regValue[i] = 0x7FE0; i++;
    regAddr[i] = 0xAC4E; regValue[i] = 0xD800; i++;

    regAddr[i] = 0xAC50; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xAC52; regValue[i] = 0xA198; i++;
    regAddr[i] = 0xAC54; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xAC56; regValue[i] = 0xA66C; i++;
    regAddr[i] = 0xAC58; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xAC5A; regValue[i] = 0xA2C8; i++;
    regAddr[i] = 0xAC5C; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xAC5E; regValue[i] = 0xA9BC; i++;

    regAddr[i] = 0xAC60; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xAC62; regValue[i] = 0x9F4C; i++;
    regAddr[i] = 0xAC64; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xAC66; regValue[i] = 0xAB38; i++;

    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }    
    OSA_waitMsecs(5);

    //apply patch
    i = 0;
    regAddr[i] = 0x098E; regValue[i] = 0x0000; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC00; regValue[i] = 0x089C; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC02; regValue[i] = 0x0403; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC04; regValue[i] = 0x6003; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);    
    i = 0;
    regAddr[i] = 0xFC06; regValue[i] = 0x0104; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC08; regValue[i] = 0x05FC; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8702; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX apply patch 2 return value = %d\n", regValue[0]);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    OSA_waitMsecs(5);

    // Sensor Reset
    i = 0;
    regAddr[i] = 0x001A; regValue[i] = 0x0001; i++;    //Reset
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0x001A; regValue[i] = 0x0000; i++;   //Reset done
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    //Issue SYSMGR_GET_STATE repeatedly until it does not return EBUSY
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8101; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX Issue SYSMGR_GET_STATE = %d\n", regValue[0]);
        if(regValue[0] == 0)
        {
            break;
        }
    }
    //Issue SENSOR_MGR_DISCOVER_SENSOR host command and poll until it returns ENOERR
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8E00; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX Issue SENSOR_MGR_DISCOVER_SENSOR = %d\n", regValue[0]);
        if(regValue[0] == 0)
        {
            break;
        }
    }
    //Reserve RAM to contain a patch
    i = 0;
    regAddr[i] = 0x098E; regValue[i] = 0x7C00; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC00; regValue[i] = 0x08D0; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC02; regValue[i] = 0x0418; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8706; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX Reserve RAM to contain a patch 3 return value = %d\n", regValue[0]);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    OSA_waitMsecs(5);
    //patch
    i = 0;
    regAddr[i] = 0x0982; regValue[i] = 0x0001; i++;
    regAddr[i] = 0x098A; regValue[i] = 0x2C68; i++;
    regAddr[i] = 0xAC68; regValue[i] = 0x72CF; i++;
    regAddr[i] = 0xAC6A; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xAC6C; regValue[i] = 0x941C; i++;
    regAddr[i] = 0xAC6E; regValue[i] = 0x9250; i++;

    regAddr[i] = 0xAC70; regValue[i] = 0x0A13; i++;
    regAddr[i] = 0xAC72; regValue[i] = 0x00DE; i++;
    regAddr[i] = 0xAC74; regValue[i] = 0x73CF; i++;
    regAddr[i] = 0xAC76; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xAC78; regValue[i] = 0xB044; i++;
    regAddr[i] = 0xAC7A; regValue[i] = 0x8348; i++;
    regAddr[i] = 0xAC7C; regValue[i] = 0xA040; i++;
    regAddr[i] = 0xAC7E; regValue[i] = 0x8309; i++;

    regAddr[i] = 0xAC80; regValue[i] = 0xF007; i++;
    regAddr[i] = 0xAC82; regValue[i] = 0x73CF; i++;
    regAddr[i] = 0xAC84; regValue[i] = 0xFF00; i++;
    regAddr[i] = 0xAC86; regValue[i] = 0x2454; i++;
    regAddr[i] = 0xAC88; regValue[i] = 0x8340; i++;
    regAddr[i] = 0xAC8A; regValue[i] = 0xA040; i++;
    regAddr[i] = 0xAC8C; regValue[i] = 0x8301; i++;
    regAddr[i] = 0xAC8E; regValue[i] = 0x7FE0; i++;

    regAddr[i] = 0xAC90; regValue[i] = 0xA100; i++;
    regAddr[i] = 0xAC92; regValue[i] = 0x78E0; i++;
    regAddr[i] = 0xAC94; regValue[i] = 0x71CF; i++;
    regAddr[i] = 0xAC96; regValue[i] = 0xFF00; i++;
    regAddr[i] = 0xAC98; regValue[i] = 0x2418; i++;
    regAddr[i] = 0xAC9A; regValue[i] = 0xB119; i++;
    regAddr[i] = 0xAC9C; regValue[i] = 0x7EE0; i++;
    regAddr[i] = 0xAC9E; regValue[i] = 0x78E0; i++;

    regAddr[i] = 0xACA0; regValue[i] = 0x73CF; i++;
    regAddr[i] = 0xACA2; regValue[i] = 0xFF00; i++;
    regAddr[i] = 0xACA4; regValue[i] = 0x2418; i++;
    regAddr[i] = 0xACA6; regValue[i] = 0x9355; i++;
    regAddr[i] = 0xACA8; regValue[i] = 0xB040; i++;
    regAddr[i] = 0xACAA; regValue[i] = 0x9316; i++;
    regAddr[i] = 0xACAC; regValue[i] = 0x7FE0; i++;
    regAddr[i] = 0xACAE; regValue[i] = 0xB100; i++;

    regAddr[i] = 0xACB0; regValue[i] = 0x73CF; i++;
    regAddr[i] = 0xACB2; regValue[i] = 0xFF00; i++;
    regAddr[i] = 0xACB4; regValue[i] = 0x2418; i++;
    regAddr[i] = 0xACB6; regValue[i] = 0x9357; i++;
    regAddr[i] = 0xACB8; regValue[i] = 0xB040; i++;
    regAddr[i] = 0xACBA; regValue[i] = 0x9318; i++;
    regAddr[i] = 0xACBC; regValue[i] = 0x7FE0; i++;
    regAddr[i] = 0xACBE; regValue[i] = 0xB100; i++;

    regAddr[i] = 0xACC0; regValue[i] = 0xC0F1; i++;
    regAddr[i] = 0xACC2; regValue[i] = 0x089A; i++;
    regAddr[i] = 0xACC4; regValue[i] = 0x0D40; i++;
    regAddr[i] = 0xACC6; regValue[i] = 0xC1A2; i++;
    regAddr[i] = 0xACC8; regValue[i] = 0x708B; i++;
    regAddr[i] = 0xACCA; regValue[i] = 0xC181; i++;
    regAddr[i] = 0xACCC; regValue[i] = 0xFFE7; i++;
    regAddr[i] = 0xACCE; regValue[i] = 0x1400; i++;

    regAddr[i] = 0xACD0; regValue[i] = 0x300A; i++;
    regAddr[i] = 0xACD2; regValue[i] = 0xC001; i++;
    regAddr[i] = 0xACD4; regValue[i] = 0x75CF; i++;
    regAddr[i] = 0xACD6; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xACD8; regValue[i] = 0xB044; i++;
    regAddr[i] = 0xACDA; regValue[i] = 0x73CF; i++;
    regAddr[i] = 0xACDC; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xACDE; regValue[i] = 0x9978; i++;

    regAddr[i] = 0xACE0; regValue[i] = 0x2A41; i++;
    regAddr[i] = 0xACE2; regValue[i] = 0x118A; i++;
    regAddr[i] = 0xACE4; regValue[i] = 0xB826; i++;
    regAddr[i] = 0xACE6; regValue[i] = 0x952E; i++;
    regAddr[i] = 0xACE8; regValue[i] = 0x8343; i++;
    regAddr[i] = 0xACEA; regValue[i] = 0xC041; i++;
    regAddr[i] = 0xACEC; regValue[i] = 0x2905; i++;
    regAddr[i] = 0xACEE; regValue[i] = 0x00BE; i++;

    regAddr[i] = 0xACF0; regValue[i] = 0x954C; i++;
    regAddr[i] = 0xACF2; regValue[i] = 0x2941; i++;
    regAddr[i] = 0xACF4; regValue[i] = 0x7101; i++;
    regAddr[i] = 0xACF6; regValue[i] = 0x7942; i++;
    regAddr[i] = 0xACF8; regValue[i] = 0x2148; i++;
    regAddr[i] = 0xACFA; regValue[i] = 0x000E; i++;
    regAddr[i] = 0xACFC; regValue[i] = 0x952F; i++;
    regAddr[i] = 0xACFE; regValue[i] = 0x8344; i++;

    regAddr[i] = 0xAD00; regValue[i] = 0x1C00; i++;
    regAddr[i] = 0xAD02; regValue[i] = 0x3280; i++;
    regAddr[i] = 0xAD04; regValue[i] = 0x2905; i++;
    regAddr[i] = 0xAD06; regValue[i] = 0x00BE; i++;
    regAddr[i] = 0xAD08; regValue[i] = 0x954D; i++;
    regAddr[i] = 0xAD0A; regValue[i] = 0x2941; i++;
    regAddr[i] = 0xAD0C; regValue[i] = 0x7101; i++;
    regAddr[i] = 0xAD0E; regValue[i] = 0x2602; i++;

    regAddr[i] = 0xAD10; regValue[i] = 0x128E; i++;
    regAddr[i] = 0xAD12; regValue[i] = 0x2208; i++;
    regAddr[i] = 0xAD14; regValue[i] = 0x0042; i++;
    regAddr[i] = 0xAD16; regValue[i] = 0x71CF; i++;
    regAddr[i] = 0xAD18; regValue[i] = 0x000F; i++;
    regAddr[i] = 0xAD1A; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xAD1C; regValue[i] = 0x2209; i++;
    regAddr[i] = 0xAD1E; regValue[i] = 0x0041; i++;

    regAddr[i] = 0xAD20; regValue[i] = 0x2102; i++;
    regAddr[i] = 0xAD22; regValue[i] = 0x0002; i++;
    regAddr[i] = 0xAD24; regValue[i] = 0x2A84; i++;
    regAddr[i] = 0xAD26; regValue[i] = 0x0901; i++;
    regAddr[i] = 0xAD28; regValue[i] = 0x2009; i++;
    regAddr[i] = 0xAD2A; regValue[i] = 0x0041; i++;
    regAddr[i] = 0xAD2C; regValue[i] = 0x086E; i++;
    regAddr[i] = 0xAD2E; regValue[i] = 0x0D60; i++;

    regAddr[i] = 0xAD30; regValue[i] = 0x702F; i++;
    regAddr[i] = 0xAD32; regValue[i] = 0x72CF; i++;
    regAddr[i] = 0xAD34; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xAD36; regValue[i] = 0x0000; i++;
    regAddr[i] = 0xAD38; regValue[i] = 0x71CF; i++;
    regAddr[i] = 0xAD3A; regValue[i] = 0x0000; i++;
    regAddr[i] = 0xAD3C; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xAD3E; regValue[i] = 0x2608; i++;

    regAddr[i] = 0xAD40; regValue[i] = 0x1083; i++;
    regAddr[i] = 0xAD42; regValue[i] = 0x2008; i++;
    regAddr[i] = 0xAD44; regValue[i] = 0x0080; i++;
    regAddr[i] = 0xAD46; regValue[i] = 0x2309; i++;
    regAddr[i] = 0xAD48; regValue[i] = 0x0043; i++;
    regAddr[i] = 0xAD4A; regValue[i] = 0x2009; i++;
    regAddr[i] = 0xAD4C; regValue[i] = 0x0040; i++;
    regAddr[i] = 0xAD4E; regValue[i] = 0xB565; i++;

    regAddr[i] = 0xAD50; regValue[i] = 0xB506; i++;
    regAddr[i] = 0xAD52; regValue[i] = 0x9D25; i++;
    regAddr[i] = 0xAD54; regValue[i] = 0x9501; i++;
    regAddr[i] = 0xAD56; regValue[i] = 0x7813; i++;
    regAddr[i] = 0xAD58; regValue[i] = 0x090B; i++;
    regAddr[i] = 0xAD5A; regValue[i] = 0x0003; i++;
    regAddr[i] = 0xAD5C; regValue[i] = 0x9500; i++;
    regAddr[i] = 0xAD5E; regValue[i] = 0x0833; i++;

    regAddr[i] = 0xAD60; regValue[i] = 0x001F; i++;
    regAddr[i] = 0xAD62; regValue[i] = 0x9D25; i++;
    regAddr[i] = 0xAD64; regValue[i] = 0x9501; i++;
    regAddr[i] = 0xAD66; regValue[i] = 0x080D; i++;
    regAddr[i] = 0xAD68; regValue[i] = 0x0043; i++;
    regAddr[i] = 0xAD6A; regValue[i] = 0x9500; i++;
    regAddr[i] = 0xAD6C; regValue[i] = 0x0823; i++;
    regAddr[i] = 0xAD6E; regValue[i] = 0x005F; i++;

    regAddr[i] = 0xAD70; regValue[i] = 0x9D26; i++;
    regAddr[i] = 0xAD72; regValue[i] = 0x9503; i++;
    regAddr[i] = 0xAD74; regValue[i] = 0x7813; i++;
    regAddr[i] = 0xAD76; regValue[i] = 0x090D; i++;
    regAddr[i] = 0xAD78; regValue[i] = 0x0003; i++;
    regAddr[i] = 0xAD7A; regValue[i] = 0x9500; i++;
    regAddr[i] = 0xAD7C; regValue[i] = 0x0813; i++;
    regAddr[i] = 0xAD7E; regValue[i] = 0x009F; i++;

    regAddr[i] = 0xAD80; regValue[i] = 0x9D26; i++;
    regAddr[i] = 0xAD82; regValue[i] = 0x9503; i++;
    regAddr[i] = 0xAD84; regValue[i] = 0x080F; i++;
    regAddr[i] = 0xAD86; regValue[i] = 0x0043; i++;
    regAddr[i] = 0xAD88; regValue[i] = 0x9500; i++;
    regAddr[i] = 0xAD8A; regValue[i] = 0x080B; i++;
    regAddr[i] = 0xAD8C; regValue[i] = 0x00DE; i++;
    regAddr[i] = 0xAD8E; regValue[i] = 0x9507; i++;

    regAddr[i] = 0xAD90; regValue[i] = 0xF022; i++;
    regAddr[i] = 0xAD92; regValue[i] = 0x9D25; i++;
    regAddr[i] = 0xAD94; regValue[i] = 0x9502; i++;
    regAddr[i] = 0xAD96; regValue[i] = 0x7813; i++;
    regAddr[i] = 0xAD98; regValue[i] = 0x090B; i++;
    regAddr[i] = 0xAD9A; regValue[i] = 0x0003; i++;
    regAddr[i] = 0xAD9C; regValue[i] = 0x9500; i++;
    regAddr[i] = 0xAD9E; regValue[i] = 0x0833; i++;

    regAddr[i] = 0xADA0; regValue[i] = 0x001F; i++;
    regAddr[i] = 0xADA2; regValue[i] = 0x9D25; i++;
    regAddr[i] = 0xADA4; regValue[i] = 0x9502; i++;
    regAddr[i] = 0xADA6; regValue[i] = 0x080D; i++;
    regAddr[i] = 0xADA8; regValue[i] = 0x0043; i++;
    regAddr[i] = 0xADAA; regValue[i] = 0x9500; i++;
    regAddr[i] = 0xADAC; regValue[i] = 0x0823; i++;
    regAddr[i] = 0xADAE; regValue[i] = 0x005F; i++;

    regAddr[i] = 0xADB0; regValue[i] = 0x9D26; i++;
    regAddr[i] = 0xADB2; regValue[i] = 0x9504; i++;
    regAddr[i] = 0xADB4; regValue[i] = 0x7813; i++;
    regAddr[i] = 0xADB6; regValue[i] = 0x090D; i++;
    regAddr[i] = 0xADB8; regValue[i] = 0x0003; i++;
    regAddr[i] = 0xADBA; regValue[i] = 0x9500; i++;
    regAddr[i] = 0xADBC; regValue[i] = 0x0813; i++;
    regAddr[i] = 0xADBE; regValue[i] = 0x009F; i++;

    regAddr[i] = 0xADC0; regValue[i] = 0x9D26; i++;
    regAddr[i] = 0xADC2; regValue[i] = 0x9504; i++;
    regAddr[i] = 0xADC4; regValue[i] = 0x080F; i++;
    regAddr[i] = 0xADC6; regValue[i] = 0x0043; i++;
    regAddr[i] = 0xADC8; regValue[i] = 0x9500; i++;
    regAddr[i] = 0xADCA; regValue[i] = 0x080B; i++;
    regAddr[i] = 0xADCC; regValue[i] = 0x00DE; i++;
    regAddr[i] = 0xADCE; regValue[i] = 0x9508; i++;

    regAddr[i] = 0xADD0; regValue[i] = 0xF002; i++;
    regAddr[i] = 0xADD2; regValue[i] = 0x9509; i++;
    regAddr[i] = 0xADD4; regValue[i] = 0xFFB0; i++;
    regAddr[i] = 0xADD6; regValue[i] = 0x0799; i++;
    regAddr[i] = 0xADD8; regValue[i] = 0x0D20; i++;
    regAddr[i] = 0xADDA; regValue[i] = 0xC0A2; i++;
    regAddr[i] = 0xADDC; regValue[i] = 0xC0F1; i++;
    regAddr[i] = 0xADDE; regValue[i] = 0x0F92; i++;

    regAddr[i] = 0xADE0; regValue[i] = 0x0620; i++;
    regAddr[i] = 0xADE2; regValue[i] = 0xD880; i++;
    regAddr[i] = 0xADE4; regValue[i] = 0xE080; i++;
    regAddr[i] = 0xADE6; regValue[i] = 0xD809; i++;
    regAddr[i] = 0xADE8; regValue[i] = 0xF21A; i++;
    regAddr[i] = 0xADEA; regValue[i] = 0x70CF; i++;
    regAddr[i] = 0xADEC; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xADEE; regValue[i] = 0x92F0; i++;

    regAddr[i] = 0xADF0; regValue[i] = 0x9004; i++;
    regAddr[i] = 0xADF2; regValue[i] = 0x220A; i++;
    regAddr[i] = 0xADF4; regValue[i] = 0x1F80; i++;
    regAddr[i] = 0xADF6; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xADF8; regValue[i] = 0xB044; i++;
    regAddr[i] = 0xADFA; regValue[i] = 0xB8C2; i++;
    regAddr[i] = 0xADFC; regValue[i] = 0xE080; i++;
    regAddr[i] = 0xADFE; regValue[i] = 0xD801; i++;

    regAddr[i] = 0xAE00; regValue[i] = 0x78C0; i++;
    regAddr[i] = 0xAE02; regValue[i] = 0x1A28; i++;
    regAddr[i] = 0xAE04; regValue[i] = 0x1002; i++;
    regAddr[i] = 0xAE06; regValue[i] = 0x2240; i++;
    regAddr[i] = 0xAE08; regValue[i] = 0x1700; i++;
    regAddr[i] = 0xAE0A; regValue[i] = 0x2240; i++;
    regAddr[i] = 0xAE0C; regValue[i] = 0x1781; i++;
    regAddr[i] = 0xAE0E; regValue[i] = 0xFFA5; i++;

    regAddr[i] = 0xAE10; regValue[i] = 0x2240; i++;
    regAddr[i] = 0xAE12; regValue[i] = 0x1600; i++;
    regAddr[i] = 0xAE14; regValue[i] = 0x2240; i++;
    regAddr[i] = 0xAE16; regValue[i] = 0x1681; i++;
    regAddr[i] = 0xAE18; regValue[i] = 0xFFA6; i++;
    regAddr[i] = 0xAE1A; regValue[i] = 0xD800; i++;
    regAddr[i] = 0xAE1C; regValue[i] = 0xC0D1; i++;
    regAddr[i] = 0xAE1E; regValue[i] = 0x7EE0; i++;

    regAddr[i] = 0xAE20; regValue[i] = 0x71CF; i++;
    regAddr[i] = 0xAE22; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xAE24; regValue[i] = 0x941C; i++;
    regAddr[i] = 0xAE26; regValue[i] = 0x9117; i++;
    regAddr[i] = 0xAE28; regValue[i] = 0x72CF; i++;
    regAddr[i] = 0xAE2A; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xAE2C; regValue[i] = 0x803C; i++;
    regAddr[i] = 0xAE2E; regValue[i] = 0xA200; i++;

    regAddr[i] = 0xAE30; regValue[i] = 0xD807; i++;
    regAddr[i] = 0xAE32; regValue[i] = 0xB110; i++;
    regAddr[i] = 0xAE34; regValue[i] = 0x7FE0; i++;
    regAddr[i] = 0xAE36; regValue[i] = 0xD800; i++;
    regAddr[i] = 0xAE38; regValue[i] = 0x080D; i++;
    regAddr[i] = 0xAE3A; regValue[i] = 0x0050; i++;
    regAddr[i] = 0xAE3C; regValue[i] = 0x080B; i++;
    regAddr[i] = 0xAE3E; regValue[i] = 0x00D0; i++;

    regAddr[i] = 0xAE40; regValue[i] = 0xD800; i++;
    regAddr[i] = 0xAE42; regValue[i] = 0x7EE0; i++;
    regAddr[i] = 0xAE44; regValue[i] = 0xF1EE; i++;
    regAddr[i] = 0xAE46; regValue[i] = 0xF1CC; i++;
    regAddr[i] = 0xAE48; regValue[i] = 0x8060; i++;
    regAddr[i] = 0xAE4A; regValue[i] = 0x214E; i++;
    regAddr[i] = 0xAE4C; regValue[i] = 0x080A; i++;
    regAddr[i] = 0xAE4E; regValue[i] = 0xBA05; i++;

    regAddr[i] = 0xAE50; regValue[i] = 0x2B04; i++;
    regAddr[i] = 0xAE52; regValue[i] = 0x02BE; i++;
    regAddr[i] = 0xAE54; regValue[i] = 0x732F; i++;
    regAddr[i] = 0xAE56; regValue[i] = 0x7A2C; i++;
    regAddr[i] = 0xAE58; regValue[i] = 0x7327; i++;
    regAddr[i] = 0xAE5A; regValue[i] = 0xE31F; i++;
    regAddr[i] = 0xAE5C; regValue[i] = 0x2B41; i++;
    regAddr[i] = 0xAE5E; regValue[i] = 0x0141; i++;

    regAddr[i] = 0xAE60; regValue[i] = 0xA020; i++;
    regAddr[i] = 0xAE62; regValue[i] = 0x2B41; i++;
    regAddr[i] = 0xAE64; regValue[i] = 0x0280; i++;
    regAddr[i] = 0xAE66; regValue[i] = 0x7FE0; i++;
    regAddr[i] = 0xAE68; regValue[i] = 0x7810; i++;
    regAddr[i] = 0xAE6A; regValue[i] = 0x78E0; i++;
    regAddr[i] = 0xAE6C; regValue[i] = 0xC0F1; i++;
    regAddr[i] = 0xAE6E; regValue[i] = 0x0EEA; i++;

    regAddr[i] = 0xAE70; regValue[i] = 0x0D00; i++;
    regAddr[i] = 0xAE72; regValue[i] = 0x77CF; i++;
    regAddr[i] = 0xAE74; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xAE76; regValue[i] = 0x803C; i++;
    regAddr[i] = 0xAE78; regValue[i] = 0x8F04; i++;
    regAddr[i] = 0xAE7A; regValue[i] = 0x75CF; i++;
    regAddr[i] = 0xAE7C; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xAE7E; regValue[i] = 0x92B4; i++;

    regAddr[i] = 0xAE80; regValue[i] = 0xC1A1; i++;
    regAddr[i] = 0xAE82; regValue[i] = 0x15E5; i++;
    regAddr[i] = 0xAE84; regValue[i] = 0x174E; i++;
    regAddr[i] = 0xAE86; regValue[i] = 0x080F; i++;
    regAddr[i] = 0xAE88; regValue[i] = 0x0311; i++;
    regAddr[i] = 0xAE8A; regValue[i] = 0x15CB; i++;
    regAddr[i] = 0xAE8C; regValue[i] = 0x1702; i++;
    regAddr[i] = 0xAE8E; regValue[i] = 0x7148; i++;

    regAddr[i] = 0xAE90; regValue[i] = 0xF028; i++;
    regAddr[i] = 0xAE92; regValue[i] = 0x70CF; i++;
    regAddr[i] = 0xAE94; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xAE96; regValue[i] = 0x972C; i++;
    regAddr[i] = 0xAE98; regValue[i] = 0x9004; i++;
    regAddr[i] = 0xAE9A; regValue[i] = 0xD908; i++;
    regAddr[i] = 0xAE9C; regValue[i] = 0x0A32; i++;
    regAddr[i] = 0xAE9E; regValue[i] = 0x06A0; i++;

    regAddr[i] = 0xAEA0; regValue[i] = 0x7228; i++;
    regAddr[i] = 0xAEA2; regValue[i] = 0x7108; i++;
    regAddr[i] = 0xAEA4; regValue[i] = 0x72CF; i++;
    regAddr[i] = 0xAEA6; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xAEA8; regValue[i] = 0x9978; i++;
    regAddr[i] = 0xAEAA; regValue[i] = 0x8204; i++;
    regAddr[i] = 0xAEAC; regValue[i] = 0x8243; i++;
    regAddr[i] = 0xAEAE; regValue[i] = 0x7842; i++;

    regAddr[i] = 0xAEB0; regValue[i] = 0x0EDA; i++;
    regAddr[i] = 0xAEB2; regValue[i] = 0x0D20; i++;
    regAddr[i] = 0xAEB4; regValue[i] = 0xB80C; i++;
    regAddr[i] = 0xAEB6; regValue[i] = 0xD904; i++;
    regAddr[i] = 0xAEB8; regValue[i] = 0x0B2A; i++;
    regAddr[i] = 0xAEBA; regValue[i] = 0x06A0; i++;
    regAddr[i] = 0xAEBC; regValue[i] = 0xDA00; i++;
    regAddr[i] = 0xAEBE; regValue[i] = 0x15CA; i++;

    regAddr[i] = 0xAEC0; regValue[i] = 0x1701; i++;
    regAddr[i] = 0xAEC2; regValue[i] = 0x15CB; i++;
    regAddr[i] = 0xAEC4; regValue[i] = 0x1702; i++;
    regAddr[i] = 0xAEC6; regValue[i] = 0x2905; i++;
    regAddr[i] = 0xAEC8; regValue[i] = 0x003E; i++;
    regAddr[i] = 0xAECA; regValue[i] = 0x210C; i++;
    regAddr[i] = 0xAECC; regValue[i] = 0xF080; i++;
    regAddr[i] = 0xAECE; regValue[i] = 0x21CA; i++;

    regAddr[i] = 0xAED0; regValue[i] = 0x008E; i++;
    regAddr[i] = 0xAED2; regValue[i] = 0x15CC; i++;
    regAddr[i] = 0xAED4; regValue[i] = 0x1700; i++;
    regAddr[i] = 0xAED6; regValue[i] = 0x21CA; i++;
    regAddr[i] = 0xAED8; regValue[i] = 0x0E4D; i++;
    regAddr[i] = 0xAEDA; regValue[i] = 0x7030; i++;
    regAddr[i] = 0xAEDC; regValue[i] = 0x21CA; i++;
    regAddr[i] = 0xAEDE; regValue[i] = 0x0005; i++;

    regAddr[i] = 0xAEE0; regValue[i] = 0x70CF; i++;
    regAddr[i] = 0xAEE2; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xAEE4; regValue[i] = 0xB044; i++;
    regAddr[i] = 0xAEE6; regValue[i] = 0x986A; i++;
    regAddr[i] = 0xAEE8; regValue[i] = 0x0E0B; i++;
    regAddr[i] = 0xAEEA; regValue[i] = 0x10C3; i++;
    regAddr[i] = 0xAEEC; regValue[i] = 0x15CC; i++;
    regAddr[i] = 0xAEEE; regValue[i] = 0x1702; i++;

    regAddr[i] = 0xAEF0; regValue[i] = 0xF01C; i++;
    regAddr[i] = 0xAEF2; regValue[i] = 0x15CE; i++;
    regAddr[i] = 0xAEF4; regValue[i] = 0x1743; i++;
    regAddr[i] = 0xAEF6; regValue[i] = 0x0B35; i++;
    regAddr[i] = 0xAEF8; regValue[i] = 0x0382; i++;
    regAddr[i] = 0xAEFA; regValue[i] = 0x1014; i++;
    regAddr[i] = 0xAEFC; regValue[i] = 0x014A; i++;
    regAddr[i] = 0xAEFE; regValue[i] = 0x0E1D; i++;

    regAddr[i] = 0xAF00; regValue[i] = 0x12A2; i++;
    regAddr[i] = 0xAF02; regValue[i] = 0x7930; i++;
    regAddr[i] = 0xAF04; regValue[i] = 0x1016; i++;
    regAddr[i] = 0xAF06; regValue[i] = 0x014A; i++;
    regAddr[i] = 0xAF08; regValue[i] = 0x0A11; i++;
    regAddr[i] = 0xAF0A; regValue[i] = 0x1382; i++;
    regAddr[i] = 0xAF0C; regValue[i] = 0x984A; i++;
    regAddr[i] = 0xAF0E; regValue[i] = 0x986B; i++;

    regAddr[i] = 0xAF10; regValue[i] = 0xC640; i++;
    regAddr[i] = 0xAF12; regValue[i] = 0x15CC; i++;
    regAddr[i] = 0xAF14; regValue[i] = 0x1700; i++;
    regAddr[i] = 0xAF16; regValue[i] = 0xF007; i++;
    regAddr[i] = 0xAF18; regValue[i] = 0x7028; i++;
    regAddr[i] = 0xAF1A; regValue[i] = 0x7148; i++;
    regAddr[i] = 0xAF1C; regValue[i] = 0xC640; i++;
    regAddr[i] = 0xAF1E; regValue[i] = 0x15CD; i++;

    regAddr[i] = 0xAF20; regValue[i] = 0x1742; i++;
    regAddr[i] = 0xAF22; regValue[i] = 0x08F2; i++;
    regAddr[i] = 0xAF24; regValue[i] = 0x0680; i++;
    regAddr[i] = 0xAF26; regValue[i] = 0x7208; i++;
    regAddr[i] = 0xAF28; regValue[i] = 0x7A50; i++;
    regAddr[i] = 0xAF2A; regValue[i] = 0x70E9; i++;
    regAddr[i] = 0xAF2C; regValue[i] = 0x15CF; i++;
    regAddr[i] = 0xAF2E; regValue[i] = 0x1701; i++;

    regAddr[i] = 0xAF30; regValue[i] = 0xFFC6; i++;
    regAddr[i] = 0xAF32; regValue[i] = 0x15C5; i++;
    regAddr[i] = 0xAF34; regValue[i] = 0x1702; i++;
    regAddr[i] = 0xAF36; regValue[i] = 0x2A05; i++;
    regAddr[i] = 0xAF38; regValue[i] = 0x003E; i++;
    regAddr[i] = 0xAF3A; regValue[i] = 0x7048; i++;
    regAddr[i] = 0xAF3C; regValue[i] = 0x15C8; i++;
    regAddr[i] = 0xAF3E; regValue[i] = 0x1702; i++;

    regAddr[i] = 0xAF40; regValue[i] = 0x712F; i++;
    regAddr[i] = 0xAF42; regValue[i] = 0x1D63; i++;
    regAddr[i] = 0xAF44; regValue[i] = 0x1E58; i++;
    regAddr[i] = 0xAF46; regValue[i] = 0x0E96; i++;
    regAddr[i] = 0xAF48; regValue[i] = 0x0AA0; i++;
    regAddr[i] = 0xAF4A; regValue[i] = 0x15C9; i++;
    regAddr[i] = 0xAF4C; regValue[i] = 0x1703; i++;
    regAddr[i] = 0xAF4E; regValue[i] = 0x0619; i++;

    regAddr[i] = 0xAF50; regValue[i] = 0x0D20; i++;
    regAddr[i] = 0xAF52; regValue[i] = 0xC0A1; i++;
    regAddr[i] = 0xAF54; regValue[i] = 0xC0F1; i++;
    regAddr[i] = 0xAF56; regValue[i] = 0x0E06; i++;
    regAddr[i] = 0xAF58; regValue[i] = 0x0D00; i++;
    regAddr[i] = 0xAF5A; regValue[i] = 0x76CF; i++;
    regAddr[i] = 0xAF5C; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xAF5E; regValue[i] = 0x9458; i++;

    regAddr[i] = 0xAF60; regValue[i] = 0x8E29; i++;
    regAddr[i] = 0xAF62; regValue[i] = 0x75CF; i++;
    regAddr[i] = 0xAF64; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xAF66; regValue[i] = 0x941C; i++;
    regAddr[i] = 0xAF68; regValue[i] = 0x9510; i++;
    regAddr[i] = 0xAF6A; regValue[i] = 0x091B; i++;
    regAddr[i] = 0xAF6C; regValue[i] = 0x009F; i++;
    regAddr[i] = 0xAF6E; regValue[i] = 0xB8E0; i++;

    regAddr[i] = 0xAF70; regValue[i] = 0x0EFC; i++;
    regAddr[i] = 0xAF72; regValue[i] = 0xFFC2; i++;
    regAddr[i] = 0xAF74; regValue[i] = 0x9510; i++;
    regAddr[i] = 0xAF76; regValue[i] = 0xB8E2; i++;
    regAddr[i] = 0xAF78; regValue[i] = 0x0D48; i++;
    regAddr[i] = 0xAF7A; regValue[i] = 0xFFC2; i++;
    regAddr[i] = 0xAF7C; regValue[i] = 0x8E09; i++;
    regAddr[i] = 0xAF7E; regValue[i] = 0xB882; i++;

    regAddr[i] = 0xAF80; regValue[i] = 0xAE09; i++;
    regAddr[i] = 0xAF82; regValue[i] = 0x9510; i++;
    regAddr[i] = 0xAF84; regValue[i] = 0xB8E1; i++;
    regAddr[i] = 0xAF86; regValue[i] = 0x09F0; i++;
    regAddr[i] = 0xAF88; regValue[i] = 0x08C2; i++;
    regAddr[i] = 0xAF8A; regValue[i] = 0x05E5; i++;
    regAddr[i] = 0xAF8C; regValue[i] = 0x0D00; i++;
    regAddr[i] = 0xAF8E; regValue[i] = 0x78E0; i++;

    regAddr[i] = 0xAF90; regValue[i] = 0xD803; i++;
    regAddr[i] = 0xAF92; regValue[i] = 0x220A; i++;
    regAddr[i] = 0xAF94; regValue[i] = 0x1F80; i++;
    regAddr[i] = 0xAF96; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xAF98; regValue[i] = 0xB044; i++;
    regAddr[i] = 0xAF9A; regValue[i] = 0x1A00; i++;
    regAddr[i] = 0xAF9C; regValue[i] = 0x1004; i++;
    regAddr[i] = 0xAF9E; regValue[i] = 0xD864; i++;

    regAddr[i] = 0xAFA0; regValue[i] = 0x1A02; i++;
    regAddr[i] = 0xAFA2; regValue[i] = 0x1004; i++;
    regAddr[i] = 0xAFA4; regValue[i] = 0xD932; i++;
    regAddr[i] = 0xAFA6; regValue[i] = 0x1A04; i++;
    regAddr[i] = 0xAFA8; regValue[i] = 0x1044; i++;
    regAddr[i] = 0xAFAA; regValue[i] = 0xDA00; i++;
    regAddr[i] = 0xAFAC; regValue[i] = 0x1A06; i++;
    regAddr[i] = 0xAFAE; regValue[i] = 0x1084; i++;

    regAddr[i] = 0xAFB0; regValue[i] = 0x1A08; i++;
    regAddr[i] = 0xAFB2; regValue[i] = 0x1084; i++;
    regAddr[i] = 0xAFB4; regValue[i] = 0xDB80; i++;
    regAddr[i] = 0xAFB6; regValue[i] = 0x1A14; i++;
    regAddr[i] = 0xAFB8; regValue[i] = 0x10C4; i++;
    regAddr[i] = 0xAFBA; regValue[i] = 0x1A16; i++;
    regAddr[i] = 0xAFBC; regValue[i] = 0x10C4; i++;
    regAddr[i] = 0xAFBE; regValue[i] = 0x228A; i++;

    regAddr[i] = 0xAFC0; regValue[i] = 0x1F8F; i++;
    regAddr[i] = 0xAFC2; regValue[i] = 0x71CF; i++;
    regAddr[i] = 0xAFC4; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xAFC6; regValue[i] = 0x978C; i++;
    regAddr[i] = 0xAFC8; regValue[i] = 0x1910; i++;
    regAddr[i] = 0xAFCA; regValue[i] = 0x0284; i++;
    regAddr[i] = 0xAFCC; regValue[i] = 0x71CF; i++;
    regAddr[i] = 0xAFCE; regValue[i] = 0xFFFF; i++;

    regAddr[i] = 0xAFD0; regValue[i] = 0x950C; i++;
    regAddr[i] = 0xAFD2; regValue[i] = 0xB141; i++;
    regAddr[i] = 0xAFD4; regValue[i] = 0xB147; i++;
    regAddr[i] = 0xAFD6; regValue[i] = 0x71CF; i++;
    regAddr[i] = 0xAFD8; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xAFDA; regValue[i] = 0x9490; i++;
    regAddr[i] = 0xAFDC; regValue[i] = 0xA968; i++;
    regAddr[i] = 0xAFDE; regValue[i] = 0xA969; i++;

    regAddr[i] = 0xAFE0; regValue[i] = 0xD902; i++;
    regAddr[i] = 0xAFE2; regValue[i] = 0x73CF; i++;
    regAddr[i] = 0xAFE4; regValue[i] = 0xFF00; i++;
    regAddr[i] = 0xAFE6; regValue[i] = 0x2418; i++;
    regAddr[i] = 0xAFE8; regValue[i] = 0xB323; i++;
    regAddr[i] = 0xAFEA; regValue[i] = 0xD910; i++;
    regAddr[i] = 0xAFEC; regValue[i] = 0xB336; i++;
    regAddr[i] = 0xAFEE; regValue[i] = 0xB335; i++;

    regAddr[i] = 0xAFF0; regValue[i] = 0x218A; i++;
    regAddr[i] = 0xAFF2; regValue[i] = 0x0004; i++;
    regAddr[i] = 0xAFF4; regValue[i] = 0xB338; i++;
    regAddr[i] = 0xAFF6; regValue[i] = 0xD95A; i++;
    regAddr[i] = 0xAFF8; regValue[i] = 0xB320; i++;
    regAddr[i] = 0xAFFA; regValue[i] = 0xB301; i++;
    regAddr[i] = 0xAFFC; regValue[i] = 0xD878; i++;
    regAddr[i] = 0xAFFE; regValue[i] = 0xB302; i++;

    regAddr[i] = 0xB000; regValue[i] = 0x70CF; i++;
    regAddr[i] = 0xB002; regValue[i] = 0x0000; i++;
    regAddr[i] = 0xB004; regValue[i] = 0x332C; i++;
    regAddr[i] = 0xB006; regValue[i] = 0x71CF; i++;
    regAddr[i] = 0xB008; regValue[i] = 0x0000; i++;
    regAddr[i] = 0xB00A; regValue[i] = 0xC7FF; i++;
    regAddr[i] = 0xB00C; regValue[i] = 0x0535; i++;
    regAddr[i] = 0xB00E; regValue[i] = 0x0AC0; i++;

    regAddr[i] = 0xB010; regValue[i] = 0xC0F1; i++;
    regAddr[i] = 0xB012; regValue[i] = 0xD900; i++;
    regAddr[i] = 0xB014; regValue[i] = 0xF009; i++;
    regAddr[i] = 0xB016; regValue[i] = 0x70CF; i++;
    regAddr[i] = 0xB018; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xB01A; regValue[i] = 0xB070; i++;
    regAddr[i] = 0xB01C; regValue[i] = 0x7835; i++;
    regAddr[i] = 0xB01E; regValue[i] = 0x8041; i++;

    regAddr[i] = 0xB020; regValue[i] = 0x8000; i++;
    regAddr[i] = 0xB022; regValue[i] = 0xE102; i++;
    regAddr[i] = 0xB024; regValue[i] = 0xA040; i++;
    regAddr[i] = 0xB026; regValue[i] = 0x09F3; i++;
    regAddr[i] = 0xB028; regValue[i] = 0x8114; i++;
    regAddr[i] = 0xB02A; regValue[i] = 0x71CF; i++;
    regAddr[i] = 0xB02C; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xB02E; regValue[i] = 0xB044; i++;

    regAddr[i] = 0xB030; regValue[i] = 0x70CF; i++;
    regAddr[i] = 0xB032; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xB034; regValue[i] = 0x9274; i++;
    regAddr[i] = 0xB036; regValue[i] = 0xB031; i++;
    regAddr[i] = 0xB038; regValue[i] = 0xFF7A; i++;
    regAddr[i] = 0xB03A; regValue[i] = 0xFFD6; i++;
    regAddr[i] = 0xB03C; regValue[i] = 0x05E1; i++;
    regAddr[i] = 0xB03E; regValue[i] = 0xFFEF; i++;

    regAddr[i] = 0xB040; regValue[i] = 0xD800; i++;
    regAddr[i] = 0xB042; regValue[i] = 0x0000; i++;
    regAddr[i] = 0xB044; regValue[i] = 0x000F; i++;
    regAddr[i] = 0xB046; regValue[i] = 0x01F4; i++;
    regAddr[i] = 0xB048; regValue[i] = 0x00C8; i++;
    regAddr[i] = 0xB04A; regValue[i] = 0x00C8; i++;
    regAddr[i] = 0xB04C; regValue[i] = 0x0064; i++;
    regAddr[i] = 0xB04E; regValue[i] = 0x0000; i++;

    regAddr[i] = 0xB050; regValue[i] = 0x0000; i++;
    regAddr[i] = 0xB052; regValue[i] = 0x003F; i++;
    regAddr[i] = 0xB054; regValue[i] = 0x000F; i++;
    regAddr[i] = 0xB056; regValue[i] = 0x0007; i++;
    regAddr[i] = 0xB058; regValue[i] = 0xFE80; i++;
    regAddr[i] = 0xB05A; regValue[i] = 0x0033; i++;
    regAddr[i] = 0xB05C; regValue[i] = 0x0000; i++;
    regAddr[i] = 0xB05E; regValue[i] = 0x0000; i++;

    regAddr[i] = 0xB060; regValue[i] = 0x0000; i++;
    regAddr[i] = 0xB062; regValue[i] = 0x0000; i++;
    regAddr[i] = 0xB064; regValue[i] = 0x0000; i++;
    regAddr[i] = 0xB066; regValue[i] = 0x0000; i++;
    regAddr[i] = 0xB068; regValue[i] = 0x0000; i++;
    regAddr[i] = 0xB06A; regValue[i] = 0x0000; i++;
    regAddr[i] = 0xB06C; regValue[i] = 0x0000; i++;
    regAddr[i] = 0xB06E; regValue[i] = 0x0000; i++;

    regAddr[i] = 0xB070; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xB072; regValue[i] = 0xA12C; i++;
    regAddr[i] = 0xB074; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xB076; regValue[i] = 0xAE38; i++;
    regAddr[i] = 0xB078; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xB07A; regValue[i] = 0xA128; i++;
    regAddr[i] = 0xB07C; regValue[i] = 0xFFFF; i++;
    regAddr[i] = 0xB07E; regValue[i] = 0xAF54; i++;

    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }    
    OSA_waitMsecs(5);

    //apply patch
    i = 0;
    regAddr[i] = 0x098E; regValue[i] = 0x0000; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC00; regValue[i] = 0x0C78; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC02; regValue[i] = 0x0503; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC04; regValue[i] = 0x6003; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);    
    i = 0;
    regAddr[i] = 0xFC06; regValue[i] = 0x0104; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC08; regValue[i] = 0x0418; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8702; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX apply patch 3 return value = %d\n", regValue[0]);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    OSA_waitMsecs(5);

    // Sensor Reset
    i = 0;
    regAddr[i] = 0x001A; regValue[i] = 0x0001; i++;    //Reset
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0x001A; regValue[i] = 0x0000; i++;   //Reset done
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    //Issue SYSMGR_GET_STATE repeatedly until it does not return EBUSY
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8101; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX Issue SYSMGR_GET_STATE = %d\n", regValue[0]);
        if(regValue[0] == 0)
        {
            break;
        }
    }
    //Issue SENSOR_MGR_DISCOVER_SENSOR host command and poll until it returns ENOERR
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8E00; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX Issue SENSOR_MGR_DISCOVER_SENSOR = %d\n", regValue[0]);
        if(regValue[0] == 0)
        {
            break;
        }
    }
    #endif

    #if 0
    i = 0;
    regAddr[i] = 0xCC02; regValue[i] = 0x0013; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D00; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX Request the CCI Manager access lock return value = %d\n", regValue[0]);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    OSA_waitMsecs(5);

    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D01; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX lock status return value = %d\n", regValue[0]);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    OSA_waitMsecs(5);

    i = 0;
    regAddr[i] = 0xFC00; regValue[i] = 0x3088; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC02; regValue[i] = 0x0280; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC04; regValue[i] = 0x0000; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D06; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer 1 return value = %d\n", regValue[0]);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D08; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer 2 return value = %d\n", regValue[0]);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    OSA_waitMsecs(5);


    i = 0;
    regAddr[i] = 0xFC00; regValue[i] = 0x3086; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC02; regValue[i] = 0x0C00; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC04; regValue[i] = 0x2550; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC06; regValue[i] = 0x502D; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC08; regValue[i] = 0x2608; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0A; regValue[i] = 0x280D; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0C; regValue[i] = 0x1709; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0E; regValue[i] = 0x2600; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D06; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer 3 return value = %d\n", regValue[0]);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D08; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer 4 return value = %d\n", regValue[0]);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    OSA_waitMsecs(5);


    i = 0;
    regAddr[i] = 0xFC00; regValue[i] = 0x3086; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC02; regValue[i] = 0x0C00; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC04; regValue[i] = 0x2805; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC06; regValue[i] = 0x26A7; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC08; regValue[i] = 0x2807; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0A; regValue[i] = 0x2580; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0C; regValue[i] = 0x8029; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0E; regValue[i] = 0x2500; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D06; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer 5 return value = %d\n", regValue[0]);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D08; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer 6 return value = %d\n", regValue[0]);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    OSA_waitMsecs(5);

    i = 0;
    regAddr[i] = 0xFC00; regValue[i] = 0x3086; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC02; regValue[i] = 0x0C00; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC04; regValue[i] = 0x4027; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC06; regValue[i] = 0x0216; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC08; regValue[i] = 0x1627; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0A; regValue[i] = 0x061F; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0C; regValue[i] = 0x1736; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0E; regValue[i] = 0x2600; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D06; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer 7 return value = %d\n", regValue[0]);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D08; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer 8 return value = %d\n", regValue[0]);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    OSA_waitMsecs(5);

    i = 0;
    regAddr[i] = 0xFC00; regValue[i] = 0x3086; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC02; regValue[i] = 0x0CA6; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC04; regValue[i] = 0x1703; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC06; regValue[i] = 0x26A4; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC08; regValue[i] = 0x171F; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0A; regValue[i] = 0x2805; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0C; regValue[i] = 0x2620; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0E; regValue[i] = 0x2800; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D06; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer 9 return value = %d\n", regValue[0]);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D08; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer 10 return value = %d\n", regValue[0]);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    OSA_waitMsecs(5);

    i = 0;
    regAddr[i] = 0xFC00; regValue[i] = 0x3086; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC02; regValue[i] = 0x0C04; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC04; regValue[i] = 0x2520; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC06; regValue[i] = 0x2027; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC08; regValue[i] = 0x0017; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0A; regValue[i] = 0x1D25; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0C; regValue[i] = 0x0020; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0E; regValue[i] = 0x1700; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D06; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer 11 return value = %d\n", regValue[0]);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D08; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer 12 return value = %d\n", regValue[0]);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    OSA_waitMsecs(5);


    
    i = 0;
    regAddr[i] = 0xFC00; regValue[i] = 0x3086; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC02; regValue[i] = 0x0C12; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC04; regValue[i] = 0x1917; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC06; regValue[i] = 0x0327; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC08; regValue[i] = 0x0617; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0A; regValue[i] = 0x2828; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0C; regValue[i] = 0x0517; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0E; regValue[i] = 0x1A00; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D06; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer 13 return value = %d\n", regValue[0]);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D08; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer 14 return value = %d\n", regValue[0]);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    OSA_waitMsecs(5);

    i = 0;
    regAddr[i] = 0xFC00; regValue[i] = 0x3086; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC02; regValue[i] = 0x0C26; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC04; regValue[i] = 0x6017; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC06; regValue[i] = 0x5A23; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC08; regValue[i] = 0x1711; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0A; regValue[i] = 0x2217; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0C; regValue[i] = 0x4125; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0E; regValue[i] = 0x0000; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D06; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer 15 return value = %d\n", regValue[0]);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D08; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer 16 return value = %d\n", regValue[0]);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    OSA_waitMsecs(5);

    i = 0;
    regAddr[i] = 0xFC00; regValue[i] = 0x3086; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC02; regValue[i] = 0x0C90; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC04; regValue[i] = 0x2700; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC06; regValue[i] = 0x2618; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC08; regValue[i] = 0x2800; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0A; regValue[i] = 0x2E2A; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0C; regValue[i] = 0x2808; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0E; regValue[i] = 0x1C00; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D06; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer 17 return value = %d\n", regValue[0]);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D08; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer 18 return value = %d\n", regValue[0]);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    OSA_waitMsecs(5);


    i = 0;
    regAddr[i] = 0xFC00; regValue[i] = 0x3086; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC02; regValue[i] = 0x0C14; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC04; regValue[i] = 0x7070; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC06; regValue[i] = 0x0314; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC08; regValue[i] = 0x7070; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0A; regValue[i] = 0x0414; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0C; regValue[i] = 0x7070; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0E; regValue[i] = 0x0500; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D06; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer 19 return value = %d\n", regValue[0]);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D08; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer 20 return value = %d\n", regValue[0]);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    OSA_waitMsecs(5);


    i = 0;
    regAddr[i] = 0xFC00; regValue[i] = 0x3086; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC02; regValue[i] = 0x0C14; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC04; regValue[i] = 0x7070; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC06; regValue[i] = 0x0917; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC08; regValue[i] = 0x0C00; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0A; regValue[i] = 0x1400; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0C; regValue[i] = 0x2000; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0E; regValue[i] = 0x1400; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D06; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer 21 return value = %d\n", regValue[0]);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D08; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer 22 return value = %d\n", regValue[0]);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    OSA_waitMsecs(5);


    i = 0;
    regAddr[i] = 0xFC00; regValue[i] = 0x3086; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC02; regValue[i] = 0x0C00; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC04; regValue[i] = 0x5003; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC06; regValue[i] = 0x1400; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC08; regValue[i] = 0x2003; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0A; regValue[i] = 0x1400; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0C; regValue[i] = 0x5004; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0E; regValue[i] = 0x1400; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D06; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer 23 return value = %d\n", regValue[0]);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D08; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer 24 return value = %d\n", regValue[0]);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    OSA_waitMsecs(5);

    i = 0;
    regAddr[i] = 0xFC00; regValue[i] = 0x3086; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC02; regValue[i] = 0x0C00; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC04; regValue[i] = 0x2004; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC06; regValue[i] = 0x1400; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC08; regValue[i] = 0x5005; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0A; regValue[i] = 0x1400; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0C; regValue[i] = 0x2024; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0E; regValue[i] = 0x0500; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D06; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer 25 return value = %d\n", regValue[0]);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D08; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer 26 return value = %d\n", regValue[0]);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    OSA_waitMsecs(5);


    i = 0;
    regAddr[i] = 0xFC00; regValue[i] = 0x3086; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC02; regValue[i] = 0x0C14; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC04; regValue[i] = 0x0050; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC06; regValue[i] = 0x0125; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC08; regValue[i] = 0x5050; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0A; regValue[i] = 0x2D26; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0C; regValue[i] = 0x0828; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0E; regValue[i] = 0x0D00; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D06; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer 27 return value = %d\n", regValue[0]);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D08; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer 28 return value = %d\n", regValue[0]);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    OSA_waitMsecs(5);

    i = 0;
    regAddr[i] = 0xFC00; regValue[i] = 0x3086; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC02; regValue[i] = 0x0C17; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC04; regValue[i] = 0x0926; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC06; regValue[i] = 0x0028; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC08; regValue[i] = 0x0526; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0A; regValue[i] = 0xA728; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0C; regValue[i] = 0x0725; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0E; regValue[i] = 0x8000; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D06; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D08; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    OSA_waitMsecs(5);


    i = 0;
    regAddr[i] = 0xFC00; regValue[i] = 0x3086; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC02; regValue[i] = 0x0C80; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC04; regValue[i] = 0x2925; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC06; regValue[i] = 0x0040; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC08; regValue[i] = 0x2702; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0A; regValue[i] = 0x1616; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0C; regValue[i] = 0x2706; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0E; regValue[i] = 0x2000; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D06; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D08; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC00; regValue[i] = 0x3086; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC02; regValue[i] = 0x0C17; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC04; regValue[i] = 0x3626; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC06; regValue[i] = 0xA617; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC08; regValue[i] = 0x0326; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0A; regValue[i] = 0xA417; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0C; regValue[i] = 0x1F28; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0E; regValue[i] = 0x0500; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D06; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D08; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC00; regValue[i] = 0x3086; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC02; regValue[i] = 0x0C26; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC04; regValue[i] = 0x2028; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC06; regValue[i] = 0x0425; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC08; regValue[i] = 0x2020; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0A; regValue[i] = 0x2700; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0C; regValue[i] = 0x171D; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0E; regValue[i] = 0x2500; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D06; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D08; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC00; regValue[i] = 0x3086; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC02; regValue[i] = 0x0C00; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC04; regValue[i] = 0x2017; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC06; regValue[i] = 0x121A; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC08; regValue[i] = 0x1703; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0A; regValue[i] = 0x2706; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0C; regValue[i] = 0x1728; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0E; regValue[i] = 0x2800; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D06; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D08; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC00; regValue[i] = 0x3086; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC02; regValue[i] = 0x0C05; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC04; regValue[i] = 0x171A; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC06; regValue[i] = 0x2660; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC08; regValue[i] = 0x17AE; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0A; regValue[i] = 0x2500; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0C; regValue[i] = 0x9027; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0E; regValue[i] = 0x0000; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D06; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D08; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC00; regValue[i] = 0x3086; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC02; regValue[i] = 0x0C26; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC04; regValue[i] = 0x1828; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC06; regValue[i] = 0x002E; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC08; regValue[i] = 0x2A28; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0A; regValue[i] = 0x081D; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0C; regValue[i] = 0x0514; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0E; regValue[i] = 0x7000; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D06; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D08; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC00; regValue[i] = 0x3086; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC02; regValue[i] = 0x0C70; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC04; regValue[i] = 0x0917; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC06; regValue[i] = 0x2014; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC08; regValue[i] = 0x0020; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0A; regValue[i] = 0x2414; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0C; regValue[i] = 0x0050; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0E; regValue[i] = 0x0200; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D06; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D08; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    OSA_waitMsecs(5);


    i = 0;
    regAddr[i] = 0xFC00; regValue[i] = 0x3086; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC02; regValue[i] = 0x0C25; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC04; regValue[i] = 0x5050; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC06; regValue[i] = 0x2D26; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC08; regValue[i] = 0x0828; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0A; regValue[i] = 0x0D17; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0C; regValue[i] = 0x0926; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0E; regValue[i] = 0x0000; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D06; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D08; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    OSA_waitMsecs(5);

    i = 0;
    regAddr[i] = 0xFC00; regValue[i] = 0x3086; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC02; regValue[i] = 0x0C28; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC04; regValue[i] = 0x0526; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC06; regValue[i] = 0xA728; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC08; regValue[i] = 0x0725; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0A; regValue[i] = 0x8080; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0C; regValue[i] = 0x2925; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0E; regValue[i] = 0x0000; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D06; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D08; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    OSA_waitMsecs(5);

    i = 0;
    regAddr[i] = 0xFC00; regValue[i] = 0x3086; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC02; regValue[i] = 0x0C40; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC04; regValue[i] = 0x2702; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC06; regValue[i] = 0x1616; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC08; regValue[i] = 0x2706; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0A; regValue[i] = 0x1736; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0C; regValue[i] = 0x26A6; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0E; regValue[i] = 0x1700; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D06; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D08; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    OSA_waitMsecs(5);

    i = 0;
    regAddr[i] = 0xFC00; regValue[i] = 0x3086; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC02; regValue[i] = 0x0C03; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC04; regValue[i] = 0x26A4; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC06; regValue[i] = 0x171F; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC08; regValue[i] = 0x2805; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0A; regValue[i] = 0x2620; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0C; regValue[i] = 0x2804; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0E; regValue[i] = 0x2500; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D06; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D08; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    OSA_waitMsecs(5);

    i = 0;
    regAddr[i] = 0xFC00; regValue[i] = 0x3086; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC02; regValue[i] = 0x0C20; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC04; regValue[i] = 0x2027; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC06; regValue[i] = 0x0017; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC08; regValue[i] = 0x1D25; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0A; regValue[i] = 0x0020; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0C; regValue[i] = 0x2117; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0E; regValue[i] = 0x1200; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D06; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D08; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    OSA_waitMsecs(5);

    i = 0;
    regAddr[i] = 0xFC00; regValue[i] = 0x3086; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC02; regValue[i] = 0x0C1B; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC04; regValue[i] = 0x1703; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC06; regValue[i] = 0x2706; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC08; regValue[i] = 0x1728; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0A; regValue[i] = 0x2805; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0C; regValue[i] = 0x171A; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0E; regValue[i] = 0x2600; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D06; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D08; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    OSA_waitMsecs(5);

    i = 0;
    regAddr[i] = 0xFC00; regValue[i] = 0x3086; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC02; regValue[i] = 0x0C60; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC04; regValue[i] = 0x17AE; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC06; regValue[i] = 0x2500; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC08; regValue[i] = 0x9027; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0A; regValue[i] = 0x0026; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0C; regValue[i] = 0x1828; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0E; regValue[i] = 0x0000; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D06; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D08; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    OSA_waitMsecs(5);

    i = 0;
    regAddr[i] = 0xFC00; regValue[i] = 0x3086; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC02; regValue[i] = 0x0C2E; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC04; regValue[i] = 0x2A28; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC06; regValue[i] = 0x081E; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC08; regValue[i] = 0x170A; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0A; regValue[i] = 0x0514; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0C; regValue[i] = 0x7070; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0E; regValue[i] = 0x0900; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D06; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D08; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    OSA_waitMsecs(5);

    i = 0;
    regAddr[i] = 0xFC00; regValue[i] = 0x3086; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC02; regValue[i] = 0x0C16; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC04; regValue[i] = 0x1616; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC06; regValue[i] = 0x1616; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC08; regValue[i] = 0x1616; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0A; regValue[i] = 0x1616; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0C; regValue[i] = 0x1616; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0E; regValue[i] = 0x1600; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D06; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D08; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    OSA_waitMsecs(5);

    i = 0;
    regAddr[i] = 0xFC00; regValue[i] = 0x3086; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC02; regValue[i] = 0x0C16; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC04; regValue[i] = 0x1616; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC06; regValue[i] = 0x1616; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC08; regValue[i] = 0x1616; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0A; regValue[i] = 0x1616; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0C; regValue[i] = 0x1616; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0E; regValue[i] = 0x1600; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D06; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D08; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    OSA_waitMsecs(5);

    i = 0;
    regAddr[i] = 0xFC00; regValue[i] = 0x3086; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC02; regValue[i] = 0x0C16; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC04; regValue[i] = 0x1616; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC06; regValue[i] = 0x1400; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC08; regValue[i] = 0x2024; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0A; regValue[i] = 0x1400; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0C; regValue[i] = 0x502B; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0E; regValue[i] = 0x2B00; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D06; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D08; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    OSA_waitMsecs(5);

    i = 0;
    regAddr[i] = 0xFC00; regValue[i] = 0x3086; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC02; regValue[i] = 0x0C2C; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC04; regValue[i] = 0x2C2C; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC06; regValue[i] = 0x2C2C; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC08; regValue[i] = 0x0002; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0A; regValue[i] = 0x2550; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0C; regValue[i] = 0x502D; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0E; regValue[i] = 0x2600; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D06; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D08; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    OSA_waitMsecs(5);

    i = 0;
    regAddr[i] = 0xFC00; regValue[i] = 0x3086; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC02; regValue[i] = 0x0C08; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC04; regValue[i] = 0x280D; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC06; regValue[i] = 0x1709; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC08; regValue[i] = 0x2600; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0A; regValue[i] = 0x2805; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0C; regValue[i] = 0x26A7; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0E; regValue[i] = 0x2800; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D06; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D08; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    OSA_waitMsecs(5);

    i = 0;
    regAddr[i] = 0xFC00; regValue[i] = 0x3086; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC02; regValue[i] = 0x0C07; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC04; regValue[i] = 0x2580; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC06; regValue[i] = 0x8029; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC08; regValue[i] = 0x1705; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0A; regValue[i] = 0x2500; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0C; regValue[i] = 0x4027; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0E; regValue[i] = 0x0200; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D06; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D08; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    OSA_waitMsecs(5);

    i = 0;
    regAddr[i] = 0xFC00; regValue[i] = 0x3086; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC02; regValue[i] = 0x0C16; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC04; regValue[i] = 0x1627; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC06; regValue[i] = 0x0617; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC08; regValue[i] = 0x3626; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0A; regValue[i] = 0xA617; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0C; regValue[i] = 0x0326; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0E; regValue[i] = 0xA400; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D06; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D08; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    OSA_waitMsecs(5);

    i = 0;
    regAddr[i] = 0xFC00; regValue[i] = 0x3086; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC02; regValue[i] = 0x0C17; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC04; regValue[i] = 0x1F28; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC06; regValue[i] = 0x0526; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC08; regValue[i] = 0x2028; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0A; regValue[i] = 0x0425; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0C; regValue[i] = 0x2020; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0E; regValue[i] = 0x2700; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D06; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D08; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    OSA_waitMsecs(5);

    i = 0;
    regAddr[i] = 0xFC00; regValue[i] = 0x3086; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC02; regValue[i] = 0x0C00; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC04; regValue[i] = 0x171E; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC06; regValue[i] = 0x2500; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC08; regValue[i] = 0x2021; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0A; regValue[i] = 0x1710; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0C; regValue[i] = 0x2805; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0E; regValue[i] = 0x1B00; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D06; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D08; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    OSA_waitMsecs(5);

    i = 0;
    regAddr[i] = 0xFC00; regValue[i] = 0x3086; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC02; regValue[i] = 0x0C17; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC04; regValue[i] = 0x0327; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC06; regValue[i] = 0x0617; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC08; regValue[i] = 0x0317; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0A; regValue[i] = 0x4726; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0C; regValue[i] = 0x6017; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0E; regValue[i] = 0xAE00; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D06; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D08; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    OSA_waitMsecs(5);

    i = 0;
    regAddr[i] = 0xFC00; regValue[i] = 0x3086; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC02; regValue[i] = 0x0C25; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC04; regValue[i] = 0x0090; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC06; regValue[i] = 0x2700; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC08; regValue[i] = 0x2618; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0A; regValue[i] = 0x2800; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0C; regValue[i] = 0x2E2A; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0E; regValue[i] = 0x2800; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D06; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D08; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    OSA_waitMsecs(5);

    i = 0;
    regAddr[i] = 0xFC00; regValue[i] = 0x3086; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC02; regValue[i] = 0x0C08; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC04; regValue[i] = 0x1E08; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC06; regValue[i] = 0x3114; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC08; regValue[i] = 0x4040; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0A; regValue[i] = 0x1420; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0C; regValue[i] = 0x2014; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0E; regValue[i] = 0x1000; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D06; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D08; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    OSA_waitMsecs(5);

    i = 0;
    regAddr[i] = 0xFC00; regValue[i] = 0x3086; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC02; regValue[i] = 0x0C10; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC04; regValue[i] = 0x3414; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC06; regValue[i] = 0x0010; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC08; regValue[i] = 0x1400; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0A; regValue[i] = 0x2014; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0C; regValue[i] = 0x0040; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0E; regValue[i] = 0x1300; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D06; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D08; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    OSA_waitMsecs(5);

    i = 0;
    regAddr[i] = 0xFC00; regValue[i] = 0x3086; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC02; regValue[i] = 0x0C18; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC04; regValue[i] = 0x0214; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC06; regValue[i] = 0x7070; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC08; regValue[i] = 0x0414; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0A; regValue[i] = 0x7070; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0C; regValue[i] = 0x0314; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0E; regValue[i] = 0x7000; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D06; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D08; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    OSA_waitMsecs(5);

    i = 0;
    regAddr[i] = 0xFC00; regValue[i] = 0x3086; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC02; regValue[i] = 0x0C70; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC04; regValue[i] = 0x1720; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC06; regValue[i] = 0x0214; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC08; regValue[i] = 0x0020; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0A; regValue[i] = 0x0214; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0C; regValue[i] = 0x0050; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0E; regValue[i] = 0x0400; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D06; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D08; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    OSA_waitMsecs(5);

    i = 0;
    regAddr[i] = 0xFC00; regValue[i] = 0x3086; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC02; regValue[i] = 0x0C14; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC04; regValue[i] = 0x0020; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC06; regValue[i] = 0x0414; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC08; regValue[i] = 0x0050; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0A; regValue[i] = 0x2203; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0C; regValue[i] = 0x1400; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0E; regValue[i] = 0x2000; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D06; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D08; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    OSA_waitMsecs(5);

    i = 0;
    regAddr[i] = 0xFC00; regValue[i] = 0x3086; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC02; regValue[i] = 0x0803; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC04; regValue[i] = 0x1400; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC06; regValue[i] = 0x502C; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC08; regValue[i] = 0x2C2C; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0A; regValue[i] = 0x2C00; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0C; regValue[i] = 0x1400; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    
    i = 0;
    regAddr[i] = 0xFC0E; regValue[i] = 0x2000; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D06; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D08; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    OSA_waitMsecs(5);

    i = 0;
    regAddr[i] = 0xFC00; regValue[i] = 0x309E; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC02; regValue[i] = 0x0201; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC04; regValue[i] = 0x8600; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D06; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D08; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX load sequencer %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    OSA_waitMsecs(5);
    
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D02; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX Release the lock %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    OSA_waitMsecs(5);

    #endif
    #if 0
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8E01; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX Initiate the Sensor %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    OSA_waitMsecs(5);
#endif
#if 1
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D00; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX Request the CCI Manager access lock %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    OSA_waitMsecs(5);


    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D01; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX lock status %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    OSA_waitMsecs(5);

#endif
#if 1
    // config the sensor
    i = 0;
    regAddr[i] = 0xFC00; regValue[i] = 0x3ED8; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC02; regValue[i] = 0x0209; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC04; regValue[i] = 0xEF00; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D06; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX config the sensor %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D08; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX config the sensor %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    OSA_waitMsecs(5);

    i = 0;
    regAddr[i] = 0xFC00; regValue[i] = 0x3EE0; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC02; regValue[i] = 0x0206; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC04; regValue[i] = 0x7D00; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D06; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX config the sensor %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D08; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX config the sensor %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    OSA_waitMsecs(5);


    i = 0;
    regAddr[i] = 0xFC00; regValue[i] = 0x3EE4; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC02; regValue[i] = 0x02D2; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xFC04; regValue[i] = 0x0800; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D06; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX config the sensor %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D08; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX config the sensor %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    OSA_waitMsecs(5);

    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8D02; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX Release the lock %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    OSA_waitMsecs(5);

    //lli
    //PLL and Timing 24MHZ clock in and 66MHZ AP0101 pixel clock out, 
    //YUV 24Min_AP0101_66Mout_25fps_2clkPerPixel_LSB8out

    i = 0;
    regAddr[i] = 0xCA84; regValue8[i] = 0x05; i++;
    
    status = DRV_i2c16Write8(&gDRV_imgsObj.i2cHndl, regAddr, regValue8, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write8()\n");
        return status;
    }
    OSA_waitMsecs(5);

    i = 0;
    regAddr[i] = 0xCA88; regValue[i] = 0x0116; i++;
    regAddr[i] = 0xCA8A; regValue[i] = 0x0116; i++;
    regAddr[i] = 0xCA8C; regValue[i] = 0x00FF; i++;
    regAddr[i] = 0xCA8E; regValue[i] = 0x007F; i++;
    regAddr[i] = 0xCA9C; regValue[i] = 0x0485; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
#if 1
    i = 0;
    regAddr[i] = 0xC838; regValue32[i] = 0x08020859; i++;
    status = DRV_i2c16Write32(&gDRV_imgsObj.i2cHndl, regAddr, regValue32, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write32()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xC838; regValue32[i] = 0; i++;
    status = DRV_i2c16Read32(&gDRV_imgsObj.i2cHndl, regAddr, regValue32, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Read32()\n");
        return status;
    }
    printf("0xC838 Value = %x\n", regValue32[0]);
    OSA_waitMsecs(5);
    
#endif
    i = 0;
    regAddr[i] = 0xC804; regValue[i] = 0x0080; i++;
    regAddr[i] = 0xC806; regValue[i] = 0x0002; i++;
    regAddr[i] = 0xC808; regValue[i] = 0x034F; i++;
    regAddr[i] = 0xC80A; regValue[i] = 0x0501; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
#if 1
    i = 0;
    regAddr[i] = 0xC80C; regValue32[i] = 0x01F78A40; i++;
    status = DRV_i2c16Write32(&gDRV_imgsObj.i2cHndl, regAddr, regValue32, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write32()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xC80C; regValue32[i] = 0; i++;
    status = DRV_i2c16Read32(&gDRV_imgsObj.i2cHndl, regAddr, regValue32, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write32()\n");
        return status;
    }
    printf("0xC80C Value = %x\n", regValue32[0]);
    OSA_waitMsecs(5);
#endif
    i = 0;
    regAddr[i] = 0xC810; regValue[i] = 0x02BC; i++;
    regAddr[i] = 0xC812; regValue[i] = 0x0672; i++;
    regAddr[i] = 0xC814; regValue[i] = 0x0320; i++;
    regAddr[i] = 0xC816; regValue[i] = 0x0672; i++;
    regAddr[i] = 0xC818; regValue[i] = 0x0000; i++;
    regAddr[i] = 0xC844; regValue[i] = 0x09C2; i++;
    regAddr[i] = 0xC846; regValue[i] = 0x0000; i++;
    regAddr[i] = 0xC89C; regValue[i] = 0x0000; i++;
    regAddr[i] = 0xC89E; regValue[i] = 0x0000; i++;
    regAddr[i] = 0xC8A0; regValue[i] = 0x0500; i++;
    regAddr[i] = 0xC8A2; regValue[i] = 0x02D0; i++;
    
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);

    i = 0;
    regAddr[i] = 0xC8A8; regValue8[i] = 0x00; i++;
    regAddr[i] = 0xC8A9; regValue8[i] = 0x00; i++;
    status = DRV_i2c16Write8(&gDRV_imgsObj.i2cHndl, regAddr, regValue8, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write8()\n");
        return status;
    }
    OSA_waitMsecs(5);

    i = 0;
    regAddr[i] = 0xCA90; regValue[i] = 0x0500; i++;
    regAddr[i] = 0xCA92; regValue[i] = 0x02D0; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);

    i = 0;
    regAddr[i] = 0xC8BC; regValue8[i] = 0x00; i++;
    status = DRV_i2c16Write8(&gDRV_imgsObj.i2cHndl, regAddr, regValue8, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write8()\n");
        return status;
    }
    OSA_waitMsecs(5);

    i = 0;
    regAddr[i] = 0xC8D4; regValue[i] = 0x0000; i++;
    regAddr[i] = 0xC8D6; regValue[i] = 0x0000; i++;
    regAddr[i] = 0xC8D8; regValue[i] = 0x0000; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
#endif

#if 0
    //AWB
    i = 0;
    regAddr[i] = 0xCA80; regValue[i] = 0x0000; i++;
    regAddr[i] = 0xC8DC; regValue[i] = 0x009C; i++;
    regAddr[i] = 0xC8DE; regValue[i] = 0x002E; i++;
    regAddr[i] = 0xC8E0; regValue[i] = 0x0035; i++;
    regAddr[i] = 0xC8E2; regValue[i] = 0xFFA8; i++;
    regAddr[i] = 0xC8E4; regValue[i] = 0x0117; i++;
    regAddr[i] = 0xC8E6; regValue[i] = 0x0041; i++;
    regAddr[i] = 0xC8E8; regValue[i] = 0xFFA2; i++;
    regAddr[i] = 0xC8EA; regValue[i] = 0x0004; i++;
    regAddr[i] = 0xC8EC; regValue[i] = 0x015A; i++;
    regAddr[i] = 0xC8EE; regValue[i] = 0x00C5; i++;
    regAddr[i] = 0xC8F0; regValue[i] = 0x0001; i++;
    regAddr[i] = 0xC8F2; regValue[i] = 0x003A; i++;
    regAddr[i] = 0xC8F4; regValue[i] = 0xFFEA; i++;
    regAddr[i] = 0xC8F6; regValue[i] = 0x00E7; i++;
    regAddr[i] = 0xC8F8; regValue[i] = 0x002F; i++;
    regAddr[i] = 0xC8FA; regValue[i] = 0x0009; i++;
    regAddr[i] = 0xC8FC; regValue[i] = 0xFFF7; i++;
    regAddr[i] = 0xC8FE; regValue[i] = 0x0100; i++;
    regAddr[i] = 0xC900; regValue[i] = 0x00A4; i++;
    regAddr[i] = 0xC902; regValue[i] = 0x004B; i++;
    regAddr[i] = 0xC904; regValue[i] = 0x0011; i++;
    regAddr[i] = 0xC906; regValue[i] = 0xFFE8; i++;
    regAddr[i] = 0xC908; regValue[i] = 0x00E4; i++;
    regAddr[i] = 0xC90A; regValue[i] = 0x0034; i++;
    regAddr[i] = 0xC90C; regValue[i] = 0x000A; i++;
    regAddr[i] = 0xC90E; regValue[i] = 0x001F; i++;
    regAddr[i] = 0xC910; regValue[i] = 0x00D8; i++;
    regAddr[i] = 0xC912; regValue[i] = 0x0011; i++;
    regAddr[i] = 0xC914; regValue[i] = 0x0059; i++;
    regAddr[i] = 0xC916; regValue[i] = 0x0061; i++;
    regAddr[i] = 0xC918; regValue[i] = 0x0101; i++;
    regAddr[i] = 0xC91A; regValue[i] = 0x007E; i++;
    regAddr[i] = 0xC91C; regValue[i] = 0x009F; i++;
    regAddr[i] = 0xC91E; regValue[i] = 0x09C4; i++;
    regAddr[i] = 0xC920; regValue[i] = 0x0D67; i++;
    regAddr[i] = 0xC922; regValue[i] = 0x1964; i++;
    regAddr[i] = 0xC924; regValue[i] = 0x09C4; i++;
    regAddr[i] = 0xC926; regValue[i] = 0x1964; i++;
    regAddr[i] = 0xC92A; regValue[i] = 0x001E; i++;
    regAddr[i] = 0xC92C; regValue[i] = 0x0020; i++;
    regAddr[i] = 0xC92E; regValue[i] = 0x009C; i++;
    regAddr[i] = 0xC930; regValue[i] = 0x0044; i++;
    regAddr[i] = 0xC932; regValue[i] = 0x0007; i++;
    regAddr[i] = 0xC934; regValue[i] = 0xFFDF; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);

    i = 0;
    regAddr[i] = 0xC936; regValue8[i] = 0x3F; i++;
    regAddr[i] = 0xC937; regValue8[i] = 0x0A; i++;
    status = DRV_i2c16Write8(&gDRV_imgsObj.i2cHndl, regAddr, regValue8, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write8()\n");
        return status;
    }
    OSA_waitMsecs(5);


    i = 0;
    regAddr[i] = 0xC938; regValue[i] = 0x1111; i++;
    regAddr[i] = 0xC93A; regValue[i] = 0x1111; i++;
    regAddr[i] = 0xC93C; regValue[i] = 0x2222; i++;
    regAddr[i] = 0xC93E; regValue[i] = 0x1111; i++;
    regAddr[i] = 0xC940; regValue[i] = 0x1222; i++;
    regAddr[i] = 0xC942; regValue[i] = 0x2223; i++;
    regAddr[i] = 0xC944; regValue[i] = 0x4555; i++;
    regAddr[i] = 0xC946; regValue[i] = 0x2221; i++;
    regAddr[i] = 0xC948; regValue[i] = 0x2466; i++;
    regAddr[i] = 0xC94A; regValue[i] = 0x6654; i++;
    regAddr[i] = 0xC94C; regValue[i] = 0x3234; i++;
    regAddr[i] = 0xC94E; regValue[i] = 0x3452; i++;
    regAddr[i] = 0xC950; regValue[i] = 0x2577; i++;
    regAddr[i] = 0xC952; regValue[i] = 0x6764; i++;
    regAddr[i] = 0xC954; regValue[i] = 0x2212; i++;
    regAddr[i] = 0xC956; regValue[i] = 0x2552; i++;
    regAddr[i] = 0xC958; regValue[i] = 0x1354; i++;
    regAddr[i] = 0xC95A; regValue[i] = 0x4565; i++;
    regAddr[i] = 0xC95C; regValue[i] = 0x4422; i++;
    regAddr[i] = 0xC95E; regValue[i] = 0x2331; i++;
    regAddr[i] = 0xC960; regValue[i] = 0x1122; i++;
    regAddr[i] = 0xC962; regValue[i] = 0x1234; i++;
    regAddr[i] = 0xC964; regValue[i] = 0x3335; i++;
    regAddr[i] = 0xC966; regValue[i] = 0x6652; i++;
    regAddr[i] = 0xC968; regValue[i] = 0x1111; i++;
    regAddr[i] = 0xC96A; regValue[i] = 0x1112; i++;
    regAddr[i] = 0xC96C; regValue[i] = 0x1224; i++;
    regAddr[i] = 0xC96E; regValue[i] = 0x5652; i++;
    regAddr[i] = 0xC970; regValue[i] = 0x1111; i++;
    regAddr[i] = 0xC972; regValue[i] = 0x1111; i++;
    regAddr[i] = 0xC974; regValue[i] = 0x1112; i++;
    regAddr[i] = 0xC976; regValue[i] = 0x2332; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);

    i = 0;
    regAddr[i] = 0xAC06; regValue8[i] = 0x63; i++;
    regAddr[i] = 0xAC07; regValue8[i] = 0x65; i++;
    regAddr[i] = 0xAC08; regValue8[i] = 0x63; i++;
    regAddr[i] = 0xAC09; regValue8[i] = 0x65; i++;
    regAddr[i] = 0xAC0A; regValue8[i] = 0x00; i++;
    regAddr[i] = 0xAC0B; regValue8[i] = 0xFF; i++;
    regAddr[i] = 0xAC0C; regValue8[i] = 0x00; i++;
    regAddr[i] = 0xAC0D; regValue8[i] = 0xFF; i++;
    status = DRV_i2c16Write8(&gDRV_imgsObj.i2cHndl, regAddr, regValue8, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write8()\n");
        return status;
    }
    OSA_waitMsecs(5);

    i = 0;
    regAddr[i] = 0xA84C; regValue[i] = 0x03FE; i++;
    regAddr[i] = 0xC9CC; regValue[i] = 0xF900; i++;
    regAddr[i] = 0xC854; regValue[i] = 0x0001; i++;
    regAddr[i] = 0xC85E; regValue[i] = 0x0020; i++;
    regAddr[i] = 0xC8C8; regValue[i] = 0x0280; i++;
    regAddr[i] = 0xCA5A; regValue[i] = 0x0000; i++;
    regAddr[i] = 0xCA66; regValue[i] = 0x0000; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xC9E4; regValue8[i] = 0x80; i++;
    regAddr[i] = 0xC9E5; regValue8[i] = 0x80; i++;
    status = DRV_i2c16Write8(&gDRV_imgsObj.i2cHndl, regAddr, regValue8, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write8()\n");
        return status;
    }
    OSA_waitMsecs(5);

    i = 0;
    regAddr[i] = 0xC99E; regValue[i] = 0x0002; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);

    i = 0;    
    regAddr[i] = 0x241E; regValue[i] = 0x0002; i++;
    regAddr[i] = 0x2444; regValue[i] = 0x0010; i++;
    regAddr[i] = 0x2442; regValue[i] = 0x0010; i++;
    regAddr[i] = 0x2446; regValue[i] = 0x0020; i++;
    regAddr[i] = 0x2448; regValue[i] = 0x0100; i++;
    regAddr[i] = 0x244A; regValue[i] = 0x0002; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    i = 0;
    regAddr[i] = 0xC9E6; regValue8[i] = 0x4D; i++;
    regAddr[i] = 0xC9E7; regValue8[i] = 0x08; i++;
    regAddr[i] = 0xC9EA; regValue8[i] = 0x4D; i++;
    regAddr[i] = 0xC9EB; regValue8[i] = 0x08; i++;
    status = DRV_i2c16Write8(&gDRV_imgsObj.i2cHndl, regAddr, regValue8, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write8()\n");
        return status;
    }
    OSA_waitMsecs(5);

    i = 0;
    regAddr[i] = 0x3222; regValue[i] = 0x0028; i++;
    regAddr[i] = 0x3224; regValue[i] = 0x0028; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);

    i = 0;    
    regAddr[i] = 0xCA34; regValue[i] = 0x0020; i++;
    regAddr[i] = 0xCA38; regValue[i] = 0x0093; i++;
    regAddr[i] = 0xCA36; regValue[i] = 0x0034; i++;
    regAddr[i] = 0xCA3C; regValue[i] = 0x0058; i++;
    regAddr[i] = 0xCA3E; regValue[i] = 0x0037; i++;
    regAddr[i] = 0xCA40; regValue[i] = 0x0093; i++;
    regAddr[i] = 0xCA44; regValue[i] = 0x0160; i++;
    regAddr[i] = 0xCA46; regValue[i] = 0x0107; i++;
    regAddr[i] = 0xCA48; regValue[i] = 0x0093; i++;
    regAddr[i] = 0xCA4C; regValue[i] = 0x02C0; i++;
    regAddr[i] = 0xCA4E; regValue[i] = 0x0105; i++;
    regAddr[i] = 0xCA50; regValue[i] = 0x0093; i++;
    regAddr[i] = 0xC9DC; regValue[i] = 0x01F4; i++;
    regAddr[i] = 0xC9DE; regValue[i] = 0x0BB8; i++;
    regAddr[i] = 0xC9E0; regValue[i] = 0x0DC0; i++;
    regAddr[i] = 0xC9E2; regValue[i] = 0x0020; i++;
    regAddr[i] = 0xCA2C; regValue[i] = 0x0006; i++;
    regAddr[i] = 0xCA2E; regValue[i] = 0x0003; i++;
    regAddr[i] = 0xCA30; regValue[i] = 0x03E8; i++;
    regAddr[i] = 0xCA32; regValue[i] = 0x0D00; i++;
    regAddr[i] = 0xA82C; regValue[i] = 0x07C0; i++;
    regAddr[i] = 0xA82E; regValue[i] = 0x081F; i++;
    regAddr[i] = 0xA830; regValue[i] = 0x0880; i++;
    regAddr[i] = 0xA832; regValue[i] = 0x08D1; i++;
    regAddr[i] = 0xA834; regValue[i] = 0x0921; i++;
    regAddr[i] = 0xA836; regValue[i] = 0x09A5; i++;
    regAddr[i] = 0xA838; regValue[i] = 0x09D0; i++;
    regAddr[i] = 0xA83A; regValue[i] = 0x09F7; i++;
    regAddr[i] = 0xA83C; regValue[i] = 0x07C0; i++;
    regAddr[i] = 0xA83E; regValue[i] = 0x081F; i++;
    regAddr[i] = 0xA840; regValue[i] = 0x0880; i++;
    regAddr[i] = 0xA842; regValue[i] = 0x08D1; i++;
    regAddr[i] = 0xA844; regValue[i] = 0x0921; i++;
    regAddr[i] = 0xA846; regValue[i] = 0x09A5; i++;
    regAddr[i] = 0xA848; regValue[i] = 0x09D0; i++;
    regAddr[i] = 0xA84A; regValue[i] = 0x09F7; i++;
    regAddr[i] = 0xC84E; regValue[i] = 0x0800; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);

    i = 0;
    regAddr[i] = 0xC9FF; regValue8[i] = 0x0F; i++;
    regAddr[i] = 0xC9FE; regValue8[i] = 0x08; i++;
    regAddr[i] = 0xCA05; regValue8[i] = 0x0F; i++;
    regAddr[i] = 0xCA04; regValue8[i] = 0x08; i++;
    regAddr[i] = 0xCA0B; regValue8[i] = 0x0C; i++;
    regAddr[i] = 0xCA0A; regValue8[i] = 0x04; i++;
    regAddr[i] = 0xCA11; regValue8[i] = 0x01; i++;
    regAddr[i] = 0xCA10; regValue8[i] = 0x01; i++;
    regAddr[i] = 0xCA1B; regValue8[i] = 0x0F; i++;
    regAddr[i] = 0xCA1A; regValue8[i] = 0x08; i++;
    regAddr[i] = 0xCA21; regValue8[i] = 0x0F; i++;
    regAddr[i] = 0xCA20; regValue8[i] = 0x08; i++;
    regAddr[i] = 0xCA27; regValue8[i] = 0x0C; i++;
    regAddr[i] = 0xCA26; regValue8[i] = 0x04; i++;
    status = DRV_i2c16Write8(&gDRV_imgsObj.i2cHndl, regAddr, regValue8, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write8()\n");
        return status;
    }
    OSA_waitMsecs(5);


    i = 0;
    regAddr[i] = 0xC9FC; regValue[i] = 0x0B54; i++;
    regAddr[i] = 0xC9FA; regValue[i] = 0x00C8; i++;
    regAddr[i] = 0xCA02; regValue[i] = 0x0B54; i++;
    regAddr[i] = 0xCA00; regValue[i] = 0x00C8; i++;
    regAddr[i] = 0xCA08; regValue[i] = 0x0B54; i++;
    regAddr[i] = 0xCA06; regValue[i] = 0x00C8; i++;
    regAddr[i] = 0xCA0E; regValue[i] = 0x0B54; i++;
    regAddr[i] = 0xCA0C; regValue[i] = 0x00C8; i++;
    regAddr[i] = 0xCA18; regValue[i] = 0x0B54; i++;
    regAddr[i] = 0xCA16; regValue[i] = 0x00C8; i++;
    regAddr[i] = 0xCA1E; regValue[i] = 0x0B54; i++;
    regAddr[i] = 0xCA1C; regValue[i] = 0x00C8; i++;
    regAddr[i] = 0xCA24; regValue[i] = 0x0B54; i++;
    regAddr[i] = 0xCA22; regValue[i] = 0x00C8; i++;
    regAddr[i] = 0xC990; regValue[i] = 0x0010; i++;
    regAddr[i] = 0xC992; regValue[i] = 0x0020; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);


    i = 0;
    regAddr[i] = 0xBC07; regValue8[i] = 0x01; i++;
    status = DRV_i2c16Write8(&gDRV_imgsObj.i2cHndl, regAddr, regValue8, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write8()\n");
        return status;
    }
    OSA_waitMsecs(5);


    i = 0;
    regAddr[i] = 0xC9F0; regValue[i] = 0x0064; i++;
    regAddr[i] = 0xC99A; regValue[i] = 0x0600; i++;
    regAddr[i] = 0xC99C; regValue[i] = 0x0800; i++;
    regAddr[i] = 0xC996; regValue[i] = 0x0800; i++;
    regAddr[i] = 0xC98A; regValue[i] = 0x0080; i++;
    regAddr[i] = 0xC994; regValue[i] = 0x00AF; i++;
    regAddr[i] = 0xC9A0; regValue[i] = 0x00C8; i++;
    regAddr[i] = 0xC9A2; regValue[i] = 0x0B54; i++;
    regAddr[i] = 0xC9A4; regValue[i] = 0x0005; i++;
    regAddr[i] = 0xC9A6; regValue[i] = 0x0008; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);

    i = 0;    
    regAddr[i] = 0x2402; regValue[i] = 0x0008; i++;
    regAddr[i] = 0x2418; regValue[i] = 0x005A; i++;
    regAddr[i] = 0x241A; regValue[i] = 0x0064; i++;
    regAddr[i] = 0x241C; regValue[i] = 0x0078; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);

    i = 0;    
    regAddr[i] = 0xC9F6; regValue[i] = 0x01F4; i++;
    regAddr[i] = 0xC9F8; regValue[i] = 0x005A; i++;
    regAddr[i] = 0xBC02; regValue[i] = 0x000F; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);


    i = 0;
    regAddr[i] = 0xC9E8; regValue8[i] = 0x01; i++;
    regAddr[i] = 0xC9E9; regValue8[i] = 0x03; i++;
    status = DRV_i2c16Write8(&gDRV_imgsObj.i2cHndl, regAddr, regValue8, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write8()\n");
        return status;
    }
    OSA_waitMsecs(5);


    i = 0;
    regAddr[i] = 0xCA58; regValue[i] = 0x0B00; i++;
    regAddr[i] = 0xCA64; regValue[i] = 0x0A00; i++;
    regAddr[i] = 0xCA70; regValue[i] = 0x0066; i++;
    regAddr[i] = 0xCA6C; regValue[i] = 0x0200; i++;
    regAddr[i] = 0xCA60; regValue[i] = 0x0200; i++;
    regAddr[i] = 0xA802; regValue[i] = 0x001C; i++;
    regAddr[i] = 0xC9A4; regValue[i] = 0x0000; i++;
    regAddr[i] = 0xC9A6; regValue[i] = 0x0000; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);

    i = 0;
    regAddr[i] = 0x8000 | (0x12 << 10) | 0x008C; regValue8[i] = 0x00; i++;
    status = DRV_i2c16Write8(&gDRV_imgsObj.i2cHndl, regAddr, regValue8, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write8()\n");
        return status;
    }
    OSA_waitMsecs(5);

    
    i = 0;
    regAddr[i] = 0x8000 | (0x12 << 10) | 0x029C; regValue[i] = 0x0284; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
#endif
#if 1
    i = 0;
    regAddr[i] = 0xFC00; regValue[i] = 0x2800; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8100; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX System State is SYS_STATE_STREAMING %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
    

    OSA_waitMsecs(5);
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8101; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX System State is SYS_STATE_STREAMING %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
#endif

#if 0
    i = 0;
    regAddr[i] = 0xDC07; regValue8[i] = 0x04; i++;
    status = DRV_i2c16Write8(&gDRV_imgsObj.i2cHndl, regAddr, regValue8, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write8()\n");
        return status;
    }
    OSA_waitMsecs(5);
#endif
#if 0

    i = 0;
    regAddr[i] = 0x8000 | (0x12 << 10) | 0x029C; regValue[i] = 0x0284; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);

#if 0
    i = 0;
    regAddr[i] = 0x8000 | (0x12 << 10) | 0x029C; regValue[i] = 0x0405; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
#endif

    i = 0;
    regAddr[i] = 0xFC00; regValue[i] = 0x2800; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8100; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX System State is SYS_STATE_STREAMING %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    

    OSA_waitMsecs(5);
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8101; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX System State is SYS_STATE_STREAMING %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    


    OSA_waitMsecs(5);
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8101; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX System State is SYS_STATE_STREAMING %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
#endif

#if 0
    i = 0;
    regAddr[i] = 0xCA84; regValue8[i] = 0x05; i++;
    
    status = DRV_i2c16Write8(&gDRV_imgsObj.i2cHndl, regAddr, regValue8, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write8()\n");
        return status;
    }
    OSA_waitMsecs(5);

    i = 0;
    regAddr[i] = 0xCA88; regValue[i] = 0x0116; i++;
    regAddr[i] = 0xCA8A; regValue[i] = 0x0116; i++;
    regAddr[i] = 0xCA8C; regValue[i] = 0x00FF; i++;
    regAddr[i] = 0xCA8E; regValue[i] = 0x007F; i++;
    regAddr[i] = 0xCA9C; regValue[i] = 0x0405; i++;
    regAddr[i] = 0xC838; regValue[i] = 0x0802; i++;
    regAddr[i] = 0xC83A; regValue[i] = 0x0859; i++;
    regAddr[i] = 0xC804; regValue[i] = 0x0080; i++;
    regAddr[i] = 0xC806; regValue[i] = 0x0002; i++;
    regAddr[i] = 0xC808; regValue[i] = 0x034F; i++;
    regAddr[i] = 0xC80A; regValue[i] = 0x0501; i++;
    regAddr[i] = 0xC80C; regValue[i] = 0x01F7; i++;
    regAddr[i] = 0xC80E; regValue[i] = 0x8A40; i++;
    regAddr[i] = 0xC810; regValue[i] = 0x02BC; i++;
    regAddr[i] = 0xC812; regValue[i] = 0x0672; i++;
    regAddr[i] = 0xC814; regValue[i] = 0x0320; i++;
    regAddr[i] = 0xC816; regValue[i] = 0x0672; i++;
    regAddr[i] = 0xC818; regValue[i] = 0x0000; i++;
    regAddr[i] = 0xC844; regValue[i] = 0x09C2; i++;
    regAddr[i] = 0xC846; regValue[i] = 0x0000; i++;
    regAddr[i] = 0xC89C; regValue[i] = 0x0000; i++;
    regAddr[i] = 0xC89E; regValue[i] = 0x0000; i++;
    regAddr[i] = 0xC8A0; regValue[i] = 0x0500; i++;
    regAddr[i] = 0xC8A2; regValue[i] = 0x02D0; i++;
    
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);

    i = 0;
    regAddr[i] = 0xC8A8; regValue8[i] = 0x00; i++;
    regAddr[i] = 0xC8A9; regValue8[i] = 0x00; i++;
    status = DRV_i2c16Write8(&gDRV_imgsObj.i2cHndl, regAddr, regValue8, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write8()\n");
        return status;
    }
    OSA_waitMsecs(5);

    i = 0;
    regAddr[i] = 0xCA90; regValue[i] = 0x0500; i++;
    regAddr[i] = 0xCA92; regValue[i] = 0x02D0; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);

    i = 0;
    regAddr[i] = 0xC8BC; regValue8[i] = 0x00; i++;
    status = DRV_i2c16Write8(&gDRV_imgsObj.i2cHndl, regAddr, regValue8, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write8()\n");
        return status;
    }
    OSA_waitMsecs(5);

    i = 0;
    regAddr[i] = 0xC8D4; regValue[i] = 0x0000; i++;
    regAddr[i] = 0xC8D6; regValue[i] = 0x0000; i++;
    regAddr[i] = 0xC8D8; regValue[i] = 0x0000; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);


    i = 0;
    regAddr[i] = 0xFC00; regValue[i] = 0x2800; i++;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);    
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(5);
    while(1)
    {
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8100; i++;
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
        OSA_waitMsecs(5);
        i = 0;
        regAddr[i] = 0x0040; regValue[i] = 0x8000; i++;
        status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);        
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Read16()\n");
            return status;
        }
        printf("XXXXXXXXXXXXX System State is SYS_STATE_STREAMING %d\n", __LINE__);
        if(regValue[0] == 0)
        {
            break;
        }
    }    
#endif
    return OSA_SOK;
}
#endif

int DRV_imgsSet50_60Hz_AP0101(Bool is50Hz)
{
    int fps;

    if( gDRV_imgsObj.curFrameTime.fps==30
            ||gDRV_imgsObj.curFrameTime.fps==25
      ) {

        if(is50Hz)
            fps = 25;
        else
            fps = 30;

        DRV_imgsSetFramerate_AP0101(fps);
    }
    return 0;
}

int DRV_imgsSetFramerate_AP0101(int fps)
{
    return 0;
}

int DRV_imgsBinEnable_AP0101(Bool enable)
{
    return OSA_SOK;
}

int DRV_imgsBinMode_AP0101(int binMode)
{
    return OSA_SOK;
}

int DRV_imgsSetDgain_AP0101(int dgain)
{
    return OSA_SOK;
}

int DRV_imgsSetAgain_AP0101(int again, int setRegDirect)
{
    return OSA_SOK; 
}

int DRV_imgsSetEshutter_AP0101(Uint32 eshutterInUsec, int setRegDirect)
{
    return OSA_SOK;
}

//Read AGain & exposure
//return value is 8 times the true analog gain
int DRV_imgsGetAgain_AP0101(int *again)
{
    return OSA_SOK;
}

int DRV_imgsGetEshutter_AP0101(Uint32 *eshutterInUsec)
{
    return OSA_SOK; 
}

int DRV_imgsSetDcSub_AP0101(Uint32 dcSub, int setRegDirect)
{
    return 0;
}

int DRV_imgsEnable_AP0101(Bool enable)
{
    Uint16 regAddr;
    Uint16 regValue;
    int status=OSA_SOK;

    DRV_ImgsFrameTime *pFrame = &gDRV_imgsObj.curFrameTime;

    if (enable)
    {
        status = DRV_imgsSetRegs_AP0101();
        if (status!=OSA_SOK)
        {
            OSA_ERROR("DRV_imgsSetRegs_AP0101()\n");
            return status;
        }
        //DRV_imgsSpecificSetting_AP0101();
    }

#ifdef SENSOR_REG_DUMP
    {
        unsigned char i;
        Uint16 dumpAddr[128];
        Uint16 dumpValue[128];
        FILE *dumpFile;

        for (i=0;i<128;i++)
            dumpAddr = i;

        DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, dumpAddr, dumpValue, 128);
        OSA_waitMsecs(100);

        dumpFile = fopen("sensor_dump.txt","wb");
        for (i=0;i<128;i++)
            fprintf(dumpFile, "0x%x \t0x%x\n", dumpAddr, dumpValue);
        fclose(dumpFile);
    }
#endif

    return status;
}

DRV_ImgsModeConfig      *DRV_imgsGetModeConfig_AP0101(int sensorMode)
{
    return &gDRV_imgsObj.curModeConfig;
}

DRV_ImgsIsifConfig      *DRV_imgsGetIsifConfig_AP0101(int sensorMode)
{
    return &gDRV_imgsIsifConfig_Common_AP0101;
}

DRV_ImgsIpipeConfig     *DRV_imgsGetIpipeConfig_AP0101(int sensorMode, int vnfDemoCfg)
{
    if (vnfDemoCfg)
        return &gDRV_imgsIpipeConfig_Vnfdemo_AP0101;
    else
        return &gDRV_imgsIpipeConfig_Common_AP0101;
}

DRV_ImgsH3aConfig       *DRV_imgsGetH3aConfig_AP0101(int sensorMode, int aewbVendor)
{
    if (aewbVendor==1)
    {
        return &gDRV_imgsH3aConfig_Appro_AP0101;
    }
    else if (aewbVendor==2)
    {
        return &gDRV_imgsH3aConfig_TI_AP0101;
    }
    else
    {
        return &gDRV_imgsH3aConfig_Common_AP0101;
    }
}

DRV_ImgsLdcConfig       *DRV_imgsGetLdcConfig_AP0101(int sensorMode, Uint16 ldcInFrameWidth, Uint16 ldcInFrameHeight)
{
    sensorMode &= 0xFF;

    switch (sensorMode)
    {
    
    case DRV_IMGS_SENSOR_MODE_720x480:

        if (ldcInFrameWidth==864)
            return &gDRV_imgsLdcConfig_736x480_0_VS;

        if (ldcInFrameWidth==352)
            return &gDRV_imgsLdcConfig_736x480_1_VS;

        if (ldcInFrameWidth==736)
            return &gDRV_imgsLdcConfig_736x480_0;

        if (ldcInFrameWidth==288)
            return &gDRV_imgsLdcConfig_736x480_1;

        if (ldcInFrameWidth==768)
            return &gDRV_imgsLdcConfig_768x512_0;

        if (ldcInFrameWidth==320)
            return &gDRV_imgsLdcConfig_768x512_1;

        if (ldcInFrameWidth==928)
            return &gDRV_imgsLdcConfig_768x512_0_VS;

        if (ldcInFrameWidth==384)
            return &gDRV_imgsLdcConfig_768x512_1_VS;

        break;

    case DRV_IMGS_SENSOR_MODE_1280x720:

        if (ldcInFrameWidth==1280)
            return &gDRV_imgsLdcConfig_1280x736_0;

        if (ldcInFrameWidth==320)
            return &gDRV_imgsLdcConfig_1280x736_1;

        if (ldcInFrameWidth==640)
            return &gDRV_imgsLdcConfig_1280x736_2;

        if (ldcInFrameWidth==1536)
            return &gDRV_imgsLdcConfig_1280x736_0_VS;

        if (ldcInFrameWidth==384)
            return &gDRV_imgsLdcConfig_1280x736_1_VS;

        if (ldcInFrameWidth==768)
            return &gDRV_imgsLdcConfig_1280x736_2_VS;

        if (ldcInFrameWidth==1312)
            return &gDRV_imgsLdcConfig_1312x768_0;

        if (ldcInFrameWidth==352)
            return &gDRV_imgsLdcConfig_1312x768_1;

        if (ldcInFrameWidth==672)
            return &gDRV_imgsLdcConfig_1312x768_2;

        if (ldcInFrameWidth==1600)
            return &gDRV_imgsLdcConfig_1312x768_0_VS;

        if (ldcInFrameWidth==448)
            return &gDRV_imgsLdcConfig_1312x768_1_VS;

        if (ldcInFrameWidth==832)
            return &gDRV_imgsLdcConfig_1312x768_2_VS;

        break;

    }

    return NULL;
}

int DRV_imgsReset_AP0101()
{
    return OSA_SOK;
}

int DRV_imgsCheckId_AP0101()
{
    Uint16 regAddr  = AP0101_CHIP_VERSION;
    Uint16 regValue = 0;
    
    int status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Read16()\n");
        return status;
    }

    if (regValue != IMGS_CHIP_ID)
    {
        return OSA_EFAIL;
    }

    return OSA_SOK;
}

int DRV_imgsSetRegs_AP0101()
{
  int i, status;
  static Uint16 regAddr[32];
  static Uint16 regValue[32];

  DRV_ImgsFrameTime *pFrame = &gDRV_imgsObj.curFrameTime;

  status = DRV_imgsReset_AP0101();
  if(status!=OSA_SOK)
    return status;
}

DRV_ImgsFuncs AP0101ImgsFuncs = 
{
    .imgsOpen            = DRV_imgsOpen_AP0101,
    .imgsClose           = DRV_imgsClose_AP0101,
    .imgsSetMirror       = NULL,
    .imgsGetImagerName   = DRV_imgsGetImagerName_AP0101,
    .imgsSetAgain        = DRV_imgsSetAgain_AP0101,
    .imgsSetDgain        = DRV_imgsSetDgain_AP0101,
    .imgsSetEshutter     = DRV_imgsSetEshutter_AP0101,
    .imgsSetDcSub        = DRV_imgsSetDcSub_AP0101,
    .imgsBinEnable       = DRV_imgsBinEnable_AP0101,
    .imgsBinMode         = DRV_imgsBinMode_AP0101,
    .imgsSetFramerate    = DRV_imgsSetFramerate_AP0101,
    .imgsSet50_60Hz      = DRV_imgsSet50_60Hz_AP0101, 
    .imgsEnable          = DRV_imgsEnable_AP0101,
    .imgsGetModeConfig   = DRV_imgsGetModeConfig_AP0101,
    .imgsGetIsifConfig   = DRV_imgsGetIsifConfig_AP0101,
    .imgsGetH3aConfig    = DRV_imgsGetH3aConfig_AP0101,
    .imgsGetIpipeConfig  = DRV_imgsGetIpipeConfig_AP0101,
    .imgsGetLdcConfig    = DRV_imgsGetLdcConfig_AP0101
};

