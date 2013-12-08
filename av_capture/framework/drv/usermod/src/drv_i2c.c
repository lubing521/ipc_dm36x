#include <drv_i2c.h>
#include <dev_i2c.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

int DRV_i2cOpen(DRV_I2cHndl *hndl, Uint8 devAddr)
{
    char deviceName[20];
    Uint32 cmd;
    int status;

    sprintf(deviceName, "/dev/%s", I2C_DRV_NAME);

    hndl->fd = open(deviceName, O_RDWR);
    if (hndl->fd < 0)
    {
        return OSA_EFAIL;
    }

    cmd = I2C_IOCTL_CMD_MAKE(I2C_CMD_SET_DEV_ADDR);
    status = ioctl(hndl->fd, cmd, devAddr);
    if (status < 0)
    {
        close(hndl->fd);
    }

    return status;
}

int DRV_i2cRead8(DRV_I2cHndl *hndl, Uint8 *reg, Uint8 *value, Uint8 count)
{
    I2C_TransferPrm prm;
    Uint32 cmd;
    int status;

    prm.dataSize = 1;
    prm.regSize  = 1;
    prm.reg      = reg;
    prm.count    = count;
    prm.value    = value;

    cmd = I2C_IOCTL_CMD_MAKE(I2C_CMD_READ);
    status = ioctl(hndl->fd, cmd, &prm);

    return status;
}

int DRV_i2cWrite8(DRV_I2cHndl *hndl, Uint8 *reg, Uint8 *value, Uint8 count)
{
    I2C_TransferPrm prm;
    Uint32 cmd;
    int status;

    prm.dataSize = 1;
    prm.regSize  = 1;
    prm.reg      = reg;
    prm.count    = count;
    prm.value    = value;

    cmd = I2C_IOCTL_CMD_MAKE(I2C_CMD_WRITE);
    status = ioctl(hndl->fd, cmd, &prm);

    return status;
}

int DRV_i2cRead16(DRV_I2cHndl *hndl, Uint8 *reg, Uint16 *value, Uint8 count)
{
    I2C_TransferPrm prm;
    Uint32 cmd;
    int status;

    prm.dataSize = 2;
    prm.regSize  = 1;
    prm.reg      = reg;
    prm.count    = count;
    prm.value    = value;

    cmd = I2C_IOCTL_CMD_MAKE(I2C_CMD_READ);
    status = ioctl(hndl->fd, cmd, &prm);

    return status;
}

int DRV_i2cWrite16(DRV_I2cHndl *hndl, Uint8 *reg, Uint16 *value, Uint8 count)
{
    I2C_TransferPrm prm;
    Uint32 cmd;
    int status;

    prm.dataSize = 2;
    prm.regSize  = 1;
    prm.reg      = reg;
    prm.count    = count;
    prm.value    = value;

    cmd = I2C_IOCTL_CMD_MAKE(I2C_CMD_WRITE);
    status = ioctl(hndl->fd, cmd, &prm);

    return status;
}

int DRV_i2c16Read8(DRV_I2cHndl *hndl, Uint16 *reg, Uint8 *value, Uint8 count)
{
    I2C_TransferPrm prm;
    Uint32 cmd;
    int status;

    prm.dataSize = 1;
    prm.regSize  = 2;
    prm.reg16    = reg;
    prm.count    = count;
    prm.value    = value;

    cmd = I2C_IOCTL_CMD_MAKE(I2C_CMD_READ);
    status = ioctl(hndl->fd, cmd, &prm);

    return status;
}

int DRV_i2c16Write8(DRV_I2cHndl *hndl, Uint16 *reg, Uint8 *value, Uint8 count)
{
    I2C_TransferPrm prm;
    Uint32 cmd;
    int status;

    prm.dataSize = 1;
    prm.regSize  = 2;
    prm.reg16    = reg;
    prm.count    = count;
    prm.value    = value;

    cmd = I2C_IOCTL_CMD_MAKE(I2C_CMD_WRITE);
    status = ioctl(hndl->fd, cmd, &prm);

    return status;
}

int DRV_i2c16Read16(DRV_I2cHndl *hndl, Uint16 *reg, Uint16 *value, Uint8 count)
{
    I2C_TransferPrm prm;
    Uint32 cmd;
    int status;
    prm.dataSize = 2;
    prm.regSize  = 2;
    prm.reg16    = reg;
    prm.count    = count;
    prm.value    = value;

    cmd = I2C_IOCTL_CMD_MAKE(I2C_CMD_READ);
    status = ioctl(hndl->fd, cmd, &prm);

    return status;
}

int DRV_i2c16Write16(DRV_I2cHndl *hndl, Uint16 *reg, Uint16 *value, Uint8 count)
{
    I2C_TransferPrm prm;
    Uint32 cmd;
    int status;

    prm.dataSize = 2;
    prm.regSize  = 2;
    prm.reg16    = reg;
    prm.count    = count;
    prm.value    = value;

    cmd = I2C_IOCTL_CMD_MAKE(I2C_CMD_WRITE);
    status = ioctl(hndl->fd, cmd, &prm);

    return status;
}

int DRV_i2c16Read32(DRV_I2cHndl *hndl, Uint16 *reg, Uint32 *value, Uint8 count)
{
    I2C_TransferPrm prm;
    Uint32 cmd;
    int status;
    prm.dataSize = 4;
    prm.regSize  = 2;
    prm.reg16    = reg;
    prm.count    = count;
    prm.value    = value;

    cmd = I2C_IOCTL_CMD_MAKE(I2C_CMD_READ);
    status = ioctl(hndl->fd, cmd, &prm);

    return status;
}

int DRV_i2c16Write32(DRV_I2cHndl *hndl, Uint16 *reg, Uint32 *value, Uint8 count)
{
    I2C_TransferPrm prm;
    Uint32 cmd;
    int status;

    prm.dataSize = 4;
    prm.regSize  = 2;
    prm.reg16    = reg;
    prm.count    = count;
    prm.value    = value;

    cmd = I2C_IOCTL_CMD_MAKE(I2C_CMD_WRITE);
    status = ioctl(hndl->fd, cmd, &prm);

    return status;
}

int DRV_i2cClose(DRV_I2cHndl *hndl)
{
    return close(hndl->fd);
}

int DRV_i2cTestShowUsage(char *str)
{
    OSA_printf(" \n");
    OSA_printf(" I2C Test Utility, \r\n");
    OSA_printf(" Usage: %s -r|-w <devAddrInHex> <regAddrInHex> <regValueInHex or numRegsToReadInDec> <regSizeInBytes> <addrSizeInBytes> \r\n", str);
    OSA_printf(" \n");
    return 0;
}

int DRV_i2cTestMain(int argc, char **argv)
{
    DRV_I2cHndl i2cHndl;
    Uint8 devAddr, numRegs;
    Bool doRead;
    Uint8 dataSize=1, addrSize=1;
    int status, i;

    static Uint8 regAddr[256], regValue8[256];
    static Uint16 regAddr16[256], regValue16[256];

    if (argc < 3)
    {
        DRV_i2cTestShowUsage(argv[0]);
        return -1;
    }

    if (strcmp(argv[1], "-r") == 0)
    {
        doRead = TRUE;
    }
    else if (strcmp(argv[1], "-w") == 0)
    {
        doRead = FALSE;
    }
    else
    {
        DRV_i2cTestShowUsage(argv[0]);
        return -1;
    }

    devAddr = 0;
    numRegs = 4;
    regValue8[0] = 0;
    regValue16[0] = 0;
    regAddr[0] = 0;
    regAddr16[0] = 0;

    if (argc > 2)
        devAddr = xstrtoi(argv[2]);

    if (argc > 3)
        regAddr[0] = xstrtoi(argv[3]);

    if (argc > 5)
        dataSize = atoi(argv[5]);

    if (dataSize>2 || dataSize<=0)
        dataSize=1;

    if (argc > 6)
        addrSize = atoi(argv[6]);

    if (addrSize > 2 || addrSize <= 0)
        addrSize=1;

    if (argc > 3 && addrSize == 2)
        regAddr16[0] = xstrtoi(argv[3]);

    if (argc > 4)
    {
        if (doRead)
        {
            numRegs = atoi(argv[4]);
        }
        else
        {
            if (dataSize == 1)
            {
                regValue8[0] = xstrtoi(argv[4]);
            }
            else if (dataSize == 2)
            {
                regValue16[0] = xstrtoi(argv[4]);
            }
        }
    }

    if (devAddr == 0)
    {
        OSA_printf(" I2C: Invalid device address\n");
        DRV_i2cTestShowUsage(argv[0]);
        return -1;
    }

//  OSA_printf(" I2C: Opening I2C at device address %x ...\n", devAddr);

    status = DRV_i2cOpen(&i2cHndl, devAddr);
    if (status != OSA_SOK)
    {
        OSA_ERROR("DRV_i2cOpen(%d)", devAddr);
        return status;
    }

    if (status == OSA_SOK)
    {
        if (doRead)
        {
            for (i = 1; i < numRegs; i++)
            {
                if (addrSize == 1)
                {
                    regAddr[i] = regAddr[0]+i;
                }
                else
                {
                    if (dataSize == 1)
                    {
                        regAddr16[i] = regAddr16[0]+i;
                    }
                    else
                    {
                        regAddr16[i] = regAddr16[0]+i*2;
                    }
                }
            }

            if (dataSize == 1 && addrSize == 1)
            {
                status = DRV_i2cRead8(&i2cHndl, regAddr, regValue8, numRegs);
            }
            else if (dataSize == 2 && addrSize == 1)
            {
                status = DRV_i2cRead16(&i2cHndl, regAddr, regValue16, numRegs);
            }
            else if (dataSize == 1 && addrSize == 2)
            {
                status = DRV_i2c16Read8(&i2cHndl, regAddr16, regValue8, numRegs);
            }
            else if (dataSize == 2 && addrSize == 2)
            {
                status = DRV_i2c16Read16(&i2cHndl, regAddr16, regValue16, numRegs);
            }

            OSA_printf(" \n");
            if (status == OSA_SOK)
            {
                for (i = 0; i < numRegs; i++)
                {
                    if (dataSize == 1 && addrSize == 1)
                    {
                        OSA_printf(" I2C: 0x%02x = 0x%02x \n", regAddr[i], regValue8[i] );
                    }
                    else if (dataSize == 2 && addrSize == 1)
                    {
                        OSA_printf(" I2C: 0x%02x = 0x%04x \n", regAddr[i], regValue16[i] );
                    }
                    else if (dataSize == 1 && addrSize == 2)
                    {
                        OSA_printf(" I2C: 0x%04x = 0x%02x \n", regAddr16[i], regValue8[i] );
                    }
                    else if (dataSize == 2 && addrSize == 2)
                    {
                        OSA_printf(" I2C: 0x%04x = 0x%04x \n", regAddr16[i], regValue16[i] );
                    }
                }
            }
            else
            {
                OSA_printf(" I2C: Read ERROR !!!\n");
            }
            OSA_printf(" \n");
        }
        else
        {
            OSA_printf(" \n");
            if (dataSize == 1 && addrSize == 1)
            {
                status = DRV_i2cWrite8(&i2cHndl, regAddr, regValue8, 1);
                if (status == OSA_SOK)
                {
                    status = DRV_i2cRead8(&i2cHndl, regAddr, regValue8, 1);
                }
            }
            else if (dataSize == 2 && addrSize == 1)
            {
                status = DRV_i2cWrite16(&i2cHndl, regAddr, regValue16, 1);
                if (status == OSA_SOK)
                {
                    status = DRV_i2cRead16(&i2cHndl, regAddr, regValue16, 1);
                }
            }
            else if (dataSize == 1 && addrSize == 2)
            {
                status = DRV_i2c16Write8(&i2cHndl, regAddr16, regValue8, 1);
                if (status == OSA_SOK)
                {
                    status = DRV_i2c16Read8(&i2cHndl, regAddr16, regValue8, 1);
                }
            }
            else if (dataSize == 2 && addrSize == 2)
            {
                status = DRV_i2c16Write16(&i2cHndl, regAddr16, regValue16, 1);
                if (status == OSA_SOK)
                {
                    status = DRV_i2c16Read16(&i2cHndl, regAddr16, regValue16, 1);
                }
            }

            if (status == OSA_SOK)
            {
                if (dataSize == 1 && addrSize == 1)
                {
                    OSA_printf(" I2C: 0x%02x = 0x%02x \n", regAddr[0], regValue8[0] );
                }
                else if (dataSize == 2 && addrSize == 1)
                {
                    OSA_printf(" I2C: 0x%02x = 0x%04x \n", regAddr[0], regValue16[0] );
                }
                else if (dataSize == 1 && addrSize == 2)
                {
                    OSA_printf(" I2C: 0x%04x = 0x%02x \n", regAddr16[0], regValue8[0] );
                }
                else if (dataSize == 2 && addrSize == 2)
                {
                    OSA_printf(" I2C: 0x%04x = 0x%04x \n", regAddr16[0], regValue16[0] );
                }
            }
            else
            {
                OSA_printf(" I2C: Write ERROR !!!\n");
            }
            OSA_printf(" \n");
        }
        DRV_i2cClose(&i2cHndl);
    }

    return 0;
}
