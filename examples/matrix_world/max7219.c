#include "max7219.h"


const uint8_t ascii_char[97][FONT_SIZE] = {/*{{{*/
  //Signs
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
  {0x18,0x00,0x18,0x18,0x3c,0x3c,0x18,0x00},
  {0x00,0x00,0x00,0x00,0x28,0x6c,0x6c,0x00},
  {0x6c,0x6c,0xfe,0x6c,0xfe,0x6c,0x6c,0x00},
  {0x10,0x3c,0x40,0x38,0x04,0x78,0x10,0x00},
  {0x60,0x66,0x0c,0x18,0x30,0x66,0x06,0x00},
  {0xfc,0x66,0xa6,0x14,0x3c,0x66,0x3c,0x00},
  {0x00,0x00,0x00,0x0c,0x18,0x18,0x18,0x00},
  {0x60,0x30,0x18,0x18,0x18,0x30,0x60,0x00},
  {0x06,0x0c,0x18,0x18,0x18,0x0c,0x06,0x00},
  {0x00,0x6c,0x38,0xfe,0x38,0x6c,0x00,0x00},
  {0x00,0x10,0x10,0x7c,0x10,0x10,0x00,0x00},
  {0x06,0x0c,0x0c,0x0c,0x00,0x00,0x00,0x00},
  {0x00,0x00,0x00,0x3c,0x00,0x00,0x00,0x00},
  {0x06,0x06,0x00,0x00,0x00,0x00,0x00,0x00},
  {0x00,0x06,0x0c,0x18,0x30,0x60,0x00,0x00},
  //Numbers
  {0x3c,0x66,0x66,0x6e,0x76,0x66,0x3c,0x00},
  {0x7e,0x18,0x18,0x18,0x1c,0x18,0x18,0x00},
  {0x7e,0x06,0x0c,0x30,0x60,0x66,0x3c,0x00},
  {0x3c,0x66,0x60,0x38,0x60,0x66,0x3c,0x00},
  {0x30,0x30,0x7e,0x32,0x34,0x38,0x30,0x00},
  {0x3c,0x66,0x60,0x60,0x3e,0x06,0x7e,0x00},
  {0x3c,0x66,0x66,0x3e,0x06,0x66,0x3c,0x00},
  {0x18,0x18,0x18,0x30,0x30,0x66,0x7e,0x00},
  {0x3c,0x66,0x66,0x3c,0x66,0x66,0x3c,0x00},
  {0x3c,0x66,0x60,0x7c,0x66,0x66,0x3c,0x00},
  //Signs
  {0x00,0x18,0x18,0x00,0x18,0x18,0x00,0x00},
  {0x0c,0x18,0x18,0x00,0x18,0x18,0x00,0x00},
  {0x60,0x30,0x18,0x0c,0x18,0x30,0x60,0x00},
  {0x60,0x30,0x18,0x0c,0x18,0x30,0x60,0x00},
  {0x06,0x0c,0x18,0x30,0x18,0x0c,0x06,0x00},
  {0x18,0x00,0x18,0x38,0x60,0x66,0x3c,0x00},
  {0x00,0x3c,0x42,0x1a,0x3a,0x22,0x1c,0x00},
  //Upper case
  {0x66,0x66,0x66,0x7e,0x66,0x66,0x3c,0x00},
  {0x3e,0x66,0x66,0x3e,0x66,0x66,0x3e,0x00},
  {0x3c,0x66,0x06,0x06,0x06,0x66,0x3c,0x00},
  {0x3e,0x66,0x66,0x66,0x66,0x66,0x3e,0x00},
  {0x7e,0x06,0x06,0x3e,0x06,0x06,0x7e,0x00},
  {0x06,0x06,0x06,0x3e,0x06,0x06,0x7e,0x00},
  {0x3c,0x66,0x76,0x06,0x06,0x66,0x3c,0x00},
  {0x66,0x66,0x66,0x7e,0x66,0x66,0x66,0x00},
  {0x3c,0x18,0x18,0x18,0x18,0x18,0x3c,0x00},
  {0x1c,0x36,0x36,0x30,0x30,0x30,0x78,0x00},
  {0x66,0x36,0x1e,0x0e,0x1e,0x36,0x66,0x00},
  {0x7e,0x06,0x06,0x06,0x06,0x06,0x06,0x00},
  {0xc6,0xc6,0xc6,0xd6,0xfe,0xee,0xc6,0x00},
  {0xc6,0xc6,0xe6,0xf6,0xde,0xce,0xc6,0x00},
  {0x3c,0x66,0x66,0x66,0x66,0x66,0x3c,0x00},
  {0x06,0x06,0x3e,0x66,0x66,0x66,0x3e,0x00},
  {0x60,0x3c,0x76,0x66,0x66,0x66,0x3c,0x00},
  {0x66,0x36,0x1e,0x3e,0x66,0x66,0x3e,0x00},
  {0x3c,0x66,0x60,0x3c,0x06,0x66,0x3c,0x00},
  {0x18,0x18,0x18,0x18,0x18,0x5a,0x7e,0x00},
  {0x7c,0x66,0x66,0x66,0x66,0x66,0x66,0x00},
  {0x18,0x3c,0x66,0x66,0x66,0x66,0x66,0x00},
  {0xc6,0xee,0xfe,0xd6,0xc6,0xc6,0xc6,0x00},
  {0xc6,0xc6,0x6c,0x38,0x6c,0xc6,0xc6,0x00},
  {0x18,0x18,0x18,0x3c,0x66,0x66,0x66,0x00},
  {0x7e,0x06,0x0c,0x18,0x30,0x60,0x7e,0x00},
  //Signs
  {0x78,0x18,0x18,0x18,0x18,0x18,0x78,0x00},
  {0x00,0x60,0x30,0x18,0x0c,0x06,0x00,0x00},
  {0x00,0x60,0x30,0x18,0x0c,0x06,0x00,0x00},
  {0x00,0x00,0x00,0x82,0x44,0x28,0x10,0x00},
  {0x3c,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
  {0x00,0x00,0x00,0x00,0x60,0x30,0x30,0x00},
  //Lower case
  {0x7c,0x66,0x7c,0x60,0x3c,0x00,0x00,0x00},
  {0x3e,0x66,0x66,0x3e,0x06,0x06,0x06,0x00},
  {0x3c,0x66,0x06,0x66,0x3c,0x00,0x00,0x00},
  {0x7c,0x66,0x66,0x7c,0x60,0x60,0x60,0x00},
  {0x3c,0x06,0x7e,0x66,0x3c,0x00,0x00,0x00},
  {0x0c,0x0c,0x3e,0x0c,0x0c,0x6c,0x38,0x00},
  {0x3c,0x60,0x7c,0x66,0x66,0x7c,0x00,0x00},
  {0x66,0x66,0x66,0x3e,0x06,0x06,0x06,0x00},
  {0x3c,0x18,0x18,0x18,0x00,0x18,0x00,0x00},
  {0x1c,0x36,0x36,0x30,0x30,0x00,0x30,0x00},
  {0x66,0x36,0x1e,0x36,0x66,0x06,0x06,0x00},
  {0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x00},
  {0xd6,0xd6,0xfe,0xee,0xc6,0x00,0x00,0x00},
  {0x66,0x66,0x66,0x7e,0x3e,0x00,0x00,0x00},
  {0x3c,0x66,0x66,0x66,0x3c,0x00,0x00,0x00},
  {0x06,0x06,0x3e,0x66,0x66,0x3e,0x00,0x00},
  {0xf0,0xb0,0x3c,0x36,0x36,0x3c,0x00,0x00},
  {0x06,0x06,0x66,0x66,0x3e,0x00,0x00,0x00},
  {0x3e,0x40,0x3c,0x02,0x7c,0x00,0x00,0x00},
  {0x18,0x18,0x18,0x7e,0x18,0x18,0x00,0x00},
  {0x7c,0x66,0x66,0x66,0x66,0x00,0x00,0x00},
  {0x18,0x3c,0x66,0x66,0x00,0x00,0x00,0x00},
  {0x7c,0xd6,0xd6,0xd6,0xc6,0x00,0x00,0x00},
  {0x66,0x3c,0x18,0x3c,0x66,0x00,0x00,0x00},
  {0x3c,0x60,0x7c,0x66,0x66,0x00,0x00,0x00},
  {0x3c,0x0c,0x18,0x30,0x3c,0x00,0x00,0x00},
  //Signs
  {0x70,0x18,0x18,0x0c,0x18,0x18,0x70,0x00},
  {0x00,0x18,0x18,0x18,0x18,0x18,0x18,0x00},
  {0x0e,0x18,0x18,0x30,0x18,0x18,0x0e,0x00},
  {0x00,0x00,0x00,0x36,0x5c,0x00,0x00,0x00},
  {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
};
/*}}}*/


uint8_t g_buffer[BUFFER_SIZE];

void max7219_init(uint8_t prescaler)/*{{{*/
{
  max7219_spi_init(prescaler);
  //MAX7219 Initialize
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  uint8_t k = 0;
  max7219_tx_data(MAX7219_ALL_DEVICES,MAX7219_SCAN_LIMIT,MATRIX_ROWS - 1);
  max7219_tx_data(MAX7219_ALL_DEVICES,MAX7219_SCAN_LIMIT,0x07);
  max7219_tx_data(MAX7219_ALL_DEVICES,MAX7219_INTENSITY,MAX7219_31);
  max7219_display_clean(MAX7219_ALL_DEVICES);
  max7219_tx_data(MAX7219_ALL_DEVICES,MAX7219_SHUTDOWN,MAX7219_ON);
  max7219_tx_data(MAX7219_ALL_DEVICES,MAX7219_DISPLAY_TEST,MAX7219_DISPLAY_TEST_OFF);
  max7219_tx_data(MAX7219_ALL_DEVICES,MAX7219_DECODE_MODE,MAX7219_NO_DECODE);
  //Clean buffer
  for( k = 0; k < BUFFER_SIZE; ++k )
  {
    g_buffer[k] = 0x00;
  }
}/*}}}*/
void max7219_test(uint8_t device_no, uint8_t test_state)/*{{{*/
{
  max7219_tx_data(device_no,MAX7219_DISPLAY_TEST,test_state);
}/*}}}*/

void max7219_tx_byte(uint8_t address,uint8_t data)/*{{{*/
{
  max7219_spi_tx(address);
  max7219_spi_tx(data);
}/*}}}*/
void max7219_tx_data(uint8_t device_no, uint8_t address, uint8_t data)/*{{{*/
{
  uint8_t k;
  uint8_t shift_data = MAX7219_NOP;
  if (device_no == MAX7219_ALL_DEVICES)
  {
    shift_data = data;
    device_no = NUMBER_OF_DEVICES;
  }
  MAX7219_SS_LOW;
  for ( k = 0; k < (NUMBER_OF_DEVICES - device_no); ++k)
  {
    //send NOP
    max7219_tx_byte(address,MAX7219_NOP);
  }
  max7219_tx_byte(address,data);
  for ( k = 1; k < device_no; ++k )
  {
    max7219_tx_byte(address,shift_data);
  }
  MAX7219_SS_HIGH;
}/*}}}*/

void max7219_display_clean(uint8_t device_no)/*{{{*/
{
  max7219_display_char(device_no,' ',UPSIDE);
}/*}}}*/
void max7219_display_scroll(char * string,uint8_t orientation, uint8_t direction,uint8_t speed)/*{{{*/
{
  uint8_t shift_cnt = 0;
  uint8_t string_cnt = 0;
  uint8_t * ptr_clear;
  uint8_t buffer_tx_cnt = direction ? 7 : 0;
  uint8_t buffer_rx_cnt = direction ? 7 : 8;
  uint8_t data;
  uint8_t ascii_data[BUFFER_ROWS];
  uint8_t k,n;
  uint8_t current_data_cnt;
  uint8_t buffer[BUFFER_SIZE];

  for(k = 0;k < BUFFER_SIZE; ++k)
  {
    //clean buffer
    buffer[k] = 0x00;
  }
  while((string[string_cnt] != '\0') || (shift_cnt < 8))
  {
    //We shifted out one character
    if(shift_cnt >= 8)
    {
      shift_cnt = 0;
      ++string_cnt;
    }
    //Time to add next character to the buffer
    if(shift_cnt == 0)
    {
      data = string[string_cnt];
      if(data == '\0')
      {
        data = 128; 

      }
      data -= ' ';
      for(n = 0; n < 8; ++n) 
      {
        ascii_data[n]= ascii_char[data][n];
      }
      //Rotate the character as instructed by paramter orientation
      max7219_rotate(ascii_data,orientation);
      //Scrolling direction
      if(direction)
      {
        //Adding the data in the reverse order because 
        //when displaying text on the display 
        //rx_cnt is decrementing
        for(n = 0; n < 8; ++n)
        {
          buffer[buffer_tx_cnt] = ascii_data[7 - n];
          --buffer_tx_cnt;
        }
        if(buffer_tx_cnt >= BUFFER_SIZE)
        {
          buffer_tx_cnt = BUFFER_SIZE - 1;
        }
      }
      else
      {
        //Adding the data in the reverse order because 
        //when displaying text on the display 
        //rx_cnt is incremeting 
        for(n = 0; n < 8; ++n)
        {
          buffer[buffer_tx_cnt] = ascii_data[n];
          ++buffer_tx_cnt;
        }
        if(buffer_tx_cnt >= BUFFER_SIZE)
        {
          buffer_tx_cnt = 0;
        }
      }
    }
    for(n = 0; n < 8; ++n)
    {
      //Setting the led state of every column. If multiple 
      //displays were used then first column of every display
      //would be updated, them second column and ect. That is 
      //why we increment current data by 8
      current_data_cnt = buffer_rx_cnt + n;
      MAX7219_SS_LOW;
      for(k = 0; k < NUMBER_OF_DEVICES; ++k )
      {
        if(current_data_cnt >= BUFFER_SIZE)
        {
          current_data_cnt = current_data_cnt - BUFFER_SIZE;   
        }
        if( n == 7 && (k == NUMBER_OF_DEVICES - 1) )
        {
          ptr_clear = (buffer + current_data_cnt);
        }
        max7219_tx_byte(n+1,buffer[current_data_cnt]);
        current_data_cnt = current_data_cnt + 8;
      }
      MAX7219_SS_HIGH;
    }
    if(direction)
    {
      *ptr_clear = 0x00;
      --buffer_rx_cnt;
      if(buffer_rx_cnt >= BUFFER_SIZE)
      {
        buffer_rx_cnt = BUFFER_SIZE - 1;
      }
    }
    else
    {
      buffer[buffer_rx_cnt] = 0x00; 
      ++buffer_rx_cnt;
      if(buffer_rx_cnt >= BUFFER_SIZE)
      {
        buffer_rx_cnt = 0;
      }
    }
    ++shift_cnt;
    delay_ms(speed);
  }
  //After the last character is sent in the buffer
  //we continue to send data until the last character 
  //shifts out from the display
  buffer_tx_cnt = buffer_rx_cnt + 1;
  buffer_tx_cnt = buffer_tx_cnt >= BUFFER_SIZE ? 0 : buffer_tx_cnt;
  shift_cnt = 0;
  for(shift_cnt = 0; shift_cnt < ((NUMBER_OF_DEVICES - 1)* 8); ++ shift_cnt)
  {
    for(n = 0; n < 8; ++n)
    {
      current_data_cnt = buffer_rx_cnt + n;
      MAX7219_SS_LOW;
      for(k = 0; k < NUMBER_OF_DEVICES; ++k )
      {
        if(current_data_cnt >= BUFFER_SIZE)
        {
          current_data_cnt = current_data_cnt - BUFFER_SIZE; 
        }
        if( n == 7 && (k == NUMBER_OF_DEVICES - 1) )
        {
          ptr_clear = (buffer + current_data_cnt);
        }
        max7219_tx_byte(n+1,buffer[current_data_cnt]);
        current_data_cnt = current_data_cnt + 8;
      }
      MAX7219_SS_HIGH;
    }
    if(direction)
    {
      *ptr_clear = 0x00;
      --buffer_rx_cnt;
      if(buffer_rx_cnt >= BUFFER_SIZE)
      {
        buffer_rx_cnt = BUFFER_SIZE - 1;
      }
    }
    else
    {
      buffer[buffer_rx_cnt] = 0x00; 
      ++buffer_rx_cnt;
      if(buffer_rx_cnt >= BUFFER_SIZE)
      {
        buffer_rx_cnt = 0;
      }
    }
    delay_ms(speed);
  }
}/*}}}*/
void max7219_display_char(uint8_t device_no,uint8_t data,uint8_t orientation)/*{{{*/
{
  uint8_t buffer[FONT_SIZE];
  uint8_t k,n,j;
  for(n = 0; n < 8; ++n) 
  {
    buffer[n]= ascii_char[data - ' '][n];
  }
  max7219_rotate(buffer,orientation);
  for(n = 0; n < 8; ++n)
  {
    g_buffer[(NUMBER_OF_DEVICES - device_no) * 8 + n] = buffer[n];
  }
  for(n = 0 ; n < MATRIX_COLS; ++n)
  {
    MAX7219_SS_LOW;
    for(k = 0, j = n; k < NUMBER_OF_DEVICES; ++k )
    {
      max7219_tx_byte(n+1,g_buffer[j]);
      j += 8;
    }
    MAX7219_SS_HIGH;
  }
}/*}}}*/

void max7219_rotate(uint8_t data[],  uint8_t rotate_directon)/*{{{*/
{
  //Transpsoing the data and then mirroring obtained data
  //we are actually rotating data 
  uint8_t k = 0;
  uint8_t n = 0;
  uint8_t j;
  uint8_t temp = 0;
  //transposing the data. 
  for(j = 0;j < rotate_directon; ++j)
  {
    for(k = 0; k < 8; ++k)
    {
      for(n = k; n < 8; ++n)
      {
        if(k != n)
        {
          temp =  (data[k] >> (7 - n)) & 0x01;
          data[k] &=  ~(0x80 >> n);
          data[k] |= (data[n] & (0x80 >> k)) >> (n - k);
          data[n] &= ~(0x80 >> k);
          data[n] |=  (temp << (7 - k));
        }
      } 
    }
    //mirroring the data
    for(k = 0; k < 4; ++k)
    {
      temp = data[k]; 
      data[k] = data[7 - k];  
      data[7 - k] = temp;
    }
  }
}/*}}}*/

