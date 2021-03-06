#ifndef __CONTROL_H
#define __CONTROL_H
/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
struct sys
{
	struct{
		uint16_t thermocouple_1;
		uint16_t thermocouple_2;
		uint16_t thermocouple_3;
		uint16_t thermocouple_4;
		uint16_t thermocouple_board;
		uint16_t MCU_temp;
	}s_temp;
	struct{
		uint16_t mcu_voltage;
	}s_power;
};

extern struct sys s_system;
extern volatile uint16_t ADC_Output[15];
extern DMA_HandleTypeDef hdma_adc;
extern ADC_HandleTypeDef hadc;
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void ADC_read(void);

#endif /* __CONTROL_H */
