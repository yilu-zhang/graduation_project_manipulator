#include "timer3.h"




void TIM3_CH1_Cap_Init(u32 arr2,u16 psc2)
{
	TIM_ICInitTypeDef  TIM3_ICInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure1;
	NVIC_InitTypeDef NVIC_InitStructure;

	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  	//TIM5时钟使能 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//使能PORTA时钟		
	
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //GPIOA1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //下拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA0

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_TIM3); //PA0复用位定时器5
	  
	TIM_TimeBaseStructure1.TIM_Prescaler=psc2;  //定时器分频
	TIM_TimeBaseStructure1.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure1.TIM_Period=arr2;   //自动重装载值
	TIM_TimeBaseStructure1.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure1);
	

	//初始化TIM5输入捕获参数
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	选择输入端 IC1映射到TI1上
  TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
  TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
  TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
  TIM3_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
  TIM_ICInit(TIM3, &TIM3_ICInitStructure);
	
		
	TIM_ITConfig(TIM3,TIM_IT_Update|TIM_IT_CC1,ENABLE);//允许更新中断 ,允许CC1IE捕获中断	
	
  TIM_Cmd(TIM3,ENABLE ); 	//使能定时器5

 
  NVIC_InitStructure.NVIC_IRQChannel= TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
	
	
}

void TIM3_CH2_Cap_Init(u32 arr3_2,u16 psc3_2)
{
	TIM_ICInitTypeDef  TIM3_ICInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure1;
	NVIC_InitTypeDef NVIC_InitStructure;

	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  	//TIM5时钟使能 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//使能PORTA时钟		
	
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //GPIOA1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //下拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA0

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_TIM3); //PA0复用位定时器5
	  
	TIM_TimeBaseStructure1.TIM_Prescaler=psc3_2;  //定时器分频
	TIM_TimeBaseStructure1.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure1.TIM_Period=arr3_2;   //自动重装载值
	TIM_TimeBaseStructure1.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure1);
	

	//初始化TIM5输入捕获参数
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_2; //CC1S=01 	选择输入端 IC1映射到TI1上
  TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;	//上升沿捕获
  TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
  TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
  TIM3_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
  TIM_ICInit(TIM3, &TIM3_ICInitStructure);
	
		
	TIM_ITConfig(TIM3,TIM_IT_Update|TIM_IT_CC2,ENABLE);//允许更新中断 ,允许CC1IE捕获中断	
	
  TIM_Cmd(TIM3,ENABLE ); 	//使能定时器5

 
  NVIC_InitStructure.NVIC_IRQChannel= TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
	
	
}


//捕获状态
//[7]:0,没有成功的捕获;1,成功捕获到一次.
//[6]:0,还没捕获到低电平;1,已经捕获到低电平了.
//[5:0]:捕获低电平后溢出的次数(对于32位定时器来说,1us计数器加1,溢出时间:4294秒)
u8  TIM3CH1_CAPTURE_STA=0;	//输入捕获状态
u8  TIM3CH2_CAPTURE_STA=0;	//输入捕获状态
//定时器5中断服务程序	 
void TIM3_IRQHandler(void)
{ 		    
	if((TIM3CH1_CAPTURE_STA&0X80)==0)//还未成功捕获	
	{
		if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)//溢出
		{	     
			if(TIM3CH1_CAPTURE_STA&0X40)//已经捕获到高电平了
			{
				if((TIM3CH1_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
				{
					TIM3CH1_CAPTURE_STA&=0X80;		//标记成功捕获了一次
				}else TIM3CH1_CAPTURE_STA++;
			}	 
		}
		if(TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)//捕获1发生捕获事件
		{	
			if(TIM3CH1_CAPTURE_STA&0X40)		//捕获到一个下降沿 		
			{	  			
				TIM3CH1_CAPTURE_STA&=0X80;		//标记成功捕获到一次高电平脉宽
	 			TIM_OC1PolarityConfig(TIM3,TIM_ICPolarity_Rising);				//CC1P=0 设置为上升沿捕
			}else  								//还未开始,第一次捕获上升沿
			{
				TIM3CH1_CAPTURE_STA=0;			//清空
				TIM3CH1_CAPTURE_STA|=0X40;		//标记捕获到了上升沿
				TIM_Cmd(TIM3,DISABLE ); 	//关闭定时器5
	 			TIM_SetCounter(TIM3,0);
	 			TIM_OC1PolarityConfig(TIM3,TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获
				TIM_Cmd(TIM3,ENABLE ); 	//使能定时器5
			}		    
		}			     	    					   
 	}
	if((TIM3CH2_CAPTURE_STA&0X80)==0)//还未成功捕获	
	{
		if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)//溢出
		{	     
			if(TIM3CH2_CAPTURE_STA&0X40)//已经捕获到高电平了
			{
				if((TIM3CH2_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
				{
					TIM3CH2_CAPTURE_STA&=0X80;		//标记成功捕获了一次
				}else TIM3CH2_CAPTURE_STA++;
			}	 
		}
		if(TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET)//捕获1发生捕获事件
		{	
			if(TIM3CH2_CAPTURE_STA&0X40)		//捕获到一个下降沿 		
			{	  			
				TIM3CH2_CAPTURE_STA&=0X80;		//标记成功捕获到一次高电平脉宽
	 			TIM_OC2PolarityConfig(TIM3,TIM_ICPolarity_Falling);				//CC1P=0 设置为上升沿捕
			}else  								//还未开始,第一次捕获上升沿
			{
				TIM3CH2_CAPTURE_STA=0;			//清空
				TIM3CH2_CAPTURE_STA|=0X40;		//标记捕获到了上升沿
				TIM_Cmd(TIM3,DISABLE ); 	//关闭定时器5
	 			TIM_SetCounter(TIM3,0);
	 			TIM_OC2PolarityConfig(TIM3,TIM_ICPolarity_Rising);		//CC1P=1 设置为下降沿捕获
				TIM_Cmd(TIM3,ENABLE ); 	//使能定时器5
			}		    
		}			     	    					   
 	}
	TIM_ClearITPendingBit(TIM3, TIM_IT_CC1|TIM_IT_CC2|TIM_IT_Update); //清除中断标志位
}
