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


uint32_t TIM_10MS_COUNTER;
uint32_t TIM_100MS_COUNTER;
uint32_t TIM_1000MS_COUNTER;

uint8_t TIM_1MS_FLAG;
uint8_t TIM_10MS_FLAG;
uint8_t TIM_100MS_FLAG;
uint8_t TIM_1000MS_FLAG;

uint8_t test_counter = 0;
uint32_t speed_counter = 0;
uint32_t speed = 0;
static volatile uint32_t TimingDelay;

inline void Delay(__IO uint32_t nTime)
{ 
  TimingDelay = nTime;
  while(TimingDelay != 0);
}

inline void Delay_tick(__IO uint32_t nCount)
{
	while(nCount--);
}
inline void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
    TimingDelay--;
}

void HAL_IncTick(void) //called each 1ms
{
		TimingDelay_Decrement();
		TIM_1MS_FLAG = 1;
		TIM_10MS_COUNTER++;	
	if (TIM_10MS_COUNTER >= 10)
	{
		TIM_10MS_COUNTER = 0;
		TIM_10MS_FLAG = 1;
		TIM_100MS_COUNTER += 10;
	}				
	if (TIM_100MS_COUNTER >= 100)
	{
		TIM_100MS_COUNTER = 0;
		TIM_100MS_FLAG = 1;
		TIM_1000MS_COUNTER += 100;
	}		
	if (TIM_1000MS_COUNTER >= 1000)
	{
		TIM_1000MS_COUNTER = 0;
		TIM_1000MS_FLAG = 1;
	}
}

void period_time_check_flags(void)
{
	if (TIM_1MS_FLAG)
	{
		tim_1ms_loop();
		TIM_1MS_FLAG = 0;
	}
	if (TIM_10MS_FLAG)
	{
		tim_10ms_loop();
		TIM_10MS_FLAG = 0;
	}
	if (TIM_100MS_FLAG)
	{
		tim_100ms_loop();
		TIM_100MS_FLAG = 0;
	}
	if (TIM_1000MS_FLAG)
	{
		tim_1000ms_loop();
		TIM_1000MS_FLAG = 0;
	}
}

void tim_1ms_loop(void)
{
	if(buzzer_mode == strobo || buzzer_mode == beeps)
	{
		if(buzzer_beep_time_counter++ >= buzzer_beep_time)
		{
			buzzer_beep_time_counter = 0;
			
			
			if(buzzer_mode == strobo)
			{
				if(buzzer_beeps_counter)
				{
					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
					buzzer_beeps_counter--;
				}else
				{
					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
					buzzer_beeps_counter++;
				}
			}else if(buzzer_mode == beeps && buzzer_beeps_counter)
			{
				if(buzzer_beeps_counter%2 == 0)
				{
					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
				}
				else
				{
					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
				}
				buzzer_beeps_counter--;
			}
	
	
		}
	}
}
void tim_10ms_loop(void)
{
	
	
}

void tim_100ms_loop(void)
{ 
	
}

void tim_1000ms_loop(void)
{
	ADC_read();
	send_temp(thermocouple_1);
//	send_temp(thermocouple_2);
//	send_temp(thermocouple_3);
//	send_temp(thermocouple_4);
//	send_temp(thermocouple_board);
//	send_temp(MCU_temp);

	buzzer(beeps,1);
	buzzer_speed(2);

	
	test_counter += 5;
	if(test_counter > 100) test_counter=0;
	triac_set_duty(1,test_counter);
	triac_set_duty(2,100-test_counter);
	
	/* SPEED meter */
	speed = speed_counter;
	speed_counter = 0;
	
	
}
