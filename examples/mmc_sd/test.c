static void mmc_select(void)/*{{{*/
{
  MMC_SS_HIGH;
  mmc_spi_rxtx_byte(0xff);
  MMC_SS_LOW;
}/*}}}*/
static void mmc_deselect(void)/*{{{*/
{
  MMC_SS_HIGH;
}/*}}}*/
static void mmc_power_on(void)/*{{{*/
{
  /* Turn socket power on and wait for 10ms+ (nothing to do if no power controls) */
	/* To be filled */


	/* Enable SPI in MODE0 */
  mmc_spi_init(SPI_BAUDRATE_PRESCALER_64);
  mmc_deselect();
}/*}}}*/
static void mmc_power_off(void)/*{{{*/
{
  /* Disable SPI function */
  /* To be filled */


  /* De-configure MOSI/MISO/SCLK/CS pins (set hi-z) */
  /* To be filled */


  /* Trun socket power off (nothing to do if no power controls) */
  /* To be filled */
}/*}}}*/

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Public functions
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
uint8_t mmc_init(void)/*{{{*/
{
  mmc_power_off();
  if (g_card_status & CARD_S_NO_DISK)
  {
    return g_card_status;	/* No card in the socket? */
  }
  mmc_power_on();
  MMC_CLK_SLOW();

  uint8_t k = 0;
  uint8_t card_type = CT_UNKNOWN;
  uint8_t command_status[4];

  //Send 80 CLK pulses need to initialize SD card
  mmc_deselect();
  for(k = 0; k < 10; ++k)
  {
    mmc_spi_rxtx_byte(0xff);
  }

  k = 10;
  while((mmc_tx_command(MMC_CMD0,0x00000000,0x95) != 0)  && --k)
  {
    delay_ms(100);
  }
  if(k == 0) 
  {
    //Card didn't respond to initialization
    g_card_status = CARD_S_NO_INIT;
    print("SYS-> MMC_CMD0 command fail\n");
  }
  else
  {
    if (mmc_tx_command(MMC_CMD8,0x1aa,0x87) == 1)
    {
      /* Is the card SDv2? */
      print("SYS-> Check if the card is SDv2\n");
      mmc_select();
      uint8_t n = 0;
      for (n = 0; n < 4; n++)
      {
        command_status[n] = mmc_spi_rxtx_byte(0xff);	/* Get trailing return value of R7 resp */
      }
      mmc_deselect();
      if (command_status[2] == 0x01 && command_status[3] == 0xAA)
      {	
        /* The card can work at vdd range of 2.7-3.6V */
        k = 100;
        while (--k && mmc_tx_command(MMC_ACMD41, 1UL << 30,0xff))
        {
          /* Wait for leaving idle state (ACMD41 with HCS bit) */
          delay_ms(1);
        }
        if (k && mmc_tx_command(MMC_CMD58,0,0xff) == 0)
        {	
          /* Check CCS bit in the OCR */
          mmc_select();
          for (n = 0; n < 4; n++)
          {
            command_status[n] = mmc_spi_rxtx_byte(0xff);
          }
          mmc_deselect();
          card_type = (command_status[0] & 0x40) ? CT_SD2 | CT_BLOCK : CT_SD2;	/* Check if the card is SDv2 */
        }
      }
    } 
    else
    {
      /* SDv1 or MMCv3 */
      print("SYS-> MMC OR SD1 test: ");
      uint8_t command;
      /* if (mmc_tx_command(MMC_ACMD41,0x00000000,0xff) > 1) */
      if(0)
      {
        card_type = CT_SD1;
        command = MMC_ACMD41;	/* SDv1 */
        print("SDv1\n");
      }
      else 
      {
        card_type = CT_MMC;
        command = MMC_CMD1;	/* MMCv3 */
        print("MMC\n");
      }
      k = 100;
      while (--k && mmc_tx_command(command,0x00000000,0xff))
      {
        /* Wait for leaving idle state */
        delay_ms(10);
      }
      if ((k == 0) || (mmc_tx_command(MMC_CMD16,512,0xff) != 0))
      {
        /* Set R/W block length to 512 */
        card_type = CT_UNKNOWN;
      }
    }
    g_card_type = card_type;
    /* deselect(); */

    if (g_card_type)
    {	
      /* Initialization succeded */
      g_card_status &= ~CARD_S_NO_INIT;		/* Clear STA_NOINIT */
      MMC_CLK_FAST();
    } 
    else
    {	
      /* Initialization failed */
      mmc_power_off();
    }
  }
  return g_card_status;

}/*}}}*/

