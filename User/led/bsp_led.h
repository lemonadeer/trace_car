#ifndef __LED_H
#define	__LED_H
#include "stm32f10x.h"

#define LED1_GPIO_PORT    	GPIOB			            
#define LED1_GPIO_CLK 	    RCC_APB2Periph_GPIOB		
#define LED1_GPIO_PIN			GPIO_Pin_13		      

#define LED2_GPIO_PORT    	GPIOB			            
#define LED2_GPIO_CLK 	    RCC_APB2Periph_GPIOB		
#define LED2_GPIO_PIN			GPIO_Pin_14			    

#define LED3_GPIO_PORT    	GPIOB			             
#define LED3_GPIO_CLK 	    RCC_APB2Periph_GPIOB		
#define LED3_GPIO_PIN			GPIO_Pin_15		       

#define	digitalHi(p,i)		 {p->BSRR=i;}	 	//输出为高电平		
#define digitalLo(p,i)		 {p->BRR=i;}		 	//输出低电平
#define digitalToggle(p,i) {p->ODR ^=i;} 	//输出反转状态

#define LED1_TOGGLE			 	digitalToggle(LED1_GPIO_PORT,LED1_GPIO_PIN)
#define LED1_OFF		  		 		digitalHi(LED1_GPIO_PORT,LED1_GPIO_PIN)
#define LED1_ON			 	  		digitalLo(LED1_GPIO_PORT,LED1_GPIO_PIN)

#define LED2_TOGGLE				digitalToggle(LED2_GPIO_PORT,LED2_GPIO_PIN)
#define LED2_OFF		   				digitalHi(LED2_GPIO_PORT,LED2_GPIO_PIN)
#define LED2_ON			   			digitalLo(LED2_GPIO_PORT,LED2_GPIO_PIN)

#define LED3_TOGGLE		 		digitalToggle(LED3_GPIO_PORT,LED3_GPIO_PIN)
#define LED3_OFF		   				digitalHi(LED3_GPIO_PORT,LED3_GPIO_PIN)
#define LED3_ON			   			digitalLo(LED3_GPIO_PORT,LED3_GPIO_PIN)

void led_init(void);

#endif /* __LED_H */
