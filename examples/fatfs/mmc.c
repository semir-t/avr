#include "mmc.h"


/*{{{ MMC/SD command (SPI mode)*/
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
/*}}}*/
/*{{{ Card type flags (g_card_type) */
#define CT_UNKNOWN				                        0x00	/* MMC ver 3 */
#define CT_MMC				                            0x01	/* MMC ver 3 */
#define CT_SD1				                            0x02	/* SD ver 1 */
#define CT_SD2				                            0x04	/* SD ver 2 */
#define CT_BLOCK			                            0x08	/* Block addressing */
/*}}}*/

static volatile uint8_t g_card_status = CARD_S_NO_INIT;
static volatile uint8_t g_card_type;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Private functions
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static uint8_t mmc_wait_ready(uint16_t cnt)/*{{{*/
{
  uint8_t status = 0x00;
  do
  {
    status = mmc_spi_rxtx_byte(0xff);
    delay_ms(1);
  }
  while (status != 0xff && --cnt);
  return (status == 0xff) ? 1 : 0;
}/*}}}*/
static uint8_t mmc_deselect(void)/*{{{*/
{
  MMC_SS_HIGH;
  return 1;
}/*}}}*/
static uint8_t mmc_select(void)/*{{{*/
{
  MMC_SS_HIGH;
  mmc_spi_rxtx_byte(0xff);
  MMC_SS_LOW;
  uint8_t status = 0x01;
  if(mmc_wait_ready(500) != 1)
  {
    //Don't select the card if card is busy
    mmc_deselect();
    status = 0x00;
  }
  return status;
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
static uint8_t mmc_tx_datablock(const uint8_t * buffer, uint8_t token)/*{{{*/
{
  uint8_t status = CARD_E_OK;
  uint8_t r1 = 0xff;

  mmc_spi_rxtx_byte(token); //Send start/stop token
  if(token != 0xfd )
  {
    uint16_t k = 0;
    for (k = 0; k < 512; ++k)
    {
#if MMC_DEBUG
      if((k % 8 == 0))
      {
        print("\n%d. ",k/8);
      }
      print(" %xb ",buffer[k]);

#endif
      mmc_spi_rxtx_byte(buffer[k]);
    }
#if MMC_DEBUG 
    print("\n");
#endif
    //Send dummy CRC bytes
    mmc_spi_rxtx_byte(0xff);
    mmc_spi_rxtx_byte(0xff);
    r1 = mmc_spi_rxtx_byte(0xff);
    if((r1 & 0x1f) != 0x05)
    {
      //READ_ONE_SECTOR_ERROR
      status = CARD_E_WRITE_DATA;
    }

  }
  else
  {
    //dummy byte
    mmc_spi_rxtx_byte(0xff);
  }
  while(mmc_spi_rxtx_byte(0xff) != 0xff)
  {
    //Wait while busy
  }
  return status;
}/*}}}*/
static uint8_t mmc_rx_datablock(uint8_t * buffer, uint8_t token)/*{{{*/
{

  uint8_t status = 0x00;
  uint8_t try_cnt = 100;
  //token 0xfe
  while (mmc_spi_rxtx_byte(0xff) != token && try_cnt)
  {
    --try_cnt;
  }
  if(try_cnt == 0)
  {
    //DATA CAN'T BE READ 
    status = CARD_E_READ_DATA;
  }
  else
  {
    uint16_t k = 0;
    for (k = 0; k < 512; ++k)
    {
      buffer[k] = mmc_spi_rxtx_byte(0xff);
    }
    //Send dummy CRC bytes
    mmc_spi_rxtx_byte(0xff);
    mmc_spi_rxtx_byte(0xff);
  }
  return status;
}/*}}}*/
static uint8_t mmc_rx_cxd(uint8_t command, uint8_t * data)/*{{{*/
{
  uint8_t status = 0x00;
  if (g_card_status & CARD_S_NO_INIT) 
  {
    status = CARD_E_NOT_READY;
  }
  if(status == 0x00)
  {
    uint8_t try_cnt = 10;
    uint8_t command_status = 0x00;
    do
    {
      command_status = mmc_tx_command(command,0x00,0xff);
      --try_cnt;  
    } while((command_status != 0x00) && try_cnt);
    if(try_cnt == 0)
    {
      //READ_ONE_SECTOR_ERROR
      status = CARD_E_CXD;
    }
    else
    {
      try_cnt = 100;
      mmc_select();
      while (mmc_spi_rxtx_byte(0xff) != 0xfe && try_cnt)
      {
        --try_cnt;
      }
      if(try_cnt == 0)
      {
        //DATA CAN'T BE READ 
        status = CARD_E_CXD;
      }
      else
      {
        uint8_t k = 0;
        for(k = 0; k < 16; ++k)
        {
          data[k] = mmc_spi_rxtx_byte(0xff);
        }
        while(mmc_spi_rxtx_byte(0xff) != 0xff)
        {
          //Wait while busy
        }
      }
      mmc_deselect();
    }
  }
  return status;
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

  //Send 80 CLK pulses need to initialize SD card
  mmc_deselect();
  for(k = 0; k < 10; ++k)
  {
    mmc_spi_rxtx_byte(0xff);
  }

  k = 10;
  while((mmc_tx_command(MMC_CMD0,0x00000000,0x95) != 1)  && --k)
  {
    delay_ms(100);
  }
  if(k == 0) 
  {
    //Card didn't respond to initialization
    g_card_status = CARD_S_NO_INIT;
  }
  else
  {
    uint8_t command_status[4];
    uint8_t status = mmc_tx_command(MMC_CMD8,0x1aa,0x87);
    if(status == 1)
    {
      /* Is the card SDv2? */
      uint8_t n = 0;
      for (n = 0; n < 4; n++)
      {
        command_status[n] = mmc_spi_rxtx_byte(0xff);	/* Get trailing return value of R7 resp */
      }
      mmc_deselect();
      if (command_status[2] == 0x01 && command_status[3] == 0xAA)
      {	
        /* The card can work at vdd range of 2.7-3.6V */
        uint16_t k = 300;
        while (--k && mmc_tx_command(MMC_ACMD41, 1UL << 30,0xff))
        {
          /* Wait for leaving idle state (ACMD41 with HCS bit) */
          delay_ms(10);
        }
        if (k && mmc_tx_command(MMC_CMD58,0,0xff) == 0)
        {	
          /* Check CCS bit in the OCR */
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
      uint8_t command;
      if (mmc_tx_command(MMC_ACMD41,0x00000000,0xff) <= 1)
      {
        card_type = CT_SD1;
        command = MMC_ACMD41;	/* SDv1 */
      }
      else 
      {
        card_type = CT_MMC;
        command = MMC_CMD1;	/* MMCv3 */
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
uint8_t mmc_tx_command(uint8_t command, uint32_t arg, uint8_t crc)/*{{{*/
{

  uint8_t status = 0x00;
  if(command & 0x80)
  {
    /* ACMD<n> is the command sequense of CMD55-CMD<n> */
    print("->ACMD\n");
    command &= 0x7f;
    status = mmc_tx_command(MMC_CMD55,0x00000000,0);
    /* if (status > 1) return res; */
  }
  if(!(status > 1))
  {
    mmc_select();
    mmc_spi_rxtx_byte(command | 0x40);
    mmc_spi_rxtx_byte((arg >> 24) & 0xff);
    mmc_spi_rxtx_byte((arg >> 16) & 0xff);
    mmc_spi_rxtx_byte((arg >> 8) & 0xff);
    mmc_spi_rxtx_byte(arg & 0xff);
    mmc_spi_rxtx_byte(crc);

    uint8_t cnt = 100;
    status = 0xff;
    while((status == 0xff) && (--cnt))
    {
      //wait for ack.
      status = mmc_spi_rxtx_byte(0xff);
    }
    if(command != MMC_CMD8 && command != MMC_CMD58)
    {
      //This command will return R1 + R7 respond.
      //To read R7 response, card must be selected
      mmc_deselect();
    }
  }
  return status;
}/*}}}*/

uint8_t mmc_read(uint32_t sector, uint8_t * buffer, uint8_t cnt)/*{{{*/
{
  uint8_t status = 0x00;

  if (!cnt)
  {
    status = CARD_E_PARAMETER;
  }
  if (g_card_status & CARD_S_NO_INIT) 
  {
    status = CARD_E_NOT_READY;
  }
  if (!(g_card_type & CT_BLOCK))
  {
    sector = sector << 9;	/* Convert to byte address if needed (*512)*/
  }
  if(status == 0x00)
  {
    uint8_t r1 = 0xff; 
    uint8_t try_cnt = 0;
    try_cnt = 10;
    uint8_t command = cnt > 1 ? MMC_CMD18 : MMC_CMD17;
    do
    {
      r1 = mmc_tx_command(command,sector,0xff);
      --try_cnt;  
    } while((r1 != 0x00) && try_cnt);
    if(try_cnt == 0)
    {
      //READ_ONE_SECTOR_ERROR
      status = CARD_E_READ_DATA;
    }
    else
    {
      try_cnt = 100;
      mmc_select();
      do
      {
        mmc_rx_datablock(buffer,0xfe);
        buffer += 512;
      }while(--cnt);
      if (command == MMC_CMD18)
      {
        mmc_tx_command(MMC_CMD12,0x00000000,0xff);	/* STOP_TRANSMISSION */
        while(mmc_spi_rxtx_byte(0xff) != 0xff)
        {
          //Wait while busy
        }
      }
      mmc_deselect();
      status = cnt ? CARD_E_READ_DATA : CARD_E_OK;
    }
  }
  return status;
}/*}}}*/
uint8_t mmc_write(uint32_t sector, const uint8_t * buffer, uint8_t cnt)/*{{{*/
{
  uint8_t status = 0x00;

  if (!cnt) 
  {
    status = CARD_E_PARAMETER;
  }
  if (g_card_status & CARD_S_NO_INIT) 
  {
    status = CARD_E_NOT_READY;
  }
  if (g_card_status & CARD_S_PROTECT )
  {
    status = CARD_E_WRITE_PROTECT;
  }
  if (!(g_card_type & CT_BLOCK))
  {
    sector *= 512;	/* Convert to byte address if needed */
  }
  if (status == 0x00 )
  {
    uint8_t r1 = 0xff;
    uint16_t try_cnt = 10;
    uint8_t command = cnt > 1 ? MMC_CMD25 : MMC_CMD24;
    uint8_t token = cnt > 1 ?  0xfc : 0xfe;
    do
    {
      r1 = mmc_tx_command(command,sector,0xff);
    } while((r1 != 0x00) && --try_cnt);
    if(try_cnt == 0)
    {
      status = CARD_E_WRITE_DATA;
    }
    else
    {
      mmc_select();
      do
      {
        mmc_tx_datablock(buffer,token);
        buffer += 512;
      } while(--cnt);
      if(command == MMC_CMD25)
      {
        if (mmc_tx_datablock(0,0xfd))
        {
          cnt = 1;	/* STOP_TRAN token */
        }

      }
      mmc_deselect();
    }
  }
  return status;
}/*}}}*/

uint8_t mmc_status(void)/*{{{*/
{
  return g_card_status;
}/*}}}*/
uint8_t mmc_csd(uint8_t * csd)/*{{{*/
{
  return mmc_rx_cxd(MMC_CMD9,csd);
}/*}}}*/
uint8_t mmc_cid(uint8_t * cid)/*{{{*/
{
  return mmc_rx_cxd(MMC_CMD10,cid);
}/*}}}*/

DRESULT mmc_ioctl( BYTE cmd, void * buff)/*{{{*/
{
  DRESULT res;
  BYTE n;
  BYTE csd[16];
  /* BYTE *ptr = buff; */
  DWORD *dp;
  DWORD st; 
  DWORD ed;
  DWORD csize;

#if _USE_ISDIO
  SDIO_CTRL *sdi;
  BYTE rc, *bp;
  UINT dc;
#endif

  if (g_card_status & STA_NOINIT)
  {
    return RES_NOTRDY;
  }

  res = RES_ERROR;
  switch (cmd) {
    case CTRL_SYNC :		/* Make sure that no pending write process. Do not remove this or written sector might not left updated. */
      if (mmc_select())
      {
        res = RES_OK;
      }
      mmc_deselect();
      break;
    case GET_SECTOR_COUNT :	/* Get number of sectors on the disk (DWORD) */
      //read csd
      if (mmc_csd(csd) == 0)
      {
        if ((csd[0] >> 6) == 1)
        {	/* SDC ver 2.00 */
          csize = csd[9] + ((WORD)csd[8] << 8) + ((DWORD)(csd[7] & 63) << 16) + 1;
          *(DWORD*)buff = csize << 10;
        }
        else
        {					/* SDC ver 1.XX or MMC*/
          n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
          csize = (csd[8] >> 6) + ((WORD)csd[7] << 2) + ((WORD)(csd[6] & 3) << 10) + 1;
          *(DWORD*)buff = csize << (n - 9);
        }
        res = RES_OK;
      }
      mmc_deselect();
      break;

        case GET_BLOCK_SIZE :	/* Get erase block size in unit of sector (DWORD) */
          if (g_card_type & CT_SD2)
          {	/* SDv2? */
            /* if (send_cmd(ACMD13, 0) == 0) */
            /* {	/1* Read SD status *1/ */
            /*   xchg_spi(0xFF); */
            /*   if (rcvr_datablock(csd, 16)) {				/1* Read partial block *1/ */
            /*     for (n = 64 - 16; n; n--) xchg_spi(0xFF);	/1* Purge trailing data *1/ */
            /*     *(DWORD*)buff = 16UL << (csd[10] >> 4); */
            /*     res = RES_OK; */
            /*   } */
            /* } */
          }
          else
          {					/* SDv1 or MMCv3 */
            if (mmc_csd(csd) == 0)
            {
              if (g_card_type & CT_SD1)
              {	/* SDv1 */
                *(DWORD*)buff = (((csd[10] & 63) << 1) + ((WORD)(csd[11] & 128) >> 7) + 1) << ((csd[13] >> 6) - 1);
              }
              else
              {					/* MMCv3 */
                *(DWORD*)buff = ((WORD)((csd[10] & 124) >> 2) + 1) * (((csd[11] & 3) << 3) + ((csd[11] & 224) >> 5) + 1);
              }
              res = RES_OK;
            }
          }
          mmc_deselect();
          break;

            case CTRL_TRIM:		/* Erase a block of sectors (used when _USE_TRIM in ffconf.h is 1) */
              /* if (!(g_card_type & CT_SDC)) */
              /* { */
                /* break;				/1* Check if the card is SDC *1/ */
              /* } */
              if (mmc_csd(csd))
              {
                break;	/* Get CSD */
              }
              if (!(csd[0] >> 6) && !(csd[10] & 0x40)) 
              {
                break;	/* Check if sector erase can be applied to the card */
              }
              dp = buff;
              st = dp[0];
              ed = dp[1];				/* Load sector block */
              if (!(g_card_type & CT_BLOCK))
              {
                st *= 512;
                ed *= 512;
              }
              if (mmc_tx_command(MMC_CMD32,st,0xff) == 0 && mmc_tx_command(MMC_CMD33, ed,0xff) == 0
                  && mmc_tx_command(MMC_CMD38, 0,0xff) == 0 && mmc_wait_ready(30000))
              {	/* Erase sector block */
                res = RES_OK;	/* FatFs does not check result of this command */
              }
              break;

              /* Following commands are never used by FatFs module */

          /*   case MMC_GET_TYPE :		/1* Get card type flags (1 byte) *1/ */
          /*     *ptr = CardType; */
          /*     res = RES_OK; */
          /*     break; */

          /*   case MMC_GET_CSD :		/1* Receive CSD as a data block (16 bytes) *1/ */
          /*     if (send_cmd(CMD9, 0) == 0 && rcvr_datablock(ptr, 16)) {	/1* READ_CSD *1/ */
          /*       res = RES_OK; */
          /*     } */
          /*     deselect(); */
          /*     break; */

          /*   case MMC_GET_CID :		/1* Receive CID as a data block (16 bytes) *1/ */
          /*     if (send_cmd(CMD10, 0) == 0 && rcvr_datablock(ptr, 16)) {	/1* READ_CID *1/ */
          /*       res = RES_OK; */
          /*     } */
          /*     deselect(); */
          /*     break; */

          /*   case MMC_GET_OCR :		/1* Receive OCR as an R3 resp (4 bytes) *1/ */
          /*     if (send_cmd(CMD58, 0) == 0) {	/1* READ_OCR *1/ */
          /*       for (n = 4; n; n--) *ptr++ = xchg_spi(0xFF); */
          /*       res = RES_OK; */
          /*     } */
          /*     deselect(); */
          /*     break; */

          /*   case MMC_GET_SDSTAT :	/1* Receive SD statsu as a data block (64 bytes) *1/ */
          /*     if (send_cmd(ACMD13, 0) == 0) {	/1* SD_STATUS *1/ */
          /*       xchg_spi(0xFF); */
          /*       if (rcvr_datablock(ptr, 64)) res = RES_OK; */
          /*     } */
          /*     deselect(); */
          /*     break; */

          /*   case CTRL_POWER_OFF :	/1* Power off *1/ */
          /*     power_off(); */
          /*     Stat |= STA_NOINIT; */
          /*     res = RES_OK; */
          /*     break; */
          /* #if _USE_ISDIO */
          /*   case ISDIO_READ: */
          /*     sdi = buff; */
          /*     if (send_cmd(CMD48, 0x80000000 | (DWORD)sdi->func << 28 | (DWORD)sdi->addr << 9 | ((sdi->ndata - 1) & 0x1FF)) == 0) { */
          /*       for (Timer1 = 100; (rc = xchg_spi(0xFF)) == 0xFF && Timer1; ) ; */
          /*       if (rc == 0xFE) { */
          /*         for (bp = sdi->data, dc = sdi->ndata; dc; dc--) *bp++ = xchg_spi(0xFF); */
          /*         for (dc = 514 - sdi->ndata; dc; dc--) xchg_spi(0xFF); */
          /*         res = RES_OK; */
          /*       } */
          /*     } */
          /*     deselect(); */
          /*     break; */

          /*   case ISDIO_WRITE: */
          /*     sdi = buff; */
          /*     if (send_cmd(CMD49, 0x80000000 | (DWORD)sdi->func << 28 | (DWORD)sdi->addr << 9 | ((sdi->ndata - 1) & 0x1FF)) == 0) { */
          /*       xchg_spi(0xFF); xchg_spi(0xFE); */
          /*       for (bp = sdi->data, dc = sdi->ndata; dc; dc--) xchg_spi(*bp++); */
          /*       for (dc = 514 - sdi->ndata; dc; dc--) xchg_spi(0xFF); */
          /*       if ((xchg_spi(0xFF) & 0x1F) == 0x05) res = RES_OK; */
          /*     } */
          /*     deselect(); */
          /*     break; */

          /*   case ISDIO_MRITE: */
          /*     sdi = buff; */
          /*     if (send_cmd(CMD49, 0x84000000 | (DWORD)sdi->func << 28 | (DWORD)sdi->addr << 9 | sdi->ndata >> 8) == 0) { */
          /*       xchg_spi(0xFF); xchg_spi(0xFE); */
          /*       xchg_spi(sdi->ndata); */
          /*       for (dc = 513; dc; dc--) xchg_spi(0xFF); */
          /*       if ((xchg_spi(0xFF) & 0x1F) == 0x05) res = RES_OK; */
          /*     } */
          /*     deselect(); */
          /*     break; */
          /* #endif */
          /*   default: */
          /*     res = RES_PARERR; */
  }

  return res;
}/*}}}*/

