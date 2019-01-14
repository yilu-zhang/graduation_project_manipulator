#ifndef __MAINSUBF_H
#define __MAINSUBF_H
#include "sys.h"

void SetId(void);     //根据上位机信号设置ID函数
void SetPosVel(void);  //根据上位机信号设置位置好和速度函数
void SetVel(void);     //根据上位机信号设置速度函数
void Teach(void);       //示教函数
void RectMotion(void);  //根据上位机信号直线运动函数



#endif



/*aa0=bb*bb+cc*cc;
				 bb0=-2*aa*bb;
				 cc0=aa*aa-cc*cc;
				 if(aa0==0&&bb0==0)
					 continue;
				 else if(aa0==0)
				 {
						s61=-cc0/bb0;
					 if(s61>1||s61<-1)
						 continue;
						an61=asin(s61);
            s62s=1;					 
				 }
				 else
				 {
				 abc0=pow(bb0,2)-4*aa0*cc0;
				 if(abc0<0)
				 {
					 continue;
				 }
				 s61=(-bb0+sqrt(abc0))/2/aa0;
				 s62=(-bb0-sqrt(abc0))/2/aa0;
				 if(s61>1||s61<-1)
					 s61s=1;
				 else
					 an61=asin(s61); //asin()范围是-90~90,an6的范围是30~330
				 if(s62>1||s62<-1)
					 s62s=1;
				 else
					 an62=asin(s62);
				 }*/
	 /*an60=an61;
          for(rt61i=1,rt61=0;rt61==0&&rt61i<3;rt61i++)
				 {
						 if(an62>=30*PI&&an62<=90*PI&&rt61i==1)
							 an62=an60;
						 else if(an62>=-90*PI&&an62<=-30*PI&&rt61i==1)
							 an62=an60+360*PI;
						 else
							 an62=180*PI-an60;*/			
 /*bb3=-2*aa3*sin(an2);
								 cc3=aa3*aa3-cos(an2)*cos(an2);
								 abc3=pow(bb3,2)-4*cc3;
								 if(abc3>=0)
								 {
									 s31=(-bb3+sqrt(abc3))/2;
									 s32=(-bb3-sqrt(abc3))/2;
									 if(s31<=1&&s31>=-1)
										an31=asin(s31);							 //asin()范围是-90~90,an6的范围是30~330
									 if(s32<=1&&s32>=-1)
										an32=asin(s32); 
									 if((s31>1||s31<-1||an31>30*PI)&&(s32>1||s32<-1||an32>30*PI))
										 {										 
											 continue;
										 }
									 else if(s31>1||s31<-1||an31>30*PI)
										 an3=s32;
									 else if(s32>1||s32<-1||an32>30*PI)
										 an3=s31;
									 else
									 {
											if(an31>an32)
												an3=an31;
											else
												an3=an32;
									 }*/
/*an60=an62;
          for(rt62i=1,rt62=0;rt62==0&&rt62i<3;rt62i++)
				 {
						 if(an62>=30*PI&&an62<=90*PI&&rt62i==1)
							 an62=an60;
						 else if(an62>=-90*PI&&an62<=-30*PI&&rt62i==1)
							 an62=an60+360*PI;
						 else
							 an62=180*PI-an60;*/	
/* bb3=-2*aa3*sin(an2);
							 cc3=aa3*aa3-cos(an2)*cos(an2);
							 abc3=pow(bb3,2)-4*cc3;
							 if(abc3>=0)
							 {
								 s31=(-bb3+sqrt(abc3))/2;
								 s32=(-bb3-sqrt(abc3))/2;
								 if(s31<=1&&s31>=-1)
									an31=asin(s31);							 //asin()范围是-90~90,an6的范围是30~330
								 if(s32<=1&&s32>=-1)
									an32=asin(s32); 
								 if((s31>1||s31<-1||an31>30*PI)&&(s32>1||s32<-1||an32>30*PI))
									 {										 
											 continue;
										}
								 else if(s31>1||s31<-1||an31>30*PI)
									 an3=s32;
								 else if(s32>1||s32<-1||an32>30*PI)
									 an3=s31;
								 else
								 {
										if(an31>an32)
											an3=an31;
										else
											an3=an32;
								 }*/
	/*unsigned short int pos;
						unsigned short int spe;
						spe=(((int)USART_RX_BUF[7]-48)*10+((int)USART_RX_BUF[8]-48))*1023/62;
						if(id<7)  //id=1~6
						{
							pos=(((int)USART_RX_BUF[4]-48)*100+((int)USART_RX_BUF[5]-48)*10+((int)USART_RX_BUF[6]-48))*1023/300;
							SetServoPosition(id,pos, spe);
							
						}
						else  //id=7
						{					 
							int t0;       //延时时间
							pos=(((int)USART_RX_BUF[4]-48)*100+((int)USART_RX_BUF[5]-48)*10+((int)USART_RX_BUF[6]-48))*2500/180;
							if(num==0)
							{
								TIM_SetCompare3(TIM3,1250);
								num+=1;
								pos0_7=1250;   //0即舵机旋转到0度，0~2500<——>0~180度
							}
							t0=1080000/(60*spe);
							if(pos>pos0_7)   //目标角度大于当前角度
							{
								 for(;pos0_7<pos;)
								{
									pos0_7=pos0_7+3;
									TIM_SetCompare3(TIM3,pos0_7);
									delay_us(t0);
								}
							}
							else if(pos<pos0_7)   //目标角度小于当前角度
							{
								 for(;pos0_7>pos;)
								{
									pos0_7=pos0_7-3;
									TIM_SetCompare3(TIM3,pos0_7);
									delay_us(t0);
								}
							}
						}	*/	
						

