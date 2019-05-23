#include "Flash.h"
#include "congfig.h"

void IAPClose()
{
 	IAP_CONTR=0X00;    //�ر�IAP����
	IAP_CMD=0X00;	   //�������Ĵ���
	IAP_TRIG=0X00;
	IAP_ADDRH=0X80;    //����Ϊ��IAP��ַ
	IAP_ADDRL=0X00;
}

uchar IAPReadByte(uchar addr)
{
	uchar dat;                           //���ݻ�����
	IAP_CONTR = 0x83;            		//ʹ��IAP
	IAP_CMD = 	0x01;                //����IAP����
	IAP_ADDRL = addr;                  //����IAP�͵�ַ
	IAP_ADDRH = addr >> 8;             //����IAP�ߵ�ַ
	IAP_TRIG = 0x5a;                    //д������(0x5a)
	IAP_TRIG = 0xa5;                    //д������(0xa5)
	delayms(1);                         //�ȴ�ISP/IAP/EEPROM�������
	dat = IAP_DATA;                     //��ISP/IAP/EEPROM����
	IAPClose();                          //�ر�IAP����
	return dat;                       
										
}

void IAPWriteByte(uchar addr, uchar dat)
{
	IAP_CONTR = 0x83;            //ʹ��IAP
	IAP_CMD = 0x02;            	//����IAP����
	IAP_ADDRL = addr;                  //����IAP�͵�ַ
	IAP_ADDRH = addr >> 8;             //����IAP�ߵ�ַ
	IAP_DATA = dat;                     //дISP/IAP/EEPROM����
	IAP_TRIG = 0x5a;                    //д?������(0x5a)
	IAP_TRIG = 0xa5;                    //д?������(0xa5)
	delayms(1);                            //�ȴ�ISP/IAP/EEPROM�������
	IAPClose();
}

void IAPErase(uchar addr)
{
	IAP_CONTR = 0x83;            //ʹ��IAP
	IAP_CMD = 0x03;            	//����IAP����	
	IAP_ADDRL = addr;                  //����IAP�͵�ַ
	IAP_ADDRH = addr >> 8;             //����IAP�ߵ�ַ
	IAP_TRIG = 0x5a;                    //д?������(0x5a)
	IAP_TRIG = 0xa5;                    //д?������(0xa5)	
	delayms(1);                            //�ȴ�ISP/IAP/EEPROM�������
	IAPClose();
} 
