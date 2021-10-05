/*
 * I2C 通讯协议实验（2016-5-13 ★5 17:35 PM)

    I2C 最麻烦的地方就是时序问题，在前面我们接触到了 Uart 串口协议和并行的 1602 液晶，I2C 比前面的两种协议都要复杂些。用文字表示的话波形图如下：
    
SCL	----__--__--__--__--__--__--__--__--__--__----
SDA	--_S_/7 \/6 \/5 \/4 \/3 \/2 \/1 \/0 \/a \_T_--

    总结一下 I2C 通讯：
    	1. SCL 作为时钟信号，器件总是在时钟的上升沿采样和发送，并且在 SCL 为高电平时保持稳定
    	2. 例外就是起始位和停止位（上图中的 S 和 T，上图中 S 和 T 位置都应该为 低电平），起始位在 SCL 和 SDA 都为 高电平时拉低 SDA 一段时间表示数据传输的开始。与之相反，当 SCL 和 SDA 都为低电平时，拉高 SCL 一段时间后在 拉高 SDA 表示数据结束。
    	3. 数据帧，与 Uart 不同的是，I2C 总线上的每一帧数据并不都是 一个起始位 + 8 位数据位 + ack 位 + 结束位，I2C 允许多个数据位。也就是说一帧数据可以是 一个起始位 + 8 位数据 + ack 位 + 8 位数据 + ack + ... + 8 位数据 + ack + 结束位。
    	4. SDA 作为数据位，因为有 ack 位的存在，SDA 数据接口应该为双向 IO 口
    	5. ack 应答位，当器件接受到了 I2C 发送的数据后会在 ack 位返回一个数据，在本例中的 AT24C02 应答位为 0。 

    I2C 通讯的时钟频率：
    	I2C 是一种低速的通讯协议，分为普通、快速和高速，频率分别为 100K、400K、3.4M，24C02 手册上说 在 2.5、2.7、5V 的电压下，最高为 400K， 而最低频率则没有标写。
    	
    我板子上的单片机为 STC89C52RC，晶振为 11.0592 MHz，可以计算出机器频率为：
    		11.0592 * 10^6/12 = 921600 ≈ 1000000 = 1000 KHz
    由此可见，单片机比 I2C 要求的速度快一些，需要手动进行延时，1000/400 = 2.5, 1000/100 = 10, 为了简单起见我们选择高速模式。

    单片机的简单延时：
    在 intrins.h 库文件中提供了一个 _nop_() 函数，它可以让机器空转一个机器周期，如果选高速模式，一个时钟周期延时至少需要 3 个 _nop()，半个周期需要至少 2 个 _nop_() 函数，为了稳妥起见，代码中使用了 4 个 _nop_() 函数延时半个 SCL 时钟周期。

    然而时序图中需要用到多种延时，为了简化起见，我照着开发板例程中画了这个简版的波形图：(S 和 T 都应为 低电平）
    
	SCL	----__--__--__--__--__--__--__--__--__--__----
	SDA	--SS<7-><6-><5-><4-><3-><2-><1-><0-><ac>TTTT--   
	--------1---2---3---4---5---6---7---8---9---A-----
	得到了产生 I2C 信号的流程：

		1. 起始位：拉低 SDA 和 SCl，延时，拉低 SDK， 延时，到图中 1 的位置
		2. 数据位：首先给 SDA 赋值，然后拉低 SCL，延时，拉高 SCL，延时，到 2 的位置
		3. 重复第二步到 9 的位置
		4. ACK 位：拉高 SDA(用于接受信号)，延时，拉高 SCL，并读取 ack，延时，最后拉低 SCL，到图中 A 的位置
		5. 结束位：首先拉低 SCL，拉低 SDA，延时，拉高 SCL，延时，拉高 SDA，延时

	理解 I2C 通讯的时序，在操作流程中 电平操作的顺序很重要（一定不能在 SCL 未知或者为高电平时变化 SDA 的值，否则程序可能会出错），由于手头没有逻辑分析仪，不能深入的调整延时。由于单片机指令速度比较慢，上述程序没有在中间位置变化和采样是可行的，如果使用的是 CPLD 、FPGA 或者更高速的 CPU 器件，建议在逻辑分析仪的帮助下实现在时钟中间时刻变化和采样。

	实验目的：
		通过 I2C 协议访问 EEPROM（型号为 AT24C02)，I2C 发送一个地址，如果地址存在对应的器件，那么 ack 回应 0，就点亮 Led，否则就不点亮 Led 小灯。
		根据器件手册，AT24C02 的地址为： 8'b1010_(A2)(A1)(A0)(R/W)，我们写这个器件，所以发送的数据为： 8'b1010_0000 最后我们发送的数据为 0xA0

	实验现象：
		当 我们发送 0xA0 时器件会回应一个 ACK(0)，这是我们点亮 LED0
		实验中还要发送一个不存在的地址 0xB0，回应的 ACK 为 1，我们熄灭 LED1
		
**/

#include <reg52.h>
#include <intrins.h>
#define delay()		{ _nop_(); _nop_(); _nop_(); _nop_(); }
sbit I2C_SCL = P3^7;
sbit I2C_SDA = P3^6;

sbit ADDR0 = P1^0;
sbit ADDR1 = P1^1;
sbit ADDR2 = P1^2;
sbit ADDR3 = P1^3;
sbit ENLED = P1^4;
sbit Led0  = P0^0;
sbit Led1  = P0^1;

unsigned char ack = 1;

void led_init(void);
void i2c_start(void);
void i2c_data(unsigned char sent_data);
void i2c_stop(void);

void main()
{
    led_init();
	i2c_start();
	i2c_data(0xA0);
	if(ack==0)
    	Led0 = 0;
	else
		Led0 = 1;
	i2c_stop();
	
	i2c_start();
	i2c_data(0xB0);
   	if(ack==0)
    	Led1 = 0;
	else
		Led1 = 1;
    i2c_stop();

    while (1);
}

void led_init(void){
	ADDR0 = 0;
	ADDR1 = 1;
	ADDR2 = 1;
	ADDR3 = 1;
	ENLED = 0;	
}

void i2c_start(void){
	I2C_SDA = 1;
	I2C_SCL = 1;
	delay();
	I2C_SDA = 0;
	delay();
	I2C_SCL = 0;
}

void i2c_data(unsigned char sent_data) {
	unsigned char mask;	
	for(mask = 0x80; mask != 0; mask = mask >> 1){
		{
		if((mask & sent_data) == 0x00)
			I2C_SDA = 0;
		else
			I2C_SDA = 1;
		}
		delay();
		I2C_SCL = 1;
		delay();
		I2C_SCL = 0;		
	}
	I2C_SDA = 1;
	delay();
	I2C_SCL = 1;
	ack = I2C_SDA;
	delay();
	I2C_SCL = 0; 
}

void i2c_stop(void){
	I2C_SCL = 0;
	I2C_SDA = 0;
	delay();
	I2C_SCL = 1;
	delay();
	I2C_SDA = 1;
	delay();
}

