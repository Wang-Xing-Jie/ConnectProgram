/**********************************************************************
****** RTC时钟部分部分************************************************
****** 作者：XXX ******************************************************
****** 时间：2015.12.10***********************************************/

#include "STC15F2K60S2.h"                //包含头文件
#include "intrins.h"
#include "delay.h"
#include "DS1302.h"
#include "IIC_Oled.h"
sbit SCLK=P1^0;   //位定义1302芯片的接口，时钟输出端口定义在P1.0引脚
sbit DATA=P1^1;   //位定义1302芯片的接口，数据输出端定义在P1.1引脚
sbit RST=P1^2;    //位定义1302芯片的接口，复位端口定义在P1.2引脚 
uchar ReadValue;   //储存从1302读取的数据


/*****************************************************
函数功能：向1302写一个字节数据
入口参数：x
***************************************************/ 
void Write1302(unsigned char dat)
{
  unsigned char i; 
  SCLK=0;            //拉低SCLK，为脉冲上升沿写入数据做好准备
  delaynus(2);       //稍微等待，使硬件做好准备
  for(i=0;i<8;i++)      //连续写8个二进制位数据
    {
	     DATA=dat&0x01;    //取出dat的第0位数据写入1302  低位在前，高位在后
		 delaynus(2);       //稍微等待，使硬件做好准备
		 SCLK=1;           //上升沿写入数据
		 delaynus(2);      //稍微等待，使硬件做好准备
		 SCLK=0;           //重新拉低SCLK，形成脉冲
		 dat>>=1;          //将dat的各数据位右移1位，准备写入下一个数据位
	  }
}


/*****************************************************
函数功能：根据命令字，向1302写一个字节数据
入口参数：Cmd，储存命令字；dat，储存待写的数据
***************************************************/ 
void WriteSet1302(unsigned char Cmd,unsigned char dat)
 {	  
        RST=0;           //禁止数据传递
        SCLK=0;          //确保写数居前SCLK被拉低
	    RST=1;           //启动数据传输
	    delaynus(2);     //稍微等待，使硬件做好准备
	    Write1302(Cmd);  //写入命令字
	    Write1302(dat);  //写数据
	    SCLK=1;          //将时钟电平置于高电平状态
	    RST=0;           //禁止数据传递
 }

/*****************************************************
函数功能：从1302读一个字节数据
入口参数：x
***************************************************/ 
 unsigned char Read1302(void)
 {
    unsigned char i,dat;
	delaynus(2);       //稍微等待，使硬件做好准备
	for(i=0;i<8;i++)   //连续读8个二进制位数据
	 {	 dat>>=1;
		 if(DATA==1)    //如果读出的数据是1
		 dat|=0x80;    //将1取出，写在dat的最高位
		 SCLK=1;       //将SCLK置于高电平，为下降沿读出
		 delaynus(2);  //稍微等待
		 SCLK=0;       //拉低SCLK，形成脉冲下降沿
		 delaynus(2);  //稍微等待
	  }	 
  return dat;        //将读出的数据返回
}

/*****************************************************
函数功能：根据命令字，从1302读取一个字节数据
入口参数：Cmd
***************************************************/ 
unsigned char  ReadSet1302(unsigned char Cmd)
 {
  unsigned char dat;
  RST=0;                 //拉低RST
  SCLK=0;                //确保写数居前SCLK被拉低
  RST=1;                 //启动数据传输
  Write1302(Cmd);       //写入命令字
  dat=Read1302();       //读出数据
  SCLK=1;              //将时钟电平置于已知状态
  RST=0;               //禁止数据传递
  return dat;          //将读出的数据返回
}


/*****************************************************
函数功能： 1302进行初始化设置
***************************************************/ 
void Init_DS1302(void)
{	
  	unsigned char flag; 

    flag= ReadSet1302(0x81);
	if(flag&0x80)
	{      //判断时钟芯片是否关闭
    	WriteSet1302(0x8E,0x00);                 //根据写状态寄存器命令字，写入不保护指令 
   		WriteSet1302(0x80,((0/10)<<4|(0%10)));   //根据写秒寄存器命令字，写入秒的初始值
		WriteSet1302(0x82,((5/10)<<4|(5%10)));   //根据写分寄存器命令字，写入分的初始值
		WriteSet1302(0x84,((8/10)<<4|(8%10))); //根据写小时寄存器命令字，写入小时的初始值
		WriteSet1302(0x86,((20/10)<<4|(20%10))); //根据写日寄存器命令字，写入日的初始值
		WriteSet1302(0x88,((4/10)<<4|(4%10))); //根据写月寄存器命令字，写入月的初始值
		WriteSet1302(0x8c,((16/10)<<4|(16%10)));  //根据写年寄存器命令字，写入年的初始值
		WriteSet1302(0x90,0xa5);                //打开充电功能 选择2K电阻充电方式
		WriteSet1302(0x8E,0x80);			   //根据写状态寄存器命令字，写入保护指令
  	}
 }


/**************************************************************
以下是1302数据的显示程序
**************************************************************/
/*****************************************************
函数功能：显示秒
入口参数：x
***************************************************/ 
void DisplaySecond(unsigned char x)
{
 	unsigned char i,j;     //j,k分别储存十位和个位
	i=x/10;//取十位
	j=x%10;//取个位     
	LCD_Dspaly_8x16_Asicc(88,2,i+16);
	delay_1ms(1);        //延时1ms给硬件一点反应时间 
	LCD_Dspaly_8x16_Asicc(96,2,j+16);
	delay_1ms(1);      //延时1ms给硬件一点反应时间    
 }

 /*****************************************************
函数功能：显示分钟
入口参数：x
***************************************************/ 
void DisplayMinute(unsigned char x)
{
 	unsigned char i,j;     //j,k十位和个位
	i=x/10;//取十位
	j=x%10;//取个位     
	LCD_Dspaly_8x16_Asicc(64,2,i+16);
	delay_1ms(1);        //延时1ms给硬件一点反应时间 
	LCD_Dspaly_8x16_Asicc(72,2,j+16);
	delay_1ms(1);        //延时1ms给硬件一点反应时间 
	delaynms(50);         //延时1ms给硬件一点反应时间    
 }
/*****************************************************
函数功能：显示小时
入口参数：x
***************************************************/ 
void DisplayHour(unsigned char x)
{
 	unsigned char i,j;     //j,k十位和个位
	i=x/10;//取十位
	j=x%10;//取个位     
	LCD_Dspaly_8x16_Asicc(41,2,i+16);
	delay_1ms(1);        //延时1ms给硬件一点反应时间 
	LCD_Dspaly_8x16_Asicc(48,2,j+16);
	delay_1ms(1);        //延时1ms给硬件一点反应时间 
	delaynms(50);         //延时1ms给硬件一点反应时间    
 }

  /*****************************************************
函数功能：显示日
入口参数：x
***************************************************/ 
void DisplayDay(unsigned char x)
{
 	unsigned char i,j;     //j,k十位和个位
	i=x/10;//取十位
	j=x%10;//取个位     
	LCD_Dspaly_8x16_Asicc(96,0,i+16);
	delay_1ms(1);        //延时1ms给硬件一点反应时间 
	LCD_Dspaly_8x16_Asicc(104,0,j+16);
	delay_1ms(1);        //延时1ms给硬件一点反应时间 
	delaynms(50);         //延时1ms给硬件一点反应时间    
 }

/*****************************************************
函数功能：显示月
入口参数：x
***************************************************/ 
void DisplayMonth(unsigned char x)
{
 	unsigned char i,j;     //j,k分别储存十位和个位
	i=x/10;//取十位
	j=x%10;//取个位     
	LCD_Dspaly_8x16_Asicc(64,0,i+16);
	delay_1ms(1);        //延时1ms给硬件一点反应时间 
	LCD_Dspaly_8x16_Asicc(72,0,j+16);    //将个位数字的字符常量写入LCD
	delay_1ms(1);        //延时1ms给硬件一点反应时间   
 }

 /*****************************************************
函数功能：显示年
入口参数：x
***************************************************/ 
void DisplayYear(unsigned char x)
{
 	unsigned char i,j;     //j,k分别储存十位和个位
	i=x/10;//取十位
	j=x%10;//取个位     
	LCD_Dspaly_8x16_Asicc(33,0,i+16);
	delay_1ms(1);        //延时1ms给硬件一点反应时间 
	LCD_Dspaly_8x16_Asicc(41,0,j+16);
	delay_1ms(1);        //延时1ms给硬件一点反应时间 
	delaynms(50);         //延时1ms给硬件一点反应时间    
 }


 /*****************************************************
函数功能：日期
入口参数：void
***************************************************/ 
void DisplayData()
{
 	  ReadValue = ReadSet1302(0x81);   //从秒寄存器读数据
      second=((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);//将读出数据转化
	  DisplaySecond(second);          //显示秒
	    
	  ReadValue = ReadSet1302(0x83);  //从分寄存器读
      minute=((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F); //将读出数据转化
	  DisplayMinute(minute);	       //显示分
	  
	  ReadValue=0;
      ReadValue = ReadSet1302(0x85);  //从时寄存器读
      hour=((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F); //将读出数据转化
	  DisplayHour(hour);	             //显示小时

	  ReadValue=0;
      ReadValue = ReadSet1302(0x87);  //日寄存器读
      day=((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F); //将读出数据转化
	  DisplayDay(day);                //显示日

	  ReadValue=0;
	  ReadValue = ReadSet1302(0x89);  
      month=((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F); 
	  DisplayMonth(month);           

	  ReadValue=0;
	  ReadValue = ReadSet1302(0x8d);  //′ó·???′??÷?á
      year=((ReadValue&0xf0)>>4)*10 + (ReadValue&0x0F); //???á3?êy?Y×a?ˉ
	  DisplayYear(year);  	  
}

 /*****************************************************
函数功能：显示需要的汉字 字符串
入口参数：void
***************************************************/ 
void DispStr()
{
	OLED_P8x16Str(0,0,"Dt:");
	OLED_P16x16Ch(48,0,3);		//年
	OLED_P16x16Ch(80,0,4);		//月
	OLED_P16x16Ch(112,0,5);		//日	
	OLED_P8x16Str(0,2,"Time:");	
	OLED_P8x16Str(56,2,":");
	OLED_P8x16Str(80,2,":");
	OLED_P8x16Str(0,4,"WDt:04-30 08-20");	
	OLED_P8x16Str(0,6,"Hum:  % Set:  %");		
}




