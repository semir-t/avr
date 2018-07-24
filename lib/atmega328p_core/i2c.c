#include "i2c.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Private functions
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
static void i2c_start(void)/*{{{*/
{
  TWCR = _BV(TWSTA) | _BV(TWINT) | _BV(TWEN); // clear interrupt flag and generate start condition
  while(!(TWCR & _BV(TWINT))); // wait until I2C master is done 
}/*}}}*/
static void i2c_stop(void)/*{{{*/
{
  TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN); // clear interrupt flag and generate stop condition
}/*}}}*/

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Public functions
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void i2c_init(uint8_t prescaler,uint8_t bit_rate)/*{{{*/
{
  TWSR |= prescaler;    // set prescaler value 
  TWBR = bit_rate;      // set bit rate value 
  TWCR |= _BV(TWEN);    // enable I2C hardware
}/*}}}*/

uint8_t i2c_rx_data(uint8_t address, uint8_t * data, uint8_t cnt)/*{{{*/
{
  i2c_start();
  uint8_t i2c_status = I2C_E_OK;
  if((TWSR & I2C_S_MASK) != I2C_S_MR_START)
  {
    /* Check value of TWI Status Register. Mask */
    /* prescaler bits. If status different from */
    /* START return ERROR. */
    i2c_status = I2C_E_START;
  }
  else
  {
    TWDR = (address << 1) | I2C_READ;    // select slave address
    TWCR = _BV(TWINT) | _BV(TWEN);        //start transmision
    while(!(TWCR & _BV(TWINT)));          // wait until I2C master is done 
    if((TWSR & I2C_S_MASK) != I2C_S_MR_ADDRESS_ACK)
    {
      /* Check value of TWI Status Register. Mask */
      /* prescaler bits. If status different from */
      /* MT_SLA_ACK go to ERROR. */
      i2c_status = I2C_E_ADDRESS;
    }
    else
    {
      uint8_t k = 0;
      for( k = 0; (k < cnt) && (i2c_status != I2C_E_ADDRESS); ++k )
      {
        TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWEA); //start transmision
        while(!(TWCR & _BV(TWINT))); // wait until I2C master is done 
        if((TWSR & I2C_S_MASK) != I2C_S_MR_DATA_ACK)
        {
          i2c_status = I2C_E_DATA;
        }
        else
        {
          data[k] = TWDR; 
        }
      }
    }
  }
  i2c_stop();
  return i2c_status;
}/*}}}*/
uint8_t i2c_tx_data(uint8_t address, uint8_t * data, uint8_t cnt)/*{{{*/
{
  i2c_start();
  uint8_t i2c_status = I2C_E_OK;
  if((TWSR & I2C_S_MASK) != I2C_S_MT_START)
  {
    /* Check value of TWI Status Register. Mask */
    /* prescaler bits. If status different from */
    /* START return ERROR. */
    i2c_status = I2C_E_START;
  }
  else
  {
    TWDR = (address << 1) | I2C_WRITE;    // select slave address
    TWCR = _BV(TWINT) | _BV(TWEN);        //start transmision
    while(!(TWCR & _BV(TWINT)));          // wait until I2C master is done 
    if((TWSR & I2C_S_MASK) != I2C_S_MT_ADDRESS_ACK)
    {
      /* Check value of TWI Status Register. Mask */
      /* prescaler bits. If status different from */
      /* MT_SLA_ACK go to ERROR. */
      i2c_status = I2C_E_ADDRESS;
    }
    else
    {
      uint8_t k = 0;
      for( k = 0; (k < cnt) && (i2c_status != I2C_E_ADDRESS); ++k )
      {
        TWDR = data[k]; 
        TWCR = _BV(TWINT) | _BV(TWEN); //start transmision
        while(!(TWCR & _BV(TWINT))); // wait until I2C master is done 
        if((TWSR & I2C_S_MASK) != I2C_S_MT_DATA_ACK)
        {
          i2c_status = I2C_E_DATA;
        }
      }
    }
  }
  i2c_stop();
  return i2c_status;
}/*}}}*/

