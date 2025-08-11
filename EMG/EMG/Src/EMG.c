#include <EMG.h>
#include "stm32f4xx.h"
#include "adc.h"
#include <math.h>
#include <stdio.h>



float EMG_read(void) {
	start_conversion_adc();
	uint32_t adc_val = adc_read();

   return adc_val;
}


