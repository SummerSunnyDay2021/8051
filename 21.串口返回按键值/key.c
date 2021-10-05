#include <reg52.h>
#include "key.h"

unsigned char scan_cnt = 0;

void delay(unsigned long time){
	while(time--);
}

void scan_init(void) {
	KeyIn1 = 1;
	KeyIn2 = 1;
	KeyIn3 = 1;
	KeyIn4 = 1;	
}

void scan_control(unsigned char s){
	switch (s){
		case 0: {
			KeyOut1 = 0;
			KeyOut2 = 1;
			KeyOut3 = 1;
			KeyOut4 = 1;
			break;
		}
		case 1: {
			KeyOut1 = 1;
			KeyOut2 = 0;
			KeyOut3 = 1;
			KeyOut4 = 1;
			break;
		}
		case 2: {
			KeyOut1 = 1;
			KeyOut2 = 1;
			KeyOut3 = 0;
			KeyOut4 = 1;
			break;			
		}
		case 3: {
			KeyOut1 = 1;
			KeyOut2 = 1;
			KeyOut3 = 1;
			KeyOut4 = 0;
			break;			
		}
		default: break;
	}
}	

void key_sent(unsigned char s) {
	switch(s){
		case 0: {
			if(KeyIn1 == 0)
				SBUF = '1';
			else if(KeyIn2 == 0)
				SBUF = '2';
			else if(KeyIn3 == 0)
				SBUF = '3';
			else if(KeyIn4 == 0)
				SBUF = 'U';
			break;
		}
		case 1: {
			if(KeyIn1 == 0)
				SBUF = '4';
			else if(KeyIn2 == 0)
				SBUF = '5';
			else if(KeyIn3 == 0)
				SBUF = '6';
			else if(KeyIn4 == 0)
				SBUF = 'L';
			break;
		}
		case 2: {
			if(KeyIn1 == 0)
				SBUF = '7';
			else if(KeyIn2 == 0)
				SBUF = '8';
			else if(KeyIn3 == 0)
				SBUF = '9';
			else if(KeyIn4 == 0)
				SBUF = 'D';
			break;
		}
		case 3: {
			if(KeyIn1 == 0)
				SBUF = '0';
			else if(KeyIn2 == 0)
				SBUF = 'Q';
			else if(KeyIn3 == 0)
				SBUF = 'E';
			else if(KeyIn4 == 0)
				SBUF = 'R';
			break;
		}
		default: break;
	}
}