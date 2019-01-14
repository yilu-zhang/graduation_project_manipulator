#ifndef __SERVO_H
#define __SERVO_H
#include "sys.h"

void SetServoId(unsigned char id);
void SendServoByte(unsigned char data);
void Servo_Init(unsigned char id, unsigned short int cw_limit, unsigned short int ccw_limit);
void SetServoPosition(unsigned char id, unsigned short int position, unsigned short int
velocity);
void SetServoVelocity(unsigned char id, signed short int velocity);
void ReadServoPositionVelocity(unsigned char id);
void SyncSetServoPosition(unsigned short int position1, unsigned short int velocity1,unsigned short int position2, unsigned short int velocity2,unsigned short int position3, unsigned short int velocity3,unsigned short int position4, unsigned short int velocity4,unsigned short int position5, unsigned short int velocity5,unsigned short int position6, unsigned short int velocity6);

#endif
