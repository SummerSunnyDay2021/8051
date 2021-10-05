#include<reg52.h>
#include"io.h"

bit pwm_out = 0;
unsigned long freq = 200;
unsigned char duty = 2;
unsigned char on_th0, on_tl0, off_th0, off_tl0;

void led_init(void);
void timer0_init(void);
void timer0_calc(void);
void timer0_config(void);

void main(void){
	led_init();
	timer0_init();
	timer0_calc();
	TF0 = 1;
	while(1){
		if(TF0){
			TF0 = 0;
			timer0_config();
			pwm_out = !pwm_out;
			Led0 = !Led0;
		}
	}
}


void led_init(void){
	ADDR0 = 0;
	ADDR1 = 1;
	ADDR2 = 1;
	ADDR3 = 1;
	ENLED = 0;	
}

void timer0_init(void){
	TMOD = 0x01;	// 16 位自动重装模式
	TR0 = 1;
}

void timer0_calc(void){
	on_th0  = (unsigned char)((65536 - (unsigned long)9126*duty/freq)>>8);
	on_tl0  = (unsigned char)(65536 - (unsigned long)9126*duty/freq);
	off_th0 = (unsigned char)((65536 - (unsigned long)9126*(100-duty)/freq)>>8);
	off_tl0 = (unsigned char)(65536 - (unsigned long)9126*(100-duty)/freq);
}


void timer0_config(void){
	if(pwm_out){
	   	TH0 = off_th0;
		TL0 = off_tl0;
	}else{
		TH0 = on_th0;
		TL0 = on_tl0;
	}
}