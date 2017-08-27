/**
******************************************************************************
    * @file     beep.h
    * @brief
    * @author
    * @version  V1.1.0
    * @date     10-August-2017
  *************************************************************************
  * @attention
*********************************************************
*/

 /**---Head Files---*/
	#include <string.h>
	#include <stdio.h>
	#include <stdarg.h> 
	#include "beep.h"
	#include "stm32f4xx.h"



 /*---Macro Definition-----*/
 /**     None              */
 
 
 
 /*---Variable Definition--*/
 /**     None              */
	
	
	
/*---Function Definiton---*/
	void BeepTimPwmInit(u32 arr,u32 psc,u16 led0pwmval) ;
	void BeepOff(void);
	void BeepOn(void);
	void BeepSound(u16 led0pwmval);



/**
********************************************************************
  * @brief    Passive buzzer adopts PWM to control sound.
  * @param    GPIOx: where x can be (A..K) to select the GPIO peripheral for STM32F4xx devices.
	            GPIO_Pin: specifies the port bits to be written.
  *                     This parameter can be any combination of GPIO_Pin_x where x can be (0..15).
  * @param    TIMx: where x can be (2..14)to select the GPIO peripheral for STM32F4xx devices.
	            TIM_OCMode_PWMx:where x can be (1 to 2)to select the GPIO peripheral for STM32F4xx devices.
  * @retval   None.
  * @par      Modification:???********<br>
	            
  *			      History
  *           None.
********************************************************************
*/
void BeepTimPwmInit(u32 arr,u32 psc,u16 led0pwmval)  
{   
		GPIO_InitTypeDef   GPIO_InitStructure;
		GPIO_InitTypeDef   GPIO_Pwm_InitStructure;
	
		TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
		TIM_OCInitTypeDef   TIM_OCInitStructure;

		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC , ENABLE);        /*Enable the GPIOC clock*/
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 ,  ENABLE);  	    /*Make the TIM3 clock*/ 

		GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM3);         /*GPIOC reuse bit TIM3*/

		GPIO_Pwm_InitStructure.GPIO_Pin = GPIO_Pin_6;                 /*GPIOF6*/
		GPIO_Pwm_InitStructure.GPIO_Mode = GPIO_Mode_AF;        
		GPIO_Pwm_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	      /*100MHz*/
		GPIO_Pwm_InitStructure.GPIO_OType = GPIO_OType_PP;      
		GPIO_Pwm_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        
		GPIO_Init(GPIOC,&GPIO_Pwm_InitStructure);             
			 
		TIM_TimeBaseStructure.TIM_Prescaler=psc;                      /*Timer frequency division:psc*/
		TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
		TIM_TimeBaseStructure.TIM_Period=arr;                         /*Automatic overload value:arr*/
		TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
		TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);

		/*Initialize the TIM3 Channel1 PWM*/	 
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;            
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;       
		TIM_OC1Init(TIM3, &TIM_OCInitStructure);                    

		TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);             /*Enable TIM3 preloaded registers on CCR1*/

		TIM_ARRPreloadConfig(TIM3,ENABLE);                            /*ARPE Enable*/

		TIM_Cmd(TIM3, ENABLE);                                        /*Enable TIM3*/

		BeepSetCompare1(TIM3,led0pwmval);                             /*Modify the PWM to account for the void ratio*/
}



/**
********************************************************************
  * @brief    The buzzer closes the function
  * @param    GPIOx: where x can be (A..K) to select the GPIO peripheral for STM32F4xx devices.
	            GPIO_Pin: specifies the port bits to be written.
  *                     This parameter can be any combination of GPIO_Pin_x where x can be (0..15).
  * @param    TIMx: where x can be (2..14)to select the GPIO peripheral for STM32F4xx devices.
	          
  * @retval   None.
  * @par      Modification:???********<br>
	            
  *			      History
  *           None.
********************************************************************
*/
void BeepOff(void)
{
		BeepReadBytes(GPIOC,GPIO_Pin_6);
		BeepSetCompare1(TIM3,0);     
		TIM_Cmd(TIM3, DISABLE);

}



/**
********************************************************************
  * @brief    The buzzer opens the function
  * @param    GPIOx: where x can be (A..K) to select the GPIO peripheral for STM32F4xx devices.
	            GPIO_Pin: specifies the port bits to be written.
  *                     This parameter can be any combination of GPIO_Pin_x where x can be (0..15).
  * @param    TIMx: where x can be (2..14)to select the GPIO peripheral for STM32F4xx devices.
	          
  * @retval   None.
  * @par      Modification:???********<br>
	            
  *			      History
  *           None.
********************************************************************
*/
void BeepOn(void)
{
		BeepSetBits(GPIOC,GPIO_Pin_6);
		BeepSetCompare1(TIM3,200);
		TIM_Cmd(TIM3, ENABLE);
}

/**
********************************************************************
  * @brief    Change the sound of the buzzer through PWM
  * @param    TIMx: where x can be (2..14)to select the GPIO peripheral for STM32F4xx devices.
  * @param    none.
	          
  * @retval   None.
  * @par      Modification:???********<br>
	            
  *			      History
  *           None.
********************************************************************
*/
void BeepSound(u16 led0pwmval)
{
		BeepSetCompare1(TIM3,led0pwmval);  
}
