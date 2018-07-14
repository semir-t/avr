/*! \file usart.h
 *  \brief USART drive for Atmega328p 
 *  
 * \author Semir Tursunovic (semir-t)
 */

#ifndef _USART_H
#define _USART_H 

#include <avr/io.h>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//USART baudrate
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define USART_BAUDRATE_9600 103
#define USART_BAUDRATE_19200 53
#define USART_BAUDRATE_115200 8

/*! \brief Initialize USART peripheral
 *
 * \param  baudrate Macro should be used as argument 
 * \return No return value 
 */
void usart_init(uint16_t baudrate);

/*! \brief Send 1 byte of data via USART 
 *
 * \param  data Data which will be sent 
 * \return No return value 
 */
void usart_tx_byte(uint8_t data);

/*! \brief Receive 1 byte of data via USART 
 * 
 * This function is blocking.
 *
 * \return Received data via USART 
 */
uint8_t usart_rx_byte(void);

#endif /* ifndef _USART_H */
