#include <avr/io.h>
#include <stdlib.h>
#include "usart.h"
#include "print.h"
#include "delay.h"
#include "spi.h"
#include "ff.h"
#include "mmc.h"


FATFS FatFs;   /* Work area (filesystem object) for logical drive */

int main(void)
{
  usart_init(USART_BAUDRATE_115200);
  print_init(usart_tx_byte);

  FIL fil;        /* File object */
  char line[100]; /* Line buffer */
  FRESULT fr;     /* FatFs return code */


  /* Register work area to the default drive */
  fr = f_mount(&FatFs, "", 0);
  if (fr)
  {
    print("SYS-> MOUNT ERROR[%d]\n",fr);
    return (int)fr;
  }

  /* Open a text file */
  fr = f_open(&fil, "helloo.txt",FA_CREATE_ALWAYS | FA_WRITE | FA_READ);
  if (fr)
  {
    print("SYS-> OPEN ERROR[%d]\n",fr);
    return (int)fr;
  }
  uint8_t k = 0;
  uint8_t n = 0;
  for( n = 0; n < 20; ++n )
  {
    if((k = f_puts("Hello world!\n",&fil)) == -1)
    {
      print("SYS-> WRITE ERROR\n");
    }
    else
    {
      print("SYS-> WRITE DONE [%d]\n",k);
    }
  }

  /* Read all lines and display it */
  f_lseek(&fil,0);
  print("SYS-> READ LINE\n");
  while (f_gets(line, sizeof line, &fil)) {
    print("%s\n",line);
  }


  /* Close the file */
  if(f_close(&fil) == FR_OK)
  {
    print("CLOSE FILE SUCCESS\n");
  }
  else
  {
    print("CLOSE FILE FAIL\n");

  }


  DDRB = 0xFF; //Define PORTB as output
  PORTB |= 0x20; // Set PB6 to HIGH

  while(1)
  {
    PORTB ^= 0x20;
    delay_ms(1000);
  }
  return 0;
}


