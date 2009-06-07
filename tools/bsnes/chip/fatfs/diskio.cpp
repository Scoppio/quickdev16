
#include "integer.h"
#include "diskio.h"

static volatile
DSTATUS Stat = STA_NOINIT;  /* Disk status */


/* Interface
** Scratch Buffer
addr        3 byte            
size        1 byte

** Call Interface
cmd         1 byte
sector      4 bytes
count       1 byte
return      1 byte

** Commands
    * disk_init
    * disk_read
    * disk_write
*/ 

/*-----------------------------------------------------------------------*/
/* Initialize Disk Drive                                                 */
/*-----------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>

#define IMAGE_NAME "disk00.vfat"

BYTE  *image_addr;

DSTATUS disk_initialize (BYTE drv) {
    if (drv) return STA_NOINIT;             /* Supports only single drive */

    Stat |= STA_NOINIT;

    int fd = open(IMAGE_NAME, O_RDWR);
    if (fd == -1) {
   	    perror("DISKIO::disk_initialize: Error opening file for writing");
   	    exit(EXIT_FAILURE);
      }

    int size = lseek(fd,0,SEEK_END);
    lseek(fd,0,SEEK_SET);
    
    image_addr = (BYTE*)mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    #ifdef MMIO_DEBUG
    printf("DISKIO::disk_initialize: Open Image (size %i) %p\n",size,image_addr);
    #endif
    if (image_addr == MAP_FAILED) {
	    close(fd);
	    perror("DISKIO::disk_initialize: Error mmapping the file");
        exit(EXIT_FAILURE);
    }
    
    Stat &= ~STA_NOINIT;                    /* When device goes ready, clear STA_NOINIT */
    return Stat;
}


/*-----------------------------------------------------------------------*/
/* Return Disk Status                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (BYTE drv){
    if (drv) return STA_NOINIT;     /* Supports only single drive */
    return Stat;
}


/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
    BYTE drv,       /* Physical drive nmuber (0) */
    BYTE *buff,     /* Data buffer to store read data */
    DWORD sector,   /* Sector number (LBA) */
    BYTE count      /* Sector count (1..255) */
)
{
    BYTE c, iord_l, iord_h;
    if (drv || !count) return RES_PARERR;
    if (Stat & STA_NOINIT) return RES_NOTRDY;

    DWORD offset = sector  * 512;
    int size = count * 512;
    //#ifdef MMIO_DEBUG
    printf("DISKIO::disk_read: sector=%li count=%i addr=%p off=%li size=%i\n",sector,count,image_addr + offset,offset,size);
    //#endif
    memcpy(buff,image_addr + offset,size);
    printf("%x %x %x %x\n",buff[0],buff[1],buff[2],buff[3]);
    #ifdef MMIO_DEBUG
    printf("DISKIO::disk_read: done\n");
    #endif
    return RES_OK;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _READONLY == 0
DRESULT disk_write (
    BYTE drv,           /* Physical drive nmuber (0) */
    const BYTE *buff,   /* Data to be written */
    DWORD sector,       /* Sector number (LBA) */
    BYTE count          /* Sector count (1..255) */
)
{

    if (drv || !count) return RES_PARERR;
    if (Stat & STA_NOINIT) return RES_NOTRDY;

    DWORD offset = sector  * 512;
    int size = count * 512;
    #ifdef MMIO_DEBUG
    printf("disk_write: sector=%li count=%i addr=%p off=%li size=%i\n",sector,count,image_addr + offset,offset,size);
    #endif
    memcpy(image_addr + offset,buff,size);
    return RES_OK;
}
#endif /* _READONLY */

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL != 0
DRESULT disk_ioctl (
    BYTE drv,       /* Physical drive nmuber (0) */
    BYTE ctrl,      /* Control code */
    void *buff      /* Buffer to send/receive data block */
)
{
    BYTE n, w, ofs, dl, dh, *ptr = (BYTE*)buff;


    if (drv) return RES_PARERR;
    if (Stat & STA_NOINIT) return RES_NOTRDY;

    switch (ctrl) {
        case GET_SECTOR_COUNT : /* Get number of sectors on the disk (DWORD) */
    #ifdef MMIO_DEBUG
            printf("disk_ioctl: GET_SECTOR_COUNT\n");
    #endif
            ofs = 60; w = 2; n = 0;
            break;

        case GET_SECTOR_SIZE :  /* Get sectors on the disk (WORD) */
    #ifdef MMIO_DEBUG
            printf("disk_ioctl: GET_SECTOR_SIZE\n");
    #endif
            *(WORD*)buff = 512;
            return RES_OK;

        case GET_BLOCK_SIZE :   /* Get erase block size in sectors (DWORD) */
    #ifdef MMIO_DEBUG
            printf("disk_ioctl: GET_BLOCK_SIZE\n");
    #endif
            *(DWORD*)buff = 32;
            return RES_OK;

        default:
            return RES_PARERR;
    }
    return RES_OK;
}
#endif /* _USE_IOCTL != 0 */




