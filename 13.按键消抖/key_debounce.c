#include<reg52.h>

sbit KeyIn1 = P2^4;
sbit KeyOut1 = P2^3;
sbit ADDR0 = P1^0;
sbit ADDR1 = P1^1;
sbit ADDR2 = P1^2;
sbit ADDR3 = P1^3;
sbit ENLED = P1^4;
sbit LED0 = P0^0;

void timer0_set(void);
void timer0_init(void);
void led_init(void);
void key_set(void);

void main(void){
	timer0_init();
	led_init();
	key_set();
	while(1);
}

void timer0_set(void){
	TH0 = 0xFC ;		// 20ms 
	TL0 = 0x65 ;	
}

void timer0_init(void){
	TMOD = 0x01;
	timer0_set();
	EA = 1;
	ET0 = 1;
	TR0 = 1;
}

void key_set(void){
	KeyIn1 = 0;
	KeyOut1 = 1;
}

void led_init(void) {
	ADDR0 = 0;
	ADDR1 = 1;
	ADDR2 = 1;
	ADDR3 = 1;
	ENLED = 0;	
}

void timer0_interrupt(void) interrupt 1 {
	static unsigned char key_buff = 0xFF;
	static bit key_state = 1;
	static bit tmp = 1;
	tmp = key_state;
	timer0_set();
	key_buff = (key_buff<<1) | KeyOut1;
	if(key_buff==0x00){
		key_state = 0;
	}else{
		key_state = 1;
	}
	if( (!tmp)&&(key_state) )
		LED0 = !LED0;
}


