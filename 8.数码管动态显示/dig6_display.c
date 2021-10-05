/*----------------------------------------------
实验：点亮六位数码管
时间： 2016-4-19 11:12 AM ★2

	数码管译码表：
	0-9		0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90,
	A-F		0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E

----------------------------------------------*/

#include <reg52.h>

sbit ADDR2 = P1^2;
sbit ADDR1 = P1^1;
sbit ADDR0 = P1^0;

sbit ADDR3 = P1^3;
sbit ENLED = P1^4;

unsigned int display[6] = { 0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92 }; 
unsigned char cnt_timer0 = 0;
unsigned char cnt_dig = 0;

void dig_init(void);
void dig_ctrl(unsigned char s);
void timer0_set(void);

void main(void){
	dig_init();
	timer0_set();
	TF0 = 1;
	while(1);
}

void dig_init(void) {
	ADDR3 = 1;
	ENLED = 0;
}

void dig_ctrl(unsigned char s) {
	P0 = 0xFF;
	switch(s) {
		case 0: ADDR2 = 0, ADDR1 = 0, ADDR0 = 0; P0 = display[0]; break;
		case 1: ADDR2 = 0, ADDR1 = 0, ADDR0 = 1; P0 = display[1]; break;
		case 2: ADDR2 = 0, ADDR1 = 1, ADDR0 = 0; P0 = display[2]; break;
		case 3: ADDR2 = 0, ADDR1 = 1, ADDR0 = 1; P0 = display[3]; break;
		case 4: ADDR2 = 1, ADDR1 = 0, ADDR0 = 0; P0 = display[4]; break;
		case 5: ADDR2 = 1, ADDR1 = 0, ADDR0 = 1; P0 = display[5]; break;
		default: break;
	}
}

void timer0_set(void) {
	TMOD = 0x02;	// 8 位自动重装模式，定时 50 us
	TH0 = 0xD2;
	TL0 = 0xD2;
	EA = 1;
	ET0 = 1;
	TR0 = 1;
}

void timer0_interrupt(void) interrupt 1 {
	if(cnt_timer0>=19){
		dig_ctrl(cnt_dig);
		if(cnt_dig>=6)
			cnt_dig = 0;
		else
			cnt_dig = cnt_dig + 1;
		cnt_timer0 = 0;
	}else
		cnt_timer0 = cnt_timer0 + 1;
} 