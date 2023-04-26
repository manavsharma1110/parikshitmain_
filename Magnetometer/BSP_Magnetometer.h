/*
 * BSP_Magnetometer.h
 *
 *  Created on: Jan 17, 2016
 *      Author: Nikhil Gupta
 */

#ifndef BSP_MAGNETOMETER_H_
#define BSP_MAGNETOMETER_H_
#include <math.h>
#include "stm32f4xx.h"
//#include "stm32f4_discovery.h"
#include "cpal_i2c.h"
#include "stdlib.h"

extern uint8_t I2C_No;

typedef struct
{
	double BSP_Mag_x;
	double BSP_Mag_y;
	double BSP_Mag_z;
}BSP_MagFetchTypeDef;

uint8_t BSP_Magnetometer_SelfTest(CPAL_InitTypeDef*);
uint32_t BSP_Magnetometer_Init(CPAL_InitTypeDef*);
uint8_t BSP_Magnetometer_GetValues(CPAL_InitTypeDef*, BSP_MagFetchTypeDef*);


#endif /* BSP_MAGNETOMETER_H_ */