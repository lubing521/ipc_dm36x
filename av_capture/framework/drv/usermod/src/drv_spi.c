

#include <drv_spi.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

static char *device = "/dev/spidev0.0";
static uint8_t mode = 16;
static uint8_t bits = 8;
static uint32_t speed = 12000000;
static uint16_t delay = 0;

int DRV_SPIOpen(DRV_SPIHndl *hndl, Uint8 devAddr)
{


	int ret;
	int fd;

	hndl->fd = fd =open(device, O_RDWR);
	if(hndl->fd<0)
		return OSA_EFAIL;

	ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
	if (ret == -1)
		OSA_ERROR("can't set spi mode");

	ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
	if (ret == -1)
		OSA_ERROR("can't get spi mode");

	/*
	 * bits per word
	 */
	ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
	if (ret == -1)
		OSA_ERROR("can't set bits per word");

	ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
	if (ret == -1)
		OSA_ERROR("can't get bits per word");

	/*
	 * max speed hz
	 */
	ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		OSA_ERROR("can't set max speed hz");

	ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		OSA_ERROR("can't get max speed hz");

	OSA_printf("spi mode: %d\n", mode);
	OSA_printf("bits per word: %d\n", bits);
	OSA_printf("max speed: %d Hz (%d KHz)\n", speed, speed/1000);

	if(ret<0)
		close(hndl->fd);
	return ret;
}

int DRV_SPIRead8(DRV_SPIHndl *hndl, Uint8 *buf,  Uint32 count, Uint8 *Obuf)
{
	int ret;
	struct spi_ioc_transfer tr = {
		.tx_buf = (unsigned long)buf,
		.rx_buf = (unsigned long)Obuf ,
		.len = count,
		.delay_usecs = delay,
		.speed_hz = speed,
		.bits_per_word = bits,
	};

	if(hndl->fd<0)
		return OSA_EFAIL;
		
	ret = ioctl(hndl->fd, SPI_IOC_MESSAGE(1), &tr);
	if (ret == 1){
		OSA_ERROR("can't send spi message");	
	}
	
	return ret;
}

int DRV_SPIWrite8(DRV_SPIHndl *hndl, Uint8 *buf, Uint32 count)
{
	int ret;
	struct spi_ioc_transfer tr = {
		.tx_buf = (unsigned long)buf,
		.rx_buf = 0 ,
		.len = count,
		.delay_usecs = delay,
		.speed_hz = speed,
		.bits_per_word = bits,
	};

	if(hndl->fd<0)
		return OSA_EFAIL;
		
	ret = ioctl(hndl->fd, SPI_IOC_MESSAGE(1), &tr);
	if (ret == 1){
		OSA_ERROR("can't send spi message");	
	}
	
	return ret;
}

int DRV_SPIClose(DRV_SPIHndl *hndl)
{
	return close(hndl->fd);
}

