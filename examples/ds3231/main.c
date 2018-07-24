#include <avr/io.h>
#include "usart.h"
#include "print.h"
#include "delay.h"
#include "i2c.h"
#include "ds3231.h"


#define I2C_DEVICE_ADDRESS 0x68

int main(void)
{
  usart_init(USART_BAUDRATE_115200);
  print_init(usart_tx_byte);

  ds3231_init(0x68);
  rtc_time time;
  time.seconds = 0;
  time.minutes = 4;
  time.hours = 22;
  time.day = 2;
  time.date = 24;
  time.month = 7;
  time.year = 2018;
  time.format = DS3231_FORMAT_24;

  ds3231_set_time(&time);

  DDRB |= 0xff;
  while (1)
  {
    PORTB ^= 0x20;
    delay_ms(1000);
    ds3231_read_time(&time) ;
    print(" %d:%d:%d ",time.hours,time.minutes,time.seconds);
    print(" %d/%d/%d\n",time.date,time.month,time.year);
  }
  return 0;
}


