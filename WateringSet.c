#include "WateringSet.h"
//uchar W_Days=1,W_Time1=0,W_Time2=0;
//uchar Mode=0; NORMER_MODE=0,SetW_Days=1,SetW_Time1=2,SetW_Time2=3;
sbit Set=P1^4;
sbit Up=P1^5;
sbit Down=P1^6;
sbit Enter=P1^7;

//void KeyScan()
//{
//	P1|=0X0F;
//	if((Set==0)||(Up==0)||(Down==0)||(Enter==0))
//	{
//		delayms(20);
//		if((Set==0)||(Up==0)||(Down==0)||(Enter==0))
//		{
// 			delayms(10);
//			if(Set==0)
//			{
//				if(Mode==NORMER_MODE)
//				{
//					Mode=SetW_Days;	
//				}
//				else if(Mode==SetW_Days)
//				{
//					Mode=SetW_Time1;	
//				}
//				else if(Mode==SetW_Time1)
//				{
//					Mode=SetW_Time2;
//				}
//				else if(Mode==SetW_Time2)
//				{
//					Mode=NORMER_MODE;
//				}
//						
//			}
//			else if(Up==0)
//			{
//				if(Mode==NORMER_MODE)
//				{
//				}
//				if(Mode==SetW_Days)
//				{
//					W_Days++;
//					if(W_Days>=10)
//					{
//						W_Days=0;
//					}	
//				}
//				if(Mode==SetW_Time1)
//				{
//					W_Time1++;
//					if(W_Time1>=3)
//					W_Time1=0;	
//				}
//				if(Mode==SetW_Time2)
//				{
//					W_Time2++;
//					if(W_Time2>=3)
//					W_Time2=0;
//				}
//			}
//			else if(Down==0)
//			{
//				if(Mode==NORMER_MODE)
//				{
//				}
//				else if(Mode==SetW_Days)
//				{
//					W_Days--;
//					if(W_Days<0)
//					{
//						W_Days=10;
//					}	
//				}
//				else if(Mode==SetW_Time1)
//				{
//					W_Time1--;
//					if(W_Time1<0)
//					W_Time1=3;	
//				}
//				else if(Mode==SetW_Time2)
//				{
//					W_Time2--;
//					if(W_Time2<0)
//					W_Time2=3;
//				}
//			}
//			else if(Enter==0)
//			{
//				
//			}
//		}
//	}
//	else
//	{
//		
//	}
//}

//void DisplayWatering()
//{
//	LCD_Dspaly_8x16_Asicc(49,4, W_Days+16);
//	LCD_Dspaly_8x16_Asicc(81,4, W_Time1+16);
//	LCD_Dspaly_8x16_Asicc(105,4,W_Time2+16);		
//}
void test()
{

}

