/*
 * 1602 液晶显示
   2016 - 4 - 28 ★4 21:10 PM

   1602 是字符液晶，我们输入的是 8 位 ASCII 码，而液晶显示的是字符，这显然需要一个转码的过程，然而转码需要时间，当我们对液晶进行操作时液晶可能正在进行其他操作，我们需要知道液晶现在是否空闲。而液晶是否空闲的标志就是状态字 STA。
   由上可知，对 1602 的任何操作都需要实现检测状态字，STA 的 最高位 STA7 显示液晶现在是否可以进行读写操作，如果 STA7 = 1 那么禁止任何读写操作，STA7 = 0,就表示允许进行操作。

  1. 基本操作：
	1. 读状态： RS = 0， RW = 1， E = 1， 液晶会将 STA 输出到 DB 口
	2. 读数据： RS = 1， RW = 1， E = 1， 液晶会输出数据到 DB 口
	3. 写指令： RS = 0， RW = 0， 输出指令到 DB 口，将 E 置高 然后 置低，液晶会读入指令
	4. 写数据： RS = 1， RW = 0， 输出数据到 DB 口，将 E 置高 然后 置低，液晶会读入数据
   注意： 拉高 E 会导致液晶输出信息，这可能会干扰其他的外设，所以我们在尽可能的让液晶的 E 口保持低电平

  2. 指令说明：
  	指令码 8'b00111000 -- 0x38 告诉液晶使用 5×7 点阵，使用 8 位数据接口
  	指令码 8'b00001DCB -- D = 1 开显示， D = 0 关闭显示
  						 C = 1 显示光标， C = 0 关闭光标
  						 B = 1 闪烁光标， B = 0 不闪烁光标
  	指令码 8'b00000001 -- 清屏指令 数据指针清零并且显示清零
  	指令码 8'b00000010 -- 数据指针清零

  3. 地址说明：
  	1602 的 RAM 缓冲区用来存放待显示的字符，第一行和第二行的地址分别为
  		0x00 ------ 0x0F 0x10 ------ 0x27
  		0x40 ------ 0x4F 0x50 ------ 0x67
  	只有前 16 个字节显示，后面的是为了移动屏幕用的。
  	当我们需要显示的时候，我们先要写地址，然后写数据，地址的写入和指令的写入相同，但前面所述的指令最高位都是 0， 地址需要区分开来，所以写地址的时候需要将地址的最高位置 1，比如我们需要在 0x00 处显示 字符 'A'
  	我们需要首先写如地址：
  		RS = 0;
  		RW = 0;
  		DB = 0x00 | 0x80;	// 0x80 = 8'b1000_0000
  		E = 1;
  		E = 0;
  	接下来写入数据：
  		RS = 1;
  		RW = 0;
  		DB = 0x41;			// 0x41 = 65 'A'
  		E = 1;
  		E = 0;

  	程序的目的是显示一个字符串 "Hello World"，需要连续显示字符，实现的方法就是
  	每次控制一个字符的显示，地址加 1 后操作下一个字符，当判断到 '\0' 时结束操作。
 *
**/
#include<reg52.h>

#define DB P0
sbit RS = P1^0;
sbit RW = P1^1;
sbit E = P1^5;

// 注意字符编码的问题，UTF-8 是坚决不显示的
unsigned char str0[] = "Hello World";

void busy_check(void);
void write_command(unsigned char command);
void write_data(unsigned char mydata);
void lcd_init(void);
void lcd_line1_display(unsigned char *str);

void main(void){
	lcd_init();
	lcd_line1_display(str0);
	while(1);
}

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



