/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <inttypes.h>
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

uint8_t RX_Buffer[5][MAX_MESSAGE_LENGHT];
uint8_t RX_r_pointer = 0;
uint8_t RX_w_pointer = 0;

volatile char *TX_Buffer;

volatile char received_string[MAX_MESSAGE_LENGHT+1];
bool next_message = true;


void USART_puts(USART_TypeDef* USARTx, volatile char *s)
{
//	if(next_message)
//	{
//		next_message = false;
//		TX_Buffer = s;
//		USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);
//	}
	while(*s){
		// wait until data register is empty
		while( !(USARTx->ISR & USART_FLAG_TC) );
		USART_SendData(USARTx, *s);
		*s++;
	}
}


void uart_parse(char* string)
{
	//int n = sscanf(string, "#%d*%d/%d*%d/%d*%d/%d*%d/%d*%d/%d*%d/%d*%d/%d*%d/%d*%d/%d*%dend", 
//	sscanf(string, "#%u*%u/%u*%u/%u*%u/%u*%u/%u*%uend", 
	sscanf(string, "z#%u*%u/%u*%u/%u*%u/end", 
		&ramp[0].duration, &ramp[0].final_temp,
		&ramp[1].duration, &ramp[1].final_temp,
		&ramp[2].duration, &ramp[2].final_temp
//		&ramp[3].duration, &ramp[3].final_temp,
//		&ramp[4].duration, &ramp[4].final_temp
//		ramp[5].duration, ramp[5].final_temp,
//		ramp[6].duration, ramp[6].final_temp,
//		ramp[7].duration, ramp[7].final_temp,
//		ramp[8].duration, ramp[8].final_temp,
//		ramp[9].duration, ramp[9].final_temp
	);
}


