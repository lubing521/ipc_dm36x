#include <dev_i2c.h>
#include <linux/config.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>           /* everything... */
#include <linux/cdev.h>
#include <linux/mm.h>
#include <linux/i2c.h>
#include <linux/kernel.h>       /* printk() */
#include <linux/slab.h>         /* kmalloc() */
#include <asm/uaccess.h>        /* copy_*_user */


static unsigned short gI2C_curAddr;

typedef struct
{
    int devAddr;
    struct i2c_client client;   //!< Data structure containing general access routines.
    struct i2c_driver driver;   //!< Data structure containing information specific to each client.
    char name[20];
    int nameSize;
    int users;
} I2C_Obj;

typedef struct
{
    struct cdev cdev;             /* Char device structure    */
    int     major;
    struct semaphore semLock;
    I2C_Obj *pObj[I2C_DEV_MAX_ADDR];
    uint8_t reg[I2C_TRANSFER_BUF_SIZE_MAX];
    uint16_t reg16[I2C_TRANSFER_BUF_SIZE_MAX];
    uint8_t buffer[I2C_TRANSFER_BUF_SIZE_MAX*4];
} I2C_Dev;

I2C_Dev gI2C_dev;

int I2C_detectClient(struct i2c_adapter *adapter, int address)
{
    I2C_Obj *pObj;
    struct i2c_client *client;
    int err = 0;

#ifdef I2C_DEBUG
    printk(KERN_INFO "I2C: I2C_detectClient() at address %x ...\n", address);
#endif

    if (address > I2C_DEV_MAX_ADDR)
    {
        printk( KERN_ERR "I2C: ERROR: Invalid device address %x\n", address);
        return -1;
    }

    pObj = gI2C_dev.pObj[address];
    if (pObj == NULL)
    {
        printk( KERN_ERR "I2C: ERROR: Object not found for address %x\n", address);
        return -1;
    }

    client = &pObj->client;

    if (client->adapter)
    {
        return -EBUSY;  /* our client is already attached */
    }

    memset(client, 0x00, sizeof(struct i2c_client));
    client->addr    = pObj->devAddr;
    client->adapter = adapter;
    client->driver  = &pObj->driver;

#ifdef I2C_DEBUG
    printk(KERN_INFO "I2C: i2c_attach_client() ...\n");
#endif

    if ((err = i2c_attach_client(client)))
    {
        printk( KERN_ERR "I2C: ERROR: Couldn't attach %s (address=%x)\n", pObj->name, pObj->devAddr);
        client->adapter = NULL;
        return err;
    }

#ifdef I2C_DEBUG
    printk( KERN_INFO "I2C: %s client registered at address %x !\n", pObj->name, pObj->devAddr );
#endif

    return 0;
}

int I2C_detachClient(struct i2c_client *client)
{
    int err;

    if (!client->adapter)
    {
        return -ENODEV; /* our client isn't attached */
    }

    if ((err = i2c_detach_client(client)))
    {
        printk( KERN_ERR "Client deregistration failed (address=%x), client not detached.\n", client->addr);
        return err;
    }

    client->adapter = NULL;

    return 0;
}

int I2C_attachAdapter(struct i2c_adapter *adapter)
{
#ifdef I2C_DEBUG
    printk(KERN_INFO "I2C: I2C_attachAdapter() ...\n");
#endif

    return I2C_detectClient(adapter, gI2C_curAddr);
}

void *I2C_create(int devAddr)
{
    int ret;
    struct i2c_driver *driver;
    struct i2c_client *client = client;

    I2C_Obj *pObj = NULL;

#ifdef I2C_DEBUG
    printk(KERN_INFO "I2C: Driver registration in progress for address %x...\n", devAddr);
#endif

    devAddr >>= 1;

    if (devAddr > I2C_DEV_MAX_ADDR)
    {
        return NULL;
    }

    if (gI2C_dev.pObj[devAddr] != NULL)
    {
        // already allocated, increment user count, and return the allocated handle
        gI2C_dev.pObj[devAddr]->users++;
        return gI2C_dev.pObj[devAddr];
    }

    pObj = (void*)kmalloc( sizeof(I2C_Obj), GFP_KERNEL);

    gI2C_dev.pObj[devAddr] = pObj;

    memset(pObj, 0, sizeof(I2C_Obj));

    pObj->client.adapter = NULL;
    pObj->users++;

    pObj->devAddr = devAddr;

    gI2C_curAddr = pObj->devAddr;

    driver = &pObj->driver;

    pObj->nameSize=0;
    pObj->name[pObj->nameSize++] = 'I';
    pObj->name[pObj->nameSize++] = '2';
    pObj->name[pObj->nameSize++] = 'C';
    pObj->name[pObj->nameSize++] = '_';
    pObj->name[pObj->nameSize++] = 'A' + ((pObj->devAddr >> 0) & 0xF);
    pObj->name[pObj->nameSize++] = 'B' + ((pObj->devAddr >> 4) & 0xF);
    pObj->name[pObj->nameSize++] = 0;

    driver->driver.name = pObj->name;
    driver->id = I2C_DRIVERID_MISC;
    driver->attach_adapter = I2C_attachAdapter;
    driver->detach_client = I2C_detachClient;

#ifdef I2C_DEBUG
    printk(KERN_INFO "I2C: i2c_add_driver() ...\n");
#endif

    if ((ret = i2c_add_driver(driver)))
    {
        printk( KERN_ERR "I2C: ERROR: Driver registration failed (address=%x), module not inserted.\n", pObj->devAddr);
    }

#ifdef I2C_DEBUG
    printk(KERN_INFO "I2C: Driver registration successful (address=%x)\n", pObj->devAddr);
#endif

    if (ret < 0)
    {
        gI2C_dev.pObj[pObj->devAddr] = NULL;
        kfree(pObj);
        return NULL;
    }

    return pObj;
}

int I2C_delete(I2C_Obj *pObj)
{
    if (pObj == NULL)
    {
        return -1;
    }

    pObj->users--;

    if (pObj->users <= 0)
    {
        if (pObj->client.adapter!=NULL)
        {
            if (i2c_del_driver(&pObj->driver))
            {
                printk(KERN_ERR "I2C: ERROR: Driver remove failed (address=%x), module not removed.\n", pObj->devAddr);
            }
            pObj->client.adapter = NULL;
            gI2C_dev.pObj[pObj->devAddr] = NULL;
            kfree(pObj);
        }
    }

    return 0;
}

int I2C_read(I2C_Obj *pObj, uint8_t *reg, uint8_t *buffer, uint8_t count, uint8_t dataSize)
{
    uint8_t i;
    int err;
    struct i2c_client *client;
    struct i2c_msg msg[1];
    unsigned char data[1];

    if (pObj == NULL)
    {
        return -ENODEV;
    }

    client = &pObj->client;
    if (!client->adapter)
    {
        return -ENODEV;
    }

    if (dataSize <= 0 || dataSize > 4)
    {
        return -1;
    }

    for (i = 0; i < count; i++)
    {
        msg->addr  = client->addr;
        msg->flags = 0;
        msg->len   = 1;
        msg->buf   = data;
        data[0]    = reg[i];
        err = i2c_transfer(client->adapter, msg, 1);
        if (err < 0)
        {
            printk( KERN_ERR " i2c_transfer(0x%x, 0x%x)\n", msg->addr, msg->buf[0]);
        }

        if (err >= 0)
        {
            msg->flags = I2C_M_RD;
            msg->len   = dataSize;
            err = i2c_transfer(client->adapter, msg, 1);
            if (err >= 0)
            {
                if (dataSize == 1)
                {
                    buffer[i] = data[0];
                }
                else if (dataSize == 2)
                {
                    buffer[2 * i]     = data[1];
                    buffer[2 * i + 1] = data[0];
                }
            }
        }
        if (err < 0)
        {
            return -ENODEV;
        }
    }

    return 0;
}

int I2C_write(I2C_Obj *pObj, uint8_t *reg, uint8_t *buffer, uint8_t count, uint8_t dataSize)
{
    uint8_t i;
    int err;
    struct i2c_client *client;
    struct i2c_msg msg[1];
    unsigned char data[8];

    if (pObj == NULL)
    {
        return -ENODEV;
    }

    client = &pObj->client;
    if (!client->adapter)
    {
        return -ENODEV;
    }

    if (dataSize <= 0 || dataSize > 4)
    {
        return -1;
    }

    for (i = 0; i < count; i++)
    {
        msg->addr = client->addr;
        msg->flags= 0;
        msg->buf  = data;

        data[0] = reg[i];

        if (dataSize == 1)
        {
            data[1]  = buffer[i];
            msg->len = 2;
        }
        else if (dataSize == 2)
        {
            data[1] = buffer[2*i+1];
            data[2] = buffer[2*i];
            msg->len = 3;
        }
        err = i2c_transfer(client->adapter, msg, 1);
        if ( err < 0 )
        {
            return err;
        }
    }

    return 0;
}

int I2C_read16(I2C_Obj *pObj, uint16_t *reg, uint8_t *buffer, uint8_t count, uint8_t dataSize)
{
    uint8_t i;
    int err;
    struct i2c_client *client;
    struct i2c_msg msg[1];
    unsigned char data[2];

    if (pObj == NULL)
    {
        return -ENODEV;
    }

    client = &pObj->client;
    if (!client->adapter)
    {
        return -ENODEV;
    }

    if (dataSize <= 0 || dataSize > 4)
    {
        return -1;
    }

    for (i = 0; i < count; i++)
    {
        msg->addr  = client->addr;
        msg->flags = 0;
        msg->len   = 2;
        msg->buf   = data;
        data[0]    = reg[i]>>8;
        data[1]    = reg[i];
        err = i2c_transfer(client->adapter, msg, 1);
        if (err < 0)
        {
#ifdef DEBUG
            printk( KERN_ERR " i2c16_transfer(0x%x, 0x%x)\n", msg->addr, msg->buf[0]);
#endif
        }

        if (err >= 0)
        {
            msg->flags = I2C_M_RD;
            msg->len   = dataSize;
            err = i2c_transfer(client->adapter, msg, 1);
            if (err >= 0)
            {
                if (dataSize==1)
                {
                    buffer[i] = data[0];
                }
                else if (dataSize == 2)
                {
                    buffer[2*i]   = data[1];
                    buffer[2*i+1] = data[0];
                }
                else if (dataSize == 4)
                {
                    buffer[4*i+3] = data[0];
                    buffer[4*i+2] = data[1];
                    buffer[4*i+1] = data[2];
                    buffer[4*i]   = data[3];
                }
            }
        }
        if (err < 0)
        {
            return -ENODEV;
        }
    }

    return 0;
}

int I2C_write16(I2C_Obj *pObj, uint16_t *reg, uint8_t *buffer, uint8_t count, uint8_t dataSize)
{
    uint8_t i;
    int err;
    struct i2c_client *client;
    struct i2c_msg msg[1];
    unsigned char data[8];

    if (pObj == NULL)
    {
        return -ENODEV;
    }

    client = &pObj->client;
    if (!client->adapter)
    {
        return -ENODEV;
    }

    if (dataSize <= 0 || dataSize > 4)
    {
        return -1;
    }

    for (i = 0; i < count; i++)
    {
        msg->addr = client->addr;
        msg->flags= 0;
        msg->buf  = data;

        data[0] = reg[i]>>8;
        data[1] = reg[i];

        if (dataSize == 1)
        {
            data[2]  = buffer[i];
            msg->len = 3;
        }
        else if (dataSize == 2)
        {
            data[2] = buffer[2*i+1];
            data[3] = buffer[2*i];
            msg->len = 4;
        }
        else if(dataSize == 4)
        {
            data[2] = buffer[4*i+3];
            data[3] = buffer[4*i+2];
            data[4] = buffer[4*i+1];
            data[5] = buffer[4*i];
            msg->len = 6;
        }
        err = i2c_transfer(client->adapter, msg, 1);
        if ( err < 0 )
        {
            return err;
        }
    }

    return 0;
}

int I2C_devOpen(struct inode *inode, struct file *filp)
{
    int minor, major;

    minor = iminor(inode);
    major = imajor(inode);

#ifdef I2C_DEBUG
    printk(KERN_INFO "I2C: I2C_devOpen()   , %4d, %2d \n", major, minor);
#endif

    filp->private_data = NULL;

    return 0;                /* success */
}

int I2C_devRelease(struct inode *inode, struct file *filp)
{
    I2C_Obj *pObj;

    pObj = (I2C_Obj *)filp->private_data;

    if (pObj==NULL)
        return 0;

#ifdef I2C_DEBUG
    printk(KERN_INFO "I2C: I2C_devRelease(), %x, %d \n", pObj->devAddr, pObj->users);
#endif

    I2C_delete(pObj);

    return 0;
}

int I2C_devIoctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
{
    I2C_Obj *pObj;
    int status = 0;
    I2C_TransferPrm transferPrm;

    pObj = (I2C_Obj *)filp->private_data;

#ifdef I2C_DEBUG
    printk(KERN_INFO "I2C: I2C_devIoctl() \n");
#endif

    if (!I2C_IOCTL_CMD_IS_VALID(cmd))
    {
        return -1;
    }

    cmd = I2C_IOCTL_CMD_GET(cmd);

    down_interruptible(&gI2C_dev.semLock);

    switch (cmd)
    {
    case I2C_CMD_SET_DEV_ADDR:
        filp->private_data = I2C_create(arg);
        if (filp->private_data == NULL)
        {
            status = -1;
        }
        break;

    case I2C_CMD_WRITE:
        status = copy_from_user(&transferPrm, (void *)arg, sizeof(transferPrm));
        if (status == 0)
        {
            if ( transferPrm.regSize == 1 )
            {
                status = copy_from_user(gI2C_dev.reg, transferPrm.reg, transferPrm.count);
                status |= copy_from_user(gI2C_dev.buffer, transferPrm.value, transferPrm.count*transferPrm.dataSize);
                if (status == 0)
                {
                    status = I2C_write(pObj, gI2C_dev.reg, gI2C_dev.buffer, transferPrm.count, transferPrm.dataSize);
                }
            }
            else
            {
                status = copy_from_user(gI2C_dev.reg16, transferPrm.reg16, transferPrm.count*transferPrm.regSize);
                status |= copy_from_user(gI2C_dev.buffer, transferPrm.value, transferPrm.count*transferPrm.dataSize);
                if (status == 0)
                {
                    status = I2C_write16(pObj, gI2C_dev.reg16, gI2C_dev.buffer, transferPrm.count, transferPrm.dataSize);
                }
            }
        }
        break;

    case I2C_CMD_READ:
        status = copy_from_user(&transferPrm, (void *)arg, sizeof(transferPrm));
        if (status == 0)
        {
            if ( transferPrm.regSize == 1 )
            {
                status = copy_from_user(gI2C_dev.reg, transferPrm.reg, transferPrm.count);
                if (status == 0)
                {
                    status = I2C_read(pObj, gI2C_dev.reg, gI2C_dev.buffer, transferPrm.count, transferPrm.dataSize);
                    if (status == 0)
                    {
                        status = copy_to_user(transferPrm.value, gI2C_dev.buffer, transferPrm.count*transferPrm.dataSize);
                    }
                }
            }
            else
            {
                status = copy_from_user(gI2C_dev.reg16, transferPrm.reg16, transferPrm.count*transferPrm.regSize);
                if (status == 0)
                {
                    status = I2C_read16(pObj, gI2C_dev.reg16, gI2C_dev.buffer, transferPrm.count, transferPrm.dataSize);
                    if (status == 0)
                    {
                        status = copy_to_user(transferPrm.value, gI2C_dev.buffer, transferPrm.count*transferPrm.dataSize);
                    }
                }
            }
        }
        break;

    default:
        status = -1;
        break;
    }

    up(&gI2C_dev.semLock);

    return status;
}


struct file_operations gI2C_devFileOps =
{
    .owner   = THIS_MODULE,
    .open    = I2C_devOpen,
    .release = I2C_devRelease,
    .ioctl   = I2C_devIoctl,
};

int I2C_devInit(void)
{
    int     result, i;
    dev_t   dev = 0;

#ifdef I2C_DEBUG
    printk(KERN_INFO "I2C: I2C_devInit() \n");
#endif

    result = alloc_chrdev_region(&dev, 0, 1, I2C_DRV_NAME);

    if (result < 0)
    {
        printk(KERN_WARNING "I2C: can't get device major num \n");
        return result;
    }

    for (i = 0; i < I2C_DEV_MAX_ADDR; i++)
    {
        gI2C_dev.pObj[i]=NULL;
    }

    gI2C_dev.major = MAJOR(dev);

    sema_init(&gI2C_dev.semLock, 1);

    cdev_init(&gI2C_dev.cdev, &gI2C_devFileOps);

    gI2C_dev.cdev.owner = THIS_MODULE;
    gI2C_dev.cdev.ops = &gI2C_devFileOps;

    result = cdev_add(&gI2C_dev.cdev, dev, 1);
    if (result)
    {
        printk(KERN_WARNING "I2C: Error [%d] while adding device [%s] \n", result, I2C_DRV_NAME);
    }

    printk(KERN_INFO "I2C: Module install successful, device major num = %d \n", gI2C_dev.major);

    return result;
}

void I2C_devExit(void)
{
    dev_t devno = MKDEV(gI2C_dev.major, 0);

#ifdef I2C_DEBUG
    printk(KERN_INFO "I2C: I2C_devExit() \n");
#endif

    cdev_del(&gI2C_dev.cdev);
    unregister_chrdev_region(devno, 1);
}



