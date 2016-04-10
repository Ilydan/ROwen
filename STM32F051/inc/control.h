#ifndef __CONTROL_H
#define __CONTROL_H
/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
extern struct sys
{
	struct{
		uint16_t		thermocouple[4];
		uint16_t		thermocouple_board;
		uint16_t		MCU_temp;
	}s_temp;
	struct{
		uint16_t mcu_voltage;
		int32_t ADC_uvolt[4];
	}s_power;
}s_system;

extern struct h_ramp
{
	unsigned int duration;
	unsigned int final_temp;
}ramp[10];

extern struct ramp_equation
{
	float k;
	float q;
}y[10];
extern struct coordinates
{
	float temp;
	float time;
}coords;
extern float profile[10];
extern volatile uint16_t ADC_Output[2];
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
struct ramp_equation* ProfileBuilder(float *profile);
void OverTempBeep(void);
void ADC_read(void);
uint16_t PID_controller(int16_t setPoint);
#endif /* __CONTROL_H */
