#include<reg52.h>
#include "uart.h"

unsigned char code uart_baud = 256 - 11059200/12/32/14400;

void uart_config(void){
//	PCON = PCON | 0x80;
	SCON = 0x50;
	ES = 1;		// 串口中断使能
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