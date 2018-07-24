/*! \file i2c.h
 *  \brief I2C drive for AVR 
 *  
 * \author Semir Tursunovic (semir-t)
 */
#ifndef __I2C_H
#define __I2C_H 

#include <avr/io.h>
#include "usart.h"
#include "print.h"

/*{{{ I2C PRESCALER*/
#define I2C_PRESCALER_1                 0x00 
#define I2C_PRESCALER_2                 0x01 
#define I2C_PRESCALER_16                0x02 
#define I2C_PRESCALER_64                0x03 
/*}}}*/
 /*{{{ I2C READ/WRITE*/
#define I2C_WRITE                       0x00
#define I2C_READ                        0x01
/*}}}*/
/*{{{ I2C ERROR*/
#define I2C_E_OK                        0x00
#define I2C_E_START                     0x01
#define I2C_E_ADDRESS                   0x02
#define I2C_E_DATA                      0x03

/*}}}*/
/*{{{ I2C STATUS*/
#define I2C_S_MASK                      0xf8 
/*{{{ Master transmit status code*/
#define I2C_S_MT_START                  0x08
#define I2C_S_MT_R_START                0x10
#define I2C_S_MT_ADDRESS_ACK            0x18
#define I2C_S_MT_ADDRESS_NACK           0x20
#define I2C_S_MT_DATA_ACK               0x28
#define I2C_S_MT_DATA_NACK              0x30 
/*}}}*/
/*{{{ Master receive status code*/
#define I2C_S_MR_START                  0x08
#define I2C_S_MR_R_START                0x10
#define I2C_S_MR_ADDRESS_ACK            0x40
#define I2C_S_MR_ADDRESS_NACK           0x48
#define I2C_S_MR_DATA_ACK               0x50
#define I2C_S_MR_DATA_NACK              0x58 
/*}}}*/
/*}}}*/

/*! \brief Initilize I2C peripheral 
 *
 * SCL frequency is defined by 2 arguments:
 *    * prescaler
 *    * bit_rate
 *
 * Using the expression:
 *  SCL_frequency = CPU_Clock_frequency / (16 + 2(bit_rate) ⋅ prescaler)
 * we can get desired SCL frequency by using appropriate prescaler and
 * bit_rate values.
 *
 * \param prescaler  Argument can be one of the following values
 *                      * I2C_PRESCALER_1
 *                      * I2C_PRESCALER_2
 *                      * I2C_PRESCALER_16
 *                      * I2C_PRESCALER_64
 * \param bit_rate  Any value from 0-255 
 * \return No return value 
 */
void i2c_init(uint8_t prescaler,uint8_t bit_rate);

/*! \brief Send data to slave device via I2C 
 *
 * \param address   Slave device address 
 * \param data      Data to send via I2C 
 * \param cnt       Number of bytes to send
 * \return Transmit data status 
 */
uint8_t i2c_tx_data(uint8_t address, uint8_t * data, uint8_t cnt);

/*! \brief Receive  data from slave device via I2C 
 *
 * \param address   Slave device address 
 * \param data      Received data 
 * \param cnt       Number of bytes to receive 
 * \return Receive data status 
 */
uint8_t i2c_rx_data(uint8_t address, uint8_t * data, uint8_t cnt);

#endif /* ifndef __I2C_H */
