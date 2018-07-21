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
  f_mount(&FatFs, "", 0);

  /* Open a text file */
  fr = f_open(&fil, "hello.txt", FA_READ | FA_WRITE);
  if (fr)
  {
    print("SYS-> OPEN ERROR\n");
    return (int)fr;
  }
  uint8_t k = 0;
  if((k = f_puts("This is working!",&fil)) == -1)
  {
    print("SYS-> WRITE ERROR\n");
  }
  else
  {
    print("SYS-> WRITE DONE [%d]\n",k);
  }

  print("SYS-> READ LINE\n");
  /* Read all lines and display it */
  while (f_gets(line, sizeof line, &fil)) {
    print("%s\n",line);
  }


  /* Close the file */
  /* f_close(&fil); */


  DDRB = 0xFF; //Define PORTB as output
  PORTB |= 0x20; // Set PB6 to HIGH

  while(1)
  {
    PORTB ^= 0x20;
    delay_ms(1000);
  }
  return 0;
}


