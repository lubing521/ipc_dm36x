#include "drv_imgs_AR0130_REG.h"
#include "drv_imgs_AR0130_1.2MP.h"
#include <drv_gpio.h>

DRV_ImgsObj gDRV_imgsObj;

#define INPUT_CLK 24
#define PLL_M     37
#define PLL_N     2
#define PLL_P1    1
#define PLL_P2    6
#define OUT_CLK ((INPUT_CLK * PLL_M) / (PLL_N * PLL_P1 * PLL_P2)) //74.25M //45M

#define LINE_LENGTH (0x056E)  // 0x07D0 =  2000    0x0672 = 1650(MIN)
//#define ROW_TIME    (LINE_LENGTH / OUT_CLK)  //26.936us  //44.444us
#define ROW_TIME    20  //26.936us  //44.444us

#define FRAME_LENGTH (0x0852)  // 0x0384 =  45M 25fps;  0x02EE = 45M 30fps   /* 0x03DE */ 
#define Y_ADDR_END   (0x03C5)  // 0x02E1 = 736; 0x03C5 =  964; 

#define DGAIN_STEP 3125
#define DGAIN_MUTI 0x40 // 0b00100000: *1, 0b00110000: *1.5

int DRV_imgsEnable(Bool enable);

int DRV_imgsCheckId()
{
    Uint16 regAddr  = AR0130_CHIP_VERSION;
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

int gImgsVPad = 0;

int DRV_imgsOpen(DRV_ImgsConfig *config)
{
    int status, retry = 10;
    Uint16 width, height;

    memset(&gDRV_imgsObj, 0, sizeof(gDRV_imgsObj));

    DRV_imgGetWidthHeight(config->sensorMode, &width, &height);

    if (config->sensorMode == DRV_IMGS_SENSOR_MODE_1280x840)
    {
        gImgsVPad = 120;
    }
    else
    {
        gImgsVPad = 4;
    }
    printf("\n-------gImgs_v_Pad = %d\n", gImgsVPad);
    
    width  += IMGS_H_PAD;
    height += IMGS_V_PAD;
    
    DRV_imgsCalcFrameTime_AR0130(config->fps, width, height, config->binEnable);
    
    status = DRV_i2cOpen(&gDRV_imgsObj.i2cHndl, IMGS_I2C_ADDR);
    if (status != OSA_SOK)
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
        status = DRV_imgsCheckId();
        if (status == OSA_SOK)
        {
            break;
        }
        OSA_waitMsecs(10);
    } while (retry--);

    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_imgsCheckId_AR0130()\n");
        DRV_i2cClose(&gDRV_imgsObj.i2cHndl);
        return status;
    }

    return 0;
}

int DRV_imgsClose()
{
    int status = DRV_imgsEnable(FALSE);
    status |= DRV_i2cClose(&gDRV_imgsObj.i2cHndl);
    return status;
}

char *DRV_imgsGetImagerName()
{
    return "APTINA_AR0130_1.2MP";
}

static int AR0130_linear_init_regs(void)
{
#if 1
    int j, status;
    for (j=0; j < sizeof(AR0130_REG)/sizeof(unsigned short); j=j+2)
    {
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, &AR0130_REG[j], &AR0130_REG[j+1], 1);  
        if(status != OSA_SOK)
        {
            OSA_printf("I2C write Error, index:%d\n",j);
            return status;
        }
        if (AR0130_REG[j] == 0x301A || AR0130_REG[j] == 0x30E8)  
        {
            OSA_waitMsecs(200);
        }
        else if(AR0130_REG[j] == 0x30B0)
        {
            OSA_waitMsecs(100);
        }
        else
        {
            OSA_waitMsecs(1);   
        }
    } 

    OSA_printf("AR0130 reg Settings: Regs %d\n", j/2);
    return OSA_SOK;

#else

    Uint16 regAddr[1000];
    Uint16 regValue[1000];
    int i      = 0;
    int status = 0;

    // [Demo Initialization_110511_INI]1: Load=Reset
    // [Reset]1: STATE=Sensor Reset, 1
    // [Reset]2: DELAY=200
    // [Reset]3: STATE=Sensor Reset, 0
    // [Reset]4: FIELD_WR=RESET_REGISTER, 1

    regAddr[i] = 0x301A; regValue[i] = 0x0001; i++;     // RESET_REGISTER
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }

    i = 0;
    OSA_waitMsecs(200);

    regAddr[i] = 0x301A; regValue[i] = 0x10D8; i++;     // RESET_REGISTER
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }

    i = 0;
    OSA_waitMsecs(200);

    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]1: REG=0x3088,    0x8000  
    regAddr[i] = 0x3088; regValue[i] = 0x8000; i++;     // SEQ_CTRL_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]2: REG=0x3086,    0x0025    
    regAddr[i] = 0x3086; regValue[i] = 0x0225; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]3: REG=0x3086,    0x5050  
    regAddr[i] = 0x3086; regValue[i] = 0x5050; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]4: REG=0x3086,    0x2D26  
    regAddr[i] = 0x3086; regValue[i] = 0x2D26; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]5: REG=0x3086,    0x0828   
    regAddr[i] = 0x3086; regValue[i] = 0x0828; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]6: REG=0x3086,    0x0D17   
    regAddr[i] = 0x3086; regValue[i] = 0x0D17; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]7: REG=0x3086,    0x0926   
    regAddr[i] = 0x3086; regValue[i] = 0x0926; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]8: REG=0x3086,    0x0028    
    regAddr[i] = 0x3086; regValue[i] = 0x0028; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]9: REG=0x3086,    0x0526   
    regAddr[i] = 0x3086; regValue[i] = 0x0526; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]10: REG=0x3086,   0xA728  
    regAddr[i] = 0x3086; regValue[i] = 0xA728; i++;     // SEQ_DATA_PORT

    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]11: REG=0x3086,   0x0725   
    regAddr[i] = 0x3086; regValue[i] = 0x0725; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]12: REG=0x3086,   0x8080  
    regAddr[i] = 0x3086; regValue[i] = 0x8080; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]13: REG=0x3086,   0x2925  
    regAddr[i] = 0x3086; regValue[i] = 0x2917; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]14: REG=0x3086,   0x0040    
    regAddr[i] = 0x3086; regValue[i] = 0x0525; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]14: REG=0x3086,   0x0040    
    regAddr[i] = 0x3086; regValue[i] = 0x0040; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]15: REG=0x3086,   0x2702  
    regAddr[i] = 0x3086; regValue[i] = 0x2702; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]16: REG=0x3086,   0x1616  
    regAddr[i] = 0x3086; regValue[i] = 0x1616; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]17: REG=0x3086,   0x2706  
    regAddr[i] = 0x3086; regValue[i] = 0x2706; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]18: REG=0x3086,   0x1F17  
    regAddr[i] = 0x3086; regValue[i] = 0x1736; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]19: REG=0x3086,   0x3626  
    regAddr[i] = 0x3086; regValue[i] = 0x26A6; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]20: REG=0x3086,   0xA617  
    regAddr[i] = 0x3086; regValue[i] = 0x1703; i++;     // SEQ_DATA_PORT

    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]21: REG=0x3086,   0x0326  
    regAddr[i] = 0x3086; regValue[i] = 0x26A4; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]22: REG=0x3086,   0xA417  
    regAddr[i] = 0x3086; regValue[i] = 0x171F; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]23: REG=0x3086,   0x1F28  
    regAddr[i] = 0x3086; regValue[i] = 0x2805; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]24: REG=0x3086,   0x0526  
    regAddr[i] = 0x3086; regValue[i] = 0x2620; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]25: REG=0x3086,   0x2028  
    regAddr[i] = 0x3086; regValue[i] = 0x2804; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]26: REG=0x3086,   0x0425  
    regAddr[i] = 0x3086; regValue[i] = 0x2520; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]27: REG=0x3086,   0x2020  
    regAddr[i] = 0x3086; regValue[i] = 0x2027; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]28: REG=0x3086,   0x2700    
    regAddr[i] = 0x3086; regValue[i] = 0x0017; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]29: REG=0x3086,   0x171D  
    regAddr[i] = 0x3086; regValue[i] = 0x1E25; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]30: REG=0x3086,   0x2500    
    regAddr[i] = 0x3086; regValue[i] = 0x0020; i++;     // SEQ_DATA_PORT

    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]31: REG=0x3086,   0x2017  
    regAddr[i] = 0x3086; regValue[i] = 0x2117; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]32: REG=0x3086,   0x1219  
    regAddr[i] = 0x3086; regValue[i] = 0x1028; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]33: REG=0x3086,   0x1703   
    regAddr[i] = 0x3086; regValue[i] = 0x051B; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]34: REG=0x3086,   0x2706  
    regAddr[i] = 0x3086; regValue[i] = 0x1703; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]35: REG=0x3086,   0x1728  
    regAddr[i] = 0x3086; regValue[i] = 0x2706; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]36: REG=0x3086,   0x2805  
    regAddr[i] = 0x3086; regValue[i] = 0x1703; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]37: REG=0x3086,   0x171A  
    regAddr[i] = 0x3086; regValue[i] = 0x1747; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]38: REG=0x3086,   0x2660  
    regAddr[i] = 0x3086; regValue[i] = 0x2660; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]39: REG=0x3086,   0x175A  
    regAddr[i] = 0x3086; regValue[i] = 0x17AE; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]40: REG=0x3086,   0x2317  
    regAddr[i] = 0x3086; regValue[i] = 0x2500; i++;     // SEQ_DATA_PORT

    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]41: REG=0x3086,   0x1122 
    regAddr[i] = 0x3086; regValue[i] = 0x9027; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]42: REG=0x3086,   0x1741 
    regAddr[i] = 0x3086; regValue[i] = 0x0026; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]43: REG=0x3086,   0x2500 
    regAddr[i] = 0x3086; regValue[i] = 0x1828; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]47: REG=0x3086,   0x002E    
    regAddr[i] = 0x3086; regValue[i] = 0x002E; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]48: REG=0x3086,   0x2A28  
    regAddr[i] = 0x3086; regValue[i] = 0x2A28; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]49: REG=0x3086,   0x081C   
    regAddr[i] = 0x3086; regValue[i] = 0x081E; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]50: REG=0x3086,   0x1470  
    regAddr[i] = 0x3086; regValue[i] = 0x0831; i++;     // SEQ_DATA_PORT

    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]51: REG=0x3086,   0x7003  
    regAddr[i] = 0x3086; regValue[i] = 0x1440; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]52: REG=0x3086,   0x1470  
    regAddr[i] = 0x3086; regValue[i] = 0x4014; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]53: REG=0x3086,   0x7004  
    regAddr[i] = 0x3086; regValue[i] = 0x2020; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]54: REG=0x3086,   0x1470  
    regAddr[i] = 0x3086; regValue[i] = 0x1410; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]55: REG=0x3086,   0x7005  
    regAddr[i] = 0x3086; regValue[i] = 0x1034; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]56: REG=0x3086,   0x1470  
    regAddr[i] = 0x3086; regValue[i] = 0x1400; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]57: REG=0x3086,   0x7009  
    regAddr[i] = 0x3086; regValue[i] = 0x1014; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]58: REG=0x3086,   0x170C  
    regAddr[i] = 0x3086; regValue[i] = 0x0020; i++;     // SEQ_DATA_PORT

    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]61: REG=0x3086,   0x0014  
    regAddr[i] = 0x3086; regValue[i] = 0x1400; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]62: REG=0x3086,   0x0050  
    regAddr[i] = 0x3086; regValue[i] = 0x4013; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]63: REG=0x3086,   0x0314  
    regAddr[i] = 0x3086; regValue[i] = 0x1802; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]64: REG=0x3086,   0x0020  
    regAddr[i] = 0x3086; regValue[i] = 0x1470; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]65: REG=0x3086,   0x0314  
    regAddr[i] = 0x3086; regValue[i] = 0x7004; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]66: REG=0x3086,   0x0050  
    regAddr[i] = 0x3086; regValue[i] = 0x1470; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]67: REG=0x3086,   0x0414   
    regAddr[i] = 0x3086; regValue[i] = 0x7003; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]68: REG=0x3086,   0x0020    
    regAddr[i] = 0x3086; regValue[i] = 0x1470; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]69: REG=0x3086,   0x0414   
    regAddr[i] = 0x3086; regValue[i] = 0x7017; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]70: REG=0x3086,   0x0050    
    regAddr[i] = 0x3086; regValue[i] = 0x2002; i++;     // SEQ_DATA_PORT

    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]71: REG=0x3086,   0x0514   
    regAddr[i] = 0x3086; regValue[i] = 0x1400; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]72: REG=0x3086,   0x0020    
    regAddr[i] = 0x3086; regValue[i] = 0x2002; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]73: REG=0x3086,   0x2405  
    regAddr[i] = 0x3086; regValue[i] = 0x1400; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]74: REG=0x3086,   0x1400  
    regAddr[i] = 0x3086; regValue[i] = 0x5004; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]75: REG=0x3086,   0x5001  
    regAddr[i] = 0x3086; regValue[i] = 0x1400; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]76: REG=0x3086,   0x2550  
    regAddr[i] = 0x3086; regValue[i] = 0x2004; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]77: REG=0x3086,   0x502D  
    regAddr[i] = 0x3086; regValue[i] = 0x1400; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]78: REG=0x3086,   0x2608  
    regAddr[i] = 0x3086; regValue[i] = 0x5022; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]79: REG=0x3086,   0x280D  
    regAddr[i] = 0x3086; regValue[i] = 0x0314; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]80: REG=0x3086,   0x1709  
    regAddr[i] = 0x3086; regValue[i] = 0x0020; i++;     // SEQ_DATA_PORT

    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]81: REG=0x3086,   0x2600  
    regAddr[i] = 0x3086; regValue[i] = 0x0314; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]82: REG=0x3086,   0x2805  
    regAddr[i] = 0x3086; regValue[i] = 0x0050; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]83: REG=0x3086,   0x26A7  
    regAddr[i] = 0x3086; regValue[i] = 0x2C2C; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]84: REG=0x3086,   0x2807  
    regAddr[i] = 0x3086; regValue[i] = 0x2C2C; i++;     // SEQ_DATA_PORT

    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]84: REG=0x3086,   0x2807  
    regAddr[i] = 0x309E; regValue[i] = 0x0000; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]84: REG=0x3086,   0x2807  
    regAddr[i] = 0x30E4; regValue[i] = 0x6372; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]84: REG=0x3086,   0x2807  
    regAddr[i] = 0x30E2; regValue[i] = 0x7253; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]84: REG=0x3086,   0x2807  
    regAddr[i] = 0x30E0; regValue[i] = 0x5470; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]84: REG=0x3086,   0x2807  
    regAddr[i] = 0x30E6; regValue[i] = 0xC4CC; i++;     // SEQ_DATA_PORT
    // [A-1000ERS Rev 4 Combined Sequencer April 1 2011]84: REG=0x3086,   0x2807  
    regAddr[i] = 0x30E8; regValue[i] = 0x8050; i++;     // SEQ_DATA_PORT

    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }

    i = 0;
    OSA_waitMsecs(200);

    // [Linear Mode Full Resolution]1: LOAD=Linear Mode Setup
    // [Linear Mode Setup]1: FIELD_WR=RESET_REGISTER, 0x10D8    
    regAddr[i] = 0x301A; regValue[i] = 0x10D8; i++;     // RESET_REGISTER
    // [Linear Mode Setup]2: FIELD_WR=OPERATION_MODE_CTRL, 0x0029 
    regAddr[i] = 0x3082; regValue[i] = 0x0029; i++;     // OPERATION_MODE_CTRL

    // [A-1000ERS Rev3 Optimized settings May 9 2011]
    // [A-1000ERS Rev3 Optimized settings]1: REG= 0x301E, 0x00C8 
    regAddr[i] = 0x301E; regValue[i] = 0x00C8; i++;     // DATA_PEDESTAL
    // [A-1000ERS Rev3 Optimized settings]2: REG= 0x3EDA, 0x0F03 
    regAddr[i] = 0x3EDA; regValue[i] = 0x0F03; i++;     // RESERVED_MFR_3EDA
    // [A-1000ERS Rev3 Optimized settings]3: REG= 0x3EDE, 0xC005
    regAddr[i] = 0x3EDE; regValue[i] = 0xC005; i++;     // RESERVED_MFR_3EDE
    // [A-1000ERS Rev3 Optimized settings]4: REG= 0x3ED8, 0x01EF
    regAddr[i] = 0x3ED8; regValue[i] = 0x01EF; i++;     // RESERVED_MFR_3ED8
    // [A-1000ERS Rev3 Optimized settings]5: REG= 0x3EE2, 0xA46B
    regAddr[i] = 0x3EE2; regValue[i] = 0xA46B; i++;     // RESERVED_MFR_3EE2 
    // [A-1000ERS Rev3 Optimized settings]6: REG= 0x3EE0, 0x047D
    regAddr[i] = 0x3EE0; regValue[i] = 0x047D; i++;      // RESERVED_MFR_3EE0 
    // [A-1000ERS Rev3 Optimized settings]7: REG= 0x3EDC, 0x0070 
    regAddr[i] = 0x3EDC; regValue[i] = 0x0070; i++;      // RESERVED_MFR_3EDC
    // [A-1000ERS Rev3 Optimized settings]8: REG= 0x3044, 0x0404
    regAddr[i] = 0x3044; regValue[i]= 0x0404; i++;       // DARK_CONTROL 
    // [A-1000ERS Rev3 Optimized settings]9: REG= 0x3EE6, 0x4303
    regAddr[i] = 0x3EE6; regValue[i] = 0x4303; i++;      // RESERVED_MFR_3EE6 
    // [A-1000ERS Rev3 Optimized settings]10: REG= 0x3EE4, 0xD308
    regAddr[i] = 0x3EE4; regValue[i] = 0xD308; i++;      // DAC_LD_24_25

    // [A-1000ERS Rev3 Optimized settings]11: REG= 0x3ED6, 0x00BD
    regAddr[i] = 0x3ED6; regValue[i] = 0x00BD; i++;     // RESERVED_MFR_3ED6
    // [A-1000ERS Rev3 Optimized settings]12: REG= 0x3EE6, 0x8303
    regAddr[i] = 0x3EE6; regValue[i] = 0x8303; i++;     // RESERVED_MFR_3EE6
    // [A-1000ERS Rev3 Optimized settings]13: REG= 0x30E4, 0x6372
    regAddr[i] = 0x30E4; regValue[i] = 0x6372; i++;     // RESERVED_MFR_30E4
    // [A-1000ERS Rev3 Optimized settings]14: REG=0x30E2,   0x7253
    regAddr[i] = 0x30E2; regValue[i] = 0x7253; i++;     // RESERVED_MFR_30E2
    // [A-1000ERS Rev3 Optimized settings]15: REG=0x30E0,   0x5470
    regAddr[i] = 0x30E0; regValue[i] = 0x5470; i++;     // RESERVED_MFR_30E0
    // [A-1000ERS Rev3 Optimized settings]16: REG=0x30E6,   0xC4CC
    regAddr[i] = 0x30E6; regValue[i] = 0xC4CC; i++;     // RESERVED_MFR_30E6
    // [A-1000ERS Rev3 Optimized settings]17: REG=0x30E8,   0x8050
    regAddr[i] = 0x30E8; regValue[i] = 0x8050; i++;     // RESERVED_MFR_30E8


    // [Column Retriggering at start up]
    regAddr[i] = 0x30B0; regValue[i] = 0x1300; i++;     // DIGITAL_TEST 
    regAddr[i] = 0x30D4; regValue[i] = 0xE007; i++;     // COLUMN_CORRECTION 
    regAddr[i] = 0x30BA; regValue[i] = 0x0008; i++;     // DIGITAL_CTRL  
    regAddr[i] = 0x301A; regValue[i] = 0x10DC; i++;     // RESET_REGISTER

    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }

    i = 0;
    OSA_waitMsecs(200);

    regAddr[i] = 0x301A; regValue[i] = 0x10D8; i++;  // RESET_REGISTER
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }

    i = 0;
    OSA_waitMsecs(200);

    // [Linear Mode Devware Color Setup]1: REG= 0x3058, 0x003F  
//    regAddr[i] = 0x3058; regValue[i] = 0x003F; i++;     // BLUE_GAIN
    // [Linear Mode Devware Color Setup]2: REG= 0x3012, 0x02A0
    regAddr[i] = 0x3012; regValue[i] = 0x02A0; i++;     // COARSE_INTEGRATION_TIME

    // [Full Resolution 45FPS Setup]
    // [Register Wizard Defaults]1: REG= 0x3032, 0x0000   
    regAddr[i] = 0x3032; regValue[i] = 0x0000; i++;     // DIGITAL_BINNING

    regAddr[i] = 0x3002; regValue[i] = 0x0002; i++;     // Y_ADDR_START
    regAddr[i] = 0x3006; regValue[i] = Y_ADDR_END; i++; // Y_ADDR_END

    regAddr[i] = 0x3004; regValue[i] = 0x0000; i++;     // X_ADDR_START
    regAddr[i] = 0x3008; regValue[i] = 0x04FF; i++;     // X_ADDR_END

    // [Register Wizard Defaults]6: REG= 0x300A, 0x03DE  
    regAddr[i] = 0x300A; regValue[i] = FRAME_LENGTH; i++;   // FRAME_LENGTH_LINES
    // [Register Wizard Defaults]7: REG= 0x300C, 0x0672E  
    regAddr[i] = 0x300C; regValue[i] = LINE_LENGTH; i++;    // LINE_LENGTH_PCK

    // [Linear Mode Full Resolution]3: IF_SERIAL=0xCC, 0x13, 0xF0, 8:16, ==0x0C, LOAD= Enable Serial HiSpi Mode ,ELSELOAD = Enable Parallel Mode 
    // [Enable Parallel Mode]1: REG=0x301A, 0x10D8 
    regAddr[i] = 0x301A; regValue[i] = 0x10D8; i++;     // RESET_REGISTER
    // [Enable Parallel Mode]2: REG=0x31D0, 0x0001 
    regAddr[i] = 0x31D0; regValue[i] = 0x0001; i++;     // HDR_COMP
    // [Enable Parallel Mode]3: Load=PLL Enabled 27Mhz to 74.25Mhz
    regAddr[i] = 0x302C; regValue[i] = PLL_P1; i++;     // VT_SYS_CLK_DIV    P1 divider in PLL
    regAddr[i] = 0x302A; regValue[i] = PLL_P2; i++;     // VT_PIX_CLK_DIV     P2 clock divider in PLL
    regAddr[i] = 0x302E; regValue[i] = PLL_N; i++;  // PRE_PLL_CLK_DIV    shows the n+1 value
    regAddr[i] = 0x3030; regValue[i] = PLL_M; i++;  // PLL_MULTIPLIER      shows 2m value
    regAddr[i] = 0x30B0; regValue[i] = 0x1380; i++;     // DIGITAL_TEST

    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }

    i = 0;
    OSA_waitMsecs(100);

//    regAddr[i] = 0x3070; regValue[i] = 0x0003; i++;
//    regAddr[i] = 0x3070; regValue[i] = 0x0003; i++;
    // [Enable Embedded Data and Stats]
    regAddr[i] = 0x3064; regValue[i] = 0x1982; i++;     // EMBEDDED_DATA_CTRL
    regAddr[i] = 0x3064; regValue[i] = 0x1982; i++;     // EMBEDDED_DATA_CTRL
    
    // [Linear Mode Devware Color Setup]
    regAddr[i] = 0x3056; regValue[i] = 0x0022; i++;     // BLUE_GAIN
    regAddr[i] = 0x3058; regValue[i] = 0x0030; i++;     // BLUE_GAIN
    regAddr[i] = 0x305A; regValue[i] = 0x0020; i++;     // BLUE_GAIN
    regAddr[i] = 0x305C; regValue[i] = 0x0022; i++;     // BLUE_GAIN
    
    //Enable BLC
    //regAddr[i] = 0x30EA; regValue[i] = 0x8C00; i++;   // Enable BLC  tYs comment

    // [Linear Mode Full Resolution]8: REG= 0x301A, 0x10D
    regAddr[i] = 0x301A; regValue[i] = 0x10DC; i++;     // RESET_REGISTER

    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }

    return status;
  #endif
}   

int DRV_imgsSetMirror(Bool flipH, Bool flipV)
{
    int status = OSA_SOK;
    Uint16 regAddr  = 0;
    Uint16 regValue = 0;

    regAddr = AR0130_READ_MODE;
    if (flipH) 
    {
        regValue &= ~(1 << 14);
    }
    else
    {
        regValue |= 1 << 14;
    }

    if (flipV) 
    {
        regValue &= ~(1 << 15);
    }
    else
    {
        regValue |= 1 << 15;
    }
    status += DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    
    //Disable streaming (R0x301A[2]=0)
    regAddr  = AR0130_ENABLE_STREAMING;
    regValue = 0x19d4 & ~(1 << 2);
    status += DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
   
    //Disable column correction (R0x30D4[15]=0)
    regAddr  = 0x30D4;
    regValue = 0xE007 & ~(1 << 15);
    status += DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);

    //Enable streaming (R0x301A[2]=1)
    regAddr  = AR0130_ENABLE_STREAMING;
    regValue = 0x19d4;
    status += DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);

    //Wait one frame time, or more
    OSA_waitMsecs(50);

    //Disable streaming (R0x301A[2]=0)
    regAddr  = AR0130_ENABLE_STREAMING;
    regValue = 0x19d4 & ~(1 << 2);
    status += DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);

    //Enable column correction (R30D4[15]=1)
    regAddr  = 0x30D4;
    regValue = 0xE007;
    status += DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);

    //Enable streaming (R0x301A[2]=1)
    regAddr  = AR0130_ENABLE_STREAMING;
    regValue = 0x19d4;
    status += DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    
    //Bayer resampling must be enabled, by setting bit 4 of register 0 x 306E[4] = 1.
    regAddr  = 0x306E;
    regValue = 0x9011;
    status += DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);

    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_imgsSetMirror Error!!!\n");
        return status;
    }
    else
    {
        OSA_printf("DRV_imgsSetMirror OK!!! flipH = %d, flipV = %d\n", flipH, flipV);
    }
    
    return OSA_SOK;
}

int DRV_imgsSpecificSetting(void)
{
    Uint16 regAddr[50];
    Uint16 regValue[50];
    int i      = 0;
    int status = 0;

#if 0 //HDR mode
    regAddr[i] = 0x301a; regValue[i] = 0x58; i++;   // Disable Streaming

    regAddr[i] = 0x3002; regValue[i] = 0x002; i++;  //Row Start (A) = 2
    regAddr[i] = 0x3004; regValue[i] = 0x000; i++;  //Column Start (A) = 0
    regAddr[i] = 0x3006; regValue[i] = 0x2d1; i++;  //Row End (A) = 969
    regAddr[i] = 0x3008; regValue[i] = 0x4ff; i++;  //Column End (A) = 1283
    regAddr[i] = 0x300A; regValue[i] = 0x02ee; i++; //Frame Lines (A) = 995
    regAddr[i] = 0x300C; regValue[i] = LINE_LENGTH; i++;    //Line Length = 1650
    regAddr[i] = 0x3012; regValue[i] = 0x0174; i++; //Coarse_IT_Time (A) = 745
    regAddr[i] = 0x3014; regValue[i] = 0x0000; i++; //Fine_IT_Time (A) = 701
    regAddr[i] = 0x3016; regValue[i] = 0x0174; i++; //Coarse_IT_Time (B) = 745
    regAddr[i] = 0x3018; regValue[i] = 0x0000; i++; //Fine_IT_Time (B) = 701
    regAddr[i] = 0x3028; regValue[i] = 0x10; i++;   //ROW_SPEED = 16
    regAddr[i] = 0x302A; regValue[i] = PLL_P2;    i++;  //VT_PIX_CLK_DIV = 16
    regAddr[i] = 0x302C; regValue[i] = PLL_P1;    i++;  //VT_SYS_CLK_DIV = 1
    regAddr[i] = 0x302E; regValue[i] = PLL_N + 1; i++;  //PRE_PLL_CLK_DIV = 2
    regAddr[i] = 0x3030; regValue[i] = PLL_M * 2; i++;  //PLL_MULTIPLIER = 44
    regAddr[i] = 0x3032; regValue[i] = 0x0000; i++; //SCALING_MODE = 2
    regAddr[i] = 0x3040; regValue[i] = 0x0000; i++; //READ_MODE = 0
    regAddr[i] = 0x3044; regValue[i] = 0x0404; i++; //Dark Control = 1028

    regAddr[i] = 0x305E; regValue[i] = 0x0020; i++; //Global gain = 1028

    regAddr[i] = 0x3064; regValue[i] = 0x1982; i++; //Dark Control = 1028

    regAddr[i] = 0x306e; regValue[i] = 0x9011; i++; //Dark Control = 1028

    regAddr[i] = 0x3082; regValue[i] = 0x28; i++;   //Column Start (B) = 0 //
    regAddr[i] = 0x3084; regValue[i] = 0x28; i++;   //Column Start (B) = 0
    regAddr[i] = 0x308A; regValue[i] = 0x00; i++;   //Column Start (B) = 0
    regAddr[i] = 0x308C; regValue[i] = 0x0002; i++; //Row Start (B) = 2
    regAddr[i] = 0x308E; regValue[i] = 0x4ff; i++;  //Column End (B) = 1283
    regAddr[i] = 0x3090; regValue[i] = 0x2d1; i++;  //Row End (B) = 969
    regAddr[i] = 0x30A6; regValue[i] = 0x01; i++;   //Y Odd Inc. (A) = 1
    regAddr[i] = 0x30A8; regValue[i] = 0x01; i++;   //Y Odd Inc. (B) = 1
    regAddr[i] = 0x30AA; regValue[i] = 0x02ee; i++; //Frame Lines (B) = 995
    regAddr[i] = 0x30B0; regValue[i] = 0x1000; i++; //DIGITAL_TEST = 4096

    regAddr[i] = 0x3100; regValue[i] = 0x0000; i++; //DIGITAL_TEST = 4096

    regAddr[i] = 0x301a; regValue[i] = 0x19d4; i++;   // Enable Streaming

    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
    if (status!=OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
#else
    /* LINEAR MODE */
    /* Disable Streaming */
    regAddr[i]  = AR0130_ENABLE_STREAMING;
    regValue[i] = 0x58;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, 1);
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }

    status = AR0130_linear_init_regs();
    if (status != OSA_SOK)
    {
        OSA_ERROR("AR0130_linear_init_regs()\n");
        return status;
    }
    
    /* Enable Streaming */
    regAddr[i]  = AR0130_ENABLE_STREAMING;
    regValue[i] = 0x19d4;
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, 1);
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
#endif
    return status;
}

int DRV_imgsSet50_60Hz(Bool is50Hz)
{
    int fps = 0;
    if (gDRV_imgsObj.curFrameTime.fps == 30 || gDRV_imgsObj.curFrameTime.fps == 25)
    {
        if (is50Hz)
        {
            fps = 25;
        }
        else
        {
            fps = 30;
        }
        DRV_imgsSetFramerate(fps);
    }
    return 0;
}

int DRV_imgsSetFramerate(int fps)
{
    Uint16 regAddr[2];
    Uint16 regValue[2];
    int i = 0;
    int status = 0;
     
    switch(fps)
    {
      case 30:
         i = 0;
         regAddr[i] = 0x300A; regValue[i] = 0x06EE; i++; 	// FRAME_LENGTH_LINES
         regAddr[i] = 0x300C; regValue[i] = 0x056E; i++; 	// LINE_LENGTH_PCK
         break;
      case 25:
         i = 0;
         regAddr[i] = 0x300A; regValue[i] = 0x0851; i++; 	// FRAME_LENGTH_LINES
         regAddr[i] = 0x300C; regValue[i] = 0x056E; i++; 	// LINE_LENGTH_PCK
         break;
      default: 
         break;
    }

    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
    if(status!=OSA_SOK) 
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }

    return status;
}

int DRV_imgsBinEnable(Bool enable)
{
    return OSA_SOK;
}

int DRV_imgsBinMode(int binMode)
{
    return OSA_SOK;
}

int DRV_imgsSetAgain(int again, int setRegDirect)
{
    Uint16 regAddr[4];
    Uint16 regValue[4];
    int status=0;
    regAddr[0] = AR0130_FIRST_AGAIN_ADDR_A;
    regAddr[1] = AR0130_SECOND_AGAIN_ADDR_A;
    regAddr[2] = 0x3100;
    regAddr[3] = AR0130_DGAIN_ADDR_A;

    if (again >= 100000)
    {
        regValue[0] = 0x30;
        regValue[1] = 0xD308;
        regValue[2] = 4;
        regValue[3] = (int)(100000/(28800.0/32));
    }
    else if (again >= 28800)
    {
        regValue[0] = 0x30;
        regValue[1] = 0xD308;
        regValue[2] = 4;
        regValue[3] = (int)(again/(28800.0/32));
    }
    else if (again >= 23040)
    {
        regValue[0] = 0x30;
        regValue[1] = 0xD208;
        regValue[2] = 4;
        regValue[3] = (int)(again/(23040.0/32));
    }
    else if (again >= 11520)
    {
        regValue[0] = 0x20;
        regValue[1] = 0xD208;
        regValue[2] = 4;
        regValue[3] = (int)(again/(11520.0/32));
    }
    else if (again >= 5760)
    {
        regValue[0] = 0x10;
        regValue[1] = 0xD208;
        regValue[2] = 4;
        regValue[3] = (int)(again/(5760.0/32));
    }
    else if (again >= 2880)
    {
        regValue[0] = 0x00;
        regValue[1] = 0xD208;
        regValue[2] = 4;
        regValue[3] = (int)(again/(2880.0/32));
    }
    else if (again >= 2000)
    {
        regValue[0] = 0x10;
        regValue[1] = 0xD208;
        regValue[2] = 0;
        regValue[3] = (int)(again/(2000.0/32));
    }
    else
    {
        regValue[0] = 0x00;
        regValue[1] = 0xD208;
        regValue[2] = 0;
        regValue[3] = (int)(again/(1000.0/32));
    }
    if (regValue[3] > 255)  regValue[3] = 255;
    //OSA_printf("------set a gain = %d, reg=%d, %d\n", again, regValue[0], (regValue[1]&0x0f00)>>8);

    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, 4);
    if(status!=OSA_SOK) {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    return status;
}

int DRV_imgsSetDgain(int dgain)
{
    Uint16 regAddr[1];
    Uint16 regValue[1];
    int status=0;
    regAddr[0] = AR0130_GLOABL_GAIN;
    regValue[0] = dgain >> 5;
    if (regValue[0] > 255)  regValue[0] = 255;

    //OSA_printf("------set d gain = %d, reg=%d\n", dgain, regValue[0]);
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, 1);
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }

    return status;
}

int DRV_imgsSetEshutter(Uint32 eshutterInUsec, int setRegDirect)
{
    int status=0;
    Uint16 regAddr;
    Uint16 regValue;

    regAddr = AR0130_COARSE_IT_TIME_A;
    regValue = eshutterInUsec/ROW_TIME;
    //OSA_printf("------set shutter = %d, reg=%d\n",eshutterInUsec, regValue);
#if 1
    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    if (status!=OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
#endif
    return status;
}

//Read AGain & exposure
int DRV_imgsGetAgain_AR0130(int *again)
{
    Uint16 regAddr;
    Uint16 regValue;
    int status;

    regAddr = AR0130_GLOABL_GAIN;
    status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);

    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Read16()\n");
        return status;
    }

    *again = regValue;
    return status;
}

int DRV_imgsGetEshutter_AR0130(Uint32 *eshutterInUsec)
{
    int status;
    Uint16 regAddr;
    Uint16 regValue;

    regAddr = AR0130_COARSE_IT_TIME_A;
    status = DRV_i2c16Read16(&gDRV_imgsObj.i2cHndl, &regAddr, &regValue, 1);
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }

    *eshutterInUsec = regValue * ROW_TIME;
    return status;
}

int DRV_imgsSetDcSub(Uint32 dcSub, int setRegDirect)
{
    return 0;
}

int DRV_imgsEnable(Bool enable)
{    
    Uint16 regAddr[5];
    Uint16 regValue[5];
    int status = OSA_EFAIL;

    if (enable)
    {
        if (DRV_imgsSpecificSetting())
        {
            OSA_ERROR("DRV_imgsSpecificSetting_AR0130()\n");
            return status;
        }  
    }
    else
    {
        regAddr[0]  = AR0130_ENABLE_STREAMING; 
        regValue[0] = 0x58;   // Disable Streaming
        status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, 1);
        if (status != OSA_SOK)
        {
            OSA_ERROR("DRV_i2c16Write16()\n");
            return status;
        }
    }

    return status;
}

DRV_ImgsModeConfig *DRV_imgsGetModeConfig(int sensorMode)
{
    return &gDRV_imgsObj.curModeConfig;
}

DRV_ImgsIsifConfig *DRV_imgsGetIsifConfig(int sensorMode)
{
    return &gDRV_imgsIsifConfig_Common_AR0130;
}

DRV_ImgsIpipeConfig *DRV_imgsGetIpipeConfig(int sensorMode, int vnfDemoCfg)
{
    if (vnfDemoCfg)
    {
        return &gDRV_imgsIpipeConfig_Vnfdemo_AR0130;
    }
    else
    {
        return &gDRV_imgsIpipeConfig_Common_AR0130;
    }
}

DRV_ImgsH3aConfig *DRV_imgsGetH3aConfig(int sensorMode, int aewbVendor)
{
    if (aewbVendor==1)
    {
        return &gDRV_imgsH3aConfig_Appro_AR0130;
    }
    else if (aewbVendor==2)
    {
        return &gDRV_imgsH3aConfig_TI_AR0130;
    }
    else
    {
        return &gDRV_imgsH3aConfig_Common_AR0130;
    }
}

DRV_ImgsLdcConfig *DRV_imgsGetLdcConfig(int sensorMode, Uint16 ldcInFrameWidth, Uint16 ldcInFrameHeight)
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

int DRV_imgsReset()
{
    return OSA_SOK;
}

int DRV_imgsSetRegs()
{
    int i, status=0;
    static Uint8 regAddr[32];
    static Uint16 regValue[32];

    DRV_ImgsFrameTime *pFrame = &gDRV_imgsObj.curFrameTime;
#ifdef SHOW_IMGSFRAMETIME
    printf("pFrame->row_start = %d\t pFrame->row_size = %d\n"
           "pFrame->col_start = %d\t pFrame->col_size = %d\n", 
           pFrame->row_start, pFrame->row_size,
           pFrame->col_start, pFrame->col_size);
#endif 
    
#if 0
    status = DRV_imgsReset_AR0130();
    if (status!=OSA_SOK)
        return status;

    OSA_waitMsecs(10);

    i=0;

    regAddr[i]  = OUTPUT_CTRL;
    regValue[i] = 0x1F83;
    i++;

    regAddr[i]  = PLL_CTRL;
    regValue[i] = 0x51;
    i++;

    regAddr[i]  = PLL_CFG1;
    regValue[i] = ( (Uint16)(pFrame->pll_M & 0xFF) << 8 )| ((pFrame->pll_N) & 0x3F);
    i++;

    regAddr[i]  = PLL_CFG2;
    regValue[i] = (pFrame->pll_DIV) & 0x1F;
    i++;

    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
    if (status!=OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }

    OSA_waitMsecs(10);

    i=0;

    regAddr[i]  = PLL_CTRL;
    regValue[i] = 0x53;  // enable PLL
    i++;

    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
    if (status!=OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }
    OSA_waitMsecs(10);
    i=0;

    regAddr[i]  = ROW_START;
    regValue[i] = pFrame->row_start;
    i++;

    regAddr[i]  = COL_START;
    regValue[i] = pFrame->col_start;
    i++;

    regAddr[i]  = ROW_SIZE;
    regValue[i] = pFrame->row_size;
    i++;

    regAddr[i]  = COL_SIZE;
    regValue[i] = pFrame->col_size;
    i++;

    regAddr[i]  = HORZ_BLANK;
    regValue[i] = pFrame->HB;
    i++;

    regAddr[i]  = VERT_BLANK;
    regValue[i] = pFrame->VB;
    i++;

    regAddr[i]  = SHUTTER_WIDTH_H;
    regValue[i] = 0;
    i++;

    regAddr[i]  = SHUTTER_WIDTH_L;
    regValue[i] = pFrame->SW;
    i++;

    regAddr[i]  = SHUTTER_DELAY;
    regValue[i] = pFrame->SD;
    i++;

    regAddr[i]  = ROW_ADDR_MODE;
    regValue[i] = (pFrame->row_skip & 0x7 ) | ((pFrame->row_bin & 0x3) << 4);
    i++;

    regAddr[i]  = COL_ADDR_MODE;
    regValue[i] = (pFrame->col_skip & 0x7 ) | ((pFrame->col_bin & 0x3) << 4);
    i++;

    regAddr[i]  = PIXEL_CLK_CTRL;
    regValue[i] = 0x8000;
    i++;

    regAddr[i]  = ROW_BLK_DEF_OFFSET  ;
    regValue[i] = 64;
    i++;

    status = DRV_i2c16Write16(&gDRV_imgsObj.i2cHndl, regAddr, regValue, i);
    if (status!=OSA_SOK)
    {
        OSA_ERROR("DRV_i2c16Write16()\n");
        return status;
    }

    DRV_imgsSetAgain(1000, 0); // default aGain
    DRV_imgsSetEshutter(8333, 0);
#endif
    return status;
}

