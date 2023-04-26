#include "stm32f4xx.h"
#include "cpal_i2c.h"
#include "cpal.h"

CPAL_TransferTypeDef    TX , RX;
   uint32_t Slave_Add2, Slave_Add1;
   uint8_t pBuf_TX;
   uint8_t pBuf_RX;


int main()
{
           
  RX.pbBuffer = &pBuf_RX;
  RX.wNumData = 1u;
  RX.wAddr1 = 0x3D;
  RX.wAddr2 = 0x0A;
           
  TX.pbBuffer = &pBuf_TX;
  TX.wNumData = 1u;
  TX.wAddr1 = 0X3C; //0xD0
  TX.wAddr2 = 0x02;
  
  
  CPAL_I2C_StructInit(&I2C2_DevStructure) ;
  
  I2C2_DevStructure.CPAL_Direction     = CPAL_DIRECTION_TXRX;                  /* Transmitter and Receiver direction selected */
  I2C2_DevStructure.CPAL_Mode          = CPAL_MODE_MASTER;                     /* Mode Master selected */
  I2C2_DevStructure.CPAL_ProgModel     = CPAL_PROGMODEL_INTERRUPT;    /* DMA Programming Model selected */
  I2C2_DevStructure.pCPAL_TransferTx   = &TX;                                /* Point pCPAL_TransferTx to a Null pointer */
  I2C2_DevStructure.pCPAL_TransferRx   = &RX;
  
  CPAL_I2C_Init(&I2C2_DevStructure); 
  
  if(CPAL_I2C_Write(&I2C2_DevStructure)== CPAL_PASS);
  while(I2C2_DevStructure.CPAL_State != CPAL_STATE_READY);

  if(CPAL_I2C_Read(&I2C2_DevStructure)== CPAL_PASS);
  while(I2C2_DevStructure.CPAL_State != CPAL_STATE_READY);
  
  while(1)
  {}
  
}