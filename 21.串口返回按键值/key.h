#ifndef KEY_H
#define KEY_H

sbit KeyIn4 = P2^7;
sbit KeyIn3 = P2^6;
sbit KeyIn2 = P2^5;
sbit KeyIn1 = P2^4;
sbit KeyOut1 = P2^3;
sbit KeyOut2 = P2^2;
sbit KeyOut3 = P2^1;
sbit KeyOut4 = P2^0;

extern unsigned char scan_cnt;

extern void scan_init(void);
extern void scan_control(unsigned char s);
extern void key_sent(unsigned char s);
extern void delay(unsigned long time);

#endif
