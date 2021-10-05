#include <reg52.h>

sbit KeyIn4 = P2^7;
sbit KeyIn3 = P2^6;
sbit KeyIn2 = P2^5;
sbit KeyIn1 = P2^4;
sbit KeyOut1 = P2^3;
sbit KeyOut2 = P2^2;

sbit ADDR0 = P1^0;
sbit ADDR1 = P1^1;
sbit ADDR2 = P1^2;
sbit ADDR3 = P1^3;
sbit ENLED = P1^4;

sbit Led0 = P0^0;
sbit Led1 = P0^1;
sbit Led2 = P0^2;
sbit Led3 = P0^3;
sbit Led4 = P0^4;
sbit Led5 = P0^5;
sbit Led6 = P0^6;
sbit Led7 = P0^7;

bit s = 0;

void led_init(void);
void line_set(bit s);
void delay(unsigned long time){
	while(time--);
}
void main(void) {
	led_init();
	while(1){
		if(s==0)
			s = 1;
		else if(s==1)
			s = 0;
		line_set(s);	
		
		
		if( (KeyIn1==0)&&(s==0) )
			Led0 = 0;
		else if( (KeyIn1==0)&&(s==1) )
			Led4 = 0;
		else if( (KeyIn2==0)&&(s==0) )
			Led1 = 0;
		else if( (KeyIn2==0)&&(s==1) )
			Led5 = 0;
		else if( (KeyIn3==0)&&(s==0) )
			Led2 = 0;
		else if( (KeyIn3==0)&&(s==1) )
			Led6 = 0;
		else if( (KeyIn4==0)&&(s==0) )
			Led3 = 0;
		else if( (KeyIn4==0)&&(s==1) )
			Led7 = 0;
		else
			P0 = 0xFF;

		delay(50);
	}
}

void led_init(void) {
	ADDR0 = 0;
	ADDR1 = 1;
	ADDR2 = 1;
	ADDR3 = 1;
	ENLED = 0;
	KeyIn1 = 1;
	KeyIn2 = 1;
	KeyIn3 = 1;
	KeyIn4 = 1;	
}

void line_set(bit s){
	if(s==0){
		KeyOut1 = 0;
		KeyOut2 = 1;
	}else if(s==1){
		KeyOut1 = 1;
		KeyOut2 = 0;
	}
}	