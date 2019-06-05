#include <reg52.h>

sbit ADDA=P1^0;
sbit ADDB=P1^1;
sbit ADDC=P1^2;

unsigned char code LedCharYin[] = {
	0x3f, 0x06, 0x5b, 0x4f,
  	0x66, 0x6d, 0x7d, 0x07,
  	0x7f, 0x6f, 0x77, 0x7c,
  	0x39, 0x5e, 0x79, 0x71,
	0x49
};
unsigned char code LedCharYang[] = {
	0xC0, 0xF9, 0xA4, 0xB0, 
  	0x99, 0x92, 0x82, 0xF8,
	0x80, 0x90, 0x88, 0x83, 
	0xC6, 0xA1, 0x86, 0x8E
};

unsigned char LedBuff[8] = {
 	0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00
};

void LedScan(){
 	static unsigned char i=0;
	P0=0x00;	
	switch(i){
		case 0:ADDC=1;ADDB=1;ADDA=1;break;
		case 1:ADDC=1;ADDB=1;ADDA=0;break;
		case 2:ADDC=1;ADDB=0;ADDA=1;break;
		case 3:ADDC=1;ADDB=0;ADDA=0;break;
		case 4:ADDC=0;ADDB=1;ADDA=1;break;
		case 5:ADDC=0;ADDB=1;ADDA=0;break;
		case 6:ADDC=0;ADDB=0;ADDA=1;break;
		case 7:ADDC=0;ADDB=0;ADDA=0;break;
		default:break;
	}
	P0=LedBuff[i];	
	if(i<sizeof(LedBuff)-1){
	 	i++;
	}else{
	 	i=0;
	}
}

void LedLoad(unsigned char index,unsigned char dat){
	LedBuff[index]=LedCharYin[dat];	
	if(index%2==0&&index>0){
		LedBuff[index]|=0x80;
	}
}