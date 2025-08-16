#include <EMG.h>
# include <stdint.h>
#include "stm32f4xx.h"
#include "UART.h"
#include "adc.h"
#include <stdio.h>
#include <math.h>

float output;
int main()

{

	//https://community.st.com/t5/stm32cubeide-mcus/fpu-is-not-initialized/td-p/620629
	#if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
	SCB->CPACR |= ((3UL << 10 * 2) |
			(3UL << 11 * 2));  /* set CP10 and CP11 Full Access */
	#endif
	USART_TX_INIT_();
	ADC_INIT_();


	while (1)
	{
		 float emg_mv = EMG_read();
		 printf("EMG: %.2f mV\n\r", emg_mv);
		 for (volatile int i = 0; i < 1000000; i++); // simple delay




	}
}

