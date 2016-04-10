/**
  ******************************************************************************
  * @file    Project/STM32F0xx_StdPeriph_Templates/stm32f0xx_it.c 
  * @author  MCD Application Team
  * @version V1.5.0
  * @date    05-December-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_it.h"

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M0 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	Period_timer_increment();
	
}

/******************************************************************************/
/*                 STM32F0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f0xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/


/**
  * @brief  This function handles External line 0 and 1 interrupt requests.
  * @param  None
  * @retval None
  */
void EXTI2_3_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line3) != RESET)
	{
		encoder_handler();
		/* Clear the EXTI line 5 pending bit */
		EXTI_ClearITPendingBit(EXTI_Line3);
	}
}


void EXTI4_15_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line4) != RESET)
	{
		if(buttons_delay[button_enc] == 0)
		{
			button_handler(button_enc);
		}
		/* Clear the EXTI line 5 pending bit */
		EXTI_ClearITPendingBit(EXTI_Line4);
	}
	if(EXTI_GetITStatus(EXTI_Line5) != RESET)
	{
		if(buttons_delay[button_4] == 0)
		{
			button_handler(button_4);
		}
		/* Clear the EXTI line 5 pending bit */
		EXTI_ClearITPendingBit(EXTI_Line5);
	}
	if(EXTI_GetITStatus(EXTI_Line12) != RESET)
	{
		if(buttons_delay[button_3] == 0)
		{
			button_handler(button_3);
		}
		/* Clear the EXTI line 12 pending bit */
		EXTI_ClearITPendingBit(EXTI_Line12);
	}
	if(EXTI_GetITStatus(EXTI_Line13) != RESET)
	{
		if(buttons_delay[button_2] == 0)
		{
			button_handler(button_2);
		}
		
		/* Clear the EXTI line 13 pending bit */
		EXTI_ClearITPendingBit(EXTI_Line13);
	}
	if(EXTI_GetITStatus(EXTI_Line14) != RESET)
	{
		if(buttons_delay[button_1] == 0)
		{
			button_handler(button_1);
		}
		/* Clear the EXTI line 14 pending bit */
		EXTI_ClearITPendingBit(EXTI_Line14);
	}
}





void I2C1_IRQHandler(void)
{
	if(I2C_GetITStatus(I2C1,I2C_IT_TXIS) != RESET)
	{
		
		// Odeslat pripravena data
		if(i2c_curr_session.params_transfered < i2c_sessions[i2c_curr_session.id].param_count)
		{	
			I2C_SendData(I2C1, parameters_master[i2c_curr_session.params_transfered++]);
		}else	if(i2c_curr_session.bytes_transfered < i2c_sessions[i2c_curr_session.id].byte_count)// && i2c_sessions[i2c_curr_session.id].rw == write)
		{
			I2C_SendData(I2C1, transfer_data_master[i2c_curr_session.bytes_transfered++]);
		}else
		{
			I2C_SendData(I2C1, 0xFF);
		}
		
		I2C_ClearITPendingBit(I2C1, I2C_IT_TXIS);
	}
	if(I2C_GetITStatus(I2C1,I2C_IT_RXNE) != RESET)
	{
		if(i2c_curr_session.bytes_transfered < i2c_sessions[i2c_curr_session.id].byte_count && i2c_sessions[i2c_curr_session.id].rw == read)
		{
			// prijmout data
			transfer_data_master[i2c_curr_session.bytes_transfered++] = I2C_ReceiveData(I2C1);
		}
		// po prijeti vsech parametru nacist data k zapisu (nemusi být zadny parametr - proto tady je to!)
		if(i2c_curr_session.bytes_transfered == i2c_sessions[i2c_curr_session.id].byte_count)
			i2c_sessions[i2c_curr_session.id].session_data(i2c_curr_session.id,receive);
	
		I2C_ClearITPendingBit(I2C1, I2C_IT_RXNE);
	}
	if(I2C_GetITStatus(I2C1,I2C_IT_NACKI) != RESET)
	{
		/* Vyhodit pripadny zapsany byte navic - neodesle se pri pristim read pozadavku*/
		I2C1->ISR = I2C_FLAG_TXE;
		
		I2C_ClearITPendingBit(I2C1, I2C_IT_NACKI);
	}
	if(I2C_GetITStatus(I2C1,I2C_IT_STOPF) != RESET)
	{
		// Zpracovat prijata data pokud sedi pocty parametru a prijatych bytu
//		if(params_transfered == i2c_sessions_slave[session_id].param_count \
//			&& bytes_transfered == i2c_sessions_slave[session_id].byte_count)
//		{
//			i2c_sessions_slave[session_id].process_it(write,session_id) ;
//		}
		// ukoncit session
		i2c_curr_session.running = false;
		
		I2C_ClearITPendingBit(I2C1, I2C_IT_STOPF);
	}
	if(I2C_GetITStatus(I2C1,I2C_IT_BERR) != RESET)
	{
		// chyba na sbernici!!!
		
		I2C_ClearITPendingBit(I2C1, I2C_IT_BERR);
	}
	
	if(I2C_GetITStatus(I2C1,I2C_IT_TC) != RESET)
	{

		if(i2c_sessions[i2c_curr_session.id].param_count == i2c_curr_session.params_transfered)
		{
			/* Configure slave address, nbytes, reload, end mode and start or stop generation */
			I2C_TransferHandling(I2C1, i2c_curr_session.dest_address, \
			i2c_sessions[i2c_curr_session.id].byte_count, \
			I2C_AutoEnd_Mode, \
			((i2c_sessions[i2c_curr_session.id].rw == read)? I2C_Generate_Start_Read : I2C_Generate_Start_Write));
		}
		I2C_ClearITPendingBit(I2C1, I2C_IT_TC);
	}
}

void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) )
	{
		static uint8_t mess_lenght = 0; //string length
		char t = USART1->RDR; // receive character
		
		if( (t != '$') && (mess_lenght < MAX_MESSAGE_LENGHT) ){
			received_string[mess_lenght] = t;
			mess_lenght++;
		}
		else
		{
			received_string[mess_lenght] = '\0';
			mess_lenght = 0;
			uart_parse((char*)received_string);
			USART_puts(USART1, received_string);
		}
	}
	if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
	{   
		/* Write one byte to the transmit data register */
		USART_SendData(USART1, *TX_Buffer);
		if(!*TX_Buffer)
		{
			next_message = true;
			USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
		}else
			*TX_Buffer++;
	}
}
/**
  * @}
  */ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
