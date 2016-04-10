#ifndef __TIME_H
#define __TIME_H
/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
extern uint32_t speed_counter;
extern uint32_t speed;
extern uint32_t real_time;
extern uint32_t start_time;
extern uint32_t setPoint;
extern uint8_t tempAlarmTimer;
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Delay(__IO uint32_t nTime);
void Delay_tick(__IO uint32_t nCount);
void TimingDelay_Decrement(void);
void Period_timer_increment(void);
void period_time_check_flags(void);
void tim_1ms_loop(void);
void tim_10ms_loop(void);
void tim_100ms_loop(void);
void tim_1000ms_loop(void);
#endif /* __TIME_H */
