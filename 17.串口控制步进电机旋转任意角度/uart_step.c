#include<reg52.h>

unsigned char code uart_baud = 256 - 11059200/12/32/9600;
signed int rotation = 0;
unsigned char motor_tab[8] = {
	0x07, 0x03, 0x0B, 0x09, 0x0D, 0x0C, 0x0E, 0x06
};

void uart_config(void);
void baud_config(void);
void timer0_set (void);

void main(void){
	uart_config();
	baud_config();
	timer0_set();
	while(1);
}

void uart_config(void){
//	PCON = PCON | 0x80;
	SCON = 0x50;
	ES = 1;					// 串口中断使能
}

void baud_config(void){
	TMOD = TMOD & 0x0F;		// 清零 高 8 位
	TMOD = TMOD | 0x20;		// 高 8 位设为 4'b0010 即自动重装模式
	EA = 1;
	IT1 = 0;				//关闭 定时器 1 中断
	TH1 = uart_baud;
	TL1 = uart_baud;
	TR1 = 1;
}

void timer0_set (void) {
	TMOD = TMOD & 0xF0;
	TMOD = TMOD | 0x01;	//  定时器0 十六位计数模式
	TL0 = 0xCD;			//	设置为 2ms
	TH0 = 0xF8;
	ET0 = 1;			// 打开定时器0 中断
	TR0 = 1;			// 打开定时器0
}

void timer0_interrupt(void) interrupt 1 {
	static unsigned char state = 0;
	unsigned char tmp;
	TL0 = 0xCD;
	TH0 = 0xF8;
	tmp = P1;
	tmp = tmp & 0xF0;
	tmp = tmp | motor_tab[state];
	P1 = tmp;
	if(rotation>0){
		state = state + 1;
	}
//	else if(rotation<0)	{
//		state = state - 1;
//	}
	if(rotation != 0)
		SBUF = rotation;
		
	if(state==8){
		state = 0;
		rotation--;
	}
//	else if(state==-1) {
//		state = 7;
//		rotation++;
//	}
}

void uart_interrupt(void) interrupt 4 {
	if(TI){
		TI = 0;
	}
	if(RI){
		RI = 0;
//		if(SBUF>0x7F)
//			rotation = (SBUF&0x7F)*-64*8;
//		if(SBUF<=0x7F)
			rotation = ((SBUF/16*10)+SBUF%16)*512/360;
	}
}