#include "stm32f4xx.h"
#include "cpal_i2c.h"
#include "cpal.h"



 CPAL_TransferTypeDef    TX , RX;
   uint32_t Slave_Add2, Slave_Add1;
   uint8_t pBuf_TX=0;
   uint8_t pBuf_RX;
 BSP_MagFetchTypeDef BSP_MagFetchStructure;
 uint8_t info[6];
 uint16_t CPAL_Not_Working = 0;         // check for this variable in other files coz it was an extern variable

  
  RX.pbBuffer = &pBuf_RX;                                
  RX.wNumData = 1u;
  RX.wAddr1 = 0x3D;
  RX.wAddr2 = 0x0A;
           
  TX.pbBuffer = &pBuf_TX;
  TX.wNumData = 1u;
  TX.wAddr1 = 0x3C; //0xD0
  TX.wAddr2 = 0x02;
  
  
  CPAL_I2C_StructInit(&I2C2_DevStructure) ;
  
  I2C2_DevStructure.CPAL_Direction     = CPAL_DIRECTION_TXRX;                   /* Transmitter and Receiver direction selected */
  I2C2_DevStructure.CPAL_Mode          = CPAL_MODE_MASTER;                      /* Mode Master selected */
  I2C2_DevStructure.CPAL_ProgModel     = CPAL_PROGMODEL_INTERRUPT;              /* DMA Programming Model selected */
  I2C2_DevStructure.pCPAL_TransferTx   = &TX;                                    /* Point pCPAL_TransferTx to a Null pointer */
  I2C2_DevStructure.pCPAL_TransferRx   = &RX;
  #include "stm32f4xx.h"
#include "cpal_i2c.h"
#include "cpal.h"
  CPAL_I2C_Init(I2C2_DevStructure);
    return