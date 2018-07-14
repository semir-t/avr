#include <usart.h>


void usart_init(uint16_t baudrate)/*{{{*/
{
  // Set baudrate
  UBRR0L = baudrate & 0x00ff;
  UBRR0H = baudrate & 0x0f00;
  
  UCSR0A |= 0x00;
  UCSR0B = _BV(RXEN0) | _BV(TXEN0); //Enable RX & TX
  UCSR0C |= 0x00; 
  //8-bit data transfet, 1-stop bit, no polarity
}/*}}}*/
void usart_tx_byte(uint8_t data)/*{{{*/
{
  while(!(UCSR0A & _BV(UDRE0) ));// wait until usart is ready to send data
  UDR0 = data;

}/*}}}*/
uint8_t usart_rx_byte(void)/*{{{*/
{
  uint8_t data;
  while(!(UCSR0A | (1 << RXC0) ));// wait until usart receives data
  data = UDR0;
  return data;
}/*}}}*/

