#include <avr/io.h>
#include <stdlib.h>
#include "usart.h"
#include "print.h"
#include "delay.h"
#include "spi.h"
#include "mmc.h"



int main(void)
{
  usart_init(USART_BAUDRATE_115200);
  print_init(usart_tx_byte);
  
  uint8_t buffer[512];
  uint16_t k = 0;
  for(k = 0; k < 512; ++k)
  {
    buffer[k] = 0x44;
  }
  print("START:\n");
  print("SYS-> WRITE_NO_INIT: [%xb]\n",mmc_write(1000,buffer,1));
  print("SYS-> READ_NO_INIT: [%xb]\n",mmc_read(1000,buffer,1));
  print("SYS-> SD_INIT: [%xb]\n",mmc_init());
  print("SYS-> WRITE_INIT: [%xb]\n",mmc_write(1000,buffer,1));
  print("SYS-> READ_INIT: [%xb]\n",mmc_read(1000,buffer,1));
  uint8_t csd[16];
  uint8_t cid[16];
  mmc_csd(csd);
  mmc_cid(cid);

  print("SYS-> CSD: ");
  for( k = 0; k < 16 ; ++k )
  {
    print(" %xb ",csd[k]);
  }
  print("\n");
  print("SYS-> CID: ");
  for( k = 0; k < 16 ; ++k )
  {
    print(" %xb ",cid[k]);
  }
  print("\n");
  DDRB = 0xFF; //Define PORTB as output
  PORTB |= 0x20; // Set PB6 to HIGH

  while(1)
  {
    PORTB ^= 0x20;
    delay_ms(1000);
  }
  return 0;
}


