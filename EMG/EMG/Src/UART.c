/*
 * UART.c
 *
 *  Created on: May 27, 2025
 *      Author: hp
 */
#include "UART.h"




#define GPIOAEN (1U<<0)
#define UART2EN  (1U<<17)
#define SYS_FREQ   16000000
#define APB1_clk   SYS_FREQ
#define UART_BAUDRATE 115200
#define CR1_TE   (1U<<3)
#define CR1_UE   (1U<<13)
#define SR_TXE   (1U<<7)

static void uart_set_baudrate(USART_TypeDef *USARTx , uint32_t PeriphCLK , uint32_t Baudrate);
static uint16_t compute_uart_bd(uint32_t PeriphCLK , uint32_t Baudrate);
void USART_TX_INIT_(void);
void uart2_write (int ch);

int __io_putchar(int ch) {
    if (ch == '\n') uart2_write('\r');
    uart2_write(ch);
    return ch;
}


void USART_TX_INIT_(void){
	// *************************************enable the GPIO for USART.
	//                                        a) enable the clock access for the GPIO pin
	//                                        b) SET the GPIO PIN to alternate FUnction mode
	//                                        c) Then the selected GPIO pin to Alternate function type  USART 2 TX e g. AF 07
	//*************************************Configure the USART MODULE
	//                                        a) Enable the clock access to the selected USART
	//                                        b)Configure the BAUDRATE.
	//                                        c) Transfer direction
	//                                        d) Enable the USART module
	RCC->AHB1ENR = GPIOAEN;

	GPIOA -> MODER &=~(1U<<4);
	GPIOA -> MODER |=(1U<<5);

	//AFR[1] is for Alternate function high mode
	GPIOA -> AFR[0] |= (1U<<8);
	GPIOA -> AFR[0] |= (1U<<9);
	GPIOA -> AFR[0] |= (1U<<10);
	GPIOA -> AFR[0] &= ~(1U<<11);


	RCC->APB1ENR |= UART2EN ;

	uart_set_baudrate(USART2,APB1_clk,UART_BAUDRATE);

	//to set direction go to control register and check which bit is set for transmitter of UART

	USART2->CR1 = CR1_TE;
	//to enable USART module go to control register 1
	USART2->CR1 |= CR1_UE;


}


void uart2_write(int ch){
	//make sure the transmit data register is empty
	//write to transmit data register
	//to read a bit u have to perform the status registers and operation with the Transmit data register
	while (!(USART2 -> SR  & SR_TXE)) {}

	USART2 -> DR = (ch & 0xFF);


}

static void uart_set_baudrate(USART_TypeDef *USARTx , uint32_t PeriphCLK , uint32_t Baudrate)
{
	USARTx->BRR = compute_uart_bd(PeriphCLK , Baudrate);

}

static uint16_t compute_uart_bd(uint32_t PeriphCLK , uint32_t Baudrate)
{
	return (PeriphCLK+(Baudrate/2U))/Baudrate;
}


