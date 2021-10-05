/*
 * led 驱动 
 * 2016-4-15 *5 17:25
 在 kst-51 开发板上实现 led 驱动

	电路图描述 8 个 led 的正极同时接到 vcc 被一个三极管控制，三极管的基极接到了 LEDS6 处，而 LEDS6 被 74138 控制。
	想要在 LEDS6 上输出 高电平，74138 的输入 ADDR 必须分别等于 110（000--0000_0001, 001--0000_0010, ..., 110--0100_0000, 111--1000_0000), 此外 74138 的使能信号 ADDR3 和 ENLED 必须适当设置。

	led 的负极连接的时 P0 口，被对应的 P0 口置低就可以点亮 led 了。
**/


#include <reg52.h>
sbit ADDR0 = P1^0;
sbit ADDR1 = P1^1;
sbit ADDR2 = P1^2;
sbit ADDR3 = P1^3;
sbit ENLED = P1^4;

void led_init(void) {
	ADDR0 = 0;
	ADDR1 = 1;
	ADDR2 = 1;
	ADDR3 = 1;
	ENLED = 0;	
}

void main(void) {
	unsigned char i;
	unsigned char num[8] = { 0, 0, 1, 0, 1, 0, 0, 0 };
	led_init();
	P0 = 0x00;
	for(i=8;i>0;i--) {
		P0 = P0 + (!num[8-i]<<(i-1));
	}
	while(1);
}