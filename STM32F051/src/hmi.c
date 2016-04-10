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
bool buttons_flags[5];
uint16_t display_block = 0;
uint16_t buttons_delay[5] = {0};
uint8_t buzzer_beeps_counter = 0;
uint16_t buzzer_beep_time_counter = 0;
uint16_t buzzer_beep_time = 0;
e_buzzer_mode buzzer_mode = off;
bool b1_flag = false;
bool b2_flag = false;
bool b3_flag = false;
bool b4_flag = false;


uint8_t display[4] = {1};
/*   display[0]
					 ______
          /  6  /
         /7   5/
        /--4--/
       /1   3/
      /__2__/ O 8
*/
/*   display[1]      
					 ______
          /  3  /
         /2   4/
        /--1--/
       /8   6/
      /__7__/ O 5
*/
/*	display[2]      
					 ______
          /  2  /
         /3   1/
        /--4--/
       /5   7/
      /__6__/ O 8

*/
//const uint8_t segments[4][8] = {
//{0x20,0x40,0x80,0x01,0x08,0x04,0x02,0x10},
//{0x04,0x02,0x01,0x80,0x10,0x20,0x40,0x08},
//{0x02,0x04,0x08,0x10,0x80,0x40,0x20,0x01},
//{0}};
/* 0,1,2,3,4,5,6,7,8,9 */
const uint8_t numbers[3][10] = {
	{0x11,0xD7,0x32,0x92,0xD4,0x98,0x18,0xD3,0x10,0x90},
	{0x88,0xEB,0x4C,0x49,0x2B,0x19,0x18,0xCB,0x08,0x09},
	{0x11,0x7d,0x23,0x29,0x4D,0x89,0x81,0x3D,0x01,0x09}
};
/* A,b,C,d,E,F,g,H,I,J,K.,L,M,n,O,P,q,r,S,t,U,v,y,Z,blank char X */
const uint8_t letters[3][25] = {
	{0x50,0x1C,0x39,0x16,0x38,0x78,0x90,0x54,0x7D,0x17,0x44,0x3D,0x5B,0x5E,0x11,0x70,0xD0,0x7E,0x98,0x3C,0x15,0x1F,0x94,0x32,0xFF},
	{0x0A,0x38,0x9C,0x68,0x1C,0x1E,0x09,0x2A,0xBE,0xE8,0x22,0xBC,0xDA,0x7A,0x88,0x0E,0x0B,0x7E,0x19,0x3C,0xA8,0xF8,0x29,0x4C,0xFF},
	{0x05,0xC1,0x93,0x61,0x83,0x87,0x09,0x45,0xD7,0x71,0x44,0xD3,0xB5,0xE5,0x11,0x07,0x0D,0xE7,0x89,0xC3,0x51,0xF1,0x49,0x23,0xFF}
	};

void buzzer(e_buzzer_mode b_mode, uint8_t beep_count)
{
	buzzer_mode = b_mode;
	switch(buzzer_mode)
	{
		case off:
			GPIO_SetBits(GPIOB, GPIO_Pin_15);
		break;
		case beep_const:
			GPIO_ResetBits(GPIOB, GPIO_Pin_15);
		break;
		case strobo:
			buzzer_beeps_counter = 1;
		break;
		case beeps:
			buzzer_beeps_counter = beep_count*2;
		break;
	}
}
void buzzer_speed(uint16_t speed)
{
	buzzer_beep_time_counter = 0;
	buzzer_beep_time = speed; //time in [ms] for one beep
}

void button_handler(e_buttons button)
{
	bool beep = false;
	
	/* ====== ENCODER button =========== */
	if(button == button_enc && !buttons_delay[button_enc])
	{
		TIM3->CR1 ^= TIM_CR1_CEN;
		if(TIM3->CR1 & TIM_CR1_CEN)
		{
			GPIOB->BRR = GPIO_Pin_6;
			
			buzzer(beeps,2);
			buzzer_speed(80);
		}else
		{
			GPIOB->BSRR = GPIO_Pin_6;
			
			TIM3->CCR1 = 0;
			TIM3->CCR2 = 0;
			TIM3->CCR3 = 0;
			TIM3->CCR4 = 0;
			
			buzzer(beeps,3);
			buzzer_speed(80);
		}
		beep = false;
	}
	/* ====== BUTTONS =========== */
	if(button == button_1)
	{	
		beep = true;
		b1_flag = !b1_flag;
		start_time = 0;
		struct ramp_equation* y = ProfileBuilder(profile);
			if(!b1_flag)
			{
				triac_set_duty(both_heat, 0);
				setPoint = 0;
				display[3] = 0xFF;
			}
	}
	if(button == button_2)
	{
		b2_flag = !b2_flag;
		
		beep = true;
	}
	if(button == button_3)
	{
		beep = true;
	}
	if(button == button_4)
	{
		triac_set_duty(both_heat, 0);
		setPoint = 0;
		b1_flag = false;
		b2_flag = false;
		b3_flag = false;
		b4_flag = false;
		beep = true;
	}
	if(beep)
	{
  		buzzer(beeps,1);
		buzzer_speed(30);
	}
	buttons_delay[button] = button_press_delay;
}
void encoder_handler(void)
{
	if(TIM3->CR1 & TIM_CR1_CEN)
	{
		GPIOB->ODR ^= GPIO_Pin_7; // toggle
		display_block = 0;
		
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15) == Bit_SET)
		{
			triac_modify_duty(top_heat,100);
			triac_modify_duty(bottom_heat,100);
		}else
		{
			triac_modify_duty(top_heat,-100);
			triac_modify_duty(bottom_heat,-100);
		}
		display_block = 3000;
		buzzer(beeps,1);
		buzzer_speed(3);
	}
}
/***************** DISPLAY  ****************/
void write_num_to_display(uint16_t number)
{
	uint8_t display_out = 3;
	if(!display_block)
	{
		display[0] = 0xff;
		display[1] = 0xff;
		display[2] = 0xff;
		if(!number)
			display[2] = numbers[2][0];
		while(number && display_out)
		{
			display[display_out-1] = numbers[display_out-1][number % 10];
			display_out--;
			number /= 10;
		}
		i2c_send_session(session_expander_set,GPIO_EXPANDER_0_ADDRESS);
		i2c_send_session(session_expander_set,GPIO_EXPANDER_1_ADDRESS);
	}
}
void write_txt_to_display(alphabet leftChar, alphabet middleChar, alphabet rightChar)
	{
		display_block = 1000;
		display[0] = letters[0][(uint8_t)leftChar];
		display[1] = letters[1][(uint8_t)middleChar];
		display[2] = letters[2][(uint8_t)rightChar];
		i2c_send_session(session_expander_set,GPIO_EXPANDER_0_ADDRESS);
		i2c_send_session(session_expander_set,GPIO_EXPANDER_1_ADDRESS);
	}	
