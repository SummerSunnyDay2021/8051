#include <reg52.h>
sbit ADDR3 = P1^3;
sbit ADDR2 = P1^2;
sbit ADDR1 = P1^1;
sbit ADDR0 = P1^0;
sbit ENLED = P1^4;

unsigned char dig_num[16] = { 
	0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8,
	0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E
};

void main(void){
	unsigned char cnt_t = 0;
	unsigned char cnt_d = 0;
	ADDR3 = 1;
	ADDR2 = 0;
	ADDR1 = 0;
	ADDR0 = 0;
	ENLED = 0;
	TMOD = 0x01;
	TH0 = 0x4C;
	TL0 = 0x00;
	TR0 = 1;
	while(1){
		if(TF0) {
			TF0 = 0;
			TH0 = 0x4C;
			TL0 = 0x00;
			cnt_t = cnt_t + 1;
			if(cnt_t>=20) {
				cnt_t = 0;
				P0 = dig_num[cnt_d];
				cnt_d = cnt_d + 1;
				if(cnt_d>15)
					cnt_d = 0;
			}
		}
	}
}