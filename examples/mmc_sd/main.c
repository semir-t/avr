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
  

  uint8_t r = mmc_init();
  print("INIT %xb",r);
  if(r != 0)
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
    print("WRITE SECTOR TEST\n");
    print("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    uint8_t buff[512];
    uint16_t k = 0;
    for(k = 0; k < 512;++k)
    {
      buff[k] = k;
    }
    if(mmc_write(0x1be,buff) == 0)
    {
    print("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    print("WRITE TO SECTOR IS SUCCESSFUL\n");
    print("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    }
    else
    {
      print("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
      print("WRITE TO SECTOR IS NOT SUCCESSFUL\n");
      print("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    }
    for(k = 0; k < 512;++k)
    {
      buff[k] = 0xff;
    }
    delay_ms(2000);
    print("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    print("READ SECTOR TEST\n");
    print("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    if(mmc_read(0x1be,buff) == 0)
    {
      print("READ FROM SECTOR IS SUCCESSFUL\n");
      print("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
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
    else
    {
      print("READ FROM SECTOR IS NOT SUCCESSFUL\n");
      print("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
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


