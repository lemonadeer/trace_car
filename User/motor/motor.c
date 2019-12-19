#include "motor.h"
#include "bsp_usart.h"
#include "simple.h"
#include "bsp_delay.h"
#include "bsp_led.h"

extern uint8_t MOTOR_NORMAL_SPEED_LEVEL;
extern uint8_t MOTOR_SPIN_SPEED_LEVEL;
extern uint8_t MOTOR_SLOW_SPEED_LEVEL;

uint8_t motor_speed_mode = 0;	

static GPIO_TypeDef *control_port_table[4] = {
	MOTOR_LEFT_CONTROL_IN1_GPIO_PORT,
	MOTOR_LEFT_CONTROL_IN2_GPIO_PORT,
	MOTOR_RIGHT_CONTROL_IN3_GPIO_PORT,
	MOTOR_RIGHT_CONTROL_IN4_GPIO_PORT
};

static uint16_t control_pin_table[4] = {
	MOTOR_LEFT_CONTROL_IN1_GPIO_PIN,
	MOTOR_LEFT_CONTROL_IN2_GPIO_PIN,
	MOTOR_RIGHT_CONTROL_IN3_GPIO_PIN,
	MOTOR_RIGHT_CONTROL_IN4_GPIO_PIN
};

static void motor_control_gpio_init(void)
{
	uint8_t i;
	GPIO_InitTypeDef GPIO_InitStructure;
	/* Cause all gpio is PB* so only need to set once */
	RCC_APB2PeriphClockCmd(MOTOR_LEFT_CONTROL_IN1_GPIO_CLK | \
	MOTOR_RIGHT_CONTROL_IN3_GPIO_CLK,ENABLE);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	
	for(i = 0; i < 4; i ++){
		GPIO_InitStructure.GPIO_Pin = control_pin_table[i]; 
		GPIO_Init(control_port_table[i], &GPIO_InitStructure);
		/* set EIN1 and EIN2 to 1 to ensure motor stop */
		GPIO_SetBits(control_port_table[i], control_pin_table[i]);
	}
}

static void motor_speed_select_gpio_config(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(SPEED_SWITCH1_GPIO_CLK | SPEED_SWITCH2_GPIO_CLK,ENABLE);										
	GPIO_InitStructure.GPIO_Pin = SPEED_SWITCH1_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(SPEED_SWITCH1_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = SPEED_SWITCH2_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(SPEED_SWITCH2_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = SPEED_SWITCH3_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(SPEED_SWITCH3_GPIO_PORT, &GPIO_InitStructure);
}

static void motor_speed_select(void)
{
	if(GPIO_ReadInputDataBit(SPEED_SWITCH3_GPIO_PORT, SPEED_SWITCH3_GPIO_PIN)){
		/* max mode */
		motor_speed_mode = 2;
		MOTOR_NORMAL_SPEED_LEVEL = 80;
		MOTOR_SPIN_SPEED_LEVEL = 72;
		MOTOR_SLOW_SPEED_LEVEL  = 60;
	}
	else if(GPIO_ReadInputDataBit(SPEED_SWITCH2_GPIO_PORT, SPEED_SWITCH2_GPIO_PIN)){
		/* turbo mode */
		motor_speed_mode = 1;
		MOTOR_NORMAL_SPEED_LEVEL = 76;
		MOTOR_SPIN_SPEED_LEVEL = 70;
		MOTOR_SLOW_SPEED_LEVEL  = 60;
	}	
	else{
		/* normal mode */
		motor_speed_mode = 0;
		MOTOR_NORMAL_SPEED_LEVEL = 73;
		MOTOR_SPIN_SPEED_LEVEL = 70;
		MOTOR_SLOW_SPEED_LEVEL = 60;		
	}
}

void motor_init(void)
{
	/* Initial of the motor pwm gpio */
	GENERAL_TIM_Init();
	motor_control_gpio_init();
	motor_speed_select_gpio_config();
	motor_speed_select();
}

/* control the motor forward and back rotation */
/* 0 : forward		1 : back	2 : brake */
void motor_fb_control(uint8_t nr_motor, uint8_t status)
{
	switch (status) {
		case 0 :				
			if(nr_motor){		/* right motor */
				GPIO_ResetBits(MOTOR_RIGHT_CONTROL_IN3_GPIO_PORT, MOTOR_RIGHT_CONTROL_IN3_GPIO_PIN);
				GPIO_SetBits(MOTOR_RIGHT_CONTROL_IN4_GPIO_PORT, MOTOR_RIGHT_CONTROL_IN4_GPIO_PIN);
			}
			else{
				GPIO_ResetBits(MOTOR_LEFT_CONTROL_IN1_GPIO_PORT, MOTOR_LEFT_CONTROL_IN1_GPIO_PIN);
				GPIO_SetBits(MOTOR_LEFT_CONTROL_IN2_GPIO_PORT, MOTOR_LEFT_CONTROL_IN2_GPIO_PIN);
			};  break;
			
		case 1 :				
			if(nr_motor){
				GPIO_ResetBits(MOTOR_RIGHT_CONTROL_IN4_GPIO_PORT, MOTOR_RIGHT_CONTROL_IN4_GPIO_PIN);
				GPIO_SetBits(MOTOR_RIGHT_CONTROL_IN3_GPIO_PORT, MOTOR_RIGHT_CONTROL_IN3_GPIO_PIN);
			}
			else{
				GPIO_ResetBits(MOTOR_LEFT_CONTROL_IN2_GPIO_PORT, MOTOR_LEFT_CONTROL_IN2_GPIO_PIN);
				GPIO_SetBits(MOTOR_LEFT_CONTROL_IN1_GPIO_PORT, MOTOR_LEFT_CONTROL_IN1_GPIO_PIN);
			};  break;
	
		case 2 :				
			if(nr_motor){
				GPIO_SetBits(MOTOR_RIGHT_CONTROL_IN3_GPIO_PORT, MOTOR_RIGHT_CONTROL_IN3_GPIO_PIN);
				GPIO_SetBits(MOTOR_RIGHT_CONTROL_IN4_GPIO_PORT, MOTOR_RIGHT_CONTROL_IN4_GPIO_PIN);
			}
			else{
				GPIO_SetBits(MOTOR_LEFT_CONTROL_IN1_GPIO_PORT, MOTOR_LEFT_CONTROL_IN1_GPIO_PIN);
				GPIO_SetBits(MOTOR_LEFT_CONTROL_IN2_GPIO_PORT, MOTOR_LEFT_CONTROL_IN2_GPIO_PIN);
			};  break;	
			
		default : ;
	}
}

void motor_slow(void)
{
	SPEED_LEFT_MOTOR = MOTOR_SLOW_SPEED_LEVEL;
	SPEED_RIGHT_MOTOR = MOTOR_SLOW_SPEED_LEVEL;
	motor_fb_control(0, 0);
	motor_fb_control(1, 0);
}

void motor_forward(void)
{
	SPEED_LEFT_MOTOR = 60;
	SPEED_RIGHT_MOTOR = 60;
	motor_fb_control(0, 0);
	motor_fb_control(1, 0);
}

void motor_stop(void)
{
	SPEED_LEFT_MOTOR = 0;
	SPEED_RIGHT_MOTOR = 0;
}

void motor_brake(void)
{
	SPEED_LEFT_MOTOR = MOTOR_NORMAL_SPEED_LEVEL;
	SPEED_RIGHT_MOTOR = MOTOR_NORMAL_SPEED_LEVEL;
	motor_fb_control(0,1);
	motor_fb_control(1,1);
	if(motor_speed_mode == 0){		/* normal */
		systick_delay_us(2000);
	}
	else{
		systick_delay_us(3000);
	}
	SPEED_LEFT_MOTOR = 0;
	SPEED_RIGHT_MOTOR = 0;
	motor_fb_control(0,0);
	motor_fb_control(1,0);
}

void motor_spin_left(void)
{
	SPEED_LEFT_MOTOR = MOTOR_SPIN_SPEED_LEVEL;
	SPEED_RIGHT_MOTOR = MOTOR_SPIN_SPEED_LEVEL;
	motor_fb_control(0,1);
	motor_fb_control(1,0);
}

void motor_spin_right(void)
{
	SPEED_LEFT_MOTOR = MOTOR_SPIN_SPEED_LEVEL;
	SPEED_RIGHT_MOTOR = MOTOR_SPIN_SPEED_LEVEL;
	motor_fb_control(1,0);
	motor_fb_control(1,1);
}
	
	
	
	
	
	