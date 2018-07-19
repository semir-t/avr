/*! \file mmc.h
 *  \brief Portable SD card drive 
 *  
 * Basic drive for SD card with basic reading/writing
 * functions.
 *
 * \author Semir Tursunovic (semir-t)
 */
#ifndef __MMC_H
#define __MMC_H 

#include <avr/io.h>
#include "spi.h"
#include "delay.h"
#include "print.h"

/*{{{ SD card SPI core*/
#define mmc_spi_init(prescaler)             spi_master_init(prescaler)
#define mmc_spi_rxtx_byte(data)             spi_rxtx_byte(data)
#define MMC_SS_LOW                          SPI_SS_LOW
#define MMC_SS_HIGH                         SPI_SS_HIGH
#define MMC_CLK_SLOW()                      spi_baudrate(SPI_BAUDRATE_PRESCALER_64)
#define MMC_CLK_FAST()                      spi_baudrate(SPI_BAUDRATE_PRESCALER_16)
/*}}}*/
/*{{{ SD card status*/
#define CARD_S_NO_INIT		                  0x01	/* Drive not initialized */
#define CARD_S_NO_DISK		                  0x02	/* No medium in the drive */
#define CARD_S_PROTECT		                  0x04	/* Write protected */
/*}}}*/
/*{{{ SD error*/
#define CARD_E_CLEAN                        0x00
#define CARD_E_PARAMETER                    0x01
#define CARD_E_NOT_READY                    0x02
#define CARD_E_WRITE_PROTECT                0x03
#define CARD_E_WRITE_DATA                   0x10
#define CARD_E_READ_DATA                    0x20
/*}}}*/
#define MMC_DEBUG                         1

uint8_t mmc_init(void);
uint8_t mmc_tx_command(uint8_t command, uint32_t arg, uint8_t crc);

uint8_t mmc_read(uint32_t sector, uint8_t * buffer);
uint8_t mmc_write(uint32_t sector, uint8_t * buffer);

uint8_t mmc(void);


#endif /* ifndef __MMC_H */
