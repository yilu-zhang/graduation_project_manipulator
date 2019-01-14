#include "servo.h"
#include "delay.h"

void SendServoByte(unsigned char data)
{
   USART_SendData(USART3, data);         //向串口1发送数据
	 while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);//等待发送结束
}
void Servo_Init(unsigned char id, unsigned short int cw_limit, unsigned short int ccw_limit)
{
	unsigned short int temp_ccw = 0; 

  unsigned short int temp_cw = 0;
  unsigned char temp_ccw_h = 0; 

  unsigned char temp_ccw_l = 0; 

  unsigned char temp_cw_h = 0;
  unsigned char temp_cw_l =0;
  unsigned char temp_sum = 0; 
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed=GPIO_High_Speed;
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	GPIO_SetBits(GPIOE,GPIO_Pin_6);
	GPIO_ResetBits(GPIOE,GPIO_Pin_5);
  

  if (ccw_limit > 1023)
  {
    temp_ccw = 1023; 

  }
  else
  {
     temp_ccw = ccw_limit;
  }
  if (cw_limit > 1023)
  {
     temp_cw = 1023;
  }
  else
  {
     temp_cw = cw_limit;
  }
  temp_ccw_h = (unsigned char)(temp_ccw >> 8);
  temp_ccw_l = (unsigned char)temp_ccw; 
  temp_cw_l = (unsigned char)temp_cw;

   PEout(5)=0;
   PEout(6)=1; 
   SendServoByte(0xFF); 
   SendServoByte(0xFF); 
   SendServoByte(id); 
   SendServoByte(7);
   SendServoByte(0x03); 
   SendServoByte(0x06); 
   SendServoByte(temp_cw_l); 
   SendServoByte(temp_cw_h); 
   SendServoByte(temp_ccw_l);
   SendServoByte(temp_ccw_h); 
   temp_sum = id + 7 + 0x03 + 0x06 + temp_cw_l + temp_cw_h + temp_ccw_l + temp_ccw_h;
   temp_sum = ~temp_sum; 
   SendServoByte(temp_sum);
   PEout(5)=1;
   PEout(6)=0; 
   delay_ms(2);	 
	 
}


 /******设置舵机ID函数*****/
void SetServoId(unsigned char id)           
{
	unsigned char temp_sum = 0; 
  PEout(5)=0;
  PEout(6)=1;  
	SendServoByte(0xFF); 
	SendServoByte(0xFF);
	SendServoByte(0xFE); 
	SendServoByte(0x04); 
	SendServoByte(0x03); 
	SendServoByte(0x03);
	SendServoByte(id); 
	temp_sum = 0xFE + 4 + 0x03 + 0x03+id;
	temp_sum = ~temp_sum; 
	SendServoByte(temp_sum); 
	PEout(5)=1;
   PEout(6)=0; 
	
	delay_ms(2); 
}


/******设置舵机位置函数******/
void SetServoPosition(unsigned char id, unsigned short int position, unsigned short int velocity)   
{
   unsigned short int temp_velocity = 0; 
   unsigned short int temp_position = 0;
   unsigned char temp_velocity_h = 0; 
   unsigned char temp_velocity_l = 0; 
   unsigned char temp_position_h = 0;
   unsigned char temp_position_l = 0;
   unsigned char temp_sum = 0; 
   if (velocity > 1023)
   {
      temp_velocity = 1023; 
   }
   else
   {
      temp_velocity = velocity;
   }
   if (position > 1023)
   {
     temp_position = 1023;
	 }
	else
	{
	temp_position = position;
	}
	temp_velocity_h = (unsigned char)(temp_velocity >> 8);
	temp_velocity_l = (unsigned char)temp_velocity;
	temp_position_h = (unsigned char)(temp_position >> 8);
	temp_position_l = (unsigned char)temp_position;
	PEout(5)=0;
   PEout(6)=1;  
	SendServoByte(0xFF); 
	SendServoByte(0xFF);
	SendServoByte(id); 
	SendServoByte(7); 
	SendServoByte(0x03); 
	SendServoByte(0x1E);
	SendServoByte(temp_position_l); 
	SendServoByte(temp_position_h);
	SendServoByte(temp_velocity_l); 
	SendServoByte(temp_velocity_h); 
	temp_sum = id + 7 + 0x03 + 0x1E + temp_position_l + temp_position_h + temp_velocity_l +
	temp_velocity_h;
	temp_sum = ~temp_sum; 
	SendServoByte(temp_sum); 
	PEout(5)=1;
   PEout(6)=0; 
	
	delay_ms(2); 
}

/*******设置舵机速度函数**********/
void SetServoVelocity(unsigned char id, signed short int velocity)
{
	unsigned char temp_sign = 0; 
	unsigned short int temp_velocity = 0;
	unsigned char temp_value_h = 0; 
	unsigned char temp_value_l = 0;
	unsigned char temp_sum = 0; 
	if (velocity < 0)
	{
	   temp_velocity = -velocity; 
	   temp_sign = 1; 
	}
	else
	{
	   temp_velocity = velocity;
	   temp_sign = 0; 
	}
	if (temp_velocity > 1023)
	{
	   temp_velocity = 1023;
	}
	temp_velocity |= (temp_sign << 10);
	temp_value_h = (unsigned char)(temp_velocity >> 8);
	
	temp_value_l = (unsigned char)temp_velocity;
	PEout(5)=0;
   PEout(6)=1; 
	SendServoByte(0xFF); 
	SendServoByte(0xFF); 
	SendServoByte(id); 
	SendServoByte(5); 
	SendServoByte(0x03); 
	SendServoByte(0x20);
	SendServoByte(temp_value_l); 
	SendServoByte(temp_value_h); 
	temp_sum = id + 5 + 0x03 + 0x20 + temp_value_l + temp_value_h;
	temp_sum = ~temp_sum;
	SendServoByte(temp_sum);
	PEout(5)=1;
  PEout(6)=0;
	delay_ms(2);

}
/*****读取舵机号为id的舵机的速度和位置******/
void ReadServoPositionVelocity(unsigned char id)
{
	unsigned char temp_sum = 0; 
  PEout(5)=0;
  PEout(6)=1; 
	SendServoByte(0xFF); 
	SendServoByte(0xFF); 
	SendServoByte(id); 
	SendServoByte(0x04); 
	SendServoByte(0x02); 
	SendServoByte(0x24);
	SendServoByte(0x04); 
	temp_sum = id + 0x04+ 0x02 + 0x24 +0x04;
	temp_sum = ~temp_sum;
	SendServoByte(temp_sum);
	PEout(5)=1;
  PEout(6)=0;
	delay_ms(4); 
}



/********同时设置多个舵机函数*****/
void SyncSetServoPosition(unsigned short int position1, unsigned short int velocity1,unsigned short int position2, unsigned short int velocity2,unsigned short int position3, unsigned short int velocity3,unsigned short int position4, unsigned short int velocity4,unsigned short int position5, unsigned short int velocity5,unsigned short int position6, unsigned short int velocity6)   
{
   unsigned short int temp_velocity[6],i; 
   unsigned short int temp_position[6];
   unsigned char temp_velocity_h[6]; 
   unsigned char temp_velocity_l[6]; 
   unsigned char temp_position_h[6];
   unsigned char temp_position_l[6];
   unsigned char temp_sum = 0; 
   if (velocity1 > 1023)
   {
      temp_velocity[0] = 1023; 
   }
   else
   {
      temp_velocity[0] = velocity1;
   }
   if (position1> 1023)
   {
     temp_position[0] = 1023;
	 }
	else
	{
	temp_position[0] = position1;
	}
	temp_velocity_h[0] = (unsigned char)(temp_velocity[0] >> 8);
	temp_velocity_l[0] = (unsigned char)temp_velocity[0];
	temp_position_h[0] = (unsigned char)(temp_position[0] >> 8);
	temp_position_l[0] = (unsigned char)temp_position[0];
	if (velocity2 > 1023)
   {
      temp_velocity[1] = 1023; 
   }
   else
   {
      temp_velocity[1] = velocity2;
   }
   if (position2> 651)
   {
     temp_position[1] = 651;
	 }
	  else if (position2< 371)
   {
     temp_position[1] = 371;
	 }
	else
	{
	temp_position[1] = position2;
	}
	temp_velocity_h[1] = (unsigned char)(temp_velocity[1] >> 8);
	temp_velocity_l[1] = (unsigned char)temp_velocity[1];
	temp_position_h[1] = (unsigned char)(temp_position[1] >> 8);
	temp_position_l[1] = (unsigned char)temp_position[1];
	if (velocity3 > 1023)
   {
      temp_velocity[2] = 1023; 
   }
   else
   {
      temp_velocity[2] = velocity3;
   }
   if (position3> 685)
   {
     temp_position[2] = 685;
	 }
	 else if (position3< 98)
   {
     temp_position[2] = 98;
	 }
	else
	{
	temp_position[2] = position3;
	}
	temp_velocity_h[2] = (unsigned char)(temp_velocity[2] >> 8);
	temp_velocity_l[2] = (unsigned char)temp_velocity[2];
	temp_position_h[2] = (unsigned char)(temp_position[2] >> 8);
	temp_position_l[2] = (unsigned char)temp_position[2];
	if (velocity4> 1023)
   {
      temp_velocity[3] = 1023; 
   }
   else
   {
      temp_velocity[3] = velocity4;
   }
   if (position4> 1023)
   {
     temp_position[3] =1023;
	 }
	else
	{
	temp_position[3] = position4;
	}
	temp_velocity_h[3] = (unsigned char)(temp_velocity[3] >> 8);
	temp_velocity_l[3] = (unsigned char)temp_velocity[3];
	temp_position_h[3] = (unsigned char)(temp_position[3] >> 8);
	temp_position_l[3] = (unsigned char)temp_position[3];
	if (velocity5 > 1023)
   {
      temp_velocity[4] = 1023; 
   }
   else
   {
      temp_velocity[4] = velocity5;
   }
   if (position5> 583)
   {
     temp_position[4] = 583;
	 }
	  else if (position5< 30)
   {
     temp_position[4] =30;
	 }
	else
	{
	temp_position[4] = position5;
	}
	temp_velocity_h[4] = (unsigned char)(temp_velocity[4] >> 8);
	temp_velocity_l[4] = (unsigned char)temp_velocity[4];
	temp_position_h[4] = (unsigned char)(temp_position[4] >> 8);
	temp_position_l[4] = (unsigned char)temp_position[4];
	if (velocity6> 1023)
   {
      temp_velocity[5] = 1023; 
   }
   else
   {
      temp_velocity[5] = velocity6;
   }
   if (position6> 1023)
   {
     temp_position[5] = 1023;
	 }
	else
	{
	temp_position[5] = position6;
	}
	temp_velocity_h[5] = (unsigned char)(temp_velocity[5] >> 8);
	temp_velocity_l[5] = (unsigned char)temp_velocity[5];
	temp_position_h[5] = (unsigned char)(temp_position[5] >> 8);
	temp_position_l[5] = (unsigned char)temp_position[5];
	PEout(5)=0;
   PEout(6)=1;  
	SendServoByte(0xFF); 
	SendServoByte(0xFF);
	SendServoByte(0xFE); 
	SendServoByte(0x22); 
	SendServoByte(0x83); 
	SendServoByte(0x1E);
	SendServoByte(0x04);
	SendServoByte(0x01);
	SendServoByte(temp_position_l[0]); 
	SendServoByte(temp_position_h[0]);
	SendServoByte(temp_velocity_l[0]); 
	SendServoByte(temp_velocity_h[0]);
SendServoByte(0x02);
	SendServoByte(temp_position_l[1]); 
	SendServoByte(temp_position_h[1]);
	SendServoByte(temp_velocity_l[1]); 
	SendServoByte(temp_velocity_h[1]);
SendServoByte(0x03);
	SendServoByte(temp_position_l[2]); 
	SendServoByte(temp_position_h[2]);
	SendServoByte(temp_velocity_l[2]); 
	SendServoByte(temp_velocity_h[2]); 
SendServoByte(0x04);
	SendServoByte(temp_position_l[3]); 
	SendServoByte(temp_position_h[3]);
	SendServoByte(temp_velocity_l[3]); 
	SendServoByte(temp_velocity_h[3]); 
SendServoByte(0x05);
	SendServoByte(temp_position_l[4]); 
	SendServoByte(temp_position_h[4]);
	SendServoByte(temp_velocity_l[4]); 
	SendServoByte(temp_velocity_h[4]); 
SendServoByte(0x06);
	SendServoByte(temp_position_l[5]); 
	SendServoByte(temp_position_h[5]);
	SendServoByte(temp_velocity_l[5]); 
	SendServoByte(temp_velocity_h[5]);  	
	temp_sum =(unsigned char)(0xFE+0x22+0x83+0x1E+0x05+0x14);
	for(i=0;i<6;i++)
	{
	temp_sum+=temp_position_l[i] + temp_position_h[i] + temp_velocity_l[i] +temp_velocity_h[i];
	}
	temp_sum = ~temp_sum; 
	SendServoByte(temp_sum); 
	PEout(5)=1;
   PEout(6)=0; 
	
	delay_ms(2); 
}
