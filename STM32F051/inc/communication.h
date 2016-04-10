#ifndef __COMUNICATION_H
#define __COMUNICATION_H
/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
/* Exported types ------------------------------------------------------------*/
typedef enum
{
	thermocouple_1,
	thermocouple_2,
	thermocouple_3,
	thermocouple_4,
	thermocouple_board,
	MCU_temp
}e_temp_sensor;
/* Exported constants --------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void uart_send_temps(void);
void uart_receive_msg(void);
#endif /* __COMUNICATION_H */
