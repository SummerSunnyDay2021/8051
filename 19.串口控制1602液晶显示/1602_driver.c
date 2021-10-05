#include<reg52.h>
#include"io.h"
#include"globl.h"

// 注意字符编码的问题，UTF-8 是坚决不显示的
unsigned char str0[16] = "0123456789abcdef";

// 忙检测，包含延时部分
void busy_check(void){
	unsigned char sta = 0;
	RS = 0;
	RW = 1;
	do{
		E = 1;
		sta = DB;
		E = 0;
	}while(sta&0x80);
}

// 写指令/地址
void write_command(unsigned char command){
	RS = 0;
	RW = 0;
	DB = command;
	E = 1;
	E = 0;
}

// 写数据
void write_data(unsigned char mydata){
	RS = 1;
	RW = 0;
	DB = mydata;
	E = 1;
	E = 0;
}

// 液晶初始化
void lcd_init(void){
	busy_check();
	write_command(0x38);	//设置5*7 显示 8 个数据位
	busy_check();
	write_command(0x0C);	//开显示，不显示光标，光标不闪烁
	busy_check();
	write_command(0x06);
	busy_check();
	write_command(0x01);	//清屏
}

void lcd_clean(void){
	busy_check();
	write_command(0x01);	//清屏
}

// 液晶第一行显示函数
void lcd_line1_display(unsigned char *str){
	unsigned char i = 0;
	while(*str != '\0'){
		busy_check();
		write_command(0x80 | i);
		busy_check();
		write_data(*str++);
		i++;
	}
}



