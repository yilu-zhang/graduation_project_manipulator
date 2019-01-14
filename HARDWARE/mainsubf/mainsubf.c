#include "stm32f4xx.h"
#include "pwm.h"
#include  "delay.h"
#include "led.h"
#include "servo.h"
#include "usart.h"
#include "usart_vs.h"
#include "math.h"
#include "trigono.h"
#include "mainsubf.h"
#define   PI   0.01745329251994 

/*存放与主函数直接相关的函数*/

extern  int pos0_7;   //手掌舵机当前位置
extern	int num;
extern	unsigned short int pos_now[20][7];
extern	unsigned short int spe_now[20][7];
extern  int id;
void SetId(void)     //设置ID函数
{
		int id_set;
		id_set=(int)USART_RX_BUF[2]-48;					
		SetServoId(id_set);
}
void SetPosVel(void)  //设置位置和速度函数
{   unsigned short int pos;
		unsigned short int spe;
		spe=(((int)USART_RX_BUF[7]-48)*10+((int)USART_RX_BUF[8]-48))*1023/62;
		if(id<7)  //id=1~6
		{
			pos=(((int)USART_RX_BUF[4]-48)*100+((int)USART_RX_BUF[5]-48)*10+((int)USART_RX_BUF[6]-48))*1023/300;
			SetServoPosition(id,pos, spe);
			LED1=!LED1;      //LED1闪烁
		}
		else  //id=7
		{					 
			int t0;       //延时时间
			pos=(((int)USART_RX_BUF[4]-48)*100+((int)USART_RX_BUF[5]-48)*10+((int)USART_RX_BUF[6]-48))*2500/180;
			if(num==0)
			{
				TIM_SetCompare3(TIM3,1250);
				num+=1;
				pos0_7=1250;   //1250即舵机旋转到0度，1250~1750<——>0~180度
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
				{ pos0_7=pos0_7-3;
					TIM_SetCompare3(TIM3,pos0_7);
					delay_us(t0);
				}
			}
		}
}
void SetVel(void)    //根据上位机信号设置速度函数
{
  int spe_s;
	spe_s=(((int)USART_RX_BUF[2]-48)*10+((int)USART_RX_BUF[3]-48))*1023/62;
	if(id>0||id<7)
	{
	 SetServoVelocity(id,spe_s);
	}
}
void Teach(void)       //示教函数
{
	unsigned short int t_tea=0;
	unsigned short int num_tea;
	 unsigned short int exit=1;		
	 while(exit)
	 {		  
			if(USART_RX_STA&0x8000)
			{
				USART_RX_STA=0;
				id=((int)USART_RX_BUF[2]-48)*10+((int)USART_RX_BUF[3]-48);
				if(USART_RX_BUF[0]=='A')    //第一个字母为A
				{
					if(USART_RX_BUF[1]=='H')//若接收第二个字母为H，则退出示教
						exit=0;
					
				 if(USART_RX_BUF[1]=='B') //若接收第二个字母为B，设置舵机位置和速度
					{
						SetPosVel();
								
					}
					if(USART_RX_BUF[1]=='E')  //记录按钮按下
					{
						unsigned short int t_e;
						/*double a2=88,a3=33,d4=104,a5=0,d6=0;  //angle角1、2、3、4、5、6,ac1:角1的精度，ac2：计算精度
		        double ang1,ang2,ang3,ang4,ang5,ang6;
						unsigned short int x=0,y=0;
			  double T01[4][4]={{0,0,0,0},{0,0,0,0},{0,0,1,0},{0,0,0,1}};
				double T12[4][4]={{0,0,0,0},{0,0,1,0},{0,0,0,0},{0,0,0,1}};
				double T23[4][4]={{0,0,0,0},{0,0,0,0},{0,0,1,0},{0,0,0,1}};
				double T34[4][4]={{0,0,0,0},{0,0,1,0},{0,0,0,0},{0,0,0,1}};
				double T45[4][4]={{0,0,0,0},{0,0,-1,0},{0,0,0,0},{0,0,0,1}};
				double T56[4][4]={{0,0,0,0},{0,0,1,0},{0,0,0,0},{0,0,0,1}};
        double T13[4][4],T14[4][4],T15[4][4],T16[4][4],T060[4][4];*/
						for(t_e=0;t_e!=1;)
						{
							USART_SER_RX_STA=0;
							ReadServoPositionVelocity(1);
							if(USART_SER_RX_STA&0x8000&&USART_SER_RX_BUF[0]==0xFF&&USART_SER_RX_BUF[1]==0xFF&&USART_SER_RX_BUF[2]==0x01&&USART_SER_RX_BUF[3]==0x06&&USART_SER_RX_BUF[4]==0x00)
							{										
									unsigned char temp_sum = 0; 
									temp_sum = USART_SER_RX_BUF[2]+USART_SER_RX_BUF[3]+USART_SER_RX_BUF[4]+USART_SER_RX_BUF[5]+USART_SER_RX_BUF[6]+USART_SER_RX_BUF[7]+USART_SER_RX_BUF[8];
									temp_sum = ~temp_sum;
									if(temp_sum==USART_SER_RX_BUF[9])
									{
										pos_now[t_tea][0]=USART_SER_RX_BUF[6]*256+USART_SER_RX_BUF[5];
										spe_now[t_tea][0]=USART_SER_RX_BUF[8]*256+USART_SER_RX_BUF[7];
										t_e=1;
										USART_SER_RX_STA=0;
									}
							}
						}
						
						for(t_e=0;t_e!=1;)
						{
							USART_SER_RX_STA=0;
							ReadServoPositionVelocity(2);
							if(USART_SER_RX_STA&0x8000&&USART_SER_RX_BUF[0]==0xFF&&USART_SER_RX_BUF[1]==0xFF&&USART_SER_RX_BUF[2]==0x02&&USART_SER_RX_BUF[3]==0x06&&USART_SER_RX_BUF[4]==0x00)
							{
									unsigned char temp_sum = 0; 
									temp_sum = USART_SER_RX_BUF[2]+USART_SER_RX_BUF[3]+USART_SER_RX_BUF[4]+USART_SER_RX_BUF[5]+USART_SER_RX_BUF[6]+USART_SER_RX_BUF[7]+USART_SER_RX_BUF[8];
									temp_sum = ~temp_sum;
									if(temp_sum==USART_SER_RX_BUF[9])
									{
										pos_now[t_tea][1]=USART_SER_RX_BUF[6]*256+USART_SER_RX_BUF[5];
										spe_now[t_tea][1]=USART_SER_RX_BUF[8]*256+USART_SER_RX_BUF[7];
										t_e=1;
										USART_SER_RX_STA=0;
									}											
							}
						}
						
						for(t_e=0;t_e!=1;)
						{
							USART_SER_RX_STA=0;
							ReadServoPositionVelocity(3);
							if(USART_SER_RX_STA&0x8000&&USART_SER_RX_BUF[0]==0xFF&&USART_SER_RX_BUF[1]==0xFF&&USART_SER_RX_BUF[2]==0x03&&USART_SER_RX_BUF[3]==0x06&&USART_SER_RX_BUF[4]==0x00)
							{
									unsigned char temp_sum = 0; 
									temp_sum = USART_SER_RX_BUF[2]+USART_SER_RX_BUF[3]+USART_SER_RX_BUF[4]+USART_SER_RX_BUF[5]+USART_SER_RX_BUF[6]+USART_SER_RX_BUF[7]+USART_SER_RX_BUF[8];
									temp_sum = ~temp_sum;
									if(temp_sum==USART_SER_RX_BUF[9])
									{
										pos_now[t_tea][2]=USART_SER_RX_BUF[6]*256+USART_SER_RX_BUF[5];
										spe_now[t_tea][2]=USART_SER_RX_BUF[8]*256+USART_SER_RX_BUF[7];
										t_e=1;
										USART_SER_RX_STA=0;
									}
							}
						}
						
						for(t_e=0;t_e!=1;)
						{
							USART_SER_RX_STA=0;
							ReadServoPositionVelocity(4);
							if(USART_SER_RX_STA&0x8000&&USART_SER_RX_BUF[0]==0xFF&&USART_SER_RX_BUF[1]==0xFF&&USART_SER_RX_BUF[2]==0x04&&USART_SER_RX_BUF[3]==0x06&&USART_SER_RX_BUF[4]==0x00)
							{
								unsigned char temp_sum = 0; 
								temp_sum = USART_SER_RX_BUF[2]+USART_SER_RX_BUF[3]+USART_SER_RX_BUF[4]+USART_SER_RX_BUF[5]+USART_SER_RX_BUF[6]+USART_SER_RX_BUF[7]+USART_SER_RX_BUF[8];
								temp_sum = ~temp_sum;
								if(temp_sum==USART_SER_RX_BUF[9])
								{
									pos_now[t_tea][3]=USART_SER_RX_BUF[6]*256+USART_SER_RX_BUF[5];
									spe_now[t_tea][3]=USART_SER_RX_BUF[8]*256+USART_SER_RX_BUF[7];
									t_e=1;
									USART_SER_RX_STA=0;
								}
							}
						}
						
						for(t_e=0;t_e!=1;)
						{
							USART_SER_RX_STA=0;
							ReadServoPositionVelocity(5);
							if(USART_SER_RX_STA&0x8000&&USART_SER_RX_BUF[0]==0xFF&&USART_SER_RX_BUF[1]==0xFF&&USART_SER_RX_BUF[2]==0x05&&USART_SER_RX_BUF[3]==0x06&&USART_SER_RX_BUF[4]==0x00)
							{
									unsigned char temp_sum = 0; 
									temp_sum = USART_SER_RX_BUF[2]+USART_SER_RX_BUF[3]+USART_SER_RX_BUF[4]+USART_SER_RX_BUF[5]+USART_SER_RX_BUF[6]+USART_SER_RX_BUF[7]+USART_SER_RX_BUF[8];
									temp_sum = ~temp_sum;
									if(temp_sum==USART_SER_RX_BUF[9])
									{
										pos_now[t_tea][4]=USART_SER_RX_BUF[6]*256+USART_SER_RX_BUF[5];
										spe_now[t_tea][4]=USART_SER_RX_BUF[8]*256+USART_SER_RX_BUF[7];
										t_e=1;
										USART_SER_RX_STA=0;
									}
							}
						}
						
						for(t_e=0;t_e!=1;)
						{
							USART_SER_RX_STA=0;
							ReadServoPositionVelocity(6);
							if(USART_SER_RX_STA&0x8000&&USART_SER_RX_BUF[0]==0xFF&&USART_SER_RX_BUF[1]==0xFF&&USART_SER_RX_BUF[2]==0x06&&USART_SER_RX_BUF[3]==0x06&&USART_SER_RX_BUF[4]==0x00)
							{
									unsigned char temp_sum = 0; 
									temp_sum = USART_SER_RX_BUF[2]+USART_SER_RX_BUF[3]+USART_SER_RX_BUF[4]+USART_SER_RX_BUF[5]+USART_SER_RX_BUF[6]+USART_SER_RX_BUF[7]+USART_SER_RX_BUF[8];
									temp_sum = ~temp_sum;
									if(temp_sum==USART_SER_RX_BUF[9])
									{
										pos_now[t_tea][5]=USART_SER_RX_BUF[6]*256+USART_SER_RX_BUF[5];
										spe_now[t_tea][5]=USART_SER_RX_BUF[6]*256+USART_SER_RX_BUF[5];
										pos_now[t_tea][6]=pos0_7;
										spe_now[t_tea][6]=512;
										t_e=1;
										USART_SER_RX_STA=0;
									}
							}
						}			
				/*ang1=((pos_now[t_tea][0]+3)*300/1023-60)*PI;   
				ang2=((pos_now[t_tea][1]+3)*300/1023-240)*PI;
				ang3=(60-(pos_now[t_tea][2]-7)*300/1023)*PI;
				ang4=((pos_now[t_tea][3]-3)*300/1023-150)*PI;
				ang5=(150-(pos_now[t_tea][4]-8)*300/1023)*PI;
				ang6=((pos_now[t_tea][5]+3)*300/1023-150)*PI;
				
	
				T01[0][0]=cos(ang1);
				T01[0][1]=-sin(ang1);
				T01[1][0]=sin(ang1);
				T01[1][1]=cos(ang1);
					
				T12[0][0]=cos(ang2);
				T12[0][1]=-sin(ang2);
				T12[2][0]=-sin(ang2);
				T12[2][1]=-cos(ang2);
				
        T23[0][0]=cos(ang3);
				T23[0][1]=-sin(ang3);
				T23[0][3]=a2;
				T23[1][0]=sin(ang3);
				T23[1][1]=cos(ang3);

				T34[0][0]=cos(ang4);
				T34[0][1]=-sin(ang4);
				T34[0][3]=a3;
				T34[1][3]=d4;	
				T34[2][0]=-sin(ang4);
				T34[2][1]=-cos(ang4);
				
				T45[0][0]=cos(ang5);
				T45[0][1]=-sin(ang5);
				T45[2][0]=sin(ang5);
				T45[2][1]=cos(ang5);
				
				T56[0][0]=cos(ang6);
				T56[0][1]=-sin(ang6);
				T56[2][0]=-sin(ang6);
				T56[2][1]=-cos(ang6);
				T56[1][3]=d6;
				T56[0][3]=a5;
				
				for(x=0;x<4;x++)
				{
					for(y=0;y<4;y++)
					{
							T13[x][y]=T12[x][0]*T23[0][y]+T12[x][1]*T23[1][y]+T12[x][2]*T23[2][y]+T12[x][3]*T23[3][y];
					}
				}
				for(x=0;x<4;x++)
				{
					for(y=0;y<4;y++)
					{
							T14[x][y]=T13[x][0]*T34[0][y]+T13[x][1]*T34[1][y]+T13[x][2]*T34[2][y]+T13[x][3]*T34[3][y];
					}
				}
				for(x=0;x<4;x++)
				{
					for(y=0;y<4;y++)
					{
							T15[x][y]=T14[x][0]*T45[0][y]+T14[x][1]*T45[1][y]+T14[x][2]*T45[2][y]+T14[x][3]*T45[3][y];
					}
				}
          for(x=0;x<4;x++)
				{
					for(y=0;y<4;y++)
					{
							T16[x][y]=T15[x][0]*T56[0][y]+T15[x][1]*T56[1][y]+T15[x][2]*T56[2][y]+T15[x][3]*T56[3][y];
					}
				}					
          for(x=0;x<4;x++)
				{
					for(y=0;y<4;y++)
					{
						  T060[x][y]=T01[x][0]*T16[0][y]+T01[x][1]*T16[1][y]+T01[x][2]*T16[2][y]+T01[x][3]*T16[3][y];
						  USART_SendData(USART1, T060[x][y]);         //向串口1发送数据
					  	printf("\r\n");//插入换行
				      while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
					}
				}*/
					t_tea++;
						num_tea=1;
				
					}
				 if(USART_RX_BUF[1]=='F')  //开始按钮按下
					{
							unsigned short int t_tea0=0;
							unsigned short int t_sta;
						 if(num_tea==1)  //如果是第一次按下开始按钮
							 t_tea-=1;
						 num_tea=0;										 
							for(t_sta=0;t_sta==0&&exit==1;t_tea0++)
						{
							unsigned short int t0;
							if(USART_RX_STA&0x8000)
							{
								USART_RX_STA=0;
								if(USART_RX_BUF[0]=='A')
									if(USART_RX_BUF[1]=='G')
										t_sta=1;
									if(USART_RX_BUF[1]=='H')
										exit=0;
										
							}
							if(t_tea0>t_tea)
							{
								 t_tea0=0;
							}
							t0=1080000/(60*spe_now[t_tea0][6]);
							if(pos_now[t_tea0][6]>pos0_7)   //目标角度大于当前角度
							{
								 for(;pos0_7<pos_now[t_tea0][6];)
								{
									pos0_7=pos0_7+3;
									TIM_SetCompare3(TIM3,pos0_7);
									delay_us(t0);
								}
							}
							else if(pos_now[t_tea0][6]<pos0_7)   //目标角度小于当前角度
							{
								 for(;pos0_7>pos_now[t_tea0][6];)
								{
									pos0_7=pos0_7-3;
									TIM_SetCompare3(TIM3,pos0_7);
									delay_us(t0);
								}
							}
						
							
							SetServoPosition(6,pos_now[t_tea0][5],spe_now[t_tea0][5]);
							delay_ms(500);
							SetServoPosition(5,pos_now[t_tea0][4],spe_now[t_tea0][4]);
							delay_ms(500);
							SetServoPosition(4,pos_now[t_tea0][3],spe_now[t_tea0][3]);
							delay_ms(500);
							SetServoPosition(3,pos_now[t_tea0][2],spe_now[t_tea0][2]);
							delay_ms(500);
							SetServoPosition(2,pos_now[t_tea0][1],spe_now[t_tea0][1]);
							delay_ms(500);
							SetServoPosition(1,pos_now[t_tea0][0],spe_now[t_tea0][0]);
							delay_ms(500);
							
						}
					}
				}
			}
	 }
}
void RectMotion(void)  //根据上位机信号直线运动函数
{
		int ii,rt=0,rtox=0,round;
		double nx0,ny0,nz0,ax0,ay0,az0,nx,ny,nz,ox,oy,oz,ax,ay,az,o_m,a_m,px1,py1,pz1,px6,py6,pz6,px=0,py=0,pz=0,px0,py0,pz0,a2=88,a3=33,d4=104,a5=33,d6=90,c,an1=0,an2,an3,an4=0,an5=0,an6=0,anp;  //angle角1、2、3、4、5、6,ac1:角1的精度，ac2：计算精度
		double ang1[1],ang2[1],ang3[1],ang4[1],ang5[1],ang6[1],k31,k32,k21,k22,k41,k42,k51,k52,k61,k62,aa1,bb1,aa2,bb2,aax1,bax1,cax1,abc1,an;
		//rti表示角i可能解数，rtii表示角i循环次数,kxy表示计算角x时第y个参数		
		unsigned short int a[3],ti=0,pos,anser1,anser2,anser3,anser4,anser5,anser6,rt1,rti1,rt3,rti3,t_e,x=0,y=0,pos_now[6],rtax;	
		double T01[4][4]={{0,0,0,0},{0,0,0,0},{0,0,1,0},{0,0,0,1}},T12[4][4]={{0,0,0,0},{0,0,1,0},{0,0,0,0},{0,0,0,1}},T23[4][4]={{0,0,0,0},{0,0,0,0},{0,0,1,0},{0,0,0,1}},T34[4][4]={{0,0,0,0},{0,0,1,0},{0,0,0,0},{0,0,0,1}};
		double T45[4][4]={{0,0,0,0},{0,0,-1,0},{0,0,0,0},{0,0,0,1}},T56[4][4]={{0,0,0,0},{0,0,1,0},{0,0,0,0},{0,0,0,1}},T13[4][4],T14[4][4],T15[4][4],T16[4][4],T060[4][4];
		for(ii=2,c=0,pos=1;c<3;ii++)  //计算由vs发来的坐标
		 { 
			 if(USART_RX_BUF[ii]=='y')
			 {
				 ti--;
				 if(c==0)
				 {
					 if(ti==2)
					 px=a[2]+a[1]*10+a[0]*100;
						else if(ti==1)
					 px=a[1]+a[0]*10;
						else if(ti==0)
					 px=a[0];
						if(pos==0)
						{
							px=-px;
							pos=1;
						}	
					 c++;
					 ti=0;
				 }
				 else if(c==1)
				 {
					 if(ti==2)
						 py=a[2]+a[1]*10+a[0]*100;
					 else if(ti==1)
						 py=a[1]+a[0]*10;
					 else if(ti==0)
						 py=a[0];
					 if(pos==0)
						{
							py=-py;
							pos=1;
						}	
					 c++;
						ti=0;
				 }
				 else if(c==2)
				 {
					 if(ti==2)
						 pz=a[2]+a[1]*10+a[0]*100;
					 else if(ti==1)
						 pz=a[1]+a[0]*10;
					 else if(ti==0)
						 pz=a[0];
					 if(pos==0)
						{
							pz=-pz;
							pos=1;
						}	
					 c++;
						
				 }
			 }
			 if(c<3&&USART_RX_BUF[ii]=='-')
				 pos=0;
			 if(c<3&&USART_RX_BUF[ii]!='y'&&USART_RX_BUF[ii]!='-')
			 {
			 a[ti]=(int)USART_RX_BUF[ii]-48;
			 ti++;
			 }					 
		 }
		
		for(t_e=0;t_e!=1;)
		{
			USART_SER_RX_STA=0;
			ReadServoPositionVelocity(1);
			if(USART_SER_RX_STA&0x8000&&USART_SER_RX_BUF[0]==0xFF&&USART_SER_RX_BUF[1]==0xFF&&USART_SER_RX_BUF[2]==0x01&&USART_SER_RX_BUF[3]==0x06&&USART_SER_RX_BUF[4]==0x00)
			{										
					unsigned char temp_sum = 0; 
					temp_sum = USART_SER_RX_BUF[2]+USART_SER_RX_BUF[3]+USART_SER_RX_BUF[4]+USART_SER_RX_BUF[5]+USART_SER_RX_BUF[6]+USART_SER_RX_BUF[7]+USART_SER_RX_BUF[8];
					temp_sum = ~temp_sum;
					if(temp_sum==USART_SER_RX_BUF[9])
					{
						pos_now[0]=USART_SER_RX_BUF[6]*256+USART_SER_RX_BUF[5];
						//spe_now[0]=USART_SER_RX_BUF[8]*256+USART_SER_RX_BUF[7];
						t_e=1;
						USART_SER_RX_STA=0;
					}
			}
		}
						
						for(t_e=0;t_e!=1;)
						{
							USART_SER_RX_STA=0;
							ReadServoPositionVelocity(2);
							if(USART_SER_RX_STA&0x8000&&USART_SER_RX_BUF[0]==0xFF&&USART_SER_RX_BUF[1]==0xFF&&USART_SER_RX_BUF[2]==0x02&&USART_SER_RX_BUF[3]==0x06&&USART_SER_RX_BUF[4]==0x00)
							{
									unsigned char temp_sum = 0; 
									temp_sum = USART_SER_RX_BUF[2]+USART_SER_RX_BUF[3]+USART_SER_RX_BUF[4]+USART_SER_RX_BUF[5]+USART_SER_RX_BUF[6]+USART_SER_RX_BUF[7]+USART_SER_RX_BUF[8];
									temp_sum = ~temp_sum;
									if(temp_sum==USART_SER_RX_BUF[9])
									{
										pos_now[1]=USART_SER_RX_BUF[6]*256+USART_SER_RX_BUF[5];
										//spe_now[1]=USART_SER_RX_BUF[8]*256+USART_SER_RX_BUF[7];
										t_e=1;
										USART_SER_RX_STA=0;
									}											
							}
						}
						
						for(t_e=0;t_e!=1;)
						{
							USART_SER_RX_STA=0;
							ReadServoPositionVelocity(3);
							if(USART_SER_RX_STA&0x8000&&USART_SER_RX_BUF[0]==0xFF&&USART_SER_RX_BUF[1]==0xFF&&USART_SER_RX_BUF[2]==0x03&&USART_SER_RX_BUF[3]==0x06&&USART_SER_RX_BUF[4]==0x00)
							{
									unsigned char temp_sum = 0; 
									temp_sum = USART_SER_RX_BUF[2]+USART_SER_RX_BUF[3]+USART_SER_RX_BUF[4]+USART_SER_RX_BUF[5]+USART_SER_RX_BUF[6]+USART_SER_RX_BUF[7]+USART_SER_RX_BUF[8];
									temp_sum = ~temp_sum;
									if(temp_sum==USART_SER_RX_BUF[9])
									{
										pos_now[2]=USART_SER_RX_BUF[6]*256+USART_SER_RX_BUF[5];
										//spe_now[2]=USART_SER_RX_BUF[8]*256+USART_SER_RX_BUF[7];
										t_e=1;
										USART_SER_RX_STA=0;
									}
							}
						}
						
						for(t_e=0;t_e!=1;)
						{
							USART_SER_RX_STA=0;
							ReadServoPositionVelocity(4);
							if(USART_SER_RX_STA&0x8000&&USART_SER_RX_BUF[0]==0xFF&&USART_SER_RX_BUF[1]==0xFF&&USART_SER_RX_BUF[2]==0x04&&USART_SER_RX_BUF[3]==0x06&&USART_SER_RX_BUF[4]==0x00)
							{
								unsigned char temp_sum = 0; 
								temp_sum = USART_SER_RX_BUF[2]+USART_SER_RX_BUF[3]+USART_SER_RX_BUF[4]+USART_SER_RX_BUF[5]+USART_SER_RX_BUF[6]+USART_SER_RX_BUF[7]+USART_SER_RX_BUF[8];
								temp_sum = ~temp_sum;
								if(temp_sum==USART_SER_RX_BUF[9])
								{
									pos_now[3]=USART_SER_RX_BUF[6]*256+USART_SER_RX_BUF[5];
									//spe_now[3]=USART_SER_RX_BUF[8]*256+USART_SER_RX_BUF[7];
									t_e=1;
									USART_SER_RX_STA=0;
								}
							}
						}
						
						for(t_e=0;t_e!=1;)
						{
							USART_SER_RX_STA=0;
							ReadServoPositionVelocity(5);
							if(USART_SER_RX_STA&0x8000&&USART_SER_RX_BUF[0]==0xFF&&USART_SER_RX_BUF[1]==0xFF&&USART_SER_RX_BUF[2]==0x05&&USART_SER_RX_BUF[3]==0x06&&USART_SER_RX_BUF[4]==0x00)
							{
									unsigned char temp_sum = 0; 
									temp_sum = USART_SER_RX_BUF[2]+USART_SER_RX_BUF[3]+USART_SER_RX_BUF[4]+USART_SER_RX_BUF[5]+USART_SER_RX_BUF[6]+USART_SER_RX_BUF[7]+USART_SER_RX_BUF[8];
									temp_sum = ~temp_sum;
									if(temp_sum==USART_SER_RX_BUF[9])
									{
										pos_now[4]=USART_SER_RX_BUF[6]*256+USART_SER_RX_BUF[5];
										//spe_now[4]=USART_SER_RX_BUF[8]*256+USART_SER_RX_BUF[7];
										t_e=1;
										USART_SER_RX_STA=0;
									}
							}
						}
						
						for(t_e=0;t_e!=1;)
						{
							USART_SER_RX_STA=0;
							ReadServoPositionVelocity(6);
							if(USART_SER_RX_STA&0x8000&&USART_SER_RX_BUF[0]==0xFF&&USART_SER_RX_BUF[1]==0xFF&&USART_SER_RX_BUF[2]==0x06&&USART_SER_RX_BUF[3]==0x06&&USART_SER_RX_BUF[4]==0x00)
							{
									unsigned char temp_sum = 0; 
									temp_sum = USART_SER_RX_BUF[2]+USART_SER_RX_BUF[3]+USART_SER_RX_BUF[4]+USART_SER_RX_BUF[5]+USART_SER_RX_BUF[6]+USART_SER_RX_BUF[7]+USART_SER_RX_BUF[8];
									temp_sum = ~temp_sum;
									if(temp_sum==USART_SER_RX_BUF[9])
									{
										pos_now[5]=USART_SER_RX_BUF[6]*256+USART_SER_RX_BUF[5];
										//spe_now[5]=USART_SER_RX_BUF[6]*256+USART_SER_RX_BUF[5];
										t_e=1;
										USART_SER_RX_STA=0;
									}
							}
						}
				an1=((pos_now[0]+3)*300/1023-60)*PI;   
				an2=((pos_now[1]+3)*300/1023-240)*PI;
				an3=(60-(pos_now[2]-7)*300/1023)*PI;
				an4=((pos_now[3]-3)*300/1023-150)*PI;
				an5=(150-(pos_now[4]-8)*300/1023)*PI;
				an6=((pos_now[5]+3)*300/1023-150)*PI;
				
        T01[0][0]=cos(an1);
				T01[0][1]=-sin(an1);
				T01[1][0]=sin(an1);
				T01[1][1]=cos(an1);
					
				T12[0][0]=cos(an2);
				T12[0][1]=-sin(an2);
				T12[2][0]=-sin(an2);
				T12[2][1]=-cos(an2);
				
        T23[0][0]=cos(an3);
				T23[0][1]=-sin(an3);
				T23[0][3]=a2;
				T23[1][0]=sin(an3);
				T23[1][1]=cos(an3);

				T34[0][0]=cos(an4);
				T34[0][1]=-sin(an4);
				T34[0][3]=a3;
				T34[1][3]=d4;	
				T34[2][0]=-sin(an4);
				T34[2][1]=-cos(an4);
				
				T45[0][0]=cos(an5);
				T45[0][1]=-sin(an5);
				T45[2][0]=sin(an5);
				T45[2][1]=cos(an5);
				
				T56[0][0]=cos(an6);
				T56[0][1]=-sin(an6);
				T56[2][0]=-sin(an6);
				T56[2][1]=-cos(an6);
				T56[1][3]=d6;
				T56[0][3]=a5;
				
				for(x=0;x<4;x++)
				{
					for(y=0;y<4;y++)
					{
							T13[x][y]=T12[x][0]*T23[0][y]+T12[x][1]*T23[1][y]+T12[x][2]*T23[2][y]+T12[x][3]*T23[3][y];
					}
				}
				for(x=0;x<4;x++)
				{
					for(y=0;y<4;y++)
					{
							T14[x][y]=T13[x][0]*T34[0][y]+T13[x][1]*T34[1][y]+T13[x][2]*T34[2][y]+T13[x][3]*T34[3][y];
					}
				}
				for(x=0;x<4;x++)
				{
					for(y=0;y<4;y++)
					{
							T15[x][y]=T14[x][0]*T45[0][y]+T14[x][1]*T45[1][y]+T14[x][2]*T45[2][y]+T14[x][3]*T45[3][y];
					}
				}
          for(x=0;x<4;x++)
				{
					for(y=0;y<4;y++)
					{
							T16[x][y]=T15[x][0]*T56[0][y]+T15[x][1]*T56[1][y]+T15[x][2]*T56[2][y]+T15[x][3]*T56[3][y];
					}
				}					
          for(x=0;x<4;x++)
				{
					for(y=0;y<4;y++)
					{
						  T060[x][y]=T01[x][0]*T16[0][y]+T01[x][1]*T16[1][y]+T01[x][2]*T16[2][y]+T01[x][3]*T16[3][y];
						  //USART_SendData(USART1, T060[x][y]);         //向串口1发送数据
					  	//printf("\r\n");//插入换行
				     // while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
					}
				}
     px6=px0=T060[0][3];    //当前位置
     py6=py0=T060[1][3];
     pz6=pz0=T060[2][3];				
		 px1=px;            //目标位置
		 py1=py;
		 pz1=pz;
		 ox=px-px0;          //求单位向量ox，与移动方向平行
		 oy=py-py0;
		 oz=pz-pz0;
		 o_m=sqrt(pow(ox,2)+pow(oy,2)+pow(oz,2));
		if(ox>=0)
		{
		 ox=ox/o_m;
		 oy=oy/o_m;
		 oz=oz/o_m;
		}
		else
		{
		 ox=-ox/o_m;
		 oy=-oy/o_m;
		 oz=-oz/o_m;
			rtox=1;
		}
		 if(ox==0&&oy==0)
		 {
			 ax=0;
			 ay=1;
			 az=0;
		 }
		 else if(oz==0)
		 {
			 az=0;
		    if(ox==0)
				{
					ay=0;
					ax=1;
				}
				else if(oy==0) 
				{
				  ax=0;
					ay=1;
				}
				else
				{
				  ay=10;
					ax=-oy/ox*ay;
					ay=ay/(sqrt(100+pow(ax,2)));
					ax=ax/(sqrt(100+pow(ax,2)));
				}
		 }
		 else if(oz>=0)   //o*(ox,oy,0)((oz>0&&oy<=0&&ox>=0)||(oz<0&&oy<=0&&ox<=0))
		 {
	     ax=-oy*oz;
       ay=ox*oz;
       az=-ox*oy;
       a_m=sqrt(pow(ax,2)+pow(ay,2)+pow(az,2));
		   ax=ax/a_m;			 
		   ay=ay/a_m;
			 az=az/a_m;
		 }
		 else
		 {
	     ax=oy*oz;
       ay=-ox*oz;
       az=-ox*oy;
       a_m=sqrt(pow(ax,2)+pow(ay,2)+pow(az,2));
		   ax=ax/a_m;			 
		   ay=ay/a_m;
			 az=az/a_m;
		 }
		 nx=oy*az-ay*oz;
		 ny=ax*oz-ox*az;
		 nz=ox*ay-ax*oy;
		 nx0=nx;
		 ny0=ny;
		 nz0=nz;
		 //ox0=ox;
		 //oy0=oy;
		 //oz0=oz;
		 ax0=ax;
		 ay0=ay;
		 az0=az;
		 /*if(rtox==0)   //ox与移动方向相同为加
		 {px6=px0+10*ox;py6=py0+10*oy;pz6=pz0+10*oz;}
		 else
			 {px6=px0-10*ox;py6=py0-10*oy;pz6=pz0-10*oz;}*/		 
		 for(an=0,rtax=1,round=0;rtox!=2;)
		 {
			 if((px6>px1&&rtox==0)||(px6<px1&&rtox==1))//最后超过目标位置，则等于目标位置
			 {
			   px6=px1;
				 py6=py1;
				 pz6=pz1;
			 }
			 if(rtax==3)
			 {
				 an=an+1*PI;
			   rtax=1;
			 }
     if(rt!=0||an>180*PI||px6==px0)
		 {
     		 
		 nx=nx0;
		 ny=ny0;
		 nz=nz0;
		 ax=ax0;
		 ay=ay0;
		 az=az0;
			 
		 if(rtox==0)  //ox与移动方向相同为加
		 {px6=px6+10*ox;py6=py6+10*oy;pz6=pz6+10*oz;}
		 else
		 {px6=px6-10*ox;py6=py6-10*oy;pz6=pz6-10*oz;}	
	    
    
		 an=0;
		 rtax=1;
		 }
    // if(round==0)  //第一次计算关节6的位置，第二次根据计算结果较准确估计关节5的位置
  		 px=px6-ax*d6;py=py6-ay*d6;pz=pz6-az*d6;
	   /*else if(round==1)
		   {
			   px=px6-a5*(cos(an10[0])*(cos(an20[0]+an30[0])*cos(an40[0])*cos(an50[0])-sin(an20[0]+an30[0])*sin(an50[0]))+sin(an10[0])*sin(an40[0])*cos(an50[0]))+d6*(cos(an10[0])*(cos(an20[0]+an30[0])*cos(an40[0])*sin(an50[0])+sin(an20[0]+an30[0])*cos(an50[0]))+sin(an10[0])*sin(an40[0])*sin(an50[0]));
         py=py6-a5*(sin(an10[0])*(cos(an20[0]+an30[0])*cos(an40[0])*cos(an50[0])-sin(an20[0]+an30[0])*sin(an50[0]))-cos(an10[0])*sin(an40[0])*cos(an50[0]))+d6*(sin(an10[0])*(cos(an20[0]+an30[0])*cos(an40[0])*sin(an50[0])+sin(an20[0]+an30[0])*cos(an50[0]))-cos(an10[0])*sin(an40[0])*sin(an50[0]));
			   pz=pz6+a5*(sin(an20[0]+an30[0])*cos(an40[0])*cos(an50[0])+cos(an20[0]+an30[0])*sin(an50[0]))-d6*(sin(an20[0]+an30[0])*cos(an40[0])*sin(an50[0])-cos(an20[0]+an30[0])*cos(an50[0]));
			 }	*/	 
		 anp=atan2(py,px);			 
		 for(rt=0,rt1=1,rti1=0;rti1<rt1;rti1++)
		 {	
       
		   if(anp>(-30*PI)&&anp<(30*PI))
				 an1=anp;
			 else if(anp>(150*PI)&&anp<=(180*PI))
				 an1=anp-180*PI;
			 else if(anp>=(-180*PI)&&anp<(-150*PI))
				 an1=anp+180*PI;
       else if(anp>=(30*PI)&&anp<=(150*PI))
			 {			    
				 if(rti1==0)
				 {
				    rt1++;
					  an1=anp;
				 }
				 else
					 an1=anp-180*PI;
			 }
			 else            //if(anp>=(-150*PI)&&anp<=(-30*PI))
			 {			    
				 if(rti1==0)
				 {
				    rt1++;
					  an1=anp;
				 }
				 else
					 an1=anp+180*PI;
			 }
			 k31=(pow(px,2)+pow(py,2)+pow(pz,2)-pow(a2,2)-pow(a3,2)-pow(d4,2))/(2*a2);
			 k32=pow(a3,2)+pow(d4,2)-pow(k31,2);
			  an3=atan2(a3,d4)-atan2(k31,sqrt(k32));
			 for(rt3=1,rti3=0;rti3<rt3;rti3++)
			 {		  
         if(an3>=(-120*PI)&&an3<=(50*PI)&&rti3==0)
           rt3++;
          if((rt3==1&&rti3==0)||(rt3==2&&rti3==1))
					{
              an3=atan2(a3,d4)-atan2(k31,-sqrt(k32));
						if(an3<(-120*PI)||an3>(50*PI))
							continue;
					}	
          k21=-(a3+a2*cos(an3))*pz+(cos(an1)*px+sin(an1)*py)*(a2*sin(an3)-d4);
          k22=(a2*sin(an3)-d4)*pz+(cos(an1)*px+sin(an1)*py)*(a2*cos(an3)+a3);	
          an2=atan2(k21,k22)-an3;
          if(an2>=(-130*PI)&&an2<=(-50*PI))
					{
						k41=ay*cos(an1)-ax*sin(an1);
						k42=az*sin(an2+an3)-ax*cos(an1)*cos(an2+an3)-ay*sin(an1)*cos(an2+an3);
						if(fabs(k41)<0.000000000001&&fabs(k42)<0.000000000001)
							an4=0;
						else 
							an4=atan2(k41,k42);
						if(an4>=(-150*PI)&&an4<=(150*PI))
						{
							k51=az*sin(an2+an3)*cos(an4)-ax*(cos(an1)*cos(an2+an3)*cos(an4)+sin(an1)*sin(an4))-ay*(sin(an1)*cos(an2+an3)*cos(an4)-cos(an1)*sin(an4));
							k52=-ax*cos(an1)*sin(an2+an3)-ay*sin(an1)*sin(an2+an3)-az*cos(an2+an3);
							an5=atan2(k51,k52);
							if(an5>=(-20*PI)&&an5<=(140*PI))
							{
								k61=nz*sin(an2+an3)*sin(an4)-nx*(cos(an1)*cos(an2+an3)*sin(an4)-sin(an1)*cos(an4))-ny*(sin(an1)*cos(an2+an3)*sin(an4)+cos(an1)*cos(an4));
								k62=nx*((cos(an1)*cos(an2+an3)*cos(an4)+sin(an1)*sin(an4))*cos(an5)-cos(an1)*sin(an2+an3)*sin(an5))+ny*((sin(an1)*cos(an2+an3)*cos(an4)-cos(an1)*sin(an4))*cos(an5)-sin(an1)*sin(an2+an3)*sin(an5))-nz*(sin(an2+an3)*cos(an4)*cos(an5)+cos(an2+an3)*sin(an5));
								an6=atan2(k61,k62);
								if(an6>=(-150*PI)&&an6<=(150*PI))
								{
									
									//if(round==1)
									
					        ang1[rt]=an1;
						      ang2[rt]=an2;
						      ang3[rt]=an3;
									ang4[rt]=an4;
						      ang5[rt]=an5;
						      ang6[rt]=an6;
						     	rt++;
									
									/*if(round==0)
									{
					        an10[rt]=an1;
						      an20[rt]=an2;
						      an30[rt]=an3;
									an40[rt]=an4;
						      an50[rt]=an5;
						      an60[rt]=an6;
									rt++;
									}*/
									
								}
							}
						}
					}						
			 }
			 
		 }

		 if(rt!=0)
		 {	
				anser1=(ang1[0]/PI+60)*1023/300-3;  
				anser2=(ang2[0]/PI+240)*1023/300-3;
				anser3=(60-ang3[0]/PI)*1023/300+7;
				anser4=(ang4[0]/PI+150)*1023/300+3;
				anser5=(150-ang5[0]/PI)*1023/300+8;
				anser6=(ang6[0]/PI+150)*1023/300-3;			  
				/*SetServoPosition(2,anser2,300);	
				delay_ms(500);*/				
				SyncSetServoPosition(anser1,200,anser2,200,anser3,200,anser4,200,anser5,200,anser6,200); 
				delay_ms(500);
			 round=2;
		 }
		/* if(rt!=0&&round==0)
			 round=1;*/
		 if(rt==0) 
		 {
			 if(rtax==1)
			 {
		    aa2=cos(an)*oy/(az0*oy-ay0*oz);
			  bb2=(ax0*oy-ay0*ox)/(az0*oy-ay0*oz);
			  aa1=cos(an)*oz/(ay0*oz-az0*oz);
			  bb1=(ax0*oz-az0*ox)/(ay0*oz-az0*oz);
			  aax1=1+pow(bb1,2)+pow(bb2,2);
			  bax1=-2*(aa1*bb1+aa2*bb2);
			  cax1=pow(aa1,2)+pow(aa2,2);
			  abc1=pow(bax1,2)-4*aax1*cax1;
			  ax=(-bax1+sqrt(abc1))/(2*aax1);
				ay=aa1-bb1*ax;
				az=aa2-bb2*ax;
				nx=oy*az-ay*oz;
		    ny=ax*oz-ox*az;
		    nz=ox*ay-ax*oy;
				rtax=2;
			 }
			 else
			 {
			  ax=(-bax1-sqrt(abc1))/(2*aax1);
				ay=aa1-bb1*ax;
				az=aa2-bb2*ax;
				nx=oy*az-ay*oz;
		    ny=ax*oz-ox*az;
		    nz=ox*ay-ax*oy;
				rtax=3;
			 }
		 
		 }
		 if((rtox==0&&px6>=px1+10*ox)||(rtox==1&&px6<=px1-10*ox))//循环结束条件
			 rtox=2;
	 }
}
 




	/*anser10=anser1;
			  anser20=anser2;
			  anser30=anser3;
			  anser40=anser4;
			  anser50=anser5;
			  anser60=anser6;
for(ac=0;ac<5||ac==10;ac++)
				{
				for(t_e=0;t_e!=1;)
		{
			USART_SER_RX_STA=0;
			ReadServoPositionVelocity(1);
			if(USART_SER_RX_STA&0x8000&&USART_SER_RX_BUF[0]==0xFF&&USART_SER_RX_BUF[1]==0xFF&&USART_SER_RX_BUF[2]==0x01&&USART_SER_RX_BUF[3]==0x06&&USART_SER_RX_BUF[4]==0x00)
			{										
					unsigned char temp_sum = 0; 
					temp_sum = USART_SER_RX_BUF[2]+USART_SER_RX_BUF[3]+USART_SER_RX_BUF[4]+USART_SER_RX_BUF[5]+USART_SER_RX_BUF[6]+USART_SER_RX_BUF[7]+USART_SER_RX_BUF[8];
					temp_sum = ~temp_sum;
					if(temp_sum==USART_SER_RX_BUF[9])
					{
						pos_now[0]=USART_SER_RX_BUF[6]*256+USART_SER_RX_BUF[5];
						//spe_now[0]=USART_SER_RX_BUF[8]*256+USART_SER_RX_BUF[7];
						t_e=1;
						USART_SER_RX_STA=0;
					}
			}
		}
						
						for(t_e=0;t_e!=1;)
						{
							USART_SER_RX_STA=0;
							ReadServoPositionVelocity(2);
							if(USART_SER_RX_STA&0x8000&&USART_SER_RX_BUF[0]==0xFF&&USART_SER_RX_BUF[1]==0xFF&&USART_SER_RX_BUF[2]==0x02&&USART_SER_RX_BUF[3]==0x06&&USART_SER_RX_BUF[4]==0x00)
							{
									unsigned char temp_sum = 0; 
									temp_sum = USART_SER_RX_BUF[2]+USART_SER_RX_BUF[3]+USART_SER_RX_BUF[4]+USART_SER_RX_BUF[5]+USART_SER_RX_BUF[6]+USART_SER_RX_BUF[7]+USART_SER_RX_BUF[8];
									temp_sum = ~temp_sum;
									if(temp_sum==USART_SER_RX_BUF[9])
									{
										pos_now[1]=USART_SER_RX_BUF[6]*256+USART_SER_RX_BUF[5];
										//spe_now[1]=USART_SER_RX_BUF[8]*256+USART_SER_RX_BUF[7];
										t_e=1;
										USART_SER_RX_STA=0;
									}											
							}
						}
						
						for(t_e=0;t_e!=1;)
						{
							USART_SER_RX_STA=0;
							ReadServoPositionVelocity(3);
							if(USART_SER_RX_STA&0x8000&&USART_SER_RX_BUF[0]==0xFF&&USART_SER_RX_BUF[1]==0xFF&&USART_SER_RX_BUF[2]==0x03&&USART_SER_RX_BUF[3]==0x06&&USART_SER_RX_BUF[4]==0x00)
							{
									unsigned char temp_sum = 0; 
									temp_sum = USART_SER_RX_BUF[2]+USART_SER_RX_BUF[3]+USART_SER_RX_BUF[4]+USART_SER_RX_BUF[5]+USART_SER_RX_BUF[6]+USART_SER_RX_BUF[7]+USART_SER_RX_BUF[8];
									temp_sum = ~temp_sum;
									if(temp_sum==USART_SER_RX_BUF[9])
									{
										pos_now[2]=USART_SER_RX_BUF[6]*256+USART_SER_RX_BUF[5];
										//spe_now[2]=USART_SER_RX_BUF[8]*256+USART_SER_RX_BUF[7];
										t_e=1;
										USART_SER_RX_STA=0;
									}
							}
						}
						
						for(t_e=0;t_e!=1;)
						{
							USART_SER_RX_STA=0;
							ReadServoPositionVelocity(4);
							if(USART_SER_RX_STA&0x8000&&USART_SER_RX_BUF[0]==0xFF&&USART_SER_RX_BUF[1]==0xFF&&USART_SER_RX_BUF[2]==0x04&&USART_SER_RX_BUF[3]==0x06&&USART_SER_RX_BUF[4]==0x00)
							{
								unsigned char temp_sum = 0; 
								temp_sum = USART_SER_RX_BUF[2]+USART_SER_RX_BUF[3]+USART_SER_RX_BUF[4]+USART_SER_RX_BUF[5]+USART_SER_RX_BUF[6]+USART_SER_RX_BUF[7]+USART_SER_RX_BUF[8];
								temp_sum = ~temp_sum;
								if(temp_sum==USART_SER_RX_BUF[9])
								{
									pos_now[3]=USART_SER_RX_BUF[6]*256+USART_SER_RX_BUF[5];
									//spe_now[3]=USART_SER_RX_BUF[8]*256+USART_SER_RX_BUF[7];
									t_e=1;
									USART_SER_RX_STA=0;
								}
							}
						}
						
						for(t_e=0;t_e!=1;)
						{
							USART_SER_RX_STA=0;
							ReadServoPositionVelocity(5);
							if(USART_SER_RX_STA&0x8000&&USART_SER_RX_BUF[0]==0xFF&&USART_SER_RX_BUF[1]==0xFF&&USART_SER_RX_BUF[2]==0x05&&USART_SER_RX_BUF[3]==0x06&&USART_SER_RX_BUF[4]==0x00)
							{
									unsigned char temp_sum = 0; 
									temp_sum = USART_SER_RX_BUF[2]+USART_SER_RX_BUF[3]+USART_SER_RX_BUF[4]+USART_SER_RX_BUF[5]+USART_SER_RX_BUF[6]+USART_SER_RX_BUF[7]+USART_SER_RX_BUF[8];
									temp_sum = ~temp_sum;
									if(temp_sum==USART_SER_RX_BUF[9])
									{
										pos_now[4]=USART_SER_RX_BUF[6]*256+USART_SER_RX_BUF[5];
										//spe_now[4]=USART_SER_RX_BUF[8]*256+USART_SER_RX_BUF[7];
										t_e=1;
										USART_SER_RX_STA=0;
									}
							}
						}
						
						for(t_e=0;t_e!=1;)
						{
							USART_SER_RX_STA=0;
							ReadServoPositionVelocity(6);
							if(USART_SER_RX_STA&0x8000&&USART_SER_RX_BUF[0]==0xFF&&USART_SER_RX_BUF[1]==0xFF&&USART_SER_RX_BUF[2]==0x06&&USART_SER_RX_BUF[3]==0x06&&USART_SER_RX_BUF[4]==0x00)
							{
									unsigned char temp_sum = 0; 
									temp_sum = USART_SER_RX_BUF[2]+USART_SER_RX_BUF[3]+USART_SER_RX_BUF[4]+USART_SER_RX_BUF[5]+USART_SER_RX_BUF[6]+USART_SER_RX_BUF[7]+USART_SER_RX_BUF[8];
									temp_sum = ~temp_sum;
									if(temp_sum==USART_SER_RX_BUF[9])
									{
										pos_now[5]=USART_SER_RX_BUF[6]*256+USART_SER_RX_BUF[5];
										//spe_now[5]=USART_SER_RX_BUF[6]*256+USART_SER_RX_BUF[5];
										t_e=1;
										USART_SER_RX_STA=0;
									}
							}
						}
				if((anser10-pos_now[0])<3)
					anser1=anser10;
				else
				  anser1+=anser10-pos_now[0];
				if((anser20-pos_now[1])<3)
					anser2=anser20;
				else
				  anser2+=anser20-pos_now[1];
				if((anser30-pos_now[2])<3)
					anser3=anser30;
				else
				  anser3+=anser30-pos_now[2];
				if((anser40-pos_now[3])<3)
					anser4=anser40;
				else
				  anser4+=anser40-pos_now[3];
				if((anser50-pos_now[4])<3)
					anser5=anser50;
				else
				  anser5+=anser50-pos_now[4];
				if((anser60-pos_now[5])<3)
					anser6=anser60;
				else
				   anser6+=anser60-pos_now[5];
       if((anser10-pos_now[0])<3&&(anser20-pos_now[1])<3&&(anser30-pos_now[2])<3&&(anser40-pos_now[3])<3&&(anser50-pos_now[4])<3&&(anser60-pos_now[5])<3)
             ac=10;				 
				SyncSetServoPosition(anser1,200,anser2,200,anser3,200,anser4,200,anser5,200,anser6,200); 
				delay_ms(500);
				}
				SyncSetServoPosition(anser10,200,anser20,200,anser30,200,anser40,200,anser50,200,anser60,200); 
				delay_ms(500);*/
 /*T06[0][3]=px;//(7)257.23(不可以)(18,-83,-32,136,148,226)//(6)175.527(可以)(25,-65,-58,97,199,188)//(5)-85.57(不可以)(130,-110,-14,97,164,212)//(4)-187.53(187可以）(153,-77,-29,167,118,242)//(3)-40.11813428338(不可以)(118,-77,-34,138,128,129)//(2)-2(2可以)(91,-82,-85,220,159,206)//(1)113(可以)(70,-70,-60,170,90,270)
		 T06[1][3]=py;//(7)39.15//164.29//136.85//133.83//248.79//57//189.9643938684
		 T06[2][3]=pz;//(7)238.84//296.38//305.49//110.54//176.69//394//129.101311235
		 nx=T06[0][0]=0;//(7)0.02043242136061;//(6)0.7551514191853//(5)0.4701508238582;//(4)-0.1145584379767//(3)0.9952367286328//(2)-0.8799137385885//(1)-0.8872406672317;//0.0000001;//0.577350269;//-0.2198463103941;
		 ny=T06[1][0]=0;//(7)0.1204322662365;//-0.6545634760056//0.5979929575947;//-0.8515417447441//-0.08936929887499//0.2639802274939//0.4417115427304;//1;//0.577350269;//-0.604022773555;
		 nz=T06[2][0]=1;//(7)-0.9925112520302;//-0.03595816989003//-0.6491245069265;//-0.5116180423387//0.03894845823584//-0.395052214442//-0.1330222215605;//0.00000000001;//0.577350269;//0.7660444431187;
		 ox=T06[0][1]=1;//(7)0.06077899487909;//0.3366061531808//-0.8245550656039;//-0.8444652876769//0.007483236538389//-0.4108486349933//0.262002630229;//-1;//0.577350269;//0.2620026302292;
		 oy=T06[1][1]=0;//(7)0.9907328153152;//0.4342334105242//0.03531585605201;//-0.187772202064//-0.3283096647355//-0.8403574228428//0.7198463103937;//0.00000000001;//0.577350269;//0.7198463103927;
		 oz=T06[2][1]=0;//(7)0.1214677012174;//-0.8355463139918//-0.5646784342423;//0.5016173621804//-0.9445405048023//0.3535573517793//0.6427876096859;//0.00000000001;//0.577350269;//0.6427876096869;
		 ax=T06[0][2]=0;//(7)0.997942097488;//0.5625323383977//-0.3147493393361;//-0.5232157702106//0.09720007793858//-0.238652910662//0.3796822621131;//0.00000000001;//0.577350269;//-0.9396926207857;
		 ay=T06[1][2]=1;//(7)-0.06280571555756;//0.6188602435639//0.8007229314678;//0.4895081787776//0.9403328625863//0.4734066342642//0.5354551357782;//0.00000000001;//0.577350269;//0.3420201433262;//0.707106781,0.577350269
		 az=T06[2][2]=0;//(7)0.01292331824544;//0.5482420698859//0.5096819011983;//-0.6975865542804//-0.3260770651072//0.8478979578152//-0.7544065067359;//1;//0.577350269;//0.00000000000001;*/
						/*double a2=88,a3=33,d4=104,a5=0,d6=0;  //angle角1、2、3、4、5、6,ac1:角1的精度，ac2：计算精度
		        double ang1,ang2,ang3,ang4,ang5,ang6;
						unsigned short int x=0,y=0;
			  double T01[4][4]={{0,0,0,0},{0,0,0,0},{0,0,1,0},{0,0,0,1}};
				double T12[4][4]={{0,0,0,0},{0,0,1,0},{0,0,0,0},{0,0,0,1}};
				double T23[4][4]={{0,0,0,0},{0,0,0,0},{0,0,1,0},{0,0,0,1}};
				double T34[4][4]={{0,0,0,0},{0,0,1,0},{0,0,0,0},{0,0,0,1}};
				double T45[4][4]={{0,0,0,0},{0,0,-1,0},{0,0,0,0},{0,0,0,1}};
				double T56[4][4]={{0,0,0,0},{0,0,1,0},{0,0,0,0},{0,0,0,1}};
        double T13[4][4],T14[4][4],T15[4][4],T16[4][4],T060[4][4];*/
				
	


