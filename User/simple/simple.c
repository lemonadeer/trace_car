#include "simple.h" 
#include "global_config.h"
#include "bsp_usart.h"
#include "bsp_delay.h"
#include "bsp_pwm.h"
#include "global_config.h"
#include "bsp_delay.h"
#include "trace_control.h"
#include "bsp_led.h"
#include "motor.h"

extern __IO uint8_t left_spin_lock;
extern __IO uint8_t right_spin_lock;
extern __IO uint8_t nr_case;
extern __IO uint8_t simple_lock;
extern __IO uint8_t need_process;
extern uint8_t motor_speed_mode;

static GPIO_TypeDef *sensor_port_table[NUM_SENSOR] = {
	SENSOR3_GPIO_PORT,
	SENSOR2_GPIO_PORT,
	SENSOR1_GPIO_PORT,
	SENSOR0_GPIO_PORT
};

static uint16_t sensor_pin_table[NUM_SENSOR] = {
	SENSOR3_GPIO_PIN,
	SENSOR2_GPIO_PIN,
	SENSOR1_GPIO_PIN,
	SENSOR0_GPIO_PIN,
};

static void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = SENSOR4_EXTI_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
  
    NVIC_InitStructure.NVIC_IRQChannel = SENSOR5_EXTI_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_Init(&NVIC_InitStructure);
}
static void exti_sensor_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;

	RCC_APB2PeriphClockCmd(SENSOR5_GPIO_CLK | SENSOR4_GPIO_CLK,ENABLE);										
	NVIC_Configuration();
	/* SENSOR 4 */
	GPIO_InitStructure.GPIO_Pin = SENSOR4_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SENSOR4_GPIO_PORT, &GPIO_InitStructure);

	GPIO_EXTILineConfig(SENSOR4_EXTI_PORTSOURCE, SENSOR4_EXTI_PINSOURCE); 
	EXTI_InitStructure.EXTI_Line = SENSOR4_EXTI_LINE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	/* SENSOR 5 */
	GPIO_InitStructure.GPIO_Pin = SENSOR5_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(SENSOR5_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_EXTILineConfig(SENSOR5_EXTI_PORTSOURCE, SENSOR5_EXTI_PINSOURCE); 
	EXTI_InitStructure.EXTI_Line = SENSOR5_EXTI_LINE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}

void sensor_init(void)
{
	uint8_t i;
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(SENSOR0_GPIO_CLK|SENSOR1_GPIO_CLK\
	|SENSOR2_GPIO_CLK|SENSOR3_GPIO_CLK,ENABLE);
	/* sensor gpio config */
	for(i = 0; i < NUM_SENSOR; i ++){
		GPIO_InitStructure.GPIO_Pin = sensor_pin_table[i]; 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
		GPIO_Init(sensor_port_table[i], &GPIO_InitStructure);
	}
	
	/* interrupt sensor config */
	exti_sensor_config();
}

static void process_startline(void)
{
	spin_timer_reload = 2 * TIMER_Period;	//500ms
	left_spin_lock = 1;
	right_spin_lock = 1;
	need_process = 0;
}

static void process_endturn(void)
{
	spin_timer_reload = 2 * TIMER_Period;	//500ms
	left_spin_lock = 1;
	right_spin_lock = 1;
	need_process = 0;
}

static void process_endline_ignore(void)
{
	spin_timer_reload = 2 * TIMER_Period;	//500ms
	left_spin_lock = 1;
	right_spin_lock = 1;
	need_process = 0;
}

static void process_island_enter(void)
{
	simple_lock = 1;
	need_process = 0;
}

static void process_island_away(void)
{
	simple_lock = 0;
	need_process = 0;
}

static void process_trap(void)
{
	if(motor_speed_mode == 0){		/* normal */
		systick_delay_us(150000);
	}
	else if(motor_speed_mode == 1){
		systick_delay_us(100000);
	}

	motor_spin_left();
	while(SENSOR1_BIT || SENSOR2_BIT);
	trace_spin_left();
	need_process = 0;
}

static void process_endline(void)
{
	spin_timer_reload = 2 * TIMER_Period;	//500ms
	left_spin_lock = 1;
	right_spin_lock = 1;
	need_process = 0;
}

void process_int(void)
{
	switch (nr_case){	
		case	start :																	break;
		case startline:					process_startline();				break;
		case endturn:					process_endturn();				break;
		case endline_ignore:		process_endline_ignore();	break;
		case island_enter:			process_island_enter();		break;
		case island_away:			process_island_away();		break;
		case trap:							process_trap();						break;
		case case8_enter:			need_process = 0	;				break;
		case	case8_turn_enter:	need_process = 0;				break;
		case case8_turn_away:	need_process = 0;				break;
		case case8_away:			need_process = 0;				break;
		case before_end:				need_process = 0;				break;
		case endline	:					process_endline();				break;
		default: break;
	}
}

uint8_t sensor_simple(void)
{
	__IO uint8_t sensor_0 = 0;
	__IO uint8_t sensor_1 = 0;
	__IO uint8_t sensor_2 = 0;
	__IO uint8_t sensor_3 = 0;
	
	sensor_0 = SENSOR0_BIT;
	sensor_1 = SENSOR1_BIT;
	sensor_2 = SENSOR2_BIT;
	sensor_3 = SENSOR3_BIT;
	
	if( ! left_spin_lock){		/* unlocked */
		if(sensor_0){
			return STATUS_TURN_LEFT;
		}
	}
	if( ! right_spin_lock){		/* unlocked */
		if(sensor_3){
			return STATUS_TURN_RIGHT;
		}
	}

	if((sensor_1 == 0) || (sensor_2 == 0)){	/* adjust */
		if(sensor_1){
			return STATUS_ADJUST_TO_LEFT;
		}
		else{
			return STATUS_ADJUST_TO_RIGHT;
		}
	}
	return STATUS_NORMOL;
}

