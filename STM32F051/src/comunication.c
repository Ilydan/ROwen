/* Includes ------------------------------------------------------------------*/
#include "main.h"


/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
void uart_send_temps(void)
{
	char send_it[100] = {0};
	sprintf(send_it, "#%d/%d/%d/%d/%d/%d/%d$\n",\
	(uint32_t)real_time/10,\
	(uint16_t)s_system.s_temp.thermocouple[2],\
	(uint8_t)s_system.s_temp.thermocouple_board,\
	(uint8_t)(s_system.s_temp.MCU_temp/10),\
	(uint16_t)(TIM3->CCR3),\
	(uint32_t)start_time/10,\
	(uint32_t)setPoint);
//	sprintf(send_it, "#Time %ds -> Thermocouplers: %dC,%dC,%dC,%dC ADC: %dC MCU: %dC   ADC voltage=%duV\n\r",\
//	(uint32_t)real_time/10,\
//	(uint8_t)s_system.s_temp.thermocouple[0],\
//	(uint8_t)s_system.s_temp.thermocouple[1],\
//	(uint8_t)s_system.s_temp.thermocouple[2],\
//	(uint8_t)s_system.s_temp.thermocouple[3],\
//	(uint8_t)s_system.s_temp.thermocouple_board,\
//	(uint8_t)(s_system.s_temp.MCU_temp/10),\
//	s_system.s_power.ADC_uvolt[2]);
	
//	sprintf(send_it, "#Time %ds -> Thermocoupler: %dC ADC: %dC MCU: %dC   ADC voltage=%duV\n\r",\
//	(uint32_t)real_time/10,\
//	(uint8_t)s_system.s_temp.thermocouple[2],\
//	(uint8_t)s_system.s_temp.thermocouple_board,\
//	(uint8_t)(s_system.s_temp.MCU_temp/10),\
//	s_system.s_power.ADC_uvolt[2]);
	
	USART_puts(USART1,send_it);
}
