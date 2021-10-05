#ifndef GLOBL_H
#define GLOBL_H

extern unsigned char str0[16];

extern void busy_check(void);
extern void write_command(unsigned char command);
extern void write_data(unsigned char mydata);
extern void lcd_init(void);
extern void lcd_line1_display(unsigned char *str);
extern void lcd_clean(void);
extern void uart_config(void);
extern void baud_config(void);

#endif
