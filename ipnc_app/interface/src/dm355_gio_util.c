/* ===========================================================================
* @file dm355_gio_util.c
*
* @path $(IPNCPATH)/util/
*
* @desc Functions to control dm355 GIO.
* .
* Copyright (c) Appro Photoelectron Inc.  2008
*
* Use of this software is controlled by the terms and conditions found
* in the license agreement under which this software has been supplied
*
* =========================================================================== */
#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <getopt.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include <dm355_gio_util.h>


#ifdef GIO_DEBUG
#define __D(fmt, args...)	fprintf(stderr,"Gio: Debug\n" fmt, ##args)
#else
#define __D(fmt, args...)
#endif

#define __E(fmt, args...)	fprintf(stderr,"Gio: Error\n" fmt, ##args)

#define GIO_MAX_ID	(103)

#define GIO_DEV_PATH	"/proc/gio/"


/**
* @brief get dm355 gio name
*
*@param *pName pointer to store GIO name
*@param gio_id GIO ID
* @return gio name
*@retval -1 gio id over range
*@retval 0 get gio name success
*/
int dm355_gio_getname( char *pName, int gio_id )
{
	if( gio_id < 0 || gio_id > GIO_MAX_ID )
	{
		__E("gio id = %d over range!!\n", gio_id);
		return -1;
	}

	sprintf( pName, GIO_DEV_PATH"gio%d", gio_id );

	//fprintf(stderr, "gio name: %s \n", pName);

	return 0;
}

/**
* @brief read dm355 gio infomation
*
*@param gio_id GIO ID
* @return GIO status
*@retval -1  error
*/
int dm355_gio_read( int gio_id )
{
	int 	fd_gio = 0;
	char	data[5];
	int		result = 0;
	char 	dev_name[25];

	if( gio_id < 0 || gio_id > GIO_MAX_ID )
	{
		__E("gio id = %d over range!!\n", gio_id);
		return -1;
	}

	if( dm355_gio_getname(dev_name,gio_id) < 0 )
	{
		__E("gio get name error!!\n");
		return -1;
	}

	fd_gio = open(dev_name, O_RDWR);

	if( !fd_gio )
	{
		__E("open device error !! gio = %d\n", gio_id);
		return -1;
	}
	result = read(fd_gio,  data, 1);
	if( result <= 0 )
	{
		__E("read device error !! gio = %d\n", gio_id);
		close(fd_gio);
		return -1;
	}
	close(fd_gio);
	if( data[0] == '1')
	{
		return 1;
	}else{
		return 0;
	}

	return -1;
}
/**
* @brief set dm355 gio infomation
*
*@param gio_id GIO ID
*@param IsOn GIO setting
* @return result of setting
*@retval -1  error
*@retval 0  success
*/
int dm355_gio_write( int gio_id , int IsOn)
{
	int 	fd_gio = 0;
	char	data[5];
	int		result = 0;
	char 	dev_name[25];
	int		cnt = 0;

	/* Don't do anything on EVM */
	if( gio_id < 0 || gio_id > GIO_MAX_ID )
	{
		__E("gio id = %d over range!!\n", gio_id);
		return -1;
	}

	if( dm355_gio_getname(dev_name,gio_id) < 0 )
	{
		__E("gio get name error!!\n");
		return -1;
	}

	fd_gio = open(dev_name, O_RDWR);

	if( !fd_gio )
	{
		__E("open device error !! gio = %d\n", gio_id);
		goto GIO_QUIT;
	}

	if( IsOn == 1 )
	{
		for( cnt = 0 ; cnt < sizeof(data) ; cnt ++ )
		{
			data[cnt]='1';
		}
	}
	else if( IsOn == 0 )
	{
		for( cnt = 0 ; cnt < sizeof(data) ; cnt ++ )
		{
			data[cnt]='0';
		}

	}else{
		goto GIO_QUIT;
	}

	result = write(fd_gio,  data, 1);
	if( result <= 0 )
	{
		__E("write device error !! gio = %d\n", gio_id);
		close(fd_gio);
		goto GIO_QUIT;
	}

GIO_QUIT:
	if( fd_gio )
		close(fd_gio);

	return 0;
}

