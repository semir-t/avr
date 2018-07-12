#include "sevenseg.h"

const uint8_t c_aa_number[10] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
const uint8_t c_cc_number[10] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
const uint8_t * g_number;
uint8_t g_current_number[NUMBER_OF_DIGITS];

void sevenseg_init(uint8_t type)/*{{{*/
{
  if(type == SEVENSEG_COMMON_ANODE)
  {
    g_number = c_aa_number;
  }
  else
  {
    g_number = c_cc_number;
  }
  //declare all pins which are connected to 7seg as outputs
  GPIO_MODE_OUTPUT(SEVENSEG_D0_PIN);
  GPIO_MODE_OUTPUT(SEVENSEG_D1_PIN);
  GPIO_MODE_OUTPUT(SEVENSEG_D2_PIN);
  GPIO_MODE_OUTPUT(SEVENSEG_D3_PIN);
  GPIO_MODE_OUTPUT(SEVENSEG_D4_PIN);
  GPIO_MODE_OUTPUT(SEVENSEG_D5_PIN);
  GPIO_MODE_OUTPUT(SEVENSEG_D6_PIN);
  GPIO_MODE_OUTPUT(SEVENSEG_D7_PIN);
  //declare pins which control 7seg digit as outputs
  GPIO_MODE_OUTPUT(SEVENSEG_COMMON_1);
  GPIO_MODE_OUTPUT(SEVENSEG_COMMON_2);
  GPIO_MODE_OUTPUT(SEVENSEG_COMMON_3);

  //reset digit output value
  SEVENSEG_DIGIT_1_OFF;
  SEVENSEG_DIGIT_2_OFF;
  SEVENSEG_DIGIT_3_OFF;
}/*}}}*/
void sevenseg_print(float number)/*{{{*/
{
  uint8_t real = (uint8_t) number;
  uint8_t k = 0;
  uint8_t digit_value;
  // 1 digit behind floating point
  for(k = 0; k < NUMBER_OF_DIGITS - 1; ++k)
  {
    digit_value = real % 10;
    real = (real - digit_value) / 10;
    g_current_number[NUMBER_OF_DIGITS - 2 - k] = c_aa_number[digit_value];
  }
  g_current_number[k] = c_aa_number[(int)((number - (int) number) * 10)];
  // add dot to second digit
  g_current_number[1] &= ~(0x80);
  /* SEVENSEG_PORT = g_current_number[1]; */
  sevenseg_refresh();
}/*}}}*/
void sevenseg_refresh(void)/*{{{*/
{
  sevenseg_tx_byte(g_current_number[0]);
  SEVENSEG_DIGIT_1_ON;
  delay_ms(1);
  SEVENSEG_DIGIT_1_OFF;
  sevenseg_tx_byte(g_current_number[1]);
  SEVENSEG_DIGIT_2_ON;
  delay_ms(1);
  SEVENSEG_DIGIT_2_OFF;
  sevenseg_tx_byte(g_current_number[2]);
  SEVENSEG_DIGIT_3_ON;
  delay_ms(1);
  SEVENSEG_DIGIT_3_OFF;
}/*}}}*/
void sevenseg_tx_byte(uint8_t data)/*{{{*/
{
  data & 0x01 ? GPIO_SET(SEVENSEG_D0_PIN) : GPIO_CLEAR(SEVENSEG_D0_PIN);
  data & 0x02 ? GPIO_SET(SEVENSEG_D1_PIN) : GPIO_CLEAR(SEVENSEG_D1_PIN);
  data & 0x04 ? GPIO_SET(SEVENSEG_D2_PIN) : GPIO_CLEAR(SEVENSEG_D2_PIN);
  data & 0x08 ? GPIO_SET(SEVENSEG_D3_PIN) : GPIO_CLEAR(SEVENSEG_D3_PIN);
  data & 0x10 ? GPIO_SET(SEVENSEG_D4_PIN) : GPIO_CLEAR(SEVENSEG_D4_PIN);
  data & 0x20 ? GPIO_SET(SEVENSEG_D5_PIN) : GPIO_CLEAR(SEVENSEG_D5_PIN);
  data & 0x30 ? GPIO_SET(SEVENSEG_D6_PIN) : GPIO_CLEAR(SEVENSEG_D6_PIN);
  data & 0x80 ? GPIO_SET(SEVENSEG_D7_PIN) : GPIO_CLEAR(SEVENSEG_D7_PIN);
}/*}}}*/

