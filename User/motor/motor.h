#ifndef MOTOR_H
#define MOTOR_H
#include "bsp_pwm.h"

#define	MOTOR_MAX_LEVEL							99
#define MOTOR_MIN_LEVEL							0

#define SPEED_LEFT_MOTOR						TIM3->CCR1
#define SPEED_RIGHT_MOTOR						TIM3->CCR2

#define SPEED_SWITCH1_GPIO_CLK								RCC_APB2Periph_GPIOA
#define SPEED_SWITCH1_GPIO_PORT								GPIOA
#define SPEED_SWITCH1_GPIO_PIN									GPIO_Pin_11

#define SPEED_SWITCH2_GPIO_CLK								RCC_APB2Periph_GPIOA
#define SPEED_SWITCH2_GPIO_PORT								GPIOA
#define SPEED_SWITCH2_GPIO_PIN									GPIO_Pin_12

#define SPEED_SWITCH3_GPIO_CLK								RCC_APB2Periph_GPIOA
#define SPEED_SWITCH3_GPIO_PORT								GPIOA
#define SPEED_SWITCH3_GPIO_PIN									GPIO_Pin_15

/*
 * left motor pwm pin PA6 connect to L298N's ENA(A) 
 * left motor control pins PB0 and PB1 connect to L298N's IN1 and IN2
 * right motor pwm pin PA7 connect to l298N's ENA(B)
 * rtght motor control pins PB5 and PB6 connect to L298N's IN3 and IN4
 */
#define    MOTOR_LEFT_CONTROL_IN1_GPIO_CLK     		RCC_APB2Periph_GPIOB
#define    MOTOR_LEFT_CONTROL_IN1_GPIO_PORT    	GPIOB			   		   
#define    MOTOR_LEFT_CONTROL_IN1_GPIO_PIN		 	GPIO_Pin_0
#define    MOTOR_LEFT_CONTROL_IN2_GPIO_CLK     		RCC_APB2Periph_GPIOB
#define    MOTOR_LEFT_CONTROL_IN2_GPIO_PORT    	GPIOB			   		   
#define    MOTOR_LEFT_CONTROL_IN2_GPIO_PIN		 	GPIO_Pin_1

#define    MOTOR_RIGHT_CONTROL_IN3_GPIO_CLK     	RCC_APB2Periph_GPIOB
#define    MOTOR_RIGHT_CONTROL_IN3_GPIO_PORT    	GPIOB			   		   
#define    MOTOR_RIGHT_CONTROL_IN3_GPIO_PIN		 	GPIO_Pin_5
#define    MOTOR_RIGHT_CONTROL_IN4_GPIO_CLK     	RCC_APB2Periph_GPIOB
#define    MOTOR_RIGHT_CONTROL_IN4_GPIO_PORT    	GPIOB			   		   
#define    MOTOR_RIGHT_CONTROL_IN4_GPIO_PIN		 	GPIO_Pin_6

#define    MOTOR_SPEED_SELECT_GPIO_CLK     			RCC_APB2Periph_GPIOB
#define    MOTOR_SPEED_SELECT_GPIO_PORT    			GPIOB			   		   
#define    MOTOR_SPEED_SELECT_GPIO_PIN		 			GPIO_Pin_5

void motor_init(void);
void motor_fb_control(uint8_t nr_motor, uint8_t status);
void motor_forward(void);
void motor_stop(void);
void motor_spin_left(void);
void motor_spin_right(void);
void motor_slow(void);
void motor_brake(void);

#endif

