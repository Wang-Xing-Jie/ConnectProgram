#include "IIC_OLED.h"
#include "codetab.h"
/*********************OLED写数据************************************/ 
void OLED_WrDat(unsigned char IIC_Data)
{
	IIC_Start();
	Write_IIC_Byte(0x78);
	Write_IIC_Byte(0x40);			//write data
	Write_IIC_Byte(IIC_Data);
	IIC_Stop();
}

/*********************OLED写命令************************************/
void OLED_WrCmd(unsigned char IIC_Command)
{
	IIC_Start();
	Write_IIC_Byte(0x78);            //Slave address,SA0=0
	Write_IIC_Byte(0x00);			//write command
	Write_IIC_Byte(IIC_Command);
	IIC_Stop();
}


/*********************OLED 设置坐标************************************/
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
	OLED_WrCmd(0xb0+y);
	OLED_WrCmd(((x&0xf0)>>4)|0x10);
	OLED_WrCmd((x&0x0f)|0x01);
}

/*********************OLED全屏************************************/
void OLED_Fill(unsigned char bmp_dat) 
{
	unsigned char y,x;
	for(y=0;y<8;y++)
	{
		OLED_WrCmd(0xb0+y);
		OLED_WrCmd(0x01);
		OLED_WrCmd(0x10);
		for(x=0;x<X_WIDTH;x++)
		OLED_WrDat(bmp_dat);
	}
}

/*********************OLED复位************************************/
void OLED_CLS(void)
{
	unsigned char y,x;
	for(y=0;y<8;y++)
	{
		OLED_WrCmd(0xb0+y);
		OLED_WrCmd(0x01);
		OLED_WrCmd(0x10);
		for(x=0;x<X_WIDTH;x++)
		OLED_WrDat(0);
	}
}


/*********************OLED初始化************************************/
void OLED_Init(void)
{
	IIC_Port_Set_Out;
	delayms(500);   //初始化之前的延时很重要！
	OLED_WrCmd(0xae);//--turn off oled panel
	OLED_WrCmd(0x00);//---set low column address
	OLED_WrCmd(0x10);//---set high column address
	OLED_WrCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WrCmd(0x81);//--set contrast control register
	OLED_WrCmd(Brightness); // Set SEG Output Current Brightness
	OLED_WrCmd(0xa1);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_WrCmd(0xc8);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	OLED_WrCmd(0xa6);//--set normal display
	OLED_WrCmd(0xa8);//--set multiplex ratio(1 to 64)
	OLED_WrCmd(0x3f);//--1/64 duty
	OLED_WrCmd(0xd3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WrCmd(0x00);//-not offset
	OLED_WrCmd(0xd5);//--set display clock divide ratio/oscillator frequency
	OLED_WrCmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WrCmd(0xd9);//--set pre-charge period
	OLED_WrCmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WrCmd(0xda);//--set com pins hardware configuration
	OLED_WrCmd(0x12);
	OLED_WrCmd(0xdb);//--set vcomh
	OLED_WrCmd(0x40);//Set VCOM Deselect Level
	OLED_WrCmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WrCmd(0x02);//
	OLED_WrCmd(0x8d);//--set Charge Pump enable/disable
	OLED_WrCmd(0x14);//--set(0x10) disable
	OLED_WrCmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
	OLED_WrCmd(0xa6);// Disable Inverse Display On (0xa6/a7) 
	OLED_WrCmd(0xaf);//--turn on oled panel
	OLED_Fill(0x00); //初始清屏
	OLED_Set_Pos(0,0);
}

/***************功能描述：显示6*8一组标准ASCII字符串	显示的坐标（x,y），y为页范围0～7****************/
void OLED_P6x8Str(unsigned char x, y,unsigned char ch[])
{
	unsigned char c=0,i=0,j=0;
	while (ch[j]!='\0')
	{
		c =ch[j]-32;
		if(x>126){x=0;y++;}
		OLED_Set_Pos(x,y);
		for(i=0;i<6;i++)
		OLED_WrDat(F6x8[c][i]);
		x+=6;
		j++;
	}
}
/*******************功能描述：显示8*16一组标准ASCII字符串	 显示的坐标（x,y），y为页范围0～7****************/
void OLED_P8x16Str(unsigned char x, y,unsigned char ch[])
{
	unsigned char c=0,i=0,j=0;
	while (ch[j]!='\0')
	{
		c =ch[j]-32;
		if(x>120){x=0;y++;}
		OLED_Set_Pos(x,y);
		for(i=0;i<8;i++)
		OLED_WrDat(F8X16[c*16+i]);
		OLED_Set_Pos(x,y+1);
		for(i=0;i<8;i++)
		OLED_WrDat(F8X16[c*16+i+8]);
		x+=8;
		j++;
	}
}
/*****************功能描述：显示16*16点阵  显示的坐标（x,y），y为页范围0～7****************************/
void OLED_P16x16Ch(unsigned char x, y, N)
{
	unsigned char wm=0;
	unsigned int adder=32*N;
	OLED_Set_Pos(x , y);
	for(wm = 0;wm < 16;wm++)
	{
		OLED_WrDat(F16x16[adder]);
		adder += 1;
	}
	OLED_Set_Pos(x,y + 1);
	for(wm = 0;wm < 16;wm++)
	{
		OLED_WrDat(F16x16[adder]);
		adder += 1;
	} 	  	
}

/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
void Draw_BMP(unsigned char x0, y0,x1, y1,unsigned char BMP[])
{
	unsigned int j=0;
	unsigned char x,y;

  	if(y1%8==0) y=y1/8;      
  	else y=y1/8+1;

	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
	    {      
	    	OLED_WrDat(BMP[j++]);
	    }
	}
}

/*************************************
功能：显示8*16Ascii码
入口："x","y","ch"
出口：无
说明：ch为ASSIC码编号
*************************************/
void LCD_Dspaly_8x16_Asicc(uchar x,uchar y,uchar ch)
{
    uchar i;
	OLED_Set_Pos(x,y);   
	for(i=0;i<8;i++)      
	OLED_WrDat(F8X16[ch*16+i]);     
	OLED_Set_Pos(x,y+1);     
	for(i=0;i<8;i++)        
	OLED_WrDat(F8X16[ch*16+i+8]);  
}

/***************************************
功能：OLED的 十,各位分离
入口：横坐标：X;  纵坐标：Y ；输入数据: date
出口：无
说明：六位数分离显示
**************************************/
void Two_Digits_Dis(uchar X,uchar Y,uint date)
{
    uchar shi=0,ge=0;
    shi=date/10;
    ge=date%10;
    LCD_Dspaly_8x16_Asicc(X+0 ,Y,shi+16);
    LCD_Dspaly_8x16_Asicc(X+8 ,Y,ge+16);          
}

/***************************************
功能：OLED的,百,十,各位分离
入口：横坐标：X;  纵坐标：Y ；输入数据: date
出口：无
说明：六位数分离显示
**************************************/
void Three_Digits_Dis(uchar X,uchar Y,uint date)
{
    uchar bai=0,shi=0,ge=0;
    bai=date/100;
    shi=(date%100)/10;
    ge=date%10;
    LCD_Dspaly_8x16_Asicc(X+0 ,Y,bai+16);
    LCD_Dspaly_8x16_Asicc(X+8 ,Y,shi+16);
    LCD_Dspaly_8x16_Asicc(X+16,Y,ge+16);            
}

/***************************************
功能：OLED的千,百,十,各位分离
入口：横坐标：X;  纵坐标：Y ；输入数据: date
出口：无
说明：六位数分离显示
**************************************/
void Four_Digits_Dis(uchar X,uchar Y,uint date)
{
    uchar qian=0,bai=0,shi=0,ge=0;
    qian=date/1000;
    bai=(date%1000)/100;
    shi=(date%100)/10;
    ge=date%10;
    LCD_Dspaly_8x16_Asicc(X+0 ,Y,qian+16);
    LCD_Dspaly_8x16_Asicc(X+8 ,Y,bai+16);
    LCD_Dspaly_8x16_Asicc(X+16,Y,shi+16);  
    LCD_Dspaly_8x16_Asicc(X+24,Y,ge+16);          
}

/***************************************
功能：OLED万,千,百,十,各位分离
入口：横坐标：X;  纵坐标：Y ；输入数据: date
出口：无
说明：六位数分离显示
**************************************/
void Five_Digits_Dis(uchar X,uchar Y,uint date)
{
    uchar wan=0,qian=0,bai=0,shi=0,ge=0;
    wan=date/10000;
    qian=(date%10000)/1000;
    bai=(date%1000)/100;
    shi=(date%100)/10;
    ge=date%10;
    LCD_Dspaly_8x16_Asicc(X+0 ,Y,wan+16);
    LCD_Dspaly_8x16_Asicc(X+8 ,Y,qian+16);
    LCD_Dspaly_8x16_Asicc(X+16,Y,bai+16);  
    LCD_Dspaly_8x16_Asicc(X+24,Y,shi+16);
    LCD_Dspaly_8x16_Asicc(X+32,Y,ge+16);            
}



