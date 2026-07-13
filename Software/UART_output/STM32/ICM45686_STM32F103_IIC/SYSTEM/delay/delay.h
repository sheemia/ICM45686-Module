#ifndef __DELAY_H
#define __DELAY_H 			   
#include "sys.h"
extern uint32_t nowtime;//100us延时
void delay_init(void);
void delay_ms(u16 nms);
void delay_us(u32 nus);
void Initial_Timer3(void);
#endif





























