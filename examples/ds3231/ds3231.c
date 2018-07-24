#include "ds3231.h"
#include "delay.h"

/*{{{ REGISTER MAP*/
#define DS3231_SECONDS                    0x00
#define DS3231_MINUTES                    0x01
#define DS3231_HOURS                      0x02
#define DS3231_DAY                        0x03
#define DS3231_DATE                       0x04
#define DS3231_MONTH                      0x05
#define DS3231_YEAR                       0x06
#define DS3231_A1_SECONDS                 0x07
#define DS3231_A1_MINUTES                 0x08
#define DS3231_A1_HOURS                   0x09
#define DS3231_A1_DAY                     0x10
#define DS3231_A1_DATE                    0x10
#define DS3231_A2_MINUTS                  0x0b
#define DS3231_A2_HOUR                    0x0c
#define DS3231_A2_DAY                     0x0d
#define DS3231_A2_DATE                    0x0d
#define DS3231_CONTROL                    0x0e
#define DS3231_CONTROL_STATUS             0x0f
#define DS3231_AGING_OFFSET               0x10
#define DS3231_MSB_TEMP                   0x11
#define DS3231_LSB_TEMP                   0x12
/*}}}*/

volatile uint8_t g_slave_address;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Private functions
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
static void ds3231_tx_byte(uint8_t register_address,  uint8_t data)/*{{{*/
{
  uint8_t tx_data[2] = {register_address,data};
  delay_us(10); // min IDLE time between STOP and START condition
  while(ds3231_i2c_tx_byte(g_slave_address,tx_data,2) != I2C_E_OK)
  {
    delay_us(10); // min IDLE time between STOP and START condition
    // Wait until data is successfully transmited 
  }
}/*}}}*/
static uint8_t ds3231_rx_byte(uint8_t register_address)/*{{{*/
{
  uint8_t data[2] = {register_address,0x00};
  delay_us(10); // min IDLE time between STOP and START condition
  while (ds3231_i2c_tx_byte(g_slave_address,data,1) != I2C_E_OK)
  {
    delay_us(10); // min IDLE time between STOP and START condition
  }
  delay_us(10); // min IDLE time between STOP and START condition
  while (ds3231_i2c_rx_byte(g_slave_address,data,1) != I2C_E_OK)
  {
    delay_us(10); // min IDLE time between STOP and START condition
  }
  return data[0];
}/*}}}*/


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Public functions
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void ds3231_init(uint8_t slave_address)/*{{{*/
{
  g_slave_address = slave_address;
  ds3231_i2c_init();
}/*}}}*/
void ds3231_read_time(rtc_time * time)/*{{{*/
{
  uint8_t data[7];
  uint8_t k = 0;
  for (k = 0; k < 7; ++k)
  {
    data[k] = ds3231_rx_byte(k);
  }
  time->seconds = ((data[0] & 0xf0) >> 4) * 10 + (data[0] & 0x0f);
  time->minutes = ((data[1] & 0xf0) >> 4) * 10 + (data[1] & 0x0f);
  if (data[2] & 0x40)
  {
    //AM/PM
    time->format = (data[2] & 0x20) ? 0x01 : 0x00;
    time->hours = ((data[2] & 0x10) >> 4) * 10 + (data[2] & 0x0f);
  }
  else
  {
    time->format = 0x02;
    time->hours = ((data[2] & 0x30) >> 4) * 10 + (data[2] & 0x0f);
    //24
  }
  time->day = data[3] & 0x07;
  time->date = ((data[4] & 0x30) >> 4) * 10 + (data[4] & 0x0f);
  time->month = ((data[5] & 0x10) >> 4) * 10 + (data[5] & 0x0f);
  time->year = ((data[6] & 0xf0) >> 4) * 10 + (data[6] & 0x0f) + (DS3231_CENTURY - 1) * 100;
}/*}}}*/
void ds3231_set_time(rtc_time * time)/*{{{*/
{
  uint8_t data[7];
  uint8_t k = 0;
  //clear data
  for( k = 0; k < 7; ++k )
  {
    data[k] = 0x00;
  }
  //set new data
  data[0] = (((time->seconds / 10) << 4) & 0xf0) | ((time->seconds % 10) & 0x0f);
  data[1] = (((time->minutes / 10) << 4) & 0xf0) | ((time->minutes % 10) & 0x0f);
  if(time->format != DS3231_FORMAT_24)
  {
    //AM/PM format
    data[2] = (((time->hours / 10) << 4) & 0x10) | ((time->hours % 10) & 0x0f);
    data[2] |= 0x40;
  }
  else
  {
    //24 hour format
    data[2] = ((time->hours % 10) & 0x0f) | (((time->hours / 10) << 4) & 0x30) ;
  }
  data[3] = time->day & 0x07;
  data[4] = (((time->date / 10) << 4) & 0x30) | ((time->date % 10) & 0x0f);
  data[5] = (((time->month / 10) << 4) & 0x10) | ((time->month % 10) & 0x0f);
  time->year = time->year % 100;
  data[6] = (((time->year / 10) << 4) & 0xf0) | ((time->year % 10) & 0x0f);
  for (k = 0; k < 7; ++k)
  {
    ds3231_tx_byte(k,data[k]);
  }
}/*}}}*/

