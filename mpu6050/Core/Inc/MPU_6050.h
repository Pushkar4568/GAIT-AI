/*
 * MPU_6050.h
 *
 *  Created on: Sep 22, 2025
 *      Author: hp
 */

#ifndef INC_MPU_6050_H_
#define INC_MPU_6050_H_

#include "stm32f4xx_hal.h"

void TEMP(void);

void MPU_6050_INIT(uint8_t acc_setting,uint8_t gyro_setting);
void GYRO_Z(void);
void GYRO_X(void);
void GYRO_Y(void);
float ACC_X(void);
float ACC_Y(void);
float ACC_Z(void);
void MPU6050_acc_CONFIG(uint8_t range);
void MPU6050_gyro_CONFIG(uint8_t range);
float ACC(int16_t raw);
float GYRO(int16_t raw);
void G_Force_Exp(float ax,float ay,float az);



#endif /* INC_MPU_6050_H_ */
