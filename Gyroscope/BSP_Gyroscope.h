/*
 * BSP_Gyroscope.h
 *
 *  Created on: Jan 17, 2016
 *      Author: Yash Vardhan Agarwal
 */

#ifndef BSP_GYROSCOPE_H_
#define BSP_GYROSCOPE_H_

#include "stm32f4xx.h"
//#include "stm32f4_discovery.h"
#include "cpal_i2c.h"
#include "stdlib.h"
#include "math.h"
#include "data_structs.h"

#define SELF_TEST_X 0x0D
#define SELF_TEST_Y 0x0E
#define SELF_TEST_Z 0x0F

extern uint8_t I2C_No;





/* The sensors can be mounted onto the board in any orientation. The mounting
 * matrix seen below tells the MPL how to rotate the raw data from the
 * driver(s).
 * The following matrices refer to the configuration on internal test
 * boards at Invensense. If needed, please modify the matrices to match the
 * chip-to-body matrix for your particular set up.
 */
typedef struct
{
	double BSP_Acc_x;
	double BSP_Acc_y;
	double BSP_Acc_z;
	double BSP_Temp;
	double BSP_Gyr_x;
	double BSP_Gyr_y;
	double BSP_Gyr_z;
}BSP_GyrFetchTypeDef;

uint8_t BSP_Gyroscope_SelfTest(CPAL_InitTypeDef*);
uint32_t BSP_Gyroscope_Init(CPAL_InitTypeDef*);
uint8_t BSP_Gyroscope_GetValues(CPAL_InitTypeDef*, BSP_GyrFetchTypeDef*);
uint8_t* BSP_Gyroscope_ReadRegister(CPAL_InitTypeDef* I2C1_DevStructure,uint8_t addr);
MATRIX find_tanh(MATRIX);

#endif /* BSP_GYROSCOPE_H_ */
