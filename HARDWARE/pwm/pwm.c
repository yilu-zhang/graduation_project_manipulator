#include "pwm.h"

/*******PWM产生函数*******/
void PWM_Init(u32 arr3,u32 psc3)
{ 
	GPIO_InitTypeDef GPIO_Initstructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);  //使能GPIOB时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);   //使能TIM3时钟
  //初始化PB0
	GPIO_Initstructure.GPIO_Pin=GPIO_Pin_0;
	GPIO_Initstructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_Initstructure.GPIO_OType=GPIO_OType_PP;
	GPIO_Initstructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Initstructure.GPIO_Speed=GPIO_High_Speed;
	GPIO_Init(GPIOB,&GPIO_Initstructure);
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource0,GPIO_AF_TIM3);  //复用PB0为TIM3
	//初始化TIM3
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;    
	TIM_TimeBaseInitStructure.TIM_CounterMode =TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Prescaler =psc3;
	TIM_TimeBaseInitStructure.TIM_Period=arr3;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	
	TIM_OCInitStructure.TIM_OCMode =TIM_OCMode_PWM1;   //选择定时器模式为PWM向上计数
	TIM_OCInitStructure.TIM_OCPolarity =TIM_OCPolarity_High;//比较输出极性高
	TIM_OCInitStructure.TIM_OutputState =TIM_OutputState_Enable;
	TIM_OC3Init(TIM3,&TIM_OCInitStructure);

  TIM_OC3PreloadConfig(TIM3,TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM3,ENABLE);
	
	TIM_Cmd(TIM3,ENABLE);

}
