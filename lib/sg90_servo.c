#include "sg90_servo.h"

void sg90_servo_init(void)/*{{{*/
{
  //Configure pin as output
  DDRB |= _BV(PB1);
  //Configure Fast PWM in non-inverting mode ICR1 TOP
  //Set prescaler to 8 
  TCCR1A |= _BV(WGM11) | _BV(COM1A1); 
  TCCR1B |= _BV(WGM13) | _BV(WGM12) | _BV(CS11);// | _BV(CS10);
  ICR1 = 0x9c40;
  /* OCR0A = 255 * 10 / 100; */
  /* OCR1A = 0x1388 * 0.075; */
  OCR1A = 0x9c40 * 0.05;
  TCNT1 = 0x00;
}/*}}}*/
void sg90_servo_position(uint8_t position)/*{{{*/
{
  float position_factor = 0.03;
  if(position > 180)
  {
    return;
  }
  position_factor += 0.09 * position / 180;
  OCR1A = 0x9c40 * position_factor;
}/*}}}*/
