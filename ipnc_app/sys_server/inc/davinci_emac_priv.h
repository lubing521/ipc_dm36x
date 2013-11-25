#ifndef __DAVINCI_EMAC_PRIV_H__
#define __DAVINCI_EMAC_PRIV_H__
/* This file should be the same whith all the definition in kernel */
/*
 * ioctls
 */
#define EMAC_IOCTL_BASE				0

#define EMAC_IOCTL_GET_STATISTICS		(EMAC_IOCTL_BASE + 0)
#define EMAC_IOCTL_CLR_STATISTICS		(EMAC_IOCTL_BASE + 1)
#define EMAC_IOCTL_GET_SWVER			(EMAC_IOCTL_BASE + 2)
#define EMAC_IOCTL_GET_HWVER			(EMAC_IOCTL_BASE + 3)
#define EMAC_IOCTL_SET_RXCFG			(EMAC_IOCTL_BASE + 4)
#define EMAC_IOCTL_GET_STATUS			(EMAC_IOCTL_BASE + 6)
#define EMAC_IOCTL_READ_PHY_REG			(EMAC_IOCTL_BASE + 7)
#define EMAC_IOCTL_WRITE_PHY_REG		(EMAC_IOCTL_BASE + 8)
/* add/delete */
#define EMAC_IOCTL_MULTICAST_ADDR		(EMAC_IOCTL_BASE + 9)
/* set/clear */
#define EMAC_IOCTL_ALL_MULTI			(EMAC_IOCTL_BASE + 10)
#define EMAC_IOCTL_TYPE2_3_FILTERING		(EMAC_IOCTL_BASE + 11)
#define EMAC_IOCTL_SET_MAC_ADDRESS		(EMAC_IOCTL_BASE + 12)
#define EMAC_IOCTL_IF_COUNTERS			(EMAC_IOCTL_BASE + 13)
#define EMAC_IOCTL_ETHER_COUNTERS		(EMAC_IOCTL_BASE + 14)
#define EMAC_IOCTL_IF_PARAMS_UPDT		(EMAC_IOCTL_BASE + 15)

#define EMAC_PRIV_FILTERING			(EMAC_IOCTL_BASE + 21)
#define EMAC_PRIV_MII_READ			(EMAC_IOCTL_BASE + 22)
#define EMAC_PRIV_MII_WRITE			(EMAC_IOCTL_BASE + 23)
#define EMAC_PRIV_GET_STATS			(EMAC_IOCTL_BASE + 24)
#define EMAC_PRIV_CLR_STATS			(EMAC_IOCTL_BASE + 25)
#define EMAC_EXTERNAL_SWITCH			(EMAC_IOCTL_BASE + 26)
/**
 * MII Read/Write PHY register
 *
 * Parameters to read/write a PHY register via MII interface
 */
typedef struct {
	__u32 phy_num;		/* phy number to be read/written */
	__u32 reg_addr;		/* register to be read/written */
	__u32 data;		/* data to be read/written */
} emac_phy_params;

#endif /*__DAVINCI_EMAC_PRIV_H__*/
