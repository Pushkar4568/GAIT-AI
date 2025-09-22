/*
 * MPU_6050.c
 *
 *  Created on: Sep 22, 2025
 *      Author: hp
 */

#include<stdio.h>
#include"string.h"
#include<math.h>
#include <stdint.h>
#include"MPU_6050.h"



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


extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart1;


#define MPU6050_ADDR (0x68<<1)


#define xhigh_gyro 0x43
#define xlow_gyro 0x44
#define yhigh_gyro 0x45
#define ylow_gyro 0x46
#define zhigh_gyro 0x47
#define zlow_gyro 0x48

#define xhigh_acc 0x3B
#define xlow_acc 0x3C
#define yhigh_acc 0x3D
#define ylow_acc 0x3E
#define zhigh_acc 0x3F
#define zlow_acc 0x40

#define temp_h 0x41
#define temp_l 0x42


#define PWRMNG 0x6B

#define acc_CONFIG 0x1C
#define gyro_CONFIG 0x1B








//global var to store the values
uint8_t acc_range=0;
uint8_t gyro_range=0;





void MPU_6050_INIT(uint8_t acc_setting,uint8_t gyro_setting){

	uint8_t data=0x00;
	HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR , PWRMNG,I2C_MEMADD_SIZE_8BIT, &data,1,  HAL_MAX_DELAY);

	acc_range=acc_setting;
	gyro_range=gyro_setting;

	MPU6050_acc_CONFIG(acc_range);  /* MPU6050_acc_CONFIG(0); is  ± 2g
	   	   	   	   	   	   	   	 MPU6050_acc_CONFIG(1); is  ± 4g
	   	   	   	   	   	   	   	 MPU6050_acc_CONFIG(2); is  ± 8g
	   	   	   	   	   	   	   	 MPU6050_acc_CONFIG(3); is  ± 16g
	   	   	   	   	   	   	   	   	   	   	   	   	   	   */

	MPU6050_gyro_CONFIG(gyro_range);/* MPU6050_gyro_CONFIG(0); is  ± 250  °/s
	   	   	   	   	   	   	   	 MPU6050_gyro_CONFIG(1); is ± 500  °/s
	   	   	   	   	   	   	   	 MPU6050_gyro_CONFIG(2); is ± 1000 °/s
	   	   	   	   	   	   	   	 MPU6050_gyro_CONFIG(3); is ± 2000 °/s
	   	   	   	   	   	   	   	   	   	   	   	   	   	   */



}


void MPU6050_acc_CONFIG(uint8_t range){
	uint8_t value = range << 3;   //bit 4:3 for range selection
	HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR , acc_CONFIG,I2C_MEMADD_SIZE_8BIT, &value,1,  HAL_MAX_DELAY);

}
void MPU6050_gyro_CONFIG(uint8_t range){
	uint8_t value = range << 3;   //bit 4:3 for range selection
	HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR , gyro_CONFIG,I2C_MEMADD_SIZE_8BIT, &value,1,  HAL_MAX_DELAY);

}



uint8_t MPU6050_readregister(uint8_t REG){
	uint8_t TX = REG;// for SPI do (REG| 0x80);
	uint8_t RX =0;
	HAL_I2C_Mem_Read(&hi2c1, MPU6050_ADDR , TX,I2C_MEMADD_SIZE_8BIT, &RX,1,  HAL_MAX_DELAY);


	return RX;
}





void TEMP(void){
	int16_t X=0;


	uint8_t high=MPU6050_readregister(temp_h);
	uint8_t low=MPU6050_readregister(temp_l);
	X=(int16_t)((high<<8) | low);


	float tempa = (X/340.0)+36.53;
	char msg[100];

	snprintf(msg,sizeof(msg),"\r\n TEMP  : %.2f °C \n",tempa);

	HAL_UART_Transmit(&huart1,(uint8_t*)msg,strlen(msg),HAL_MAX_DELAY);
}



void GYRO_X(void){
	int16_t X=0;


	uint8_t high=MPU6050_readregister(xhigh_gyro);
	uint8_t low=MPU6050_readregister(xlow_gyro);
	X=(int16_t)((high<<8) | low);

	float Xr=GYRO(X);
	char msg[100];

	snprintf(msg,sizeof(msg)," GYRO X  : %.2f °/s ",Xr);

	HAL_UART_Transmit(&huart1,(uint8_t*)msg,strlen(msg),HAL_MAX_DELAY);

}
void GYRO_Y(void){
	int16_t X=0;


	uint8_t high=MPU6050_readregister(yhigh_gyro);
	uint8_t low=MPU6050_readregister(ylow_gyro);
	X=(int16_t)((high<<8) | low);

	float Xr=GYRO(X);
	char msg[100];

	snprintf(msg,sizeof(msg),"  GYRO Y  : %.2f °/s  ",Xr);

	HAL_UART_Transmit(&huart1,(uint8_t*)msg,strlen(msg),HAL_MAX_DELAY);

}
void GYRO_Z(void){
	int16_t X=0;


	uint8_t high=MPU6050_readregister(zhigh_gyro);
	uint8_t low=MPU6050_readregister(zlow_gyro);
	X=(int16_t)((high<<8) | low);
	float Xr=GYRO(X);
	char msg[100];

	snprintf(msg,sizeof(msg)," GYRO Z  : %.2f °/s  \n",Xr);

	HAL_UART_Transmit(&huart1,(uint8_t*)msg,strlen(msg),HAL_MAX_DELAY);

}


float ACC_X(void){
	int16_t X=0;


	uint8_t high=MPU6050_readregister(xhigh_acc);
	uint8_t low=MPU6050_readregister(xlow_acc);
	X=(int16_t)((high<<8) | low);

	float Xr=ACC(X);
	char msg[100];

	snprintf(msg,sizeof(msg)," ACC X  : %.2f g ",Xr);

	HAL_UART_Transmit(&huart1,(uint8_t*)msg,strlen(msg),HAL_MAX_DELAY);

	return Xr;

}
float ACC_Y(void){
	int16_t X=0;


	uint8_t high=MPU6050_readregister(yhigh_acc);
	uint8_t low=MPU6050_readregister(ylow_acc);
	X=(int16_t)((high<<8) | low);
	float Xr=ACC(X);
	char msg[100];

	snprintf(msg,sizeof(msg)," ACC Y  : %.2f g ",Xr);

	HAL_UART_Transmit(&huart1,(uint8_t*)msg,strlen(msg),HAL_MAX_DELAY);
	return Xr;

}
float ACC_Z(void){
	int16_t X=0;


	uint8_t high=MPU6050_readregister(zhigh_acc);
	uint8_t low=MPU6050_readregister(zlow_acc);
	X=(int16_t)((high<<8) | low);
	float Xr=ACC(X);
	char msg[100];

	snprintf(msg,sizeof(msg)," ACC Z  : %.2f g \n",Xr);

	HAL_UART_Transmit(&huart1,(uint8_t*)msg,strlen(msg),HAL_MAX_DELAY);
	return Xr;

}




float ACC(int16_t raw){
	float factor;
	switch (acc_range){
			case 0 : factor = 16384.0;
					break;
			case 1 : factor = 8192.0;
					break;
			case 2 : factor = 4096.0;
					break;
			case 3 : factor = 2048.0;
					break;

	}

	return (raw/factor);

}

float GYRO(int16_t raw){
	float factor;
	switch (gyro_range){
			case 0 : factor = 131.0;
					break;
			case 1 : factor = 65.5;
					break;
			case 2 : factor = 32.8;
					break;
			case 3 : factor = 16.4;
					break;

	}

	return (raw/factor);

}


void G_Force_Exp(float ax,float ay,float az){

	float truede=sqrt(ax*ax+ay*ay+az*az);
	// if the sesnsor is mounted on a particular axis then only use => float exp=true-1;

	char msg[100];

	snprintf(msg,sizeof(msg),"  The G_Force experienced is %.2f g \n",truede);

	HAL_UART_Transmit(&huart1,(uint8_t *)msg,strlen(msg),HAL_MAX_DELAY);


}
















