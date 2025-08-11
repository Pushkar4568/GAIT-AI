/*
 * adc.h
 *
 *  Created on: May 27, 2025
 *      Author: hp
 */

#ifndef ADC_H_
#define ADC_H_
#include<stdint.h>

void ADC_INIT_(void);
void start_conversion_adc (void);
uint32_t adc_read(void);


#endif /* ADC_H_ */
