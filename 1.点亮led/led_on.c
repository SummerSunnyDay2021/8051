/*
 * led ���� 
 * 2016-4-15 *5 17:25
 �� kst-51 ��������ʵ�� led ����

	��·ͼ���� 8 �� led ������ͬʱ�ӵ� vcc ��һ�������ܿ��ƣ������ܵĻ����ӵ��� LEDS6 ������ LEDS6 �� 74138 ���ơ�
	��Ҫ�� LEDS6 ����� �ߵ�ƽ��74138 ������ ADDR ����ֱ���� 110��000--0000_0001, 001--0000_0010, ..., 110--0100_0000, 111--1000_0000), ���� 74138 ��ʹ���ź� ADDR3 �� ENLED �����ʵ����á�

	led �ĸ������ӵ�ʱ P0 �ڣ�����Ӧ�� P0 ���õ;Ϳ��Ե��� led �ˡ�
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