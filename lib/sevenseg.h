/*! \file sevenseg.h 
 *  \brief Drive for controlling 7 segment display.
 *
 *  Drive is written so it can be easily ported to other Atmels 8 bit MCUs.
 *  Macros are used to specify pins which are connected to 7 segment display,
 *  and number of digits on the 7 segment display.
 *
 *  When more then one digit is used, then MCU has to perform multiplexing.Thus 
 *  we have functions:
 *   - sevenseg_print(float)
 *       set the number that we want to show on the display
 *   - sevenseg_refresh(void)
 *       actually show the number on the display, and this function
 *       should be called frequently in the main loop 
 * 
 * \author Semir Tursunovic (semir-t)
 */

#ifndef __SEVENSEG_H 
#define __SEVENSEG_H

#include "avr/io.h"
#include "avr_core.h"
#include "delay.h"

/*{{{ PIN CONFIGURATION
*/
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//PIN CONFIGURATION                         PORT,PIN
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//COMMON:
#define SEVENSEG_COMMON_1                     C,PC2
#define SEVENSEG_COMMON_2                     C,PC3
#define SEVENSEG_COMMON_3                     C,PC4
//DATA
#define SEVENSEG_D0_PIN                       B,PB0
#define SEVENSEG_D1_PIN                       B,PB1
#define SEVENSEG_D2_PIN                       D,PD2
#define SEVENSEG_D3_PIN                       D,PD3
#define SEVENSEG_D4_PIN                       D,PD4
#define SEVENSEG_D5_PIN                       D,PD5
#define SEVENSEG_D6_PIN                       D,PD6
#define SEVENSEG_D7_PIN                       D,PD7
/*}}}*/


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//DISPLAY CONFIGURATION                     
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define SEVENSEG_COMMON_ANODE               0x00
#define SEVENSEG_COMMON_CATHODE             0x01
#define NUMBER_OF_DIGITS                    3 

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//User defined macros
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define SEVENSEG_DIGIT_1_ON                 GPIO_SET(SEVENSEG_COMMON_1)
#define SEVENSEG_DIGIT_1_OFF                GPIO_CLEAR(SEVENSEG_COMMON_1) 
#define SEVENSEG_DIGIT_2_ON                 GPIO_SET(SEVENSEG_COMMON_2)
#define SEVENSEG_DIGIT_2_OFF                GPIO_CLEAR(SEVENSEG_COMMON_2) 
#define SEVENSEG_DIGIT_3_ON                 GPIO_SET(SEVENSEG_COMMON_3)
#define SEVENSEG_DIGIT_3_OFF                GPIO_CLEAR(SEVENSEG_COMMON_3) 

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//User defined function 
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/*! \brief Initialize 7 segment display 
 *
 * \param  type Used 7 segment display has common anode or common
 * anode:
 *        - SEVENSEG_COMMON_ANODE
 *        - SEVENSEG_COMMON_CATHODE
 * \return No return value 
 */
void sevenseg_init(uint8_t type);

/*! \brief Load number to the buffer 
 *
 * Number is loaded to the output buffer and shown on the display
 * after a call to sevenseg_refresh() function.
 * \param  number Number loaded to the output buffer 
 * \return No return value 
 */
void sevenseg_print(float number);

/*! \brief  Shows number from the output buffer on the display
 *
 * Implementing multiplexing. This function should be called 
 * frequently.
 * \return No return value
 */
void sevenseg_refresh(void);

/*! \brief Send byte via parallel port 
 *
 * Core function which enables us to send data to 7 segment display
 * with arbitrary pins.
 *
 * \param  data Send data via parallel port to 7 segment display
 * \return Return parameter description
 */
void sevenseg_tx_byte(uint8_t data);

#endif 
