#include <reg52.h>

sbit KeyIn4 = P2^7;
sbit KeyIn3 = P2^6;
sbit KeyIn2 = P2^5;
sbit KeyIn1 = P2^4;
sbit KeyOut1 = P2^3;
sbit KeyOut2 = P2^2;
sbit KeyOut3 = P2^1;
sbit KeyOut4 = P2^0;

sbit ADDR0 = P1^0;
sbit ADDR1 = P1^1;
sbit ADDR2 = P1^2;
sbit ADDR3 = P1^3;
sbit ENLED = P1^4;
sbit Led0 = P0^0;

void led_init(void);
void key1_set(void);

void main(void) {
	led_init();
	key1_set();
	while(1){
		if(KeyOut1)
			Led0 = 1;
		else
			Led0 = 0;
	}
}

void led_init(void) {
	ADDR0 = 0;
	ADDR1 = 1;
	ADDR2 = 1;
	ADDR3 = 1;
	ENLED = 0;	
}

void key1_set(void){
	KeyIn1 = 0;
	KeyOut1 = 1;
}