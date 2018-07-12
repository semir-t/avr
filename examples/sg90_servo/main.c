#include <avr/io.h>
#include "delay.h"
#include "sg90_servo.h"


int main(void)
{
  sg90_servo_init();
  int16_t position = 0;
  int8_t step = 1;
  while(1)
  {

    sg90_servo_position(position);
    position += step;
    if(position >= 180)
    {
      step = -1;
    }
    else if( position <= 0 )
    {
      step = 1;
    }
    delay_ms(5);
  }
  return 0;
}
