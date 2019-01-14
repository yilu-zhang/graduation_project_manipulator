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


int pos0_7=1250;   //手掌舵机当前位置
int num=0;
unsigned short int pos_now[20][7];
unsigned short int spe_now[20][7];
int id;
int main()
{	
	//static u8 t=0;
  uart_ser_init(1000000);	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	uart_init(115200);	//串口初始化波特率为115200
	delay_init(168);
	PWM_Init(19999,83);	//84M/84=1Mhz的计数频率,重装载值20000，所以PWM频率为 1M/20000=50hz.		
	Servo_Init(1,0,1023);
	Servo_Init(2,371,651);
	Servo_Init(3,98,685);
	Servo_Init(4,0,1023);
	Servo_Init(5,30,583);
	Servo_Init(6,0,1023);
	LED_Init();
	TIM_SetCompare3(TIM3,1250);
	delay_ms(1000);
	SetServoPosition(6,508, 511);
	delay_ms(1000);
	SetServoPosition(5,519, 511);
	delay_ms(500);
	SetServoPosition(4,514, 511);
	delay_ms(500);
	SetServoPosition(3,518, 511);
	delay_ms(500);
	SetServoPosition(2,508, 511);
	delay_ms(500);
	SetServoPosition(1,508, 511);
	delay_ms(500);
	while(1)
	{       
		 if(USART_RX_STA&0x8000)
		{				
			USART_RX_STA=0;
			id=((int)USART_RX_BUF[2]-48)*10+((int)USART_RX_BUF[3]-48);
			if(USART_RX_BUF[0]=='A')    //字头为A
			{
				if(USART_RX_BUF[1]=='A')  //若第二个字母为A，设置舵机ID
				{
					SetId();
				}
			  else if(USART_RX_BUF[1]=='B') //若接收第二个字母为B，设置舵机位置和速度
				{
					SetPosVel();				
				}	
        else if(USART_RX_BUF[1]=='C')	//若接收第二个字母为C，设置舵机速度
				{
				   SetVel();					
				}	
       else if(USART_RX_BUF[1]=='D')
			 {
				 Teach();
			 }
       else if(USART_RX_BUF[1]=='I')   //直线运动
			 {
				 RectMotion();
			 }
		 
			}		
		}
  }
}
/**曾用函数***/
	/*if(t_tea0==0)
											{
												if((pos_now[0][5]-pos_now[t_tea][5])>1||(pos_now[0][5]-pos_now[t_tea][5])<-1)
												{
													SetServoPosition(6,pos_now[t_tea0][5],spe_now[t_tea0][5]);
											    delay_ms(500);
												}
											}
											else if((pos_now[t_tea0][5]-pos_now[t_tea0-1][5])>1||(pos_now[t_tea0-1][5]-pos_now[t_tea0-1][5])<-1)
											{
											   SetServoPosition(6,pos_now[t_tea0][5],spe_now[t_tea0][5]);
											   delay_ms(500);
											}
											if(t_tea0==0)
											{
												if((pos_now[0][3]-pos_now[t_tea][3])>1||(pos_now[0][3]-pos_now[t_tea][3])<-1)
												{
													SetServoPosition(4,pos_now[t_tea0][3],spe_now[t_tea0][3]);
											   delay_ms(500);
												}
											}
											else if((pos_now[t_tea0][3]-pos_now[t_tea0-1][3])>1||(pos_now[t_tea0-1][3]-pos_now[t_tea0-1][3])<-1)
											{
											   SetServoPosition(4,pos_now[t_tea0][3],spe_now[t_tea0][3]);
											   delay_ms(500);
											}
											if(t_tea0==0)
											{
												if((pos_now[0][4]-pos_now[t_tea][4])>1||(pos_now[0][4]-pos_now[t_tea][4])<-1)
												{
													SetServoPosition(5,pos_now[t_tea0][4],spe_now[t_tea0][4]);
											   delay_ms(500);
												}
											}
											else if((pos_now[t_tea0][4]-pos_now[t_tea0-1][4])>1||(pos_now[t_tea0-1][4]-pos_now[t_tea0-1][4])<-1)
											{
											   SetServoPosition(5,pos_now[t_tea0][4],spe_now[t_tea0][4]);
											   delay_ms(500);
											}*/
											/* SetServoPosition(6,pos_now[t_tea][5],spe_now[t_tea][5]);
											delay_ms(500);
										  SetServoPosition(4,pos_now[t_tea][3],spe_now[t_tea][3]);
											 delay_ms(500);
										 SetServoPosition(5,pos_now[t_tea][4],spe_now[t_tea][4]);
											 delay_ms(500);*/
											 /*int t_e;
		
		               for(t_e=0;t_e!=1;)
										{
											USART_SER_RX_STA=0;
											ReadServoPositionVelocity(3);
										  if(USART_SER_RX_STA&0x8000)
		                  {
												
			                  if(USART_SER_RX_BUF[0]==0xFF)
													if(USART_SER_RX_BUF[1]==0xFF)
														if(USART_SER_RX_BUF[2]==0x03)
														if(USART_SER_RX_BUF[3]==0x06)
															if(USART_SER_RX_BUF[4]==0x00)
														{
														  unsigned char temp_sum = 0; 
												      temp_sum = USART_SER_RX_BUF[2]+USART_SER_RX_BUF[3]+USART_SER_RX_BUF[4]+USART_SER_RX_BUF[5]+USART_SER_RX_BUF[6]+USART_SER_RX_BUF[7]+USART_SER_RX_BUF[8];
	                            temp_sum = ~temp_sum;
															if(temp_sum==USART_SER_RX_BUF[9])
															{
																posn=USART_SER_RX_BUF[6]*256+USART_SER_RX_BUF[5];
															  spen=USART_SER_RX_BUF[8]*256+USART_SER_RX_BUF[7];
																t_e=1;
																USART_SER_RX_STA=0;
															}
																
														}
														
													}
										}
		if(posn+100>1023)
			len=1;
		if(posn-100<0)
			len=0;
		if(len==1)
			posn-=100;
		if(len==0)
			posn+=100;
		if(spen+10>800)
			len1=1;
		if(spen-10<300)
			len1=0;
		if(len1==1)
			spen-=50;
		if(len1==0)
			spen+=50;
		SetServoPosition(3,posn, 511);
		delay_ms(500);
		SetServoPosition(3,posn, 511);
		delay_ms(500);*/
		/*len=0;
		
			//delay_ms(1);
		
			//len=USART_VS_RX_STA&0x3fff;//得到此次接收到的数据长度
			printf("\r\nThe message is:\r\n");
			for(t=0;t<9;t++)
			{
				USART_SendData(USART1, USART_RX_BUF[t]);         //向串口1发送数据
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
			}
			printf("\r\n\r\n");//插入换*/
		
  
