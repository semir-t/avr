/*! \file spi.h
 *  \brief SPI drive for atmega328p 
 *  IMPORTANT: If SPI is master mode, then SS pin should 
 *  be defined as output. If there is need to define this
 *  pin as input then one should be aware that if this pin
 *  gets in low logical state then MSTR bit in SPCR is 
 *  cleared. This means that SPI is now in slave mode.
 *  So if there is need to work in master mode with 
 *  SS pin declared as input, then before sending data
 *  one should test MSTR bit value. More info can be
 *  found in MCU datasheet.
 * 
 *  NOTE: spi_tx_byte() and spi_rx_byte() could be merged
 *  in one function if there is need for less memory 
 *  consumption.  
 * 
 * \author Semir Tursunovic (semir-t)
 */
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Library for controling SPI. 
//

//
//Author: Semir Tursunovic
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifndef __SPI_H
#define __SPI_H 

#include <avr/io.h>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Pin Configuration
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define SPI_SS                          PB2
#define SPI_MOSI                        PB3
#define SPI_MISO                        PB4
#define SPI_SCK                         PB5
#define SPI_DDR                         DDRB
#define SPI_PORT                        PORTB
#define SPI_PIN                         PINB

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//User defined macros
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define SPI_SS_LOW           (SPI_PORT &= ~_BV(SPI_SS))
#define SPI_SS_HIGH          (SPI_PORT |= _BV(SPI_SS))

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Data transfer speed
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define SPI_BAUDRATE_PRESCALER_2        0x04
#define SPI_BAUDRATE_PRESCALER_4        0x00
#define SPI_BAUDRATE_PRESCALER_8        0x05
#define SPI_BAUDRATE_PRESCALER_16       0x01
#define SPI_BAUDRATE_PRESCALER_32       0x06
#define SPI_BAUDRATE_PRESCALER_64       0x02
#define SPI_BAUDRATE_PRESCALER_64_2X    0x07
#define SPI_BAUDRATE_PRESCALER_128      0x03


/*! \brief Initialize SPI in master mode 
 *
 * \param  prescaler SPI baudrate prescaler 
 * \return No return value 
 */
void spi_master_init(uint8_t prescaler);

/*! \brief Initialize SPI in slave mode 
 *
 * \return No return value
 */
void spi_slave_init(void);

/*! \brief Configure SPI for sending 1 byte of data 
 *
 * For successful transfer of 1 byte od data SS(CS) pin must
 * be pulled low.
 *
 * \param  data Data which will be sent 
 * \return No return value 
 */
void spi_tx_byte(uint8_t data);

/*! \brief Configure SPI for reading 1 byte of data 
 *
 * For successful transfer of 1 byte od data from slave
 * SS(CS) pin must  be pulled low.
 *
 * \return Value received via SPI 
 */
uint8_t spi_rx_byte(void);

/*! \brief Configure SPI for sending and receiving 1 byte of data 
 *
 * For successful transfer of 1 byte od data SS(CS) pin must
 * be pulled low.
 *
 * \param  data Data which will be sent 
 * \return Value received via SPI 
 */
uint8_t spi_rxtx_byte(uint8_t data);

/*! \brief Send arbitrary number of data via SPI 
 *
 * \param data Array of data which will be sent 
 * \param n_value Number of elements in array 
 * \return No return value 
 */
void spi_tx(uint8_t * data, uint8_t n_value);

/*! \brief Receive arbitrary number of data via SPI 
 *
 * \param data Received data will be stored in this array 
 * \param n_value Number of elements to read via SPI 
 * \return No return value 
 */
void spi_rx(uint8_t * data, uint8_t n_value);

/*! \brief Send and Receive arbitrary number of data via SPI 
 *
 * \param data Array of data which will be sent is rewritten by received data
 * \param n_value Number of elements to send/read via SPI 
 * \return No return value 
 */
void spi_rxtx(uint8_t * data, uint8_t n_value);

/*! \brief Change SPI baudrate speed 
 *
 * \param  prescaler New SPI baudrate value 
 * \return No return value
 */
void spi_baudrate(uint8_t prescaler);


#endif /* ifndef __SPI_H */
