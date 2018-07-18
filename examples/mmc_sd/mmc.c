#include "mmc.h"

/*--------------------------------------------------------------------------

  Module Private Functions

  ---------------------------------------------------------------------------*/

/* MMC/SD command (SPI mode) */

#define MMC_CMD0                                      (0)     /* GO_IDLE_STATE */
#define MMC_CMD1                                      (1)     /* SEND_OP_COND */
#define MMC_ACMD41                                    (0x80+41) /* SEND_OP_COND (SDC) */
#define MMC_CMD8                                      (8)     /* SEND_IF_COND */
#define MMC_CMD9                                      (9)     /* SEND_CSD */
#define MMC_CMD10                                     (10)    /* SEND_CID */
#define MMC_CMD12                                     (12)    /* STOP_TRANSMISSION */
#define MMC_CMD13                                     (13)    /* SEND_STATUS */
#define MMC_ACMD13                                    (0x80+13) /* SD_STATUS (SDC) */
#define MMC_CMD16                                     (16)    /* SET_BLOCKLEN */
#define MMC_CMD17                                     (17)    /* READ_SINGLE_BLOCK */
#define MMC_CMD18                                     (18)    /* READ_MULTIPLE_BLOCK */
#define MMC_CMD23                                     (23)    /* SET_BLOCK_COUNT */
#define MMC_ACMD23                                    (0x80+23) /* SET_WR_BLK_ERASE_COUNT (SDC) */
#define MMC_CMD24                                     (24)    /* WRITE_BLOCK */
#define MMC_CMD25                                     (25)    /* WRITE_MULTIPLE_BLOCK */
#define MMC_CMD32                                     (32)    /* ERASE_ER_BLK_START */
#define MMC_CMD33                                     (33)    /* ERASE_ER_BLK_END */
#define MMC_CMD38                                     (38)    /* ERASE */
#define MMC_CMD55                                     (55)    /* APP_CMD */
#define MMC_CMD58                                     (58)    /* READ_OCR */

/* Card type flags (CardType) */
#define CT_MMC				                            0x01	/* MMC ver 3 */
#define CT_SD1				                            0x02	/* SD ver 1 */
#define CT_SD2				                            0x04	/* SD ver 2 */
#define CT_BLOCK			                            0x08	/* Block addressing */

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Private functions
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static void mmc_select(void)/*{{{*/
{
  MMC_SS_HIGH;
  spi_tx_byte(0xff);
  MMC_SS_LOW;
}/*}}}*/
static void mmc_deselect(void)/*{{{*/
{
  MMC_SS_HIGH;
  /* spi_tx_byte(0xff); */
}/*}}}*/

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Public functions
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

uint8_t mmc_init(void)/*{{{*/
{
  print("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
  print("SYS-> SD card begin initialization\n");
  print("SYS-> Initialize SPI --- ");
  spi_master_init(SPI_BAUDRATE_PRESCALER_64);
  print("DONE\n");

  uint8_t k = 0;
  uint8_t status = 0x00;
  //Send 80 CLK pulses need to initialize SD card
  MMC_SS_HIGH; 
  print("SYS-> SEND 80 dummy CLK pulses --- ");
  for(k = 0; k < 10; ++k)
  {
    sd_spi_rxtx_byte(0xff);
  }
  print("DONE\n");

  k = 0;
  print("SYS-> Send command [SD_CMDO]\n");
  while((mmc_tx_command(MMC_CMD0,0x00000000,0x95) != 1)  && (k < 10))
  {
    ++k;
    delay_ms(100);
  }
  if(k == 10) 
  {
    //Card didn't respond to initialization
    status = 0x01; 
  }
  else
  {
    k = 0;
    // CMD1 until card comes out of idle, but maximum of 10 times
    print("\n\n\t%d\n",k);
    while((mmc_tx_command(MMC_CMD1,0x00000000,0xff) != 0) && (k < 20))
    {
      ++k;
      print("\n\n\t%d\n",k);
      delay_ms(100);
    }
    if(k == 10)
    {
      //card did not came out of idle SPI state
      status = 0x02;
    }
    else
    {

      //SET_BLOCKLEN to 512
      mmc_tx_command(MMC_CMD16, 0x00000200, 0xFF);
    }
  }
  MMC_CLK_FAST;
  print("SYS-> SD card initialization end\n");
  print("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
  return status;
  //
}/*}}}*/
uint8_t mmc_tx_command(uint8_t command, uint32_t arg, uint8_t crc)/*{{{*/
{
  mmc_select();
  print("\t SS_LOW\n");
  sd_spi_rxtx_byte(command | 0x40);
  sd_spi_rxtx_byte((arg >> 24) & 0xff);
  sd_spi_rxtx_byte((arg >> 16) & 0xff);
  sd_spi_rxtx_byte((arg >> 8) & 0xff);
  sd_spi_rxtx_byte(arg & 0xff);
  sd_spi_rxtx_byte(crc);

  uint8_t cnt = 100;
  uint8_t status = 0xff;
  while((status == 0xff) && (--cnt))
  {
    //wait for ack.
    status = sd_spi_rxtx_byte(0xff);
  }
  print("\t CMD [%xb]  --- status [%xb] \n",command,status);
  /* if(command != MMC_CMD24) */
  /* { */
  mmc_deselect();
  /* } */
  print("\t SS_HIGH\n");
  return status;
}/*}}}*/

uint8_t mmc_read(uint32_t sector, uint8_t * buffer)/*{{{*/
{
  uint8_t status = 0x00;
  uint8_t command_status = 0xff;
  uint8_t try_cnt = 0;

  try_cnt = 10;
  do
  {
    command_status = mmc_tx_command(MMC_CMD17,sector << 9,0xff);
    --try_cnt;  
  } while((command_status != 0x00) && try_cnt);

  if(try_cnt == 0)
  {
    //READ_ONE_SECTOR_ERROR
    status = 0x01;
  }
  else
  {
    try_cnt = 100;
    mmc_select();
    while (sd_spi_rxtx_byte(0xff)!= 0xfe && try_cnt)
    {
      --try_cnt;
    }
    if(try_cnt == 0)
    {
      //DATA CAN'T BE READ 
      status = 0x02;
    }
    else
    {
      uint16_t k = 0;
      for (k = 0; k < 512; ++k)
      {
        buffer[k] = sd_spi_rxtx_byte(0xff);
      }
      //Send dummy CRC bytes
      sd_spi_rxtx_byte(0xff);
      sd_spi_rxtx_byte(0xff);
    }
    mmc_deselect();
  }
  return status;
}/*}}}*/
uint8_t mmc_write(uint32_t sector, uint8_t * buffer)
{
  uint8_t status = 0x00;
  uint8_t r1 = 0xff;
  uint16_t try_cnt = 0;

  try_cnt = 10;
  do
  {
    r1 = mmc_tx_command(MMC_CMD24,sector << 9,0xff);
  } while((r1 != 0x00) && --try_cnt);
  if(try_cnt == 0)
  {
    status = 0x01;
  }
  else
  {
    print("SYS-> SEND START TOKEN\n");
    MMC_SS_LOW;
    /* sd_spi_rxtx_byte(0xff); //Send start token */
    /* mmc_select(); */
    sd_spi_rxtx_byte(0xfe); //Send start token
    print("SYS-> SEND DATA\n");
    uint16_t k = 0;
    for (k = 0; k < 512; ++k)
    {
      if((k % 8 == 0))
      {
        print("\n%d. ",k/8);
      }
      print(" %xb ",buffer[k]);
      sd_spi_rxtx_byte(buffer[k]);
    }
    print("\n");
    //Send dummy CRC bytes
    print("SYS-> SEND DUMMY CRC\n");
    sd_spi_rxtx_byte(0xff);
    sd_spi_rxtx_byte(0xff);
    r1 = sd_spi_rxtx_byte(0xff);
    if((r1 & 0x1f) != 0x05)
    {
      print("CHECH WRITE DATA ERROR\n");
      //READ_ONE_SECTOR_ERROR
      status = 0x02;
    }
    print("SYS-> WAIT WHILE BUSY: ");
    while(sd_spi_rxtx_byte(0xff) != 0xff)
    {
      print("*");
    }
    print("\n");
    mmc_deselect();
  }


  return status;
}





