#ifndef __LED_H
#define __LED_H
#include "sys.h"

#define LED0 PFout(9)	// DS0
#define LED1 PFout(10)	// DS1	 
void LED_Init(void);
	
#endif

