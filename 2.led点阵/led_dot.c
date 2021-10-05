#include <reg52.h>
sbit ADDR0 = P1^0;
sbit ADDR1 = P1^1;
sbit ADDR2 = P1^2;
sbit ADDR3 = P1^3;
sbit ENLED = P1^4;

void delay(unsigned int time){
	while(time--);
}

void main(void){
	unsigned int pic[8] = { 0xFF,0xFF,0xFF,0xEF,0xFF,0xEF,0xC7,0xD7 };
	ENLED = 0;
	ADDR3 = 0;
	while(1){
		char cnt;
		for(cnt=0;cnt<8;cnt++){
			switch(cnt) {
				case 0: ADDR2 = 0, ADDR1 = 0, ADDR0 = 0; break;
				case 1: ADDR2 = 0, ADDR1 = 0, ADDR0 = 1; break;
				case 2: ADDR2 = 0, ADDR1 = 1, ADDR0 = 0; break;
				case 3: ADDR2 = 0, ADDR1 = 1, ADDR0 = 1; break;
				case 4: ADDR2 = 1, ADDR1 = 0, ADDR0 = 0; break;
				case 5: ADDR2 = 1, ADDR1 = 0, ADDR0 = 1; break;
				case 6: ADDR2 = 1, ADDR1 = 1, ADDR0 = 0; break;
				case 7: ADDR2 = 1, ADDR1 = 1, ADDR0 = 1; break;
				default: break;
			}

			switch(cnt) {
				case 0: P0 = pic[0]; break;
				case 1: P0 = pic[1]; break;
				case 2: P0 = pic[2]; break;
				case 3: P0 = pic[3]; break;
				case 4: P0 = pic[4]; break;
				case 5: P0 = pic[5]; break;
				case 6: P0 = pic[6]; break;
				case 7: P0 = pic[7]; break;
				default:P0 = 0xFF;	 break;
			}
			delay(100);
			P0 = 0xFF;
		}
	}	
}