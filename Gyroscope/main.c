//Almost out of storage … If you run out, you can't create or edit files, send or receive email on Gmail, or back up to Google Photos.
/*
 * BSP_Gyroscope.c
 *
 *  Created on: Jan 17, 2016
 *      Author: Yash Vardhan Agarwal
 */
#include "BSP_Gyroscope.h"
#include "globals.h"
#include "data_structs.h"
#include "mat_ops.h"

uint8_t ready ;
uint16_t CPAL_Not_Working=0;
CPAL_TransferTypeDef RxStruct, TxStruct;
uint8_t *RxBuf, *TxBuf;
BSP_GyrFetchTypeDef BSP_GyrFetchStructure;

uint32_t initialize_gyroscope(CPAL_InitTypeDef* I2C2_DevStructure)
{

	RxStruct.pbBuffer = RxBuf;
	RxStruct.wAddr1 = 0xD0;
	TxStruct.pbBuffer= TxBuf;
	TxStruct.wAddr1 = 0xD0;

	// Initialization
	CPAL_I2C_StructInit(I2C2_DevStructure);
	I2C2_DevStructure->CPAL_Mode = CPAL_MODE_MASTER;
	I2C2_DevStructure->CPAL_ProgModel = CPAL_PROGMODEL_INTERRUPT;
	I2C2_DevStructure->pCPAL_I2C_Struct->I2C_ClockSpeed = 400000;
	I2C2_DevStructure->pCPAL_TransferRx = &RxStruct;
	I2C2_DevStructure->pCPAL_TransferTx = &TxStruct;
	if(CPAL_I2C_Init(I2C2_DevStructure)!=CPAL_PASS)
	{
		return ERROR;
	}
	return SUCCESS;
}
float  factoryTrim[6];
float destination[6];
uint8_t BSP_Gyroscope_SelfTest(CPAL_InitTypeDef* I2C2_DevStructure)
{
	uint8_t rawData[4];
        float selfTest[6];
        int i;
	CPAL_TransferTypeDef* TxStruct=I2C2_DevStructure->pCPAL_TransferTx;
	uint8_t* TxBuf=I2C2_DevStructure->pCPAL_TransferTx->pbBuffer;

        CPAL_TransferTypeDef* RxStruct=I2C2_DevStructure->pCPAL_TransferRx;
        uint8_t* RxBuf=I2C2_DevStructure->pCPAL_TransferRx->pbBuffer;

	TxStruct->wAddr2 = 0x1B;
	TxStruct->wNumData = 1;
	TxBuf[0]=0xE0;
	if (CPAL_I2C_Write(I2C2_DevStructure) != CPAL_PASS)
	{ /* I2C bus or peripheral is not able to start communication: Error management */}
	/* Wait the end of transfer */

        while((I2C2_DevStructure->CPAL_State != CPAL_STATE_READY)&&(CPAL_Not_Working==0));
        if(CPAL_Not_Working==1) return FAIL;

	TxStruct->wAddr2 = 0x1C;
	TxStruct->wNumData = 1;
	TxBuf[0]=0xF0;
	if (CPAL_I2C_Write(I2C2_DevStructure) != CPAL_PASS)
	{ /* I2C bus or peripheral is not able to start communication: Error management */  }
	/* Wait the end of transfer */
	while((I2C2_DevStructure->CPAL_State != CPAL_STATE_READY)&&(CPAL_Not_Working==0));
        if(CPAL_Not_Working==1) return FAIL;



        RxStruct->wAddr2 =SELF_TEST_X;
	RxStruct->wNumData = 1;
	if (CPAL_I2C_Read(I2C2_DevStructure) != CPAL_PASS)
	{
          return ERROR;
        }
	/* Wait the end of transfer */
	while((I2C2_DevStructure->CPAL_State != CPAL_STATE_READY)&&(CPAL_Not_Working==0));
        if(CPAL_Not_Working==1) return FAIL;
        rawData[0] = RxBuf[0];

        RxStruct->wAddr2 =SELF_TEST_Y;
	RxStruct->wNumData = 1;
	if (CPAL_I2C_Read(I2C2_DevStructure) != CPAL_PASS)
	{
          return ERROR;
        }
	/* Wait the end of transfer */
	while((I2C2_DevStructure->CPAL_State != CPAL_STATE_READY)&&(CPAL_Not_Working==0));
        if(CPAL_Not_Working==1) return FAIL;
        rawData[1] = RxBuf[0];

        RxStruct->wAddr2 =SELF_TEST_Z;
	RxStruct->wNumData = 1;
	if (CPAL_I2C_Read(I2C2_DevStructure) != CPAL_PASS)
	{
          return ERROR;
        }
	/* Wait the end of transfer */
	while((I2C2_DevStructure->CPAL_State != CPAL_STATE_READY)&&(CPAL_Not_Working==0));
        if(CPAL_Not_Working==1) return FAIL;
        rawData[2] = RxBuf[0];


        RxStruct->wAddr2 =0x10;
	RxStruct->wNumData = 1;
	if (CPAL_I2C_Read(I2C2_DevStructure) != CPAL_PASS)
	{
          return ERROR;
        }
	/* Wait the end of transfer */
	while((I2C2_DevStructure->CPAL_State != CPAL_STATE_READY)&&(CPAL_Not_Working==0));
        if(CPAL_Not_Working==1) return FAIL;
        rawData[3] = RxBuf[0];


//	rawData[0]=*BSP_Gyroscope_ReadRegister(I2C2_DevStructure,SELF_TEST_X);
//	rawData[1]=*BSP_Gyroscope_ReadRegister(I2C2_DevStructure,SELF_TEST_Y);
//	rawData[2]=*BSP_Gyroscope_ReadRegister(I2C2_DevStructure,SELF_TEST_Z);
//	rawData[3]=*BSP_Gyroscope_ReadRegister(I2C2_DevStructure,0x10);

	// Extract the acceleration test results first
	selfTest[0] = (rawData[0] >> 3) | (rawData[3] & 0x30) >> 4 ; // XA_TEST result is a five-bit unsigned integer
	selfTest[1] = (rawData[1] >> 3) | (rawData[3] & 0x0C) >> 2 ; // YA_TEST result is a five-bit unsigned integer
	selfTest[2] = (rawData[2] >> 3) | (rawData[3] & 0x03) >> 0 ; // ZA_TEST result is a five-bit unsigned integer
	// Extract the gyration test results first
	selfTest[3] = rawData[0]  & 0x1F ; // XG_TEST result is a five-bit unsigned integer
	selfTest[4] = rawData[1]  & 0x1F ; // YG_TEST result is a five-bit unsigned integer
	selfTest[5] = rawData[2]  & 0x1F ; // ZG_TEST result is a five-bit unsigned integer

	// Process results to allow final comparison with factory set values
	factoryTrim[0] = (int)(4096.0*0.34)*(int)(pow( (0.92/0.34) , (((float)selfTest[0] - 1.0)/30.0))); // FT[Xa] factory trim calculation
	factoryTrim[1] = (int)(4096.0*0.34)*(int)(pow( (0.92/0.34) , (((float)selfTest[1] - 1.0)/30.0))); // FT[Ya] factory trim calculation
	factoryTrim[2] = (int)(4096.0*0.34)*(int)(pow( (0.92/0.34) , (((float)selfTest[2] - 1.0)/30.0))); // FT[Za] factory trim calculation
	factoryTrim[3] =  (int)( 25.0*131.0)*(int)(pow( 1.046 , ((float)selfTest[3] - 1.0) ));             // FT[Xg] factory trim calculation
	factoryTrim[4] =  (int)(-25.0*131.0)*(int)(pow( 1.046 , ((float)selfTest[4] - 1.0) ));             // FT[Yg] factory trim calculation
	factoryTrim[5] =  (int)( 25.0*131.0)*(int)(pow( 1.046 , ((float)selfTest[5] - 1.0) ));             // FT[Zg] factory trim calculation

	 for (i = 0; i < 6; i++)
	     destination[i] = 100.0 + (100.0*((float)selfTest[i] - factoryTrim[i])/factoryTrim[i]); // Report percent differences

	 if(destination[0]<1 && destination[1]<1 && destination[2] < 1 && destination[3] < 1 && destination[4] < 1 && destination[5] < 1)
		 return SUCCESS;
	 return		ERROR;
}

uint8_t* BSP_Gyroscope_ReadRegister(CPAL_InitTypeDef* I2C2_DevStructure,uint8_t addr)
{
	uint8_t* RxBuf=I2C2_DevStructure->pCPAL_TransferRx->pbBuffer;
	CPAL_TransferTypeDef RxStruct;

	RxStruct.wAddr2 = addr;
	RxStruct.wNumData = 1;
	if (CPAL_I2C_Read(I2C2_DevStructure) != CPAL_PASS)
          return ERROR;        
	/* Wait the end of transfer */
	while((I2C2_DevStructure->CPAL_State != CPAL_STATE_READY)&&(CPAL_Not_Working==0));
        if(CPAL_Not_Working==1) return FAIL;

	return RxBuf;
}


int16_t acc_x,acc_y,acc_z,gyr_x,gyr_y,gyr_z;
//float b,c,d,e;
uint8_t addr;


uint8_t fifo[2];
uint8_t rec[256];
uint8_t BSP_Gyroscope_GetValues(CPAL_InitTypeDef* I2C2_DevStructure, BSP_GyrFetchTypeDef* BSP_GyrFetchStructure)
{
  int16_t temp;
  acc_x = acc_y = acc_z = gyr_x =  gyr_y = gyr_z = temp = 0;

  for(int i=0;i<256;i++)
  {

    rec[i]=0;
    }
	uint16_t info[14];
        ready = 0;
	CPAL_TransferTypeDef* TxStruct=I2C2_DevStructure->pCPAL_TransferTx;
	CPAL_TransferTypeDef* RxStruct=I2C2_DevStructure->pCPAL_TransferRx;
	uint8_t* RxBuf=I2C2_DevStructure->pCPAL_TransferRx->pbBuffer;
        uint8_t* TxBuf=I2C2_DevStructure->pCPAL_TransferTx->pbBuffer;



      /*******/
       TxStruct->wAddr2 = 0x6B;
	TxStruct->wNumData = 1;
	TxBuf[0]=0x00;
	if (CPAL_I2C_Write(I2C2_DevStructure) != CPAL_PASS)
	{ /* I2C bus or peripheral is not able to start communication: Error management */  }
	/* Wait the end of transfer */
	while((I2C2_DevStructure->CPAL_State != CPAL_STATE_READY)&&(CPAL_Not_Working==0));
        if(CPAL_Not_Working==1) return FAIL;

        TxStruct->wAddr2 = 0x1C;
	TxStruct->wNumData = 1;
	TxBuf[0]=0x00;
	if (CPAL_I2C_Write(I2C2_DevStructure) != CPAL_PASS)
	{ /* I2C bus or peripheral is not able to start communication: Error management */  }
	/* Wait the end of transfer */
	while((I2C2_DevStructure->CPAL_State != CPAL_STATE_READY)&&(CPAL_Not_Working==0));
        if(CPAL_Not_Working==1) return FAIL;
        /*************/
        TxStruct->wAddr2 = 0x1B;
	TxStruct->wNumData = 1;
	TxBuf[0]=0x00;
	if (CPAL_I2C_Write(I2C2_DevStructure) != CPAL_PASS)
	{ /* I2C bus or peripheral is not able to start communication: Error management */  }
	/* Wait the end of transfer */
	while((I2C2_DevStructure->CPAL_State != CPAL_STATE_READY)&&(CPAL_Not_Working==0));
        if(CPAL_Not_Working==1) return FAIL;



        RxStruct->wAddr2 = 0x41;
	RxStruct->wNumData =1;
	if (CPAL_I2C_Read(I2C2_DevStructure) != CPAL_PASS)
	{ return ERROR;  }
	/* Wait the end of transfer */
	while((I2C2_DevStructure->CPAL_State != CPAL_STATE_READY)&&(CPAL_Not_Working==0));
        if(CPAL_Not_Working==1) return FAIL;
        temp=temp|RxBuf[0];
        temp=temp<<8;

        RxStruct->wAddr2 = 0x42;
	RxStruct->wNumData =1;
	if (CPAL_I2C_Read(I2C2_DevStructure) != CPAL_PASS)
	{ return ERROR;  }
	/* Wait the end of transfer */
	while((I2C2_DevStructure->CPAL_State != CPAL_STATE_READY)&&(CPAL_Not_Working==0));
        if(CPAL_Not_Working==1) return FAIL;
        temp=temp | RxBuf[0];


        RxStruct->wAddr2 = 0x43;
	RxStruct->wNumData =1;
	if (CPAL_I2C_Read(I2C2_DevStructure) != CPAL_PASS)
	{
          return ERROR;
        }
	/* Wait the end of transfer */
	while((I2C2_DevStructure->CPAL_State != CPAL_STATE_READY)&&(CPAL_Not_Working==0));
        if(CPAL_Not_Working==1) return FAIL;
        gyr_x=gyr_x|RxBuf[0];
        gyr_x=gyr_x<<8;

        RxStruct->wAddr2 = 0x44;
	RxStruct->wNumData =1;
	if (CPAL_I2C_Read(I2C2_DevStructure) != CPAL_PASS)
	{ return ERROR;  }
	/* Wait the end of transfer */
	while((I2C2_DevStructure->CPAL_State != CPAL_STATE_READY)&&(CPAL_Not_Working==0));
        if(CPAL_Not_Working==1) return FAIL;
        gyr_x=gyr_x | RxBuf[0];

        RxStruct->wAddr2 = 0x45;
	RxStruct->wNumData =1;
	if (CPAL_I2C_Read(I2C2_DevStructure) != CPAL_PASS)
	{ return ERROR;  }
	/* Wait the end of transfer */
	while((I2C2_DevStructure->CPAL_State != CPAL_STATE_READY)&&(CPAL_Not_Working==0));
        if(CPAL_Not_Working==1) return FAIL;
        gyr_y=gyr_y|RxBuf[0];
        gyr_y=gyr_y<<8;

        RxStruct->wAddr2 = 0x46;
	RxStruct->wNumData =1;
	if (CPAL_I2C_Read(I2C2_DevStructure) != CPAL_PASS)
	{ return ERROR;  }
	/* Wait the end of transfer */
	while((I2C2_DevStructure->CPAL_State != CPAL_STATE_READY)&&(CPAL_Not_Working==0));
        if(CPAL_Not_Working==1) return FAIL;
        gyr_y= gyr_y|RxBuf[0];


         RxStruct->wAddr2 = 0x47;
	RxStruct->wNumData =1;
	if (CPAL_I2C_Read(I2C2_DevStructure) != CPAL_PASS)
	{ return ERROR;  }
	/* Wait the end of transfer */
	while((I2C2_DevStructure->CPAL_State != CPAL_STATE_READY)&&(CPAL_Not_Working==0));
        if(CPAL_Not_Working==1) return FAIL;
        gyr_z=gyr_z|RxBuf[0];
        gyr_z=gyr_z<<8;

        RxStruct->wAddr2 = 0x48;
	RxStruct->wNumData =1;
	if (CPAL_I2C_Read(I2C2_DevStructure) != CPAL_PASS)
	{ return ERROR;  }
	/* Wait the end of transfer */
	while((I2C2_DevStructure->CPAL_State != CPAL_STATE_READY)&&(CPAL_Not_Working==0));
        if(CPAL_Not_Working==1) return FAIL;
        gyr_z =gyr_z|RxBuf[0];

        RxStruct->wAddr2 = 0x3B;
	RxStruct->wNumData =1;
	if (CPAL_I2C_Read(I2C2_DevStructure) != CPAL_PASS)
	{ return ERROR;  }
	/* Wait the end of transfer */
	while((I2C2_DevStructure->CPAL_State != CPAL_STATE_READY)&&(CPAL_Not_Working==0));
        if(CPAL_Not_Working==1) return FAIL;
        acc_x=acc_x|RxBuf[0];
        acc_x=acc_x<<8;

        RxStruct->wAddr2 = 0x3C;
	RxStruct->wNumData =1;
	if (CPAL_I2C_Read(I2C2_DevStructure) != CPAL_PASS)
	{ return ERROR;  }
	/* Wait the end of transfer */
	while((I2C2_DevStructure->CPAL_State != CPAL_STATE_READY)&&(CPAL_Not_Working==0));
        if(CPAL_Not_Working==1) return FAIL;
        acc_x = acc_x|RxBuf[0];

        RxStruct->wAddr2 = 0x3D;
	RxStruct->wNumData =1;
	if (CPAL_I2C_Read(I2C2_DevStructure) != CPAL_PASS)
	{ return ERROR;  }
	/* Wait the end of transfer */
	while((I2C2_DevStructure->CPAL_State != CPAL_STATE_READY)&&(CPAL_Not_Working==0));
        if(CPAL_Not_Working==1) return FAIL;
        acc_y=acc_y|RxBuf[0];
        acc_y=acc_y<<8;

        RxStruct->wAddr2 = 0x3E;
	RxStruct->wNumData =1;
	if (CPAL_I2C_Read(I2C2_DevStructure) != CPAL_PASS)
	{ return ERROR;  }
	/* Wait the end of transfer */
	while((I2C2_DevStructure->CPAL_State != CPAL_STATE_READY)&&(CPAL_Not_Working==0));
        if(CPAL_Not_Working==1) return FAIL;
        acc_y = acc_y|RxBuf[0];

         RxStruct->wAddr2 = 0x3F;
	RxStruct->wNumData =1;
	if (CPAL_I2C_Read(I2C2_DevStructure) != CPAL_PASS)
	{ return ERROR;  }
	/* Wait the end of transfer */
	while((I2C2_DevStructure->CPAL_State != CPAL_STATE_READY)&&(CPAL_Not_Working==0));
        if(CPAL_Not_Working==1) return FAIL;
      acc_z= acc_z|RxBuf[0];
      acc_z=acc_z<<8;

           RxStruct->wAddr2 = 0x40;
	RxStruct->wNumData =1;
	if (CPAL_I2C_Read(I2C2_DevStructure) != CPAL_PASS)
	{ return ERROR;  }
	/* Wait the end of transfer */
	while((I2C2_DevStructure->CPAL_State != CPAL_STATE_READY)&&(CPAL_Not_Working==0));
        if(CPAL_Not_Working==1) return FAIL;
       acc_z= acc_z|RxBuf[0];

       RxStruct->wAddr2 = 0x68;
	RxStruct->wNumData =1;
	if (CPAL_I2C_Read(I2C2_DevStructure) != CPAL_PASS)
	{ return ERROR;  }
	/* Wait the end of transfer */
	while((I2C2_DevStructure->CPAL_State != CPAL_STATE_READY)&&(CPAL_Not_Working==0));
        if(CPAL_Not_Working==1) return FAIL;
       addr = RxBuf[0];

	BSP_GyrFetchStructure->BSP_Acc_x=((float)acc_x)/16384.0;
  BSP_GyrFetchStructure->BSP_Acc_y=((float)acc_y)/16384.0;
	BSP_GyrFetchStructure->BSP_Acc_z=((float)acc_z)/16384.0;
	BSP_GyrFetchStructure->BSP_Temp=temp/340.0+ 36.53;
  double raw_gyr_x=gyr_x/131.0;
	double raw_gyr_y=gyr_y/131.0;
	double raw_gyr_z=gyr_z/131.0;

        //calibration model
        MATRIX input,w1,w2,w2_1,w2_2,w3,b1,b2,b2_1,b2_2,b3,out1,ind1,ind2,ind3,out2;
        MATRIX buf1,buf2,buf3;

        input.row=1;
        input.col=4;
        input.mat[0][0]=raw_gyr_x;
        input.mat[0][1]=raw_gyr_y;
        input.mat[0][2]=raw_gyr_z;
        input.mat[0][3]=BSP_GyrFetchStructure->BSP_Temp;

        w1.row=4;
        w1.col=6;
        w1.mat[0][0]=0.31936058;
        w1.mat[0][1]=-2.6009953;
        w1.mat[0][2]=-0.4564155;
        w1.mat[0][3]=-2.515823;
        w1.mat[0][4]=-2.0009804;
        w1.mat[0][5]=-3.4780917;

        w1.mat[1][0]=1.0896544;
        w1.mat[1][1]=-2.3642986;
        w1.mat[1][2]=1.3925648;
        w1.mat[1][3]=1.1508482;
        w1.mat[1][4]=-2.1232738;
        w1.mat[1][5]=1.9126301;

        w1.mat[2][0]=-3.0366654;
        w1.mat[2][1]=2.7739253;
        w1.mat[2][2]=-2.0702853;
        w1.mat[2][3]=0.96741384;
        w1.mat[2][4]=0.41574967;
        w1.mat[2][5]=0.13569477;

        w1.mat[3][0]=-0.44741294;
        w1.mat[3][1]=-1.1441109;
        w1.mat[3][2]=-0.84899235;
        w1.mat[3][3]=-2.5238047;
        w1.mat[3][4]=-1.7859284;
        w1.mat[3][5]=-3.7112577;

        b1.row=1;
        b1.col=6;
        b1.mat[0][0]=1.6170639;
        b1.mat[0][1]=2.7869668;
        b1.mat[0][2]=-2.266306;
        b1.mat[0][3]=-0.43439335;
        b1.mat[0][4]=-3.613583;
        b1.mat[0][5]=-6.2766066;

        w2.row=6;
        w2.col=6;
        w2.mat[0][0]=1.7341793;
        w2.mat[0][1]=-1.9494470;
        w2.mat[0][2]=0.79175878;
        w2.mat[0][3]=-0.17124987;
        w2.mat[0][4]=-1.2239969;
        w2.mat[0][5]=-1.6512760;

        w2.mat[1][0]=7.0758492e-01;
        w2.mat[1][1]=4.1206270e-01;
        w2.mat[1][2]=-2.5838280e+00;
        w2.mat[1][3]=2.4855896e-03;
        w2.mat[1][4]=-2.2766089e+00;
        w2.mat[1][5]=8.7712528e-03;

        w2.mat[2][0]=1.3370147e+00;
        w2.mat[2][1]=2.1540157e-01;
        w2.mat[2][2]=8.8955358e-02;
        w2.mat[2][3]=-1.1142963e+00;
        w2.mat[2][4]=1.1172103e+00;
        w2.mat[2][5]=-5.0187576e-01;

        w2.mat[3][0]=3.8891971e-01;
        w2.mat[3][1]=-2.0590763e-01;
        w2.mat[3][2]=-2.3827226e+00;
        w2.mat[3][3]=-1.9148028e+00;
        w2.mat[3][4]=8.6134911e-02;
        w2.mat[3][5]=8.7192401e-02;

        w2.mat[4][0]=2.2220581e+00;
        w2.mat[4][1]=3.0762010e+00;
        w2.mat[4][2]=-6.7610490e-01;
        w2.mat[4][3]=-1.1297929e+00;
        w2.mat[4][4]=1.8178669e+00;
        w2.mat[4][5]=9.2582321e-01;

        w2.mat[5][0]=1.4619968e+00;
        w2.mat[5][1]=2.4988828e+00;
        w2.mat[5][2]=-2.6566300e+00;
        w2.mat[5][3]=-3.7121477e+00;
        w2.mat[5][4]=3.3740714e+00;
        w2.mat[5][5]=1.0392234e+00;

        b2.row=1;
        b2.col=6;
        b2.mat[0][0]=0.2542543;
        b2.mat[0][1]=-3.4225528;
        b2.mat[0][2]=-0.60725355;
        b2.mat[0][3]=0.53659534;
        b2.mat[0][4]=-1.1277087;
        b2.mat[0][5]=-2.1230037;

        w2_1.row=6;
        w2_1.col=6;
        w2_1.mat[0][0]=-2.0618224;
        w2_1.mat[0][1]=-1.4588958;
        w2_1.mat[0][2]=2.084737;
        w2_1.mat[0][3]=1.010787;
        w2_1.mat[0][4]=-1.0135934;
        w2_1.mat[0][5]=-0.22515653;

        w2_1.mat[1][0]=-0.73635954;
        w2_1.mat[1][1]=0.5597856;
        w2_1.mat[1][2]=1.7433496;
        w2_1.mat[1][3]=-1.4761766;
        w2_1.mat[1][4]=0.780058;
        w2_1.mat[1][5]=0.94303465;

        w2_1.mat[2][0]=-1.2623887;
        w2_1.mat[2][1]=-2.2794883;
        w2_1.mat[2][2]=-1.8168932;
        w2_1.mat[2][3]=1.258003;
        w2_1.mat[2][4]=-0.02687381;
        w2_1.mat[2][5]=0.71225506;

        w2_1.mat[3][0]=0.18423775;
        w2_1.mat[3][1]=1.0415602;
        w2_1.mat[3][2]=-0.6376245;
        w2_1.mat[3][3]=0.21132298;
        w2_1.mat[3][4]=-0.8285717;
        w2_1.mat[3][5]=-2.2027907;

        w2_1.mat[4][0]=2.6423428;
        w2_1.mat[4][1]=-1.5705659;
        w2_1.mat[4][2]=-0.3245618;
        w2_1.mat[4][3]=-0.4478229;
        w2_1.mat[4][4]=2.139473;
        w2_1.mat[4][5]=1.1822929;

        w2_1.mat[5][0]=-0.88529617;
        w2_1.mat[5][1]=-0.04955272;
        w2_1.mat[5][2]=0.18947457;
        w2_1.mat[5][3]=-1.5383314;
        w2_1.mat[5][4]=0.22308686;
        w2_1.mat[5][5]=-0.52912587;

        b2_1.row=1;
        b2_1.col=6;
        b2_1.mat[0][0]=-0.38070208;
        b2_1.mat[0][1]=0.550774;
        b2_1.mat[0][2]=-0.51262033;
        b2_1.mat[0][3]=-0.1275009;
        b2_1.mat[0][4]=-0.76151294;
        b2_1.mat[0][5]=1.0274585;

        w2_2.row=6;
        w2_2.col=6;
        w2_2.mat[0][0]=-1.6287065;
        w2_2.mat[0][1]=0.39115748;
        w2_2.mat[0][2]=-0.34100255;
        w2_2.mat[0][3]=0.43753186;
        w2_2.mat[0][4]=2.361678;
        w2_2.mat[0][5]=1.5805402;

        w2_2.mat[1][0]=-1.9506291;
        w2_2.mat[1][1]=-2.8624384;
        w2_2.mat[1][2]=-0.02971571;
        w2_2.mat[1][3]=-1.1537133;
        w2_2.mat[1][4]=-0.11043975;
        w2_2.mat[1][5]=-1.112121;

        w2_2.mat[2][0]=-1.4136935;
        w2_2.mat[2][1]=0.6062272;
        w2_2.mat[2][2]=2.2087169;
        w2_2.mat[2][3]=-1.449996;
        w2_2.mat[2][4]=0.29961544;
        w2_2.mat[2][5]=0.09736916;

        w2_2.mat[3][0]=-0.31286487;
        w2_2.mat[3][1]=0.644787;
        w2_2.mat[3][2]=-1.9684527;
        w2_2.mat[3][3]=1.9787844;
        w2_2.mat[3][4]=-0.7386548;
        w2_2.mat[3][5]=-0.9283117;

        w2_2.mat[4][0]=-0.2925054;
        w2_2.mat[4][1]=0.7229331;
        w2_2.mat[4][2]=0.7684511;
        w2_2.mat[4][3]=-0.28806895;
        w2_2.mat[4][4]=1.0990748;
        w2_2.mat[4][5]=0.43662214;

        w2_2.mat[5][0]=-1.4258054;
        w2_2.mat[5][1]=-1.0397558;
        w2_2.mat[5][2]=0.29590875;
        w2_2.mat[5][3]=-0.19892225;
        w2_2.mat[5][4]=1.7172257;
        w2_2.mat[5][5]=1.1087803;

        b2_2.row=1;
        b2_2.col=6;
        b2_2.mat[0][0]=-0.07408538;
        b2_2.mat[0][1]=0.3301433;
        b2_2.mat[0][2]=1.1533238;
        b2_2.mat[0][3]=1.3164594;
        b2_2.mat[0][4]=-0.33238387;
        b2_2.mat[0][5]=0.09627291;

        w3.row=6;
        w3.col=3;
        w3.mat[0][0]=3.3188357;
        w3.mat[0][1]=0.09755524;
        w3.mat[0][2]=0.03457886;

        w3.mat[1][0]=-0.17274041;
        w3.mat[1][1]=-0.01912737;
        w3.mat[1][2]=-2.5354614;

        w3.mat[2][0]=1.6138729;
        w3.mat[2][1]=2.4440875;
        w3.mat[2][2]=0.05691579;

        w3.mat[3][0]=0.63936573;
        w3.mat[3][1]=2.4103;
        w3.mat[3][2]=-3.729947;

        w3.mat[4][0]=2.3220782;
        w3.mat[4][1]=3.6165566;
        w3.mat[4][2]=-0.33413523;

        w3.mat[5][0]=-0.10538989;
        w3.mat[5][1]=-0.4653684;
        w3.mat[5][2]=2.9227357;

        b3.row=1;
        b3.col=3;
        b3.mat[0][0]=0.04143953;
        b3.mat[0][1]=3.08944;
        b3.mat[0][2]=3.8763628;

        out1=add_mat(mat_mul(input,w1),b1);
        buf1=add_mat(mat_mul(out1,w2),b2);
        ind1=find_tanh(buf1);
        buf2=add_mat(mat_mul(ind1,w2_1),b2_1);
        ind2=find_tanh(buf2);
        buf3=add_mat(mat_mul(ind2,w2_2),b2_2);
        ind3=find_tanh(buf3);
        out2=add_mat(mat_mul(ind3,w3),b3);

        BSP_GyrFetchStructure->BSP_Gyr_x=out2.mat[0][0];
        BSP_GyrFetchStructure->BSP_Gyr_y=out2.mat[0][1];
        BSP_GyrFetchStructure->BSP_Gyr_z=out2.mat[0][2];

	return SUCCESS;
}

MATRIX find_tanh(MATRIX a)
{
  int i,j;
  for(i=0;i<a.row;i++)
  {
    for(j=0;j<a.col;j++)
      a.mat[i][j]=tanh(a.mat[i][j]);
  }
  return a;
}

int main(){
  initialize_gyroscope(&I2C2_DevStructure);
  while(BSP_Gyroscope_GetValues(&I2C2_DevStructure,&BSP_GyrFetchStructure)!= SUCCESS);
  while(1){}

}