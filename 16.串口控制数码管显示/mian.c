#include <reg52.h>
#include "io.h"
#include "globl.h"

void main(void){
	dig_init();
	timer0_set();
	uart_config();
	baud_config();
	TF0 = 1;
	while(1){
		sec_calc(sec, display);
	}
}

void timer0_interrupt (void) interrupt 1 {
	TH0 = 0xFC;
	TL0 = 0x66;

	if(sel<6)
		sel = sel + 1;
	else
		sel = 0;
	
	if( (sec<100000)&&(sel>4) )
		sel = 0;
	else if( (sec<10000)&&(sel>3) )
		sel = 0;
	else if( (sec<1000)&&(sel>2) )
		sel = 0;
	else if( (sec<100)&&(sel>1) )
		sel = 0;
	else if( (sec<10)&&(sel>0) )
		sel = 0;

	dig_scan(sel, display);
}

void uart_interrupt(void) interrupt 4 {
	if(TI)
		TI = 0;
	if(RI) {
		RI = 0;
		if(SBUF<10)
			sec = sec*10 + SBUF%16;
		else if(SBUF==0x0C)
			sec = 0x00;
		else
			sec = sec*100 + (SBUF/16%16)*10 + (SBUF%16);
	}
}
