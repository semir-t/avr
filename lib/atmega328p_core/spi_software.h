//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//SPI bitbanging library
//
//
//Author: Semir Tursunovic
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#ifndef __SPI_SOFTWARE_H
#define __SPI_SOFTWARE_H 

#include <avr/io.h>
#include "delay.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Pin Configuration
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#define SPI_SSN                 PB2
#define SPI_MOSI                PB3
#define SPI_MISO                0 
#define SPI_SCK                 PB0
#define SPI_PIN                 PINB
#define SPI_PORT                PORTB
#define SPI_DDR                 DDRB

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Pin Configuration
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define SSN_LOW                 (SPI_PORT &= ~_BV(SPI_SSN))
#define SSN_HIGH                (SPI_PORT |= _BV(SPI_SSN))
#define SCK_LOW                 (SPI_PORT &= ~_BV(SPI_SCK))
#define SCK_HIGH                 (SPI_PORT |= _BV(SPI_SCK))
#define MOSI_LOW                 (SPI_PORT &= ~_BV(SPI_MOSI))
#define MOSI_HIGH                 (SPI_PORT |= _BV(SPI_MOSI))

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//SOFTWARE SPI 
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define SPI_MSB                         0x00
#define SPI_LSB                         0x01
#define SPI_PERIOD                      10

void spi_software_init(void);
void spi_software_tx_byte(uint8_t, uint8_t);
#endif /* ifndef __SPI_SOFTWARE_H */
