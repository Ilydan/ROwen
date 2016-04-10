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
uint8_t parameters_master[0x0A];
uint8_t transfer_data_master[0x0A];

struct i2c_sess i2c_sessions[32];
struct i2c_curr_sess i2c_curr_session;

/* I2C FIFO ring buffer */
/* pointers pointing to next r/w */

struct i2c_sess_buff i2c_sessions_buffer[21];
uint8_t i2c_sb_r_pointer = 0;
uint8_t i2c_sb_w_pointer = 0;



bool i2c_send_session(e_i2c_session session_id, uint8_t slave_address)
{
	if(i2c_sb_w_pointer+1 != i2c_sb_r_pointer && !(i2c_sb_w_pointer == 20 && i2c_sb_r_pointer == 0))
	{
		i2c_sessions_buffer[i2c_sb_w_pointer].session_id = session_id;
		i2c_sessions_buffer[i2c_sb_w_pointer].slave_address = slave_address;
		if(++i2c_sb_w_pointer > 20) i2c_sb_w_pointer = 0;
		return false;
	}
	else
		return true;
}
void i2c_process_session(void)
{
	if(!i2c_curr_session.running && (i2c_sb_r_pointer != i2c_sb_w_pointer))
	{
		i2c_curr_session.running = true;
		i2c_curr_session.id = i2c_sessions_buffer[i2c_sb_r_pointer].session_id;
		i2c_curr_session.params_transfered = 0;
		i2c_curr_session.bytes_transfered = 0;
		i2c_curr_session.dest_address = i2c_sessions_buffer[i2c_sb_r_pointer].slave_address;
		i2c_sessions[i2c_curr_session.id].session_data(i2c_curr_session.id, transmit);
		
		/* Configure slave address, nbytes, reload, end mode and start or stop generation */
		I2C_TransferHandling(I2C1, i2c_curr_session.dest_address, \
		i2c_sessions[i2c_curr_session.id].param_count, \
		((i2c_sessions[i2c_curr_session.id].byte_count) ? I2C_SoftEnd_Mode : I2C_AutoEnd_Mode), \
		I2C_Generate_Start_Write);
		
		/* read pointer increment */
		if(++i2c_sb_r_pointer > 20)i2c_sb_r_pointer = 0;
	}
}
void Define_I2C_Sessions(void)
{
	
	// PREDELAT na KONSTANTU !!!!!!!!
	for(int i = 0; i < 64;i++)
	{
		i2c_sessions[i].rw = write;
		i2c_sessions[i].param_count = 0;
		i2c_sessions[i].byte_count = 0;
		i2c_sessions[i].session_data = NULL;
	}
	
	// config i2c expander
	i2c_sessions[0x01].rw = write;
	i2c_sessions[0x01].param_count = 3;
	i2c_sessions[0x01].byte_count = 0;
	i2c_sessions[0x01].session_data = expander_config;
	
	// set i2c expander port 0
	i2c_sessions[0x02].rw = write;
	i2c_sessions[0x02].param_count = 3;
	i2c_sessions[0x02].byte_count = 0;
	i2c_sessions[0x02].session_data = expander_set;
	
	// config tcn75
	i2c_sessions[0x03].rw = write;
	i2c_sessions[0x03].param_count = 2;
	i2c_sessions[0x03].byte_count = 0;
	i2c_sessions[0x03].session_data = config_tcn75_temp;
	
	// get temp from tcn75
	i2c_sessions[0x04].rw = read;
	i2c_sessions[0x04].param_count = 1;
	i2c_sessions[0x04].byte_count = 2;
	i2c_sessions[0x04].session_data = get_tcn75_temp;
	
	// get ADC
	i2c_sessions[0x05].rw = read;
	i2c_sessions[0x05].param_count = 1;
	i2c_sessions[0x05].byte_count = 4;
	i2c_sessions[0x05].session_data = get_ext_ADC_voltages;
}


void expander_config(uint8_t session_id, e_direction direction)
{
	parameters_master[0] = 0x06;
	parameters_master[1] = 0x00;
	parameters_master[2] = 0x00;
}
void expander_set(uint8_t session_id, e_direction direction)
{
	parameters_master[0] = 0x02;

	if(i2c_curr_session.dest_address == GPIO_EXPANDER_0_ADDRESS)
	{
		parameters_master[1] = display[1];
		parameters_master[2] = display[0];
	}else if(i2c_curr_session.dest_address == GPIO_EXPANDER_1_ADDRESS)
	{
		parameters_master[1] = display[2];
		parameters_master[2] = display[3];
	}
}
void config_tcn75_temp(uint8_t session_id, e_direction direction)
{
	parameters_master[0] = 0x01;
	parameters_master[1] = 0x60;
}
void get_tcn75_temp(uint8_t session_id, e_direction direction)
{
	if(direction == transmit)
		parameters_master[0] = 0x00;
	else
		s_system.s_temp.thermocouple_board = (float)((((transfer_data_master[0] << 8) | transfer_data_master[1])>>4)*0.0625f);
}
void get_ext_ADC_voltages(uint8_t session_id, e_direction direction)
{
	static uint16_t ADC_channel = 2;
//	int attempts = 0;
	int32_t ADC_read;

	if(direction == transmit)
	{
		parameters_master[0] = (0x1<<7)|(ADC_channel<<5)|0x1F;
	}else
	{
		ADC_read = (int32_t)(((uint32_t)transfer_data_master[0]<<24) |  
			((uint32_t)transfer_data_master[1]<<16) | 
			((uint32_t)transfer_data_master[2]<<8))/256;
		
		/*
			ADC_read*1.953125f => napeti v uV
			K thermocouple -> 40uV/stupenC
			ADC_read/20.48 = stupne C
		*/
		
		s_system.s_power.ADC_uvolt[(transfer_data_master[3]>>5) & 0x03] = (int32_t)(ADC_read*1.953125f);
		s_system.s_temp.thermocouple[(transfer_data_master[3]>>5) & 0x03] = (ADC_read/20.48f)+s_system.s_temp.thermocouple_board;
		
//		if(ADC_channel++ < 3 && attempts++ < 10)
//			i2c_send_session(session_get_ext_ADC_voltages,ADC_EXT_ADDRESS);
//		else
//		{
//			ADC_channel = 0;
//			attempts = 0;
//		}
	}
}
