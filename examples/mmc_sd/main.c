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
  

  if(mmc_init() != 0)
  {
    print("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    print("SD CARD INITIALIZATION IS NOT SUCCESSFUL\n");
    print("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
  }
  else
  {
    print("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    print("SD CARD INITIALIZATION IS SUCCESSFUL\n");
    print("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    print("READ SECTOR TEST\n");
    print("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    uint8_t buff[512];
    if(mmc_read(0,buff) == 0)
    {
      print("SYS-> Read sector is successful\n");
      uint16_t k = 0;
      print("DATA:\n");
      for (k = 0; k < 512; ++k)
      {
        if((k % 8 == 0))
        {
          print("\n");
        }
        print(" %xb ",buff[k]);
     }

    }

  }

  /* mmc_disk_read(buff,0x1be,count); */



  DDRB = 0xFF; //Define PORTB as output
  PORTB |= 0x20; // Set PB6 to HIGH

  while(1)
  {
    PORTB ^= 0x20;
    delay_ms(1000);
  }
  return 0;
}


