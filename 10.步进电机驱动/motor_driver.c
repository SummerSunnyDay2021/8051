/*
 *	2016-4-21 21:08 PM ★4
 ================================================================

 	开发板上使用的步进电机型号为： 28BYJ-48，型号的含义如下：
 	
 		28——步进电机的有效最大外径是28毫米
		B——表示是步进电机
		Y——表示是永磁式
		J——表示是减速型
		48——表示四相八拍
		
 ================================================================

	步进电机有多种工作模式，主要的工作模式称为 4 相 8 拍
	这里假设定子线圈名称分别为 ABCD 四项，驱动器为低电平有效，八拍如下：
		1. 首先导通 A，所以控制信号应该为	0111
		2. 接着导通 A 和 B，控制信号应该为	0011
		3. 接着导通 B，控制信号应该为		1011
		4. 接着导通 B 和 C，控制信号应该为	1001
		5. 接着导通 C，控制信号应该为		1101
		6. 接着导通 C 和 D，控制信号应该为	1100
		7. 接着导通 D，控制信号应该为		1110
		8. 接着导通 D 和 A，控制信号应该为	0110
		9. 回到第一步

	所以输入按十六进制应该如下：
		0x7， 0x3， 0xB， 0x9， 0xD， 0xC， 0xE， 0x6
	如需反转步进电机，逆序即可
	
 =================================================================
 
	得到了控制信号，接下来就是找到控制信号变化的速度了，器件手册上注明控制信号应当小于 550Hz
	550 Hz ---> 0.001818s ---> 1.8 ms ，可以得出结论定时器定时 2ms 就可以了。

 =================================================================
 
	开发板上步进电机接 P1 口的低四位 P1^0、P1^1、P1^2、P1^3，显然操作 P1 口时不能改动其高四位的值
	在 C 语言中有如下方法：
		unsigned char tmp = 0;	// 定义中间变量
		tmp = P1;				// 读入 P1 的值
		tmp = tmp & 0xF0;		// 本操作将低四位清零
		
			//	tmp   = 8'bxxxx_xxxx
			//	0xF0  = 8'b1111_0000
			//	&       8'bxxxx_0000
									
		tmp = tmp | 0x0X;		// 修该 tmp 的低四位，0x0X 中的 X 为需要写入的值

			// tmp    = 8'bxxxx_0000
			// 0xFX   = 8'b0000_xxxx
			//	|       8'bxxxx_xxxx

		P1 = tmp;				// 将修改写回到 P1 口

 ==================================================================
 
	假设计数 cnt 需要从 0 记到 7，然后归 0 后从新计数，可以用以下判断语句实现

		if(cnt>=8)
			cnt = 0;
			
	这里有一个小技巧 cnt = cnt & 0x7; 可代替上面的判断（自然逼格也高出了不少 (*^__^*) )

				0x07 = 8'b0000_0111
		cnt	=	0x00 = 8'b0000_0000
			     	 = 8'b0000_0111		// 不变

		cnt =	0x01 = 8'b0000_0001
					 = 8'b0000_0001		// 还是不变

		cnt =	0x02、0x03 …… 0x06、0x07 时都不超过 3 位，和 0x07 相与后 都不会发生变化

	但是，当计数器记到 0x08 时，也就是 8'b0000_1000， 这时 0x08 | 0x07 如下

		cnt =	0x08 = 8'b0000_1000
				0x07 = 8'b0000_0111
					 = 8'b0000_0000		// cnt 自动归零了
		
 ====================================================================	
**/

#include <reg52.h>
unsigned char motor_tab[8] = {
	0x07, 0x03, 0x0B, 0x09, 0x0D, 0x0C, 0x0E, 0x06
};

void timer0_set (void) {
	TMOD = 0x01;	//  定时器0 十六位计数模式
	TL0 = 0xCD;		//	设置为 2ms
	TH0 = 0xF8;
	EA = 1;			// 打开总中断
	ET0 = 1;		// 打开定时器0 中断
	TR0 = 1;		// 打开定时器0
}

void main(void){
	timer0_set();
	while(1);
}

void timer0_interrupt(void) interrupt 1 {
	static unsigned char state = 0;
	unsigned char tmp;
	TL0 = 0xCD;
	TH0 = 0xF8;
	tmp = P1;
	tmp = tmp & 0xF0;
	tmp = tmp | motor_tab[state];
	P1 = tmp;
	state = state + 1;
	state = state & 0x07;
}