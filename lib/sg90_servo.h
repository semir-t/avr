/*! \file sg90_servo.h
 *  \brief dxw90 servo drive for AVR. This drive was written
 *  for Atmega328p, but can easily be ported to other 8 bit
 *  Atmel MCUs
 *  
 * \author Semir Tursunovic (semir-t)
 */
#ifndef __SG90_SERVO_H
#define __SG90_SERVO_H 

#include <avr/io.h>

/*! \brief Initialize MCU for servo control 
 *
 * \return No return value
 */
void sg90_servo_init(void);

/*! \brief Set servo position [0 - 180]
 *
 * \param  position Desired position of the servo 
 * \return No return value 
 */
void sg90_servo_position(uint8_t position);

#endif /* ifndef __DXW90_SERVO_H */

