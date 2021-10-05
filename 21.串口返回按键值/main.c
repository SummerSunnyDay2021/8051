#include<reg52.h>
#include "key.h"
#include "uart.h"

void main(void) {
	uart_config();
	baud_config();
	scan_init();
	while(1){
		scan_cnt = scan_cnt + 1;
		if(scan_cnt>=4)		
			scan_cnt = 0;
		scan_control(scan_cnt);
		key_sent(scan_cnt);
		delay(100);
	}
}

void uart_interrupt(void) interrupt 4 {
	if(TI)
		TI = 0;
	if(RI)
		RI = 0;
}
