#include <avr/io.h>
#include "max7219.h"
#include "delay.h"

int main(void)
{
  max7219_init(PRESCALER_64);
  max7219_test(1,MAX7219_DISPLAY_TEST_ON);
  delay_ms(200);
  max7219_test(1,MAX7219_DISPLAY_TEST_OFF);
  delay_ms(200);
  max7219_test(2,MAX7219_DISPLAY_TEST_ON);
  delay_ms(200);
  max7219_test(2,MAX7219_DISPLAY_TEST_OFF);
  delay_ms(200);
  max7219_test(3,MAX7219_DISPLAY_TEST_ON);
  delay_ms(200);
  max7219_test(3,MAX7219_DISPLAY_TEST_OFF);
  delay_ms(1000);
  max7219_display_scroll("Hello there",UPSIDE_DOWN,LEFT,100);
  max7219_display_char(3,'B',UPSIDE_DOWN);
  max7219_display_char(2,'i',UPSIDE_DOWN);
  max7219_display_char(1,'H',UPSIDE_DOWN);
  uint8_t k = 'a';
  while (1)
  {
  max7219_display_clean(1);
  max7219_display_clean(3);
  delay_ms(1000);
  max7219_display_char(2,k++,UPSIDE_DOWN);
    if (k == 'z')
    {
      max7219_display_char(3,'B',UPSIDE_DOWN);
      max7219_display_char(2,'i',UPSIDE_DOWN);
      max7219_display_char(1,'H',UPSIDE_DOWN);
      delay_ms(1000);
      k = 'a';
    }
  }
  return 0;
}


