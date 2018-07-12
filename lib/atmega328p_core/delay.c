#include "delay.h"

volatile uint32_t systimer = 0;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//SYSTIMER
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void systimer_init(void)/*{{{*/
{
  TCCR2A |= _BV(WGM21); // Clear on compare mode
  TCCR2B |= _BV(CS20) | _BV(CS22); // Set prescaler to 128
  OCR2A = 0x7D; // Set Output compare register value 125 (1 ms period)
  TIMSK2 |= 0x02; // Enable interupt for OCRA register
  sei();
}/*}}}*/
ISR(TIMER2_COMPA_vect)/*{{{*/
{
  ++systimer;
}/*}}}*/
uint32_t systimer_get(void)/*{{{*/
{
  uint32_t time = 0;
  cli();
  time = systimer;
  sei();
  return time;
}/*}}}*/
uint8_t systimer_timeout_flag(uint32_t start_time,uint32_t period)/*{{{*/
{
  uint32_t current_time;
  current_time = systimer_get();
  if(current_time >= start_time)
  {
    if(current_time >= (start_time + period))
    {
      return SYSTIMER_TIMEOUT;
    }
    else
    {
      return SYSTIMER_KEEPALIVE;
    }
  }
  else
  {
    uint32_t utmp_32 = 0xffffffff - start_time;
    if((current_time + utmp_32) >= period)
    {
      return SYSTIMER_TIMEOUT;
    }
    else
    {
      return SYSTIMER_KEEPALIVE;
    }
  }
}/*}}}*/

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//BLOCKING DELAY FUNCTION
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void delay_ms(uint32_t delay)/*{{{*/
{
  TCCR2A |= _BV(WGM21); // enable CTC mode
  TCCR2B |= _BV(CS22); // set prescaler to 64
  OCR2A = 0xF9; // set Output compare register value to 249
  TCNT2 = 0x00; // set timer/counter value to 0
  while(delay > 0)
  {
    loop_until_bit_is_set(TIFR2,OCF2A);
    TIFR2 |= _BV(OCF2A); // reset flag
    --delay;
  }

  /* Reset timer settings */
  TCCR2A = 0x00; 
  TCCR2B = 0x00; 
}/*}}}*/
void delay_us(uint32_t delay)/*{{{*/
{
  TCCR2A |= _BV(WGM21); // enable CTC mode
  TCCR2B |= _BV(CS20); // no prescaling
  OCR2A = 0x10; // set Output compare registar value to 16
  TCNT2 = 0x0; // reset timer/counter 
  while(delay > 0)
  {
    loop_until_bit_is_set(TIFR2,OCF2A);
    TIFR2 |= _BV(OCF2A); // reset flag
    --delay;
  }
  /* Reset timer settings */
  TCCR2A = 0x00; 
  TCCR2B = 0x00; 
}/*}}}*/

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//STOPWATCH
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Max stopwatch period is about 4.2 seconds, then timer overflows.
void stopwatch_init(void)/*{{{*/
{
  TCCR1B |= _BV(CS12) | _BV(CS10); // set prescaler to 1024 
  TIFR1 |= 0x01; // enable overflow interrupt;
}/*}}}*/
void stopwatch_start(void)/*{{{*/
{
  TCNT1H = 0;
  TCNT1L = 0;
}/*}}}*/
uint16_t stopwatch_stop(void)/*{{{*/
{
  uint8_t time_low, time_high;
  time_low = TCNT1L;
  time_high = TCNT1H;
  uint16_t timer_value = (time_high << 8) | time_low;
  return timer_value;
}/*}}}*/
