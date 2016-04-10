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

#define TS_CAL1     (*((uint16_t*) 0x1FFFF7B8))               // TS_CAL1 = 1727
#define TS_CAL2     (*((uint16_t*) 0x1FFFF7C2))               // TS_CAL2 = 1308
#define VREFINT_CAL (*((uint16_t*) 0x1FFFF7BA))
#define AVG_SLOPE   (float)((TS_CAL1 - TS_CAL2)/80.0f) 
	
struct sys s_system;
struct h_ramp ramp[10];
struct ramp_equation y[10];
int8_t prevErr;

volatile uint16_t ADC_Output[2];
uint16_t temp[10] = {0};
uint8_t temp_counter = 0;

struct ramp_equation* ProfileBuilder(float *profile)
{	
	uint8_t i=0,j = 0;
	
	uint8_t const ramps_count = 4;
	struct coordinates s[ramps_count] = {0};
	struct coordinates n[ramps_count] = {0};
		
	int c[ramps_count];
	
		
	for(i=0;i<ramps_count;++i)
	{
		s[i].time = profile[j+2] - profile[j];
		s[i].temp = profile[j+3] - profile[j+1];
		n[i].time = s[i].temp;
		n[i].temp = -s[i].time;
		c[i] = -n[i].time*profile[j] - n[i].temp*profile[j+1];
		y[i].k = -n[i].time/n[i].temp;
		y[i].q = -c[i]/n[i].temp;
		j = j+2;
	}
return(y);
}


void OverTempBeep(void){
	uint8_t i;
	uint16_t max;
	
	for(i = 3; i<10;)
		{
			if(profile[i]>profile[i-2])
				max = profile[i];
			i = i+2;
		}
		if(s_system.s_temp.thermocouple[2]>=max || (s_system.s_temp.thermocouple[2] > setPoint))
		{
			tempAlarmTimer++;
			if(tempAlarmTimer>=7)
			{				
				if(tempAlarmTimer>=20)
				{
					return;
				}
				else{
					buzzer(beeps,2);
					buzzer_speed(80);
				}		
			}
		else if(s_system.s_temp.thermocouple[2]>=max || (s_system.s_temp.thermocouple[2] - setPoint) < 3)
		{
			tempAlarmTimer = 0;
		}
		
		}
}	

uint16_t PID_controller(int16_t setPoint)
{
	//Saturation limits
	uint16_t const max = 1000;
	uint16_t const min = 0;
	
	
	
	float P_term, I_term, D_term;
	/*
	float Kp = 102.20470723988018;
	float Kd = 105.111562700724;
	float Ki = 0.0115608926354802;
	*/
	float Kp = 160;
	float Kd = 110;
	float Ki = 0.2;
	
	float regErr;
	
	regErr = setPoint - s_system.s_temp.thermocouple[2];
		
	P_term  = regErr;   //Current error
	I_term += prevErr;  //Sum of previous errors
	D_term  = regErr - prevErr;  //Difference with previous error
	
	int32_t duty = (Kp*P_term + Ki*I_term + Kd*D_term);
	if(regErr<0)
		duty = 0;
	if(duty > max)
		duty = max;
	if(duty < min)
		duty = min;
	prevErr = regErr;
	
	
	if(duty>=0)
		display[3] = 0xFF;
	if(duty>10)
		display[3] = 0x3F;
	if(duty>284)
		display[3] = 0x1F;
	if(duty>426)
		display[3] = 0x0F;
	if(duty>568)
		display[3] = 0x07;
	if(duty>710)
		display[3] = 0x03;
	if(duty>852)
		display[3] = 0x01;
	if(duty==1000)
		display[3] = 0x00;
	return duty;
}
void ADC_read(void)
{
	/* MCU voltage */
	s_system.s_power.mcu_voltage = 3300.0f * VREFINT_CAL / ADC_Output[1];
	
	/* Temperature */
	temp[temp_counter] = (uint16_t)((((((ADC_Output[0] / 3300.0f) * s_system.s_power.mcu_voltage) - TS_CAL1) / (-AVG_SLOPE)) + 30)*10);
	s_system.s_temp.MCU_temp = (temp[0] + temp[1] + temp[2] + temp[3] + temp[4] + temp[5] + temp[6] + temp[7] + temp[8] + temp[9])/10;
	if(++temp_counter > 9)
		temp_counter = 0;
}
