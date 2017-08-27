/**
******************************************************************************
    * @file       agv_handle.h
    * @brief      function                         Connect the upper machine and the AGV trolley  
									Upper machine control            AGV:	Enabled	 	Speed   	
									AGV  feedback                    The current speed
									AGV The data format              0X00 0X01 0X02 0X03 0X04 0X05 0X06 0X07 0X08 0X09    
									0X00:	Stand no. ;   0x01:Read(0x40)/write(0x23) command ; 		0x02~0x04:	address ;   0x05~0x08:data ;    0x09: ACK=-Sum(0x00+0x01+...0x08);
									
									communication                    Usart data Read/write---DMA       
									The data package                 Timer interrupt call
									Upper computer data format			 0xa5 0x5a 0x(Low byte length) 0x(High byte length) 0x(SUMCRC) 0x(channel) 0x(Control word) 0x(data0) 0x(data1)...0x(dataN);
        				
    * @author   
    * @version    V1.1.0
    * @date       10-August-2017
    * @par        Modification:????,??********<br>
    *			   History
    *           None
  *************************************************************************
  * @attention
*********************************************************
 */



/**
 * @brief Define to prevent recursive inclusion
 */
#ifndef __AGV_HANDLE_H__
#define __AGV_HANDLE_H__


/**
 * @brief Head Files
          BCBGlobalType.h:Data type conversion
					stm32f4xx_usart_config.h":serial communication,Send receive registers that require DMA interrupts
 */
#include<stdio.h>
#include<string.h>
#include "stm32f4xx_usart_config.h"
#include  "BCBGlobalType.h"
//#include "stm32f4xx_it.h"


 /**
  * @brief Interface Function
  */
#ifndef   AgvSendBytesInfoProc  
#define   AgvSendBytesInfoProc(pSendInfo,nSendCount)        SendBytesInfoProc(pSendInfo,nSendCount)     /** SendBytesInfoProc is serial communication DMA send function to AGV data      IN: "stm32f4xx_usart_config.h" */
#endif

#ifndef  AgvImuGetYawPitchRoll  
#define  AgvImuGetYawPitchRoll(angle,gyro,period)           IMU_getYawPitchRoll(angle,gyro,period)       /** The data of the gyroscope*/
#endif	

#ifndef   AgvU1SendBytesInfoProc 
#define   AgvU1SendBytesInfoProc(pSendInfo,nSendCount)      u1_SendBytesInfoProc(pSendInfo,nSendCount)   /**  u1_SendBytesInfoProc is serial communication DMA send function to upper machine      IN: "stm32f4xx_usart_config.h" */
#endif



/* Program internal call*/
/**
********************************************************************
  * @brief    The AGV car makes it switch.  AgvSendBytesInfoProc is serial communication DMA send function      IN: "stm32f4xx_usart_config.h"
  * @param    a=0:The AGV car doesn't make it;  a=1:AGV car makes energy;
	            
	* @param    AgvSendBytesInfoProc:
	
  * @retval   return 0:No Enabled ;  return 1:Enabled  return -1:Command error.
  * @par      Modification:???********<br>
	            
  *			      History
  *           None.
********************************************************************
*/
extern  int AgvEnToggle(int a);                                          /**  a=0 N0 Enabled ;a=1  Enabled()        */



/**
********************************************************************
  * @brief    The AGV car makes it switch. AGV The data format     0X00 0X01 0X02 0X03 0X04 0X05 0X06 0X07 0X08 0X09    
				                                                           0X00:	Stand no. ;   0x01:Read(0x40)/write(0x23) command ; 		0x02~0x04:	address ;   0x05~0x08:data ;    0x09: ACK=-Sum(0x00+0x01+...0x08);
  * @param   number£ºThe drive shaft1,2; agv_speed:Trolley drive speed.(The low post is behind the previous high,Negative high position FF)
	            
	* @param    AgvSendBytesInfoProc:
	
  * @retval   None.
  * @par      Modification:???********<br>
	            
  *			      History
  *           None.
********************************************************************
*/
extern  void AgvGiveSpeed(int number,int16_t AgvGiveSpeed);              /** number is The drive shaft    AgvGiveSpeed is AGV speed */



/**
********************************************************************
  * @brief    Detect the current speed of the AGV car
  * @param   none.
	            
	* @param    AgvSendBytesInfoProc:
	
  * @retval  none.
  * @par      Modification:???********<br>
	            
  *			      History
  *           None.
********************************************************************
*/
extern  void AgvMonitoring(void);                                        /** AGV car speed feedback*/



/**
********************************************************************
  * @brief    The upper machine sends data to the underlying layer
  * @param    data : The upper machine sends data     
	            
	* @param    RxBuffer : Transit register,Easy to call  AgvSendBytesInfoProc:
	
  * @retval   return 0: correct; return -1:Command error.
  * @par      Modification:???********<br>
	            
  *			      History
  *           None.
********************************************************************
*/

extern int AgvRecvData(u8 *Data );             /**  data : The upper machine sends data        function : The upper machine sends data to the underlying layer */


/**
********************************************************************
  * @brief    The AGV uploader sends data.
  * @param    DmaRxBuffer: The upper machine receives the data 
	            
  * @param    olj,gyro,Z_olj:£gGyro data.
	
  * @retval   return 0: correct; return -1:Command error.
  * @par      Modification:???********<br>
	            
  *			      History
  *           None.
********************************************************************
*/

extern void AgvSendData(u8 *DmaRxBuffer);      /** The car data */







#endif


