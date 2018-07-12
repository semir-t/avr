#include "spi_software.h"

void spi_software_init(void)/*{{{*/
{
  SPI_DDR |= _BV(SPI_MOSI) | _BV(SPI_SCK) | _BV(SPI_SSN);
  SSN_HIGH;
  SCK_LOW;
}/*}}}*/
void spi_software_tx_byte(uint8_t data, uint8_t first_bit)/*{{{*/
{
  uint8_t k,bit;
  for(k = 0; k < 8; ++k)
  {
    bit = first_bit ? k : (7 - k);
    bit = (data >> bit) & 0x01;
    if(bit)
    {
      MOSI_HIGH;
    }
    else
    {
      MOSI_LOW;
    }
    SCK_HIGH;
    delay_us(SPI_PERIOD);
    SCK_LOW;
    delay_us(SPI_PERIOD);
  }
}/*}}}*/
