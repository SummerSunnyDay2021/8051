/*
 * 1602 Һ����ʾ
   2016 - 4 - 28 ��4 21:10 PM

   1602 ���ַ�Һ��������������� 8 λ ASCII �룬��Һ����ʾ�����ַ�������Ȼ��Ҫһ��ת��Ĺ��̣�Ȼ��ת����Ҫʱ�䣬�����Ƕ�Һ�����в���ʱҺ���������ڽ�������������������Ҫ֪��Һ�������Ƿ���С���Һ���Ƿ���еı�־����״̬�� STA��
   ���Ͽ�֪���� 1602 ���κβ�������Ҫʵ�ּ��״̬�֣�STA �� ���λ STA7 ��ʾҺ�������Ƿ���Խ��ж�д��������� STA7 = 1 ��ô��ֹ�κζ�д������STA7 = 0,�ͱ�ʾ������в�����

  1. ����������
	1. ��״̬�� RS = 0�� RW = 1�� E = 1�� Һ���Ὣ STA ����� DB ��
	2. �����ݣ� RS = 1�� RW = 1�� E = 1�� Һ����������ݵ� DB ��
	3. дָ� RS = 0�� RW = 0�� ���ָ� DB �ڣ��� E �ø� Ȼ�� �õͣ�Һ�������ָ��
	4. д���ݣ� RS = 1�� RW = 0�� ������ݵ� DB �ڣ��� E �ø� Ȼ�� �õͣ�Һ�����������
   ע�⣺ ���� E �ᵼ��Һ�������Ϣ������ܻ�������������裬���������ھ����ܵ���Һ���� E �ڱ��ֵ͵�ƽ

  2. ָ��˵����
  	ָ���� 8'b00111000 -- 0x38 ����Һ��ʹ�� 5��7 ����ʹ�� 8 λ���ݽӿ�
  	ָ���� 8'b00001DCB -- D = 1 ����ʾ�� D = 0 �ر���ʾ
  						 C = 1 ��ʾ��꣬ C = 0 �رչ��
  						 B = 1 ��˸��꣬ B = 0 ����˸���
  	ָ���� 8'b00000001 -- ����ָ�� ����ָ�����㲢����ʾ����
  	ָ���� 8'b00000010 -- ����ָ������

  3. ��ַ˵����
  	1602 �� RAM ������������Ŵ���ʾ���ַ�����һ�к͵ڶ��еĵ�ַ�ֱ�Ϊ
  		0x00 ------ 0x0F 0x10 ------ 0x27
  		0x40 ------ 0x4F 0x50 ------ 0x67
  	ֻ��ǰ 16 ���ֽ���ʾ���������Ϊ���ƶ���Ļ�õġ�
  	��������Ҫ��ʾ��ʱ��������Ҫд��ַ��Ȼ��д���ݣ���ַ��д���ָ���д����ͬ����ǰ��������ָ�����λ���� 0�� ��ַ��Ҫ���ֿ���������д��ַ��ʱ����Ҫ����ַ�����λ�� 1������������Ҫ�� 0x00 ����ʾ �ַ� 'A'
  	������Ҫ����д���ַ��
  		RS = 0;
  		RW = 0;
  		DB = 0x00 | 0x80;	// 0x80 = 8'b1000_0000
  		E = 1;
  		E = 0;
  	������д�����ݣ�
  		RS = 1;
  		RW = 0;
  		DB = 0x41;			// 0x41 = 65 'A'
  		E = 1;
  		E = 0;

  	�����Ŀ������ʾһ���ַ��� "Hello World"����Ҫ������ʾ�ַ���ʵ�ֵķ�������
  	ÿ�ο���һ���ַ�����ʾ����ַ�� 1 �������һ���ַ������жϵ� '\0' ʱ����������
 *
**/
#include<reg52.h>

#define DB P0
sbit RS = P1^0;
sbit RW = P1^1;
sbit E = P1^5;

// ע���ַ���������⣬UTF-8 �Ǽ������ʾ��
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

// æ��⣬������ʱ����
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

// дָ��/��ַ
void write_command(unsigned char command){
	RS = 0;
	RW = 0;
	DB = command;
	E = 1;
	E = 0;
}

// д����
void write_data(unsigned char mydata){
	RS = 1;
	RW = 0;
	DB = mydata;
	E = 1;
	E = 0;
}

// Һ����ʼ��
void lcd_init(void){
	busy_check();
	write_command(0x38);	//����5*7 ��ʾ 8 ������λ
	busy_check();
	write_command(0x0C);	//����ʾ������ʾ��꣬��겻��˸
	busy_check();
	write_command(0x06);
	busy_check();
	write_command(0x01);	//����
}

// Һ����һ����ʾ����
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



