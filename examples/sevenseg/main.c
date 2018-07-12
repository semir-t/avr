#include <avr/io.h>
#include "sevenseg.h"


int main(void)
{
  sevenseg_init(SEVENSEG_COMMON_ANODE);
  float value = 11.5;
  uint16_t systick = 0;
  sevenseg_print(value);
  while(1)
  {

    if(systick >= 500)
    {
      sevenseg_print(value);
      value += 1;
      systick = 0;
      if(value > 100)
      {
        value = 0;
      }
    }
    sevenseg_refresh();
    delay_ms(1);
    ++systick;
  }
  return 0;
}
