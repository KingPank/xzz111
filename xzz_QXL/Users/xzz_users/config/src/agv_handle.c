/**
******************************************************************************
    * @file     agv_handle.h
    * @brief
    * @author
    * @version  V1.1.0
    * @date     10-August-2017
  *************************************************************************
  * @attention
*********************************************************
*/

 /**---Head Files---*//*"MTSProtocol.h""BCBCtrlIMU.h":Gyro data reception*/
#include "stm32f4xx_it.h"
#include<stdio.h>
#include<string.h>
#include "stm32f4xx_usart_config.h"
#include "agv_handle.h"
#include "MTSProtocol.h"
#include "BCBCtrlIMU.h"
#include "stdint.h"
#include "delay.h"
#include "BCBGLOBALTYPE.h"








/*---Macro Definition-----*/
#define	 RecvBz1   0x01;
#define	 RecvBz2   0x11;

/*---Variable Definition--*/
u8 AgvEnOne[10]={0X01,0X23,0X40,0X60,0X00,0X0F,0X00,0X00,0X00,0X2D};   /* AGV1 Enabled the command   */      
u8 AgvNoEnOne[10]={0X01,0X23,0X40,0X60,0X00,0X06,0X00,0X00,0X00,0X36};  /* AGV1 No Enabled the command  */       
u8 AgvEnTwo[10]={0X02,0X23,0X40,0X60,0X00,0X0F,0X00,0X00,0X00,0X2C};    /* AGV2 Enabled the command   */   
u8 AgvNoEnTwo[10]={0X02,0X23,0X40,0X60,0X00,0X06,0X00,0X00,0X00,0X35};   /* AGV2 No Enabled the command  */     
//u8 agv1_model[10]={0X01,0X23,0X60,0X60,0X00,0X03,0X00,0X00,0X00,0X19};
//u8 agv2_model[10]={0X02,0X23,0X60,0X60,0X00,0X03,0X00,0X00,0X00,0X18};
u8 SpeedSports[4]={0X00,0X00,0X00,0X00};
u8 AgvSpeedOne[10]={0X01,0X23,0XFF,0X60,0X00};                                
u8 AgvSpeedTwo[10]={0X02,0X23,0XFF,0X60,0X00};                                 
u8 AgvMonitoringOne[10]={0X01,0X40,0XF9,0X60,0X18,0X00,0X00,0X00,0X00,0X4E}; /* AGV1 monitoring the command*/
u8 AgvMonitoringTwo[10]={0X02,0X40,0XF9,0X60,0X18,0X00,0X00,0X00,0X00,0X4D}; /* AGV1 monitoring the command*/

u8 RxIndex=0;
u8 RxDataBz=0;
u8 Bz;
int16_t RxBufferSd;


u8 RxBufferAgvSd[15];                 /*   */
u8 RxBufferInstruct[10];
int sj=0;


/*---Function Definiton---*/
 int AgvEnToggle(int a);                                            /*  a=0 N0 Enabled ;a=1  Enabled()        */
 void AgvGiveSpeed(int number,int16_t AgvGiveSpeed);                  /* number is The drive shaft    AgvGiveSpeed is AGV speed */
 void AgvMonitoring(void);                                     /* AGV car speed feedback*/
/* External interface*/
int AgvRecvData(u8 *Data );             /*  data : The upper machine sends data        function : The upper machine sends data to the underlying layer */
void AgvSendData(u8 *DmaRxBuffer);                      /* The car data */



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


int AgvEnToggle(int a)                    
{
		static int j = 0;

		if( a == 0 )
		{
				if( j == 0 )
				{
						j=1;
				//	USART3_SendBytes( AgvNoEnOne,10);	 
						AgvSendBytesInfoProc(AgvNoEnOne,10);			
				}
				else
				{
						j=0;
					//USART3_SendBytes( AgvNoEnTwo,10);
						AgvSendBytesInfoProc(AgvNoEnTwo,10);
				}
				return 0;
		}
		else if( a == 1 )
		{
				if( j == 0 )
				{
						j=1;
					//	USART3_SendBytes( AgvEnOne,10);
						AgvSendBytesInfoProc(AgvEnOne,10);
				}
			  else
				{
						j=0;
				//	USART3_SendBytes( AgvEnTwo,10);
						AgvSendBytesInfoProc(AgvEnTwo,10);
					
				}
				return 0;
		}
		else
		{
			return -1;
		}
}



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

void AgvGiveSpeed(int number,int16_t agv_speed)
{
		int b=0,j=0,k=0,n=0;
		u16 sum=0;
		float_u speed;
		if( agv_speed >= 0 )
		{
				if( number == 1 )
				{
						speed.fdata = agv_speed*512*4096/1875;
						SpeedSports[0]=speed.udata[0];
						SpeedSports[1]=speed.udata[1];
						SpeedSports[2]=speed.udata[2];
						SpeedSports[3]=speed.udata[3];

						for( j = 5 , k = 0 ; j < 10 ; j++ , k++ )
						{
						    AgvSpeedOne[j]=SpeedSports[k];
						}
						for( n = 0 , sum = 0 ; n < 9 ; n++ )
						{
							  sum=sum + *(AgvSpeedOne+n);
						}
						AgvSpeedOne[9]=(-sum)&0x000000ff;
						AgvSendBytesInfoProc(AgvSpeedOne,10);		
						b=0,j=0,k=0,n=0,sum=0;
				}
				
				else if( number == 2 )
				{
						speed.fdata = agv_speed*512*4096/1875;
						SpeedSports[0]=speed.udata[0];
						SpeedSports[1]=speed.udata[1];
						SpeedSports[2]=speed.udata[2];
						SpeedSports[3]=speed.udata[3];
						for( j = 5 , k = 0 ; j < 10 ; j++ , k++ )
						{
								AgvSpeedTwo[j]=SpeedSports[k];
						}
						for( n = 0 ; n < 9 ; n++ )
						{
								sum=sum + *(AgvSpeedTwo+n);
						}
						AgvSpeedTwo[9]=(-sum)&0x000000ff;

						AgvSendBytesInfoProc(AgvSpeedTwo,10);		
				}
		}
		if( agv_speed < 0 )
			{		
					if( number == 1 )
						{
								speed.fdata = (-agv_speed)*512*4096/1875;
								SpeedSports[0]=speed.udata[0];
								SpeedSports[1]=speed.udata[1];
								SpeedSports[2]=speed.udata[2];
								SpeedSports[3]=0xff;

								for( j = 5 , k = 0 ; j < 10 ; j++ , k++ )
								{
								  	AgvSpeedOne[j]=SpeedSports[k];
								}
								for( n = 0 , sum = 0 ; n < 9 ; n++ )
								{
								  	sum=sum + *(AgvSpeedOne+n);
								}
								AgvSpeedOne[9]=(-sum)&0x000000ff;
								AgvSendBytesInfoProc(AgvSpeedOne,10);		
								b=0,j=0,k=0,n=0,sum=0;
						}
					else if( number == 2 )
						{
								speed.fdata = (-agv_speed)*512*4096/1875;
								SpeedSports[0]=speed.udata[0];
								SpeedSports[1]=speed.udata[1];
								SpeedSports[2]=speed.udata[2];
								SpeedSports[3]=0xff;
								for( j = 5 , k = 0 ; j < 10 ; j++ , k++ )
								{
								    AgvSpeedTwo[j]=SpeedSports[k];
								}
								for( n = 0 ; n < 9 ; n++ )
								{
								    sum=sum + *(AgvSpeedTwo+n);
								}
								AgvSpeedTwo[9]=(-sum)&0x000000ff;

								AgvSendBytesInfoProc(AgvSpeedTwo,10);		
						}
			}
}



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

void AgvMonitoring(void)
{

		static int i = 0;
		if( i == 0 )
		{
				i = 1;
				//USART3_SendBytes(AgvMonitoringOne,10);
				AgvSendBytesInfoProc(AgvMonitoringOne,10);
		
		}
		else
		{
				i = 0;
				//USART3_SendBytes(AgvMonitoringTwo,10);
				AgvSendBytesInfoProc(AgvMonitoringTwo,10);
			
		}
}



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

int AgvRecvData(u8 *data )
{
		int n;
		u8 sum;	
	  u8 RxBuffer[50];
		Bz=0;
		RxIndex=0;
		for( n = 0 ; n < 50 ; n++ )
		{
			  RxBuffer[n]=0;
		}	
		if( *data == 0xa5 && Bz == 0 )
		{
				Bz=RecvBz1;
				RxIndex++;
				if(*(data+1)==0x5a && Bz==0x01)
						{
								Bz=RecvBz2;
								RxIndex=0; 		
						}
				if( Bz == 0x11 )
					 {
							for( RxDataBz = 0 ; RxDataBz < 11 ; RxDataBz++ )
							 {
										RxBuffer[RxDataBz]=*(data+2+RxDataBz);
										RxIndex++;
							 }
				//				if(RxBuffer[0]==RxIndex)
				//				{		
				RxBufferSd=RxBuffer[5]|(RxBuffer[6]&0x00ff<<8);	
				if( RxBufferSd > 10000 )
				{
						RxBufferSd=RxBufferSd-65536;
				}
				for( n = 0 , sum = 0 ; n < RxIndex ; n++ )
				{
						if( n!=2 )
						{
								sum=+RxBuffer[n];
						}
				}			
				switch ( RxBuffer[3] )
				{
						case 0x01:AgvEnToggle(0);
							break;
						case 0x02:AgvEnToggle(1);
							break;
						case 0x03:AgvGiveSpeed(RxBuffer[2],RxBufferSd);
							break;
						case 0x04:
							break;
				}		
				return 0;	
				}
			
    }
		else
		{
			  return -1;
		}
	
}



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

void AgvSendData(u8 *DmaRxBuffer)
{
		u8 n;
		u16 sum;
		Angle_t olj;
		Gyro_t gyro;
		float_u Z_olj;

		if(*(DmaRxBuffer+1)!=0x80&&*(DmaRxBuffer+2)==0xf9&&*(DmaRxBuffer+3)==0x60&&*(DmaRxBuffer+4)==0x18)
		{	
				for( sj=0 ; sj < 10 ; sj++ )
				{
						RxBufferInstruct[sj]=*(DmaRxBuffer+sj);
				}

				RxBufferAgvSd[0]=0xa5;
				RxBufferAgvSd[1]=0x5a;
				RxBufferAgvSd[2]=0x0f;
				RxBufferAgvSd[3]=0;
				RxBufferAgvSd[5]=0x01;
				RxBufferAgvSd[6]= RxBufferInstruct[0];
				if(RxBufferInstruct[1]!=0x80&&RxBufferInstruct[2]==0xf9&&RxBufferInstruct[3]==0x60&&RxBufferInstruct[4]==0x18)
				{			 
						RxBufferAgvSd[7]= RxBufferInstruct[5];
						RxBufferAgvSd[8]= RxBufferInstruct[6];
						RxBufferAgvSd[9]= RxBufferInstruct[7];
						RxBufferAgvSd[10]= RxBufferInstruct[8];

						AgvImuGetYawPitchRoll(&olj,&gyro,0.007);                        
						Z_olj.fdata=olj.Yaw;
						RxBufferAgvSd[11]=Z_olj.udata[0];
						RxBufferAgvSd[12]=Z_olj.udata[1];
						RxBufferAgvSd[13]=Z_olj.udata[2];
						RxBufferAgvSd[14]=Z_olj.udata[3];
				for( n = 0 , sum = 0 ; n < 15 ; n++ )
				{
						if(n!=4)
						{	 
						    sum = sum +RxBufferAgvSd[n];
						}
				}
				RxBufferAgvSd[4]=sum&0x00ff;

				AgvU1SendBytesInfoProc(RxBufferAgvSd,15);

				}

		 }
		for( n = 0 ; n < 15 ; n++ )
		{
		     RxBufferAgvSd[n]=0;
		}

}


