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

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//SD card SPI core
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define sd_spi_init(prescaler)            spi_master_init(prescaler)
#define sd_spi_rxtx_byte(data)            spi_rxtx_byte(data)
#define MMC_SS_LOW                         SPI_SS_LOW
#define MMC_SS_HIGH                        SPI_SS_HIGH
#define MMC_CLK_SLOW                      spi_baudrate(SPI_BAUDRATE_PRESCALER_64)
#define MMC_CLK_FAST                      spi_baudrate(SPI_BAUDRATE_PRESCALER_16)




uint8_t mmc_init(void);
uint8_t mmc_tx_command(uint8_t command, uint32_t arg, uint8_t crc);

uint8_t mmc_read(uint32_t sector, uint8_t * buffer);
uint8_t mmc_write(uint32_t sector, uint8_t * buffer);



#endif /* ifndef __MMC_H */
