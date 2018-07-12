#include <avr/io.h>
#include "delay.h"

int main(void)
{
  DDRB = 0xFF; //Define PORTB as output
  PORTB |= 0x20; // Set PB6 to HIGH

  while(1)
  {
    PORTB ^= 0x20;
    delay_ms(500);
  }
  return 0;
}
