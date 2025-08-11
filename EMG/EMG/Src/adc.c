/*
 * adc.c
 *
 *  Created on: May 27, 2025
 *      Author: hp
 */

#include "stm32f4xx.h"
#include "adc.h"
#include<stdint.h>

#define ADCEN  (1U<<8)
#define GPIOAEN (1U<<0)
#define ADC_CH1 (1U<<0)
#define ADC_SEQ_LEN 0x00
#define CR2_ADON (1U<<0)
#define start_conversion (1U<<30)
#define adc_end_flag (1U<<1)

void ADC_INIT_(void){

	//enable clock access to GPIOA
	RCC -> AHB1ENR |= GPIOAEN;


	// SET THE MODE OF PA1 to ANALOG MODE Using MODER

	GPIOA -> MODER |= (1U<<2);
	GPIOA -> MODER |= (1U<<3);
	//Configure the ADC peripheral
	//enable clock access to ADC peripheral
	RCC -> APB2ENR |= ADCEN;

	//configure ADC peripheral includes:
	//conversion sequence start
	ADC1 -> SQR3 = ADC_CH1;
	//conversion sequence length
	ADC1 -> SQR1 = ADC_SEQ_LEN;

	//enable the ADC module

	ADC1-> CR2 |= CR2_ADON;
}


void start_conversion_adc (void){
	// start the ADC conversion
	ADC1 -> CR2 |= start_conversion;

}


uint32_t adc_read(void){
	//wait for the conversion to be completed
	//check flag
	// end of conversion flag
	while (!(ADC1-> SR & adc_end_flag)){}
	//read the converted result
	return ADC1->DR;
}

