#ifndef __SIMPLE_H
#define	__SIMPLE_H
#include "stm32f10x.h"

#define		spin_timer_reload									TIMER->ARR

#define    NUM_SENSOR										4

#define    STATUS_NORMOL								(uint8_t)0
#define 	STATUS_ADJUST_TO_RIGHT				(uint8_t)1
#define 	STATUS_ADJUST_TO_LEFT				(uint8_t)2
#define		STATUS_TURN_LEFT							(uint8_t)3
#define		STATUS_TURN_RIGHT						(uint8_t)4

#define    SENSOR0_GPIO_CLK     						RCC_APB2Periph_GPIOA
#define    SENSOR0_GPIO_PORT    					GPIOA			   		   
#define    SENSOR0_GPIO_PIN		 					GPIO_Pin_2

#define    SENSOR1_GPIO_CLK     						RCC_APB2Periph_GPIOA
#define    SENSOR1_GPIO_PORT    					GPIOA			   
#define    SENSOR1_GPIO_PIN		 					GPIO_Pin_3

#define    SENSOR2_GPIO_CLK     						RCC_APB2Periph_GPIOA
#define    SENSOR2_GPIO_PORT    					GPIOA			   
#define    SENSOR2_GPIO_PIN		 					GPIO_Pin_4

#define    SENSOR3_GPIO_CLK     						RCC_APB2Periph_GPIOA
#define    SENSOR3_GPIO_PORT    					GPIOA			   
#define    SENSOR3_GPIO_PIN		 					GPIO_Pin_5

#define    SENSOR4_GPIO_CLK     						RCC_APB2Periph_GPIOA
#define    SENSOR4_GPIO_PORT    					GPIOA			   
#define    SENSOR4_GPIO_PIN		 					GPIO_Pin_0
#define		SENSOR4_EXTI_PORTSOURCE   		GPIO_PortSourceGPIOA
#define 	SENSOR4_EXTI_PINSOURCE    			GPIO_PinSource0
#define 	SENSOR4_EXTI_LINE   			  			EXTI_Line0
#define 	SENSOR4_EXTI_IRQ         					EXTI0_IRQn
#define 	SENSOR4_IRQHandler           				EXTI0_IRQHandler

#define    SENSOR5_GPIO_CLK     						RCC_APB2Periph_GPIOA
#define    SENSOR5_GPIO_PORT    					GPIOA			   
#define    SENSOR5_GPIO_PIN		 					GPIO_Pin_1
#define		SENSOR5_EXTI_PORTSOURCE   		GPIO_PortSourceGPIOA
#define 	SENSOR5_EXTI_PINSOURCE    			GPIO_PinSource1
#define 	SENSOR5_EXTI_LINE       	  				EXTI_Line1
#define 	SENSOR5_EXTI_IRQ         					EXTI1_IRQn
#define 	SENSOR5_IRQHandler           				EXTI1_IRQHandler

#define 	SENSOR0_BIT										(uint8_t)(SENSOR0_GPIO_PORT->IDR & SENSOR0_GPIO_PIN)
#define 	SENSOR1_BIT										(uint8_t)(SENSOR1_GPIO_PORT->IDR & SENSOR1_GPIO_PIN)
#define 	SENSOR2_BIT										(uint8_t)(SENSOR2_GPIO_PORT->IDR & SENSOR2_GPIO_PIN)
#define 	SENSOR3_BIT										(uint8_t)(SENSOR3_GPIO_PORT->IDR & SENSOR3_GPIO_PIN)
#define 	SENSOR4_BIT										(uint8_t)(SENSOR4_GPIO_PORT->IDR & SENSOR4_GPIO_PIN)
#define 	SENSOR5_BIT										(uint8_t)(SENSOR5_GPIO_PORT->IDR & SENSOR5_GPIO_PIN)

typedef enum{
	start = 0,
	startline,
	endturn,
	endline_ignore,
	island_enter,			
	island_away,			
	trap ,							
	case8_enter,
	case8_turn_enter ,
	case8_turn_away ,
	case8_away ,
	before_end ,				
	endline						
}case_status;


void sensor_init(void);
uint8_t sensor_simple(void);
void deal_nr_case(void);
void process_int(void);

#endif 

