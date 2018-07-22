/*-----------------------------------------------------------------------*/
/* Low level disk I/O module glue functions         (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
#include "mmc.h"	/* Header file of existing SD control module */


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
		return mmc_status();
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
		return mmc_init();
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address in LBA */
	UINT count		/* Number of sectors to read */
)
{
		return mmc_read(sector, buff, count);
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

/* #if _USE_WRITE */
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address in LBA */
	UINT count			/* Number of sectors to write */
)
{
		return mmc_write(sector, buff, count);
}
/* #endif */


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

/* #if _USE_IOCTL */
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
  return mmc_ioctl(cmd, buff);
}
/* #endif */

DWORD get_fattime (void)
{
  /* bit31:25 */
  /*    Year origin from the 1980 (0..127, e.g. 37 for 2017) */
  /* bit24:21 */
  /*    Month (1..12) */
  /* bit20:16 */
  /*    Day of the month (1..31) */
  /* bit15:11 */
  /*    Hour (0..23) */
  /* bit10:5 */
  /*    Minute (0..59) */
  /* bit4:0 */
  /*    Second / 2 (0..29, e.g. 25 for 50) */ 
  DWORD year = 38;
  DWORD month = 1;
  DWORD day = 1;
  DWORD hour = 0;
  DWORD minute = 0;
  DWORD second = 0;
  DWORD time =  ((year & 0x7f) << 25)  |
    ((month & 0x0f) << 21) |
    ((day & 0x1f) << 16)   |
    ((hour & 0x1f) << 11)  |
    ((minute & 0x3f) << 5) |
    ((second & 0x1f));
  return time;
}
/*-----------------------------------------------------------------------*/
/* Timer driven procedure                                                */
/*-----------------------------------------------------------------------*/


void disk_timerproc (void)
{
#ifdef DRV_CFC
  cf_disk_timerproc();
#endif
#ifdef DRV_MMC
  mmc_disk_timerproc();
#endif
}



