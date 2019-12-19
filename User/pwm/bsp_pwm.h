#ifndef __BSP_GENERALTIME_H
#define __BSP_GENERALTIME_H


#include "stm32f10x.h"
#include "motor.h"
/* TIM3 PWM */
#define            GENERAL_TIM                  				TIM3
#define            GENERAL_TIM_APBxClock_FUN     	RCC_APB1PeriphClockCmd
#define            GENERAL_TIM_CLK              			RCC_APB1Periph_TIM3
#define            GENERAL_TIM_Period            			MOTOR_MAX_LEVEL
#define            GENERAL_TIM_Prescaler         		71
// TIM3 输出比较通道1
#define            GENERAL_TIM_CH1_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            GENERAL_TIM_CH1_PORT          	GPIOA
#define            GENERAL_TIM_CH1_PIN          		GPIO_Pin_6

// TIM3 输出比较通道2
#define            GENERAL_TIM_CH2_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            GENERAL_TIM_CH2_PORT         	GPIOA
#define            GENERAL_TIM_CH2_PIN          		GPIO_Pin_7

/* TIM 4 as timer */
#define            TIMER                   							TIM4
#define            TIMER_APBxClock_FUN     				RCC_APB1PeriphClockCmd
#define            TIMER_CLK               						RCC_APB1Periph_TIM4
/* 1ms between left and right spin */
#define            TIMER_Period           						1000			/* 100 ms */
#define            TIMER_Prescaler        						7199			/* 10000hz */
#define            TIMER_IRQ              							TIM4_IRQn
#define            TIMER_IRQHandler       					TIM4_IRQHandler

/* TIM 2 as a case_timer */
/* 在两个case中间有一个间隔 */
#define            CASE_TIMER                   					TIM2
#define            CASE_TIMER_APBxClock_FUN     	RCC_APB1PeriphClockCmd
#define            CASE_TIMER_CLK               			RCC_APB1Periph_TIM2
/* 1s between case simple */
#define            CASE_TIMER_Period           				1000 	/* 100ms */
#define            CASE_TIMER_Prescaler        			7199			/* 10000hz */
#define            CASE_TIMER_IRQ              				TIM2_IRQn
#define            CASE_TIMER_IRQHandler       			TIM2_IRQHandler

#define            END_TIMER                   					TIM1
#define            END_TIMER_APBxClock_FUN     		RCC_APB1PeriphClockCmd
#define            END_TIMER_CLK               				RCC_APB1Periph_TIM1

/**************************函数声明********************************/

void GENERAL_TIM_Init(void);
void timer_Init(void);

#endif	/* __BSP_GENERALTIME_H */


