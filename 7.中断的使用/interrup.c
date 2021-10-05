/* 
 * �жϺ�����ʹ��
	ʱ�䣺	2016-4-19 *2 	9:28 AM
	ʵ��Ŀ�ģ� ʹ�ö�ʱ���ж������� led ��˸��Ƶ��Ϊ 1Hz��

	�ж�ʹ�ܼĴ���(IE)��ʹ�÷�����
	IE -->  7  6  5   4  3   2   1   0		���� ����λѰַ���� 1 ʹ��
			EA -- ET2 ES ET1 EX1 ET0 EX0
		-- EA Ϊ�ܿ��ƣ��� EA Ϊ 0 ʱ�����е��ж϶����ٴ���
		-- ����λ ������û��ʹ�ã���������������
		-- ET2 Ϊ��ʱ��2�ж�ʹ�ܣ��ж���ں�Ϊ 5
		-- ES  Ϊ�����ж�ʹ�ܣ��ж���ں�Ϊ 4
		-- ET1 Ϊ��ʱ��1�ж�ʹ�ܣ��ж���ں�Ϊ 3
		-- EX1 Ϊ�ⲿ�ж�1ʹ�ܣ��ж���ں�Ϊ 2
		-- ET0 Ϊ��ʱ��0�ж�ʹ�ܣ��ж���ں�Ϊ 1
		-- EX0 Ϊ�ⲿ�ж�0ʹ�ܣ��ж���ں�Ϊ 0

	�ж�ע�ắ����
	�����������ں���β ʹ�ùؼ��� interrupt ��������һ���жϴ�����������ж���ںš�

	��ʱ���жϻ��Զ����� TF0 λ���� 0 �ǵ� 9 ��ʮ�Σ���ʱ������ֵΪ 50ms������ÿ��תһ�ε�ʱ��Ϊ 50ms*10 = 0.05s * 10 = 0.5s����˸��Ƶ��Ϊ 1/(0.5s*2) = 1Hz
	
**/

include<reg52.h>
sbit ENLED = P1^4;
sbit ADDR3 = P1^3;
sbit ADDR2 = P1^2;
sbit ADDR1 = P1^1;
sbit ADDR0 = P1^0;
sbit LED0  = P0^0;

// led ����
void led_set(void) {
	ADDR2 = 1;
	ADDR1 = 1;
	ADDR0 = 0;
	ENLED = 0;
	ADDR3 = 1;
}

// ��ʱ��0 ����Ϊ 50ms
void timer0_set(void) {
	TMOD = 0x01;
	TH0 = 0x4C;
	TL0 = 0x00;
	TR0 = 1;
}

// �򿪶�ʱ��0 �ж�
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

// �жϴ�����
void interrupt_timer0(void) interrupt 1 {
	TL0 = 0x00;				// ��װ ��ʱ��0
	TH0 = 0x4C;
	if(cnt >= 9) {			// ��� 10 �κ�0.5s�� ��ת��� led0
		LED0 = !LED0;
		cnt = 0;			// 10 �κ����������
	}else					// ��������£����������� 1
		cnt = cnt + 1;
}