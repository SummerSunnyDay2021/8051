#include <reg52.h>
sbit ADDR0 = P1^0;
sbit ADDR1 = P1^1;
sbit ADDR2 = P1^2;
sbit ADDR3 = P1^3;
sbit ENLED = P1^4;

static 	unsigned int mypic[4][8] = {
	{0xFF,0xFF,0xFF,0xEF,0xFF,0xEF,0xC7,0xD7},
	{0xFF,0xFF,0xEF,0xFF,0xFF,0xEF,0xC7,0xD7},
	{0xFF,0xEF,0xFF,0xFF,0xFF,0xEF,0xC7,0xD7},
	{0xEF,0xFF,0xFF,0xFF,0xFF,0xEF,0xC7,0xD7}
};

static unsigned char m, j, cnt;
static unsigned int pic[8];
static unsigned int *p = pic;


void main(void){

	ENLED = 0;
	ADDR3 = 0;
	// timer0 set
	TMOD = 0x11;
	TH0 = 0x4C;
	TL0 = 0x00;
	TH1 = 0xFC;
	TL1 = 0x66;
	// open timer 0
	TR0 = 1;
	TR1 = 1;
	P0 = 0xFF;
	while(1){
		if(TF1){
			TF1 = 0;
			TH1 = 0xFC;
			TL1 = 0x66;
			if(j>7)
				j = 0;
			else
				j = j + 1;
			P0 = 0xFF;
			switch(j) {
				case 0: ADDR2 = 0, ADDR1 = 0, ADDR0 = 0; P0 = pic[0]; break;
				case 1: ADDR2 = 0, ADDR1 = 0, ADDR0 = 1; P0 = pic[1]; break;
				case 2: ADDR2 = 0, ADDR1 = 1, ADDR0 = 0; P0 = pic[2]; break;
				case 3: ADDR2 = 0, ADDR1 = 1, ADDR0 = 1; P0 = pic[3]; break;
				case 4: ADDR2 = 1, ADDR1 = 0, ADDR0 = 0; P0 = pic[4]; break;
				case 5: ADDR2 = 1, ADDR1 = 0, ADDR0 = 1; P0 = pic[5]; break;
				case 6: ADDR2 = 1, ADDR1 = 1, ADDR0 = 0; P0 = pic[6]; break;
				case 7: ADDR2 = 1, ADDR1 = 1, ADDR0 = 1; P0 = pic[7]; break;
				default: break;
			}
		}

		
		if(TF0){
			TF0 = 0;
			TH0 = 0x4C;
			TL0 = 0x00;
			if(m>=4)
				m = 0;
			if(cnt>=10){
				*(p+0) = mypic[m][0];
				*(p+1) = mypic[m][1];
				*(p+2) = mypic[m][2];
				*(p+3) = mypic[m][3];
				*(p+4) = mypic[m][4];
				*(p+5) = mypic[m][5];
				*(p+6) = mypic[m][6];
				*(p+7) = mypic[m][7];
				m = m + 1;
				cnt = 0;
			}
			cnt = cnt + 1;
		}		
	}	
}
	






