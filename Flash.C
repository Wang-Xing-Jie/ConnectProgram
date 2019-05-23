#include "Flash.h"
#include "congfig.h"

void IAPClose()
{
 	IAP_CONTR=0X00;    //关闭IAP功能
	IAP_CMD=0X00;	   //清除命令寄存器
	IAP_TRIG=0X00;
	IAP_ADDRH=0X80;    //设置为非IAP地址
	IAP_ADDRL=0X00;
}

uchar IAPReadByte(uchar addr)
{
	uchar dat;                           //数据缓冲区
	IAP_CONTR = 0x83;            		//使能IAP
	IAP_CMD = 	0x01;                //设置IAP命令
	IAP_ADDRL = addr;                  //设置IAP低地址
	IAP_ADDRH = addr >> 8;             //设置IAP高地址
	IAP_TRIG = 0x5a;                    //写发命令(0x5a)
	IAP_TRIG = 0xa5;                    //写发命令(0xa5)
	delayms(1);                         //等待ISP/IAP/EEPROM操作完成
	dat = IAP_DATA;                     //读ISP/IAP/EEPROM数据
	IAPClose();                          //关闭IAP功能
	return dat;                       
										
}

void IAPWriteByte(uchar addr, uchar dat)
{
	IAP_CONTR = 0x83;            //使能IAP
	IAP_CMD = 0x02;            	//设置IAP命令
	IAP_ADDRL = addr;                  //设置IAP低地址
	IAP_ADDRH = addr >> 8;             //设置IAP高地址
	IAP_DATA = dat;                     //写ISP/IAP/EEPROM数据
	IAP_TRIG = 0x5a;                    //写?发命令(0x5a)
	IAP_TRIG = 0xa5;                    //写?发命令(0xa5)
	delayms(1);                            //等待ISP/IAP/EEPROM操作完成
	IAPClose();
}

void IAPErase(uchar addr)
{
	IAP_CONTR = 0x83;            //使能IAP
	IAP_CMD = 0x03;            	//设置IAP命令	
	IAP_ADDRL = addr;                  //设置IAP低地址
	IAP_ADDRH = addr >> 8;             //设置IAP高地址
	IAP_TRIG = 0x5a;                    //写?发命令(0x5a)
	IAP_TRIG = 0xa5;                    //写?发命令(0xa5)	
	delayms(1);                            //等待ISP/IAP/EEPROM操作完成
	IAPClose();
} 

