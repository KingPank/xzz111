/**
******************************************************************************
    * @file     beep.h
    * @brief    The buzzer packaging function
    * @author   
    * @version  V1.1.0
    * @date     10-August-2017
    * @par      Modification:????,??********<br>
    *			History
    *           None
  *************************************************************************
  * @attention
*********************************************************
 */


/**
  * @brief Define to prevent recursive inclusion
 */
#ifndef __BEEP_H
#define __BEEP_H

 /**
  * @brief Head Files
  */
#include "stm32f4xx.h"

 /**
  * @brief Interface Function
  */
#ifndef   BeepReadBytes  
#define   BeepReadBytes(GPIOx,GPIO_Pin)        GPIO_ResetBits(GPIOx,GPIO_Pin)   /** Clears the selected data port bits.*/
#endif

#ifndef   BeepSetBits  
#define   BeepSetBits(GPIOx,GPIO_Pin)          GPIO_SetBits(GPIOx,GPIO_Pin)     /**Sets the selected data port bits.*/
#endif

#ifndef   BeepSetCompare1  
#define   BeepSetCompare1(TIMx,Compare1)       TIM_SetCompare1(TIMx,Compare1)   /** Sets the TIMx Capture Compare1 Register value*/
#endif
		
		
		

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
extern void BeepTimPwmInit(u32 arr,u32 psc,u16 led0pwmval) ;        /**The buzzer initializes,PWM wave controls the sound of the buzzer     arr:Automatic overload value  psc:Timer frequency division    led0pwmval:PWM duty cycle,Related to the timer cycle*/



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
extern void BeepOff(void);                                          /**The buzzer opens*/


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
extern void BeepOn(void);                                           /**The buzzer was off*/



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
extern void BeepSound(u16 led0pwmval);                              /**Control the sound of the buzzer      led0pwmval:PWM duty cycle,Related to the timer cycle*/




#endif

















