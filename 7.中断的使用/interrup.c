/* 
 * 中断函数的使用
	时间：	2016-4-19 *2 	9:28 AM
	实验目的： 使用定时器中断来控制 led 闪烁，频率为 1Hz。

	中断使能寄存器(IE)的使用方法：
	IE -->  7  6  5   4  3   2   1   0		—— 可以位寻址，置 1 使能
			EA -- ET2 ES ET1 EX1 ET0 EX0
		-- EA 为总控制，当 EA 为 0 时，所有的中断都不再触发
		-- 第六位 被保留没有使用，但厂商有所定义
		-- ET2 为定时器2中断使能，中断入口号为 5
		-- ES  为串口中断使能，中断入口号为 4
		-- ET1 为定时器1中断使能，中断入口号为 3
		-- EX1 为外部中断1使能，中断入口号为 2
		-- ET0 为定时器0中断使能，中断入口号为 1
		-- EX0 为外部中断0使能，中断入口号为 0

	中断注册函数：
	声明方法，在函数尾 使用关键字 interrupt 声明这是一个中断处理函数，后跟中断入口号。

	定时器中断会自动清零 TF0 位，从 0 记到 9 共十次，定时器设置值为 50ms，所以每翻转一次的时间为 50ms*10 = 0.05s * 10 = 0.5s，闪烁的频率为 1/(0.5s*2) = 1Hz
	
**/

include<reg52.h>
sbit ENLED = P1^4;
sbit ADDR3 = P1^3;
sbit ADDR2 = P1^2;
sbit ADDR1 = P1^1;
sbit ADDR0 = P1^0;
sbit LED0  = P0^0;

// led 设置
void led_set(void) {
	ADDR2 = 1;
	ADDR1 = 1;
	ADDR0 = 0;
	ENLED = 0;
	ADDR3 = 1;
}

// 定时器0 设置为 50ms
void timer0_set(void) {
	TMOD = 0x01;
	TH0 = 0x4C;
	TL0 = 0x00;
	TR0 = 1;
}

// 打开定时器0 中断
void interrupt_set(void){
	EA = 1;
	ET0 = 1;
}

static unsigned char cnt = 0;

void main(void) {
	led_set();
	interrupt_set();
	timer0_set();

	while(1);
}

// 中断处理函数
void interrupt_timer0(void) interrupt 1 {
	TL0 = 0x00;				// 重装 定时器0
	TH0 = 0x4C;
	if(cnt >= 9) {			// 溢出 10 次后（0.5s） 翻转输出 led0
		LED0 = !LED0;
		cnt = 0;			// 10 次后清零计数器
	}else					// 其他情况下，计数器自增 1
		cnt = cnt + 1;
}