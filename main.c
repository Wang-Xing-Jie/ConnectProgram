/*********************************************************************
project Name:			 GPS程序
			
Author   :	
Hardware Connection:     基于STC15F2K60S2		  
						 @11.0592MHz
						 GPS模块

Software Connection: 	 串口1  默认波特率	9600


Description:	时间/经纬度/海拔	
							EMEA0183 

Data:    16.6.22		
version :v1				
**********************************************************************/

#include "STC15F2K60S2.h"
#include<stdio.h>
#include "gps.h"
#include "usart.h"
#include "main.h"
#include "delay.h"
#define uchar unsigned char	   
#define uint  unsigned int
unsigned char  Display_GPGGA_Buffer[68];	     
unsigned char  Display_GPRMC_Buffer[68];
bit Flag_OV = 0;
uchar Flag_Calc_GPGGA_OK = 0;
uchar Flag_Calc_GPRMC_OK = 0;
uchar  xdata buf[200];	  		  	  //sprintf函数数据缓存
uchar SecData;
uchar MinData;			
int cirsx=100,cirsy=100;
int TimeCount=9000;	
#define LATITUDE_START  5348616
#define LONGITUDE_START 4001816
#define LATITUDE_END  	5266733    //5201733  
#define LONGITUDE_END 	4062350
#define LATITUDE_INTER  (LATITUDE_START-LATITUDE_END)/(320-1) 
#define LONGITUDE_INTER (LONGITUDE_END-LONGITUDE_START)/(240-1)

	long LatitudeTemp,LongitudeTemp;
	long LatitudeStart,LongitudeStart,LatitudeInter,LongitudeInter;
	int iLatitudeTemp,iLongitudeTemp;
/*******************************************************
Function Name:主函数
Author       :
Description  : 
Param        :
Return Code  :	
********************************************************/
void main()
{	
	Gps_Uart_Init();		   				//串口初始化
	USART2_Init();					 //液晶
	DisplayLCD("");	
	while(1)
	{ 
		GPS_Process();	
 		delay_1ms(1);
		TimeCount++;
		if(TimeCount>9000)TimeCount=0;
		if(TimeCount==0)
		{	
			sprintf(buf,"cirs %d,%d,10,WHITE",cirsx,cirsy);	//cir显示
			DisplayLCD(buf); 
			sprintf(buf,"cirs %d,%d,4,BLUE",cirsx,cirsy);	//cir显示
			DisplayLCD(buf);
		}
		else if(TimeCount==2000)
		{
			sprintf(buf,"cirs %d,%d,5,BLUE",cirsx,cirsy);	//cir显示
			DisplayLCD(buf);
		} 
		else if(TimeCount==4000)
		{
			sprintf(buf,"cirs %d,%d,6,BLUE",cirsx,cirsy);	//cir显示
			DisplayLCD(buf);
		}		 		
	}											 			 			
}

/************************************ *******************
Function Name:GPS数据处理
Author       :
Description  : 
Param        :
Return Code  :	
********************************************************/
void GPS_Process()
{
	unsigned char i = 0;
	if(Flag_GPS_OK ==1&& RX_Buffer[4] == 'G' && RX_Buffer[6] == ',' && RX_Buffer[13] == '.')		
	{
		for( i = 0; i < 68 ; i++)
		{
			Display_GPGGA_Buffer[i] = RX_Buffer[i];	
		}
 		LatitudeTemp= 	 (Display_GPGGA_Buffer[19]-0x30)*1000000   	
						+(Display_GPGGA_Buffer[20]-0x30)*100000
						+(Display_GPGGA_Buffer[22]-0x30)*10000
				   		+(Display_GPGGA_Buffer[23]-0x30)*1000
				   		+(Display_GPGGA_Buffer[24]-0x30)*100
				   		+(Display_GPGGA_Buffer[25]-0x30)*10
				   		+(Display_GPGGA_Buffer[26]-0x30);
		if((Display_GPGGA_Buffer[20]-0x30)==2)  //52'
		{
			LatitudeTemp=LatitudeTemp+65000;  //39*100000/60;
		}	
		LongitudeTemp=	 (Display_GPGGA_Buffer[33]-0x30)*1000000   	
						+(Display_GPGGA_Buffer[34]-0x30)*100000
						+(Display_GPGGA_Buffer[36]-0x30)*10000
				   		+(Display_GPGGA_Buffer[37]-0x30)*1000
				   		+(Display_GPGGA_Buffer[38]-0x30)*100
				   		+(Display_GPGGA_Buffer[39]-0x30)*10
				   		+(Display_GPGGA_Buffer[40]-0x30);	

		LatitudeStart= LATITUDE_START;
		LongitudeStart=LONGITUDE_START;
		LatitudeInter= LATITUDE_INTER;
		LongitudeInter= LONGITUDE_INTER;
						
		iLatitudeTemp=(int)((LatitudeStart-LatitudeTemp)/LatitudeInter);	
		iLongitudeTemp=(int)((LongitudeTemp-LongitudeStart)/LongitudeInter);

		if(iLatitudeTemp<0)iLatitudeTemp=0;
		if(iLongitudeTemp<0)iLongitudeTemp=0;
		if(iLatitudeTemp>320-1)iLatitudeTemp=320-1;
		if(iLongitudeTemp>240-1)iLongitudeTemp=240-1;
		if((iLatitudeTemp!=cirsy)|| (iLongitudeTemp!=cirsx))    //??x or y??
		{
		  	cirsy=iLatitudeTemp;
			cirsx=iLongitudeTemp;
			sprintf(buf,"picq 0,0,240,320,0");	//切图
			DisplayLCD(buf);
			TimeCount=9000;					
		}	
		sprintf(buf,"t1.txt=\"%d%d″\"",(int)(LongitudeTemp/10000),(int)(LongitudeTemp%10000));	  	        //经度显示
		DisplayLCD(buf);
		sprintf(buf,"t2.txt=\"%d%d″\"",(int)(LatitudeTemp/10000),(int)(LatitudeTemp%10000));	  	      //纬度显示 
		DisplayLCD(buf);
																		
		if( Hour >= 24)				
		{
			Hour %= 24;				
			Flag_OV = 1;			//满时间标志位
		}
		else
		{
			Flag_OV = 0;
		}
		Flag_Calc_GPGGA_OK = 1;
	}
	
	if( Flag_Calc_GPGGA_OK == 1)
	{																													 
		Flag_Calc_GPGGA_OK = 0;			 				
	}		
	if(Flag_GPS_OK == 1 && RX_Buffer[4] == 'M' && RX_Buffer[52] == ',' && RX_Buffer[59] == ',')			//è·?¨ê?·?ê?μ?"GPRMC"?aò???êy?Y
	{
		for( i = 0; i < 68 ; i++)
		{
			Display_GPRMC_Buffer[i] = RX_Buffer[i];	
		}
		Year_High = Display_GPRMC_Buffer[57];
		Year_Low = Display_GPRMC_Buffer[58];

		Month_High = Display_GPRMC_Buffer[55];
		Month_Low = Display_GPRMC_Buffer[56];

		Day_High = Display_GPRMC_Buffer[53];
		Day_Low = Display_GPRMC_Buffer[54];


		if(Flag_OV == 1)			
		{
			UTCDate2LocalDate();			
		}

		Flag_Calc_GPRMC_OK = 1;
	}

	if(Flag_Calc_GPRMC_OK == 1)
	{

		Flag_Calc_GPRMC_OK = 0;				
	}		
}








 
