/*
	将无符号二进制转成BCD编码
	
 CHAR_BIT         8 
 CHAR_MAX         127
 CHAR_MIN       (-128)
 SCHAR_MAX        127
 SCHAR_MIN      (-128)
 UCHAR_MAX        255			0xFF
 SHRT_MAX         32767
 SHRT_MIN       (-32768)
 USHRT_MAX        65534			0xFFFF 
 INT_MAX          32767 
 INT_MIN        (-32768)
 UINT_MAX         				0xFFFF 
 LONG_MAX         2147483647
 LONG_MIN        -2147483648
 ULONG_MAX        4294967294	0xFFFFFFFF
**/

#include <reg52.h>

void bin2bcd(unsigned char bin, unsigned char *p) {
	unsigned char bcd[3] = {0};
	unsigned int tmp = 0;
	unsigned char i;
	for(i = 0; i <= 7; i++) {

		tmp = tmp << 1;
		tmp = tmp | ( (unsigned int) ( (bin & 0x80)>>7) );
		bin = bin << 1;

		bcd[0] = (unsigned char) ( (tmp & 0x000F) >> 0 );
		bcd[1] = (unsigned char) ( (tmp & 0x00F0) >> 4 );
		bcd[2] = (unsigned char) ( (tmp & 0x0F00) >> 8 );

		if(bcd[0]>=5)
			tmp = tmp + 0x0003;
		if(bcd[1]>=5)
			tmp = tmp + 0x0030;
		if(bcd[2]>=5)
			tmp = tmp + 0x0300;
	}
	
	*p     = bcd[0];
	*(p+1) = bcd[1];
	*(p+2) = bcd[2];
}	

int main(void) {
	unsigned char bin = 134;	// 必须小于 255
	unsigned char bcd[3] = {0};
	unsigned char *ptr = bcd;

	bin2bcd(bin, ptr);

	return 0;
}