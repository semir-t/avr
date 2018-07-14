/*! \file max7219.h
 *  \brief MAX7219 drive for controling 8x8 matrix display 
 *  
 * 
 * \author Semir Tursunovic (semir-t)
 */
#ifndef __MAX7219_H  
#define __MAX7219_H

#include "avr/io.h"
#include "delay.h"
#include "spi.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//SPI core
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#define MAX7219_SS_LOW                SPI_SS_LOW
#define MAX7219_SS_HIGH               SPI_SS_HIGH
#define max7219_spi_tx(data)          spi_tx_byte(data)
#define max7219_spi_init(prescaler)   spi_master_init(prescaler)

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Matrix configuration
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define NUMBER_OF_DEVICES               3
#define MATRIX_ROWS                     8
#define MATRIX_COLS                     8 
#define FONT_SIZE                       8

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//User defined macros
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define BUFFER_ROWS                     8
#define BUFFER_COLS             (NUMBER_OF_DEVICES + 1)
#define BUFFER_SIZE             ((NUMBER_OF_DEVICES + 1) * 8)

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Data transfer speed
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define PRESCALER_2             SPI_BAUDRATE_PRESCALER_2 
#define PRESCALER_4             SPI_BAUDRATE_PRESCALER_4 
#define PRESCALER_8             SPI_BAUDRATE_PRESCALER_8 
#define PRESCALER_16            SPI_BAUDRATE_PRESCALER_16 
#define PRESCALER_32            SPI_BAUDRATE_PRESCALER_32 
#define PRESCALER_64            SPI_BAUDRATE_PRESCALER_64 
#define PRESCALER_128           SPI_BAUDRATE_PRESCALER_128 
#define PRESCALER_256           SPI_BAUDRATE_PRESCALER_256 

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//MAX7219 ADDRESS MAP
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define MAX7219_NOP                     0x00
#define MAX7219_DIG0                    0x01
#define MAX7219_DIG1                    0x02
#define MAX7219_DIG2                    0x03
#define MAX7219_DIG3                    0x04
#define MAX7219_DIG4                    0x05
#define MAX7219_DIG5                    0x06
#define MAX7219_DIG6                    0x07
#define MAX7219_DIG7                    0x08
#define MAX7219_DECODE_MODE             0x09
#define MAX7219_INTENSITY               0x0a
#define MAX7219_SCAN_LIMIT              0x0b
#define MAX7219_SHUTDOWN                0x0c
#define MAX7219_DISPLAY_TEST            0x0f

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//MAX7219 SHUTDOWN REGISTER
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define MAX7219_OFF                     0x00
#define MAX7219_ON                      0x01
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//MAX7219 DECODE REGISTER
//  -no BCD decode
//  -first digit BCD decode
//  - 0-3 digits BCD decode
//  - 0-7 (all) digits BCD decode
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define MAX7219_NO_DECODE               0x00
#define MAX7219_DECODE0                 0x01
#define MAX7219_DECODE0_3               0x0f
#define MAX7219_DECODE0_7               0xff

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//MAX7219 INTENSITY REGISTER
//  MAX7129_X where X is all odd numbers from 0 to 31
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define MAX7219_1                       0x00
#define MAX7219_3                       0x01
#define MAX7219_5                       0x02
#define MAX7219_7                       0x03
#define MAX7219_9                       0x04
#define MAX7219_11                      0x05
#define MAX7219_13                      0x06
#define MAX7219_15                      0x07
#define MAX7219_17                      0x08
#define MAX7219_19                      0x09
#define MAX7219_21                      0x0a
#define MAX7219_23                      0x0b
#define MAX7219_25                      0x0c
#define MAX7219_27                      0x0d
#define MAX7219_29                      0x0e
#define MAX7219_31                      0x0f

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//MAX7219 TEST DISPLAY REGISTER
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define MAX7219_DISPLAY_TEST_OFF        0x00
#define MAX7219_DISPLAY_TEST_ON         0x01

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//MAX7219 ROTATION VALUES 
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define MAX7219_COUNTER_CLOCKWISE_90    0x01
#define MAX7219_COUNTER_CLOCKWISE_180   0x02
#define MAX7219_COUNTER_CLOCKWISE_270   0x03
#define MAX7219_CLOCKWISE_90            0x03
#define MAX7219_CLOCKWISE_180           0x02
#define MAX7219_CLOCKWISE_270           0x01
#define UPSIDE                          0x03
#define UPSIDE_DOWN                     0x01
#define RIGHT                           0x02
#define LEFT                            0x00

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//MAX7219 SCROLL VALUES 
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define SCROLL_RIGHT                    0x00
#define SCROLL_LEFT                     0x01

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//MAX7219 ACCESS CONTROL 
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define MAX7219_ALL_DEVICES             0x00

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//USER DEFINED FUNCTIONS
////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/*! \brief Initialize max7219
*
* Initialize SPI peripheral of the used MCU. 
* Configure the MAX7219 IC with desired settings
*
* \param Prescaler SPI baudrate prescaler 
* \return Returns void
*/
void max7219_init(uint8_t prescaler);

/*! \brief Test max7219 
 *
 * \param device_no  Ordinal number of the device 
 * \param test_state Defined macros should be used as arguments
 *                   when calling this function:
 *                      - MAX7210_DISPLAY_TEST_OFF
 *                      - MAX7210_DISPLAY_TEST_ON
 * \return No return value 
 */
void max7219_test(uint8_t device_no, uint8_t test_state);

/*! \brief Clear the internal buffers of the max7219 IC
 *
 * \param device_no  Ordinal number of the device 
 *
 * \return No return value
 */
void max7219_display_clean(uint8_t device_no);

/*! \brief Configure SPI peripheral for sending data and address
 *
 * This function doesn't pull SS(CS) line low !
 *
 * \param address Address of the desired register 
 * \param data Data which will be written to the desired register 
 * \return No return value 
 */
void max7219_tx_byte(uint8_t address, uint8_t data);

/*! \brief Send 1 byte od data to arbitrary address via SPI peripheral 
 *
 * This function  pulls SS(CS) line low and sends data via SPI.
 *
 * \param device_no  Ordinal number of the device 
 * \param address Address of the desired register 
 * \param data Data which will be written to the desired register 
 * \return No return value 
 */
void max7219_tx_data(uint8_t device_no, uint8_t address, uint8_t data);

/*! \brief Change the orientation of the character on the display 
 *
 * \param data[]  Data that represents the character 
 * \param rotate_direction Defined macros should be used as arguments.
 *                         Any macro defined under section MAX7219 ROTATION VALUES
 *                         can be used as argument for the function.
 *      
 * \return No return value 
 */
void max7219_rotate(uint8_t data[], uint8_t rotate_direction);

/*! \brief Show string on the display with scrolling animation 
 *
 * This function is blocking.
 *
 * \param string String which will be shown on display 
 * \param orientaiton Orientation of the text. Defined macros should be used as
 *                    arguments:
 *                      - UPSIDE
 *                      - UPSIDE_DOWN
 *                      - LEFT
 *                      - RIGHT
 * \param direction Direction of the scrollin effect. Defined macros should be used
 *                  as arguments: SCROLL_RIGHT or SCROLL_LEFT
 * \param speed Refresh rate of the display. 
 * \return No return value
 */
void max7219_display_scroll(char * string, uint8_t orientation, uint8_t direction,uint8_t speed);

/*! \brief Show character on arbitrary 8x8 dispaly 
 *
 * \param device_no Number of the device on which we want to display the character 
 * \param data Character which will be shown on desired 8x8 display 
 * \param orientaiton Orientation of the character. Defined macros should be used as
 *                    arguments:
 *                      - UPSIDE
 *                      - UPSIDE_DOWN
 *                      - LEFT
 *                      - RIGHT
 *                  as arguments: SCROLL_RIGHT or SCROLL_LEFT
 * \return No return value
 */
void max7219_display_char(uint8_t device_no,uint8_t data,uint8_t orientation);


#endif /* ifndef  */
