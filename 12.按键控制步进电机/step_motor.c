#include <reg52.h>

sbit KeyIn4 = P2^7;
sbit KeyIn3 = P2^6;
sbit KeyIn2 = P2^5;
sbit KeyIn1 = P2^4;
sbit KeyOut1 = P2^3;
sbit KeyOut2 = P2^2;
sbit KeyOut3 = P2^1;
sbit KeyOut4 = P2^0;

unsigned char motor_tab[8] = {
	0x07, 0x03, 0x0B, 0x09, 0x0D, 0x0C, 0x0E, 0x06
};

static bit is_res = 1;
static bit is_stop = 0;

void timer0_set (void) {
	TMOD = 0x01;	//  定时器0 十六位计数模式
	TL0 = 0xCD;		//	设置为 2ms
	TH0 = 0xF8;
	EA = 1;			// 打开总中断
	ET0 = 1;		// 打开定时器0 中断
	TR0 = 1;		// 打开定时器0
}


void key1_set(void);

void main(void) {
	timer0_set();
	key1_set();
	while(1){
		if(!KeyOut1){
			is_stop = 0;
			is_res = 0;
		}else if(!KeyOut3){
			is_stop = 0;
			is_res = 1;
		}else
			is_stop = 1;
	}
}

void key1_set(void){
	KeyIn4 = 0;
	KeyOut1 = 1;
	KeyOut3 = 1;
}

void timer0_interrupt(void) interrupt 1 {
	static signed char state = 0;
	unsigned char tmp;
	TL0 = 0xCD;
	TH0 = 0xF8;
	tmp = P1;
	tmp = tmp & 0xF0;
	tmp = tmp | motor_tab[state];
	
	if(is_stop)
		P1 = 0xFF;
	else
		P1 = tmp;

	if(is_res)
		state = state + 1;
	else
		state = state - 1;	

	if(state>=8)
		state = 0x00;
	else if(state<=0)
		state = 0x07;
}