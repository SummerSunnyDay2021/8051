#include<reg52.h>
#include"io.h"
#include"globl.h"

void main(void){
	unsigned char tmp[16] = {0};
	unsigned char i = 0;
	uart_config();
	baud_config();
	lcd_init();
	while(1){
		for(i=0;i<16;i++){
			if(tmp[i]!=str0[i]){
				tmp[i] = str0[i];
				lcd_line1_display(str0);
			}
		}
	};
}

void uart_interrupt(void) interrupt 4 {
	static unsigned char i = 0;
	if(TI)
		TI = 0;
	if(RI) {
		RI = 0;
		if(SBUF!=' ')
			if(str0[i]!='\0'){
				str0[i] = SBUF;
				i++;
			}else
				i = 0;
		else{
			i = 0;
			lcd_clean();
		}
	}
}