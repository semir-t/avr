/*! \file DS3231.h
 *  \brief Basic DS3231 RTC drive 
 *  
 * \author Semir Tursunovic (semir-t)
 */
#ifndef __DS3231_H
#define __DS3231_H value

#include <avr/io.h>
#include "i2c.h"

/*{{{ I2C CORE*/
#define ds3231_i2c_init()                       i2c_init(I2C_PRESCALER_1,72)
#define ds3231_i2c_tx_byte(address,data,cnt)    i2c_tx_data(address,data,cnt)
#define ds3231_i2c_rx_byte(address,data,cnt)    i2c_rx_data(address,data,cnt)
#define DS3231_CENTURY                          21
/*}}}*/
/*{{{ DS3231 TIME FORMAT*/
#define DS3231_FORMAT_AM                        0x00
#define DS3231_FORMAT_PM                        0x01
#define DS3231_FORMAT_24                        0x02
/*}}}*/

/*! \struct rtc_time 
 *  \brief Store date,time and time format 
 *
 * User should make sure that data written to struct is correct. 
 */
typedef struct
{
  uint8_t seconds;/*!< Valid value is 0-59*/
  uint8_t minutes;/*!< Valid value is 0-59*/
  uint8_t hours;/*!< Valid value is 0-23 for DS3231_FORMAT_24, else 0-12*/
  uint8_t date;/*!< Valid value is 1-31*/
  uint8_t month;/*!< Valid value is 1-12*/
  uint16_t year;/*!< Valid value is 0-99*/
  uint8_t day;/*!< Valid value is 0-7*/
  uint8_t format;/*!< Valid values are DS3231_FORMAT_24, DS3231_FORMAT_AM, DS3231_FORMAT_PM */
} rtc_time;

/*! \brief Initialize DS3231 
 *
 *  Initialize I2C peripheral and define ds3231 address
 *
 * \param  slave_address ds3231 address 
 * \return No return value 
 */
void ds3231_init(uint8_t slave_address);

/*! \brief Read time from DS3231 
 *
 *  Read date and time from DS3231 and store it to struct.
 *
 * \param time  Time received from DS3231 
 * \return No return value 
 */
void ds3231_read_time(rtc_time * time);

/*! \brief Write time from DS3231 
 *
 *  Read date from struct and write it to DS3231.
 *
 * \param time  Time transmited to DS3231 
 * \return No return value 
 */
void ds3231_set_time(rtc_time * time);

#endif /* ifndef __DS3231_H */
