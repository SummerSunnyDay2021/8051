#ifndef GLOBL_H
#define GLOBL_H

extern unsigned char dig_tab[];
extern unsigned char sel;
extern unsigned long sec_cnt;
extern unsigned long sec;
extern unsigned char display[];

extern void dig_init(void);
extern void dig_scan(unsigned char s, unsigned char *p);
extern void timer0_set(void);
extern void sec_calc(unsigned long sec, unsigned char *p);
extern void uart_config(void);
extern void baud_config(void);
#endif

