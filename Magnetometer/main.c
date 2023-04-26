#include "stm32f4xx.h"
#include "cpal_i2c.h"
#include "cpal.h"
#include "BSP_Magnetometer.h"
#include "stdlib.h"
#include "globals.h"

CPAL_TransferTypeDef    TX , RX;
   uint32_t Slave_Add2, Slave_Add1;
   uint8_t pBuf_TX=0;
   uint8_t pBuf_RX;
 BSP_MagFetchTypeDef BSP_MagFetchStructure;
 uint8_t info[6];
 uint16_t CPAL_Not_Working = 0;         // check for this variable in other files coz it was an extern variable



int initilize_magnetometer()                                                      //initilize the configurations
{
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
  
  CPAL_I2C_Init(&I2C2_DevStructure);
    return 1;
}
   
uint8_t BSP_Magnetometer_GetValues(CPAL_InitTypeDef* I2C2_DevStructure, BSP_MagFetchTypeDef* BSP_MagFetchStructure)
{

	int16_t x,y,z;
	// uint8_t info[6];
	CPAL_TransferTypeDef* TxStruct=I2C2_DevStructure->pCPAL_TransferTx;
	CPAL_TransferTypeDef* RxStruct=I2C2_DevStructure->pCPAL_TransferRx;
	uint8_t* TxBuf=I2C2_DevStructure->pCPAL_TransferTx->pbBuffer;
	uint8_t*  RxBuf=I2C2_DevStructure->pCPAL_TransferRx->pbBuffer;

	TxStruct->wAddr2 = 0x00;
	TxStruct->wNumData = 1;
	TxBuf[0]=0x10;
	if (CPAL_I2C_Write(I2C2_DevStructure) != CPAL_PASS)
	{ return ERROR;}
	/* Wait the end of transfer */
	while((I2C2_DevStructure->CPAL_State != CPAL_STATE_READY)&&(CPAL_Not_Working==0));
        if(CPAL_Not_Working==1) return FAIL;
        
	TxStruct->wAddr2 = 0x01;
	TxStruct->wNumData = 1;
	TxBuf[0]=0x00;
	if (CPAL_I2C_Write(I2C2_DevStructure) != CPAL_PASS)
	{ return ERROR;  }
	/* Wait the end of transfer */
	while((I2C2_DevStructure->CPAL_State != CPAL_STATE_READY)&&(CPAL_Not_Working==0));
        if(CPAL_Not_Working==1) return FAIL;

	TxStruct->wAddr2 = 0x02;
	TxStruct->wNumData = 1;
	TxBuf[0]=0x00;
	if (CPAL_I2C_Write(I2C2_DevStructure) != CPAL_PASS)
	{ return ERROR;  }
	/* Wait the end of transfer */
	while((I2C2_DevStructure->CPAL_State != CPAL_STATE_READY)&&(CPAL_Not_Working==0));
        if(CPAL_Not_Working==1) return FAIL;

	RxStruct->wAddr2 = 0x03;
	RxStruct->wNumData = 1;
	if (CPAL_I2C_Read(I2C2_DevStructure) != CPAL_PASS)
	{ return ERROR;  }
	/* Wait the end of transfer */
	while((I2C2_DevStructure->CPAL_State != CPAL_STATE_READY)&&(CPAL_Not_Working==0));
        if(CPAL_Not_Working==1) return FAIL;
	info[0]=RxBuf[0];

	RxStruct->wAddr2 = 0x04;
	RxStruct->wNumData = 1;
	if (CPAL_I2C_Read(I2C2_DevStructure) != CPAL_PASS)
	{ return ERROR;  }
	/* Wait the end of transfer */
	while((I2C2_DevStructure->CPAL_State != CPAL_STATE_READY)&&(CPAL_Not_Working==0));
        if(CPAL_Not_Working==1) return FAIL;
	info[1]=RxBuf[0];

	RxStruct->wAddr2 = 0x05;
	RxStruct->wNumData = 1;
	if (CPAL_I2C_Read(I2C2_DevStructure) != CPAL_PASS)
	{ return ERROR;  }
	/* Wait the end of transfer */
	while((I2C2_DevStructure->CPAL_State != CPAL_STATE_READY)&&(CPAL_Not_Working==0));
        if(CPAL_Not_Working==1) return FAIL;
	info[2]=RxBuf[0];

	RxStruct->wAddr2 = 0x06;
	RxStruct->wNumData = 1;
	if (CPAL_I2C_Read(I2C2_DevStructure) != CPAL_PASS)
	{ return ERROR;  }
	/* Wait the end of transfer */
	while((I2C2_DevStructure->CPAL_State != CPAL_STATE_READY)&&(CPAL_Not_Working==0));
        if(CPAL_Not_Working==1) return FAIL;
	info[3]=RxBuf[0];

	RxStruct->wAddr2 = 0x07;
	RxStruct->wNumData = 1;
	if (CPAL_I2C_Read(I2C2_DevStructure) != CPAL_PASS)
	{ return ERROR;  }
	/* Wait the end of transfer */
	while((I2C2_DevStructure->CPAL_State != CPAL_STATE_READY)&&(CPAL_Not_Working==0));
        if(CPAL_Not_Working==1) return FAIL;
	info[4]=RxBuf[0];

	RxStruct->wAddr2 = 0x08;
	RxStruct->wNumData = 1;
	if (CPAL_I2C_Read(I2C2_DevStructure) != CPAL_PASS)
	{ return ERROR; }
	/* Wait the end of transfer */
	while((I2C2_DevStructure->CPAL_State != CPAL_STATE_READY)&&(CPAL_Not_Working==0));
        if(CPAL_Not_Working==1) return FAIL;
	info[5]=RxBuf[0];

	x=info[0]<<8 | info[1];
	z=info[2]<<8 | info[3];
	y=info[4]<<8 | info[5];

	double raw_x=x*0.73/1000;  /* change the conversion factor according to the gain*/
	double raw_y=y*0.73/1000;
	double raw_z=z*0.73/1000;

  //calibration model
  BSP_MagFetchStructure->BSP_Mag_x=0.964963*raw_x-0.0115305*raw_y-0.05477*raw_z-11.5491;
  BSP_MagFetchStructure->BSP_Mag_y=0.00796178*raw_x+0.9088137*raw_y+0.0395747*raw_z+76.33987;
  BSP_MagFetchStructure->BSP_Mag_z=0.087091*raw_x-0.13882*raw_y+1.05917*raw_z-24.361795;


	return SUCCESS;
}

   
int main()
{
   initilize_magnetometer();
   while(BSP_Magnetometer_GetValues(&I2C2_DevStructure,&BSP_MagFetchStructure)!= SUCCESS);

  
  while(1)
  {}
  
}