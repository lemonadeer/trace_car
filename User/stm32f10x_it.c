/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTI
  
  AL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "bsp_pwm.h"
#include "simple.h"
#include "trace_control.h"
#include "bsp_delay.h"

extern __IO uint8_t left_spin_lock;
extern __IO uint8_t right_spin_lock;
extern __IO uint8_t case_timer_lock;
extern __IO uint8_t nr_case;
extern __IO uint8_t need_process;
extern __IO uint8_t flag_spcecial;
extern uint8_t motor_speed_mode;

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
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
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
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
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
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
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

void  TIMER_IRQHandler (void)
{
	if ( TIM_GetITStatus( TIMER, TIM_IT_Update) != RESET ) 
	{	
		/* unlocked */
		left_spin_lock = 0;
		right_spin_lock = 0;
		TIM_ClearITPendingBit(TIMER , TIM_FLAG_Update);  		 
	}		 	
}

void  CASE_TIMER_IRQHandler (void)
{
	if ( TIM_GetITStatus( CASE_TIMER, TIM_IT_Update) != RESET ) 
	{	
		case_timer_lock = 0;/* unlock */
		if(nr_case == endline){
			motor_stop();
			while(1);
		}
		TIM_ClearITPendingBit( CASE_TIMER , TIM_FLAG_Update);  		 
	}		 	
}

void SENSOR4_IRQHandler(void)
{	
	systick_delay_us(10);
	if(SENSOR4_BIT) 
	{
		if(!case_timer_lock){
			case_timer_lock = 1;
			need_process = 1;
			LED1_TOGGLE; 			
			nr_case ++;
			
			if(nr_case == case8_turn_enter){
				flag_spcecial = 1;	//turn left when enter case8
			}
			if(nr_case == before_end){
				flag_spcecial = 0;	
			}
			
			switch (nr_case){
				case startline:					CASE_TIMER->ARR = 2 * CASE_TIMER_Period;		break;
				case endturn:					CASE_TIMER->ARR = 3 * CASE_TIMER_Period;		break;
				case endline_ignore:		CASE_TIMER->ARR = 2 * CASE_TIMER_Period;		break;
				case island_enter:			CASE_TIMER->ARR = 5 * CASE_TIMER_Period;		break;
				case island_away:			CASE_TIMER->ARR = 5 * CASE_TIMER_Period;		break;
				case trap:							CASE_TIMER->ARR = 30 * CASE_TIMER_Period;	break;
				case case8_away:			CASE_TIMER->ARR = 20 * CASE_TIMER_Period;	break;
				case before_end:				CASE_TIMER->ARR = 20 * CASE_TIMER_Period;	break;	
				case endline:					
					if(motor_speed_mode == 0)	{
						CASE_TIMER->ARR = 20 * CASE_TIMER_Period;
					}
					else if(motor_speed_mode == 1){
						CASE_TIMER->ARR = 14 * CASE_TIMER_Period;
					}
					else{
						CASE_TIMER->ARR = 5 * CASE_TIMER_Period;
					}
					break;
					
				default:								CASE_TIMER->ARR = 10 * CASE_TIMER_Period;	break;
			}	
			
		}
	}
	EXTI_ClearITPendingBit(SENSOR4_EXTI_LINE); 
}

void SENSOR5_IRQHandler(void)
{
	systick_delay_us(10);
	
	if(SENSOR5_BIT) {
		if(!case_timer_lock){
			case_timer_lock = 1;	
			need_process = 1;
			LED2_TOGGLE; 
			nr_case ++;
			
			if(nr_case == case8_turn_enter){
				flag_spcecial = 2;	//turn right when enter case8
			}
			if(nr_case == before_end){
				flag_spcecial = 0;	
			}
			
			switch (nr_case){
				case startline:					CASE_TIMER->ARR = 2 * CASE_TIMER_Period;		break;
				case endturn:					CASE_TIMER->ARR = 3 * CASE_TIMER_Period;		break;
				case endline_ignore:		CASE_TIMER->ARR = 2 * CASE_TIMER_Period;		break;
				case case8_away:			CASE_TIMER->ARR = 20 * CASE_TIMER_Period;	break;
				case before_end:				CASE_TIMER->ARR = 20 * CASE_TIMER_Period;	break;
				case endline:
					if(motor_speed_mode == 0)	{
						CASE_TIMER->ARR = 20 * CASE_TIMER_Period;
					}
					else if(motor_speed_mode == 1){
						CASE_TIMER->ARR = 14 * CASE_TIMER_Period;
					}
					else{
						CASE_TIMER->ARR = 5 * CASE_TIMER_Period;
					}
					break;
				default:								CASE_TIMER->ARR = 8 * CASE_TIMER_Period;	break;
			}	
			
		}
	}
	EXTI_ClearITPendingBit(SENSOR5_EXTI_LINE); 
}
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
