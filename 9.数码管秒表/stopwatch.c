#include <reg52.h>
#define DIG P0

sbit ADDR2 = P1^2;
sbit ADDR1 = P1^1;
sbit ADDR0 = P1^0;

sbit ADDR3 = P1^3;
sbit ENLED = P1^4;

unsigned char dig_tab[] = {
	0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8,
	0x80, 0x90
};
unsigned char sel = 0;
unsigned long sec_cnt = 0;
unsigned long sec = 0;
unsigned char display[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

void dig_init(void);
void dig_scan(unsigned char s, unsigned char *p);
void timer0_set(void);
void sec_calc(unsigned long sec, unsigned char *p);

void main(void){
	dig_init();
	timer0_set();
	TF0 = 1;
	while(1){
		sec_calc(sec, display);
	}
}

void sec_calc(unsigned long sec, unsigned char *p){
	unsigned char tmp = 0;
	tmp = (unsigned char)(sec%10);
	*p  = dig_tab[tmp];
	tmp = (unsigned char)(sec/10%10);
	*(p+1) = dig_tab[tmp];
	tmp = (unsigned char)(sec/100%10);
	*(p+2) = dig_tab[tmp];
	tmp = (unsigned char)(sec/1000%10);
	*(p+3) = dig_tab[tmp];
	tmp = (unsigned char)(sec/10000%10);
	*(p+4) = dig_tab[tmp];
	tmp = (unsigned char)(sec/100000%10);
	*(p+5) = dig_tab[tmp];
}

void dig_init(void) {
	ADDR3 = 1;
	ENLED = 0;
}

void dig_scan(unsigned char s, unsigned char *p) {
	DIG = 0xFF;
	switch(s) {
		case 0: ADDR2 = 0, ADDR1 = 0, ADDR0 = 0; DIG = *(p+0); break;
		case 1: ADDR2 = 0, ADDR1 = 0, ADDR0 = 1; DIG = *(p+1); break;
		case 2: ADDR2 = 0, ADDR1 = 1, ADDR0 = 0; DIG = *(p+2); break;
		case 3: ADDR2 = 0, ADDR1 = 1, ADDR0 = 1; DIG = *(p+3); break;
		case 4: ADDR2 = 1, ADDR1 = 0, ADDR0 = 0; DIG = *(p+4); break;
		case 5: ADDR2 = 1, ADDR1 = 0, ADDR0 = 1; DIG = *(p+5); break;
		default: break;
	}
}

void timer0_set(void) {
	TMOD = TMOD & 0xF0;
	TMOD = TMOD | 0x01;		// 8 位自动重装模式，定时 250 us
	TH0 = 0xFC;				// 1ms 
	TL0 = 0x66;
	EA = 1;
	ET0 = 1;
	TR0 = 1;
}

void timer0_interrupt (void) interrupt 1 {
	TH0 = 0xFC;
	TL0 = 0x66;

	if(sel<6)
		sel = sel + 1;
	else
		sel = 0;
	
	if( (sec<100000)&&(sel>4) )
		sel = 0;
	else if( (sec<10000)&&(sel>3) )
		sel = 0;
	else if( (sec<1000)&&(sel>2) )
		sel = 0;
	else if( (sec<100)&&(sel>1) )
		sel = 0;
	else if( (sec<10)&&(sel>0) )
		sel = 0;
		
	if(sec_cnt<10)				// sec_cnt<1000 即可使用秒计数
		sec_cnt = sec_cnt + 1;
	else {	
		sec_cnt = 0;
		sec = sec + 1;
		
	}
	dig_scan(sel, display);
}

