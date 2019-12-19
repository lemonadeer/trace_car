/* PA6 		left motors pwm control
 * PA7 		right motors pwm control
 * PA9 		UART TX
 * PA10 	UART RX
 * PA0		sensor 0
 * PA1		sensor 1 
 * PA2		sensor 2
 * PA3		sensor 3
 * 
 * 
 *
 *
 *
 *
 *
 */

#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "motor.h"
#include "simple.h"
#include "trace_control.h"
#include "bsp_delay.h"
#include "global_config.h"

/* 0 mean unlocked */
/* before use spin check the lock status,and after using lock it */
__IO uint8_t left_spin_lock = 0;
__IO uint8_t right_spin_lock = 0;
__IO uint8_t simple_lock = 0;
__IO uint8_t case_timer_lock = 0;
__IO uint8_t nr_case = 0;			
__IO uint8_t need_process = 0;
__IO uint8_t flag_spcecial = 0;

uint8_t MOTOR_NORMAL_SPEED_LEVEL;
uint8_t MOTOR_SPIN_SPEED_LEVEL;
uint8_t MOTOR_SLOW_SPEED_LEVEL;

int main(void)
{	
	uint8_t status = 0;
	nr_case = 0;
	//USART_Config();
	
	led_init();
	motor_init();
	sensor_init();
	timer_Init();
	
	motor_forward();
	systick_delay_us(10000);
			
	while (1){
		if(need_process){
			process_int();
		}
		
		if(! simple_lock){
			status = sensor_simple();
			switch(status){
				case STATUS_NORMOL:						trace_normal();				break;
				case STATUS_ADJUST_TO_LEFT:		trace_adjust_left();		break;
				case STATUS_ADJUST_TO_RIGHT:	trace_adjust_right();		break;
				case STATUS_TURN_LEFT:					trace_spin_left();			break;
				case STATUS_TURN_RIGHT:				trace_spin_right();		break;
			}
		}
		else{			/* when simple_lock is set */
			SPEED_LEFT_MOTOR = MOTOR_NORMAL_SPEED_LEVEL;
			SPEED_RIGHT_MOTOR = MOTOR_NORMAL_SPEED_LEVEL;
		}
	}
}




