#ifndef __POWER_H
#define __POWER_H
/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
/* Exported types ------------------------------------------------------------*/
typedef enum
{	
	both_heat,
	top_heat,
	bottom_heat,
	light,
	fan
}e_triacs;

/* Exported constants --------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void triac_set_duty(e_triacs triac, uint16_t duty);
void triac_modify_duty(e_triacs triac, int16_t count);
#endif /* __POWER_H */
