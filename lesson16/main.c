#include <reg51.h>

unsigned char code LedCharYin[] = {
	0x3f, 0x06, 0x5b, 0x4f,
  	0x66, 0x6d, 0x7d, 0x07,
  	0x7f, 0x6f, 0x77, 0x7c,
  	0x39, 0x5e, 0x79, 0x71
};

unsigned char LedBuff[8] = {
 	0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff
};

unsigned char T0RH=0;
unsigned char T0RL=0;
extern bit irflag;
extern unsigned char ircode[4];
extern void InitInfrared();
void ConfigTimer0(unsigned int ms);

void main(void){
	EA=1;
	InitInfrared();
	ConfigTimer0(1);
	PT0=1;

	while(1){
	 	if(irflag){
		 	irflag=0;
			LedBuff[5] = LedCharYin[ircode[0]>>4];
			LedBuff[4] = LedCharYin[ircode[0]&0x0f];
			LedBuff[1] = LedCharYin[ircode[2]>>4];
			LedBuff[0] = LedCharYin[ircode[2]&0x0f];
		}
	} 	
}

void ConfigTimer0(unsigned int ms){
	unsigned long tmp;
	tmp = 12000000/12/1000*ms;
	tmp = 65536-tmp;
	tmp+=13;
	T0RH=(unsigned char)(tmp>>8);
	T0RL=(unsigned char)tmp;

	TMOD&=0xF0;
	TMOD|=0x01;
	TH0=T0RH;
	TL0=T0RL;
	ET0=1;
	TR0=1;
}

void LedScan(){
	static unsigned char i=0;
	P0=0x00;
	P1=(P1&0xf8)|~i;
	P0=LedBuff[i];
	if(i<sizeof(LedBuff)-1){
	 	i++;
	}else{
	 	i=0;
	}
}

void InterruptTimer() interrupt 1{
	TH0=T0RH;
	TL0=T0RL;
	LedScan();
}
