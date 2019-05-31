#include <reg52.h>
unsigned char code LedCharYang[] = {
	0xC0, 0xF9, 0xA4, 0xB0, 
  	0x99, 0x92, 0x82, 0xF8,
	0x80, 0x90, 0x88, 0x83, 
	0xC6, 0xA1, 0x86, 0x8E	
};

unsigned char code LedCharYin[] = {
	0x3f, 0x06, 0x5b, 0x4f,
  	0x66, 0x6d, 0x7d, 0x07,
  	0x7f, 0x6f, 0x77, 0x7c,
  	0x39, 0x5e, 0x79, 0x71
};

unsigned char LedBuff[8] = {
 	0xff,0xff,0x76,0x3f,
	0x40,0x40,0x40,0x40
};

unsigned char T0RH=0;
unsigned char T0RL=0;
unsigned char RxdByte=0;

void ConfigTimer0(unsigned int ms);
void ConfigUART(unsigned int baud);

void main(void){
	EA=1;
	ConfigTimer0(1);
	ConfigUART(9600);
	while(1){
	 	LedBuff[0] = LedCharYin[RxdByte&0x0f];
		LedBuff[1] = LedCharYin[RxdByte>>4];
	}
}


void ConfigTimer0(unsigned int ms){
 	unsigned long temp;
	temp=12000000/12;
	temp=temp/1000*ms;
	temp=65536-temp;
	temp=temp+13;
	T0RH=(unsigned char)(temp>>8);
	T0RL=(unsigned char)temp;
	TMOD&=0xf0;
	TMOD|=0x01;
	TH0=T0RH;
	TL0=T0RL;
	ET0=1;
	TR0=1;
}

void ConfigUART(unsigned int baud){
	SCON  =0x50;
	TMOD &=0x0f;
	TMOD |=0x20;
	PCON |=0x80;
	TH1	=256-(11973000/12/16)/baud;
	TL1 = TH1;
	ET1 = 0;
	ES = 1;
	TR1 = 1;
}

void LedScan(){
 	static unsigned char i = 0;
	P0=0x00;
	P1=(P1&0xF8)|~i;
	P0=LedBuff[i];
	if(i<7){
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

void InterruptUART() interrupt 4{
	if(RI){
	 	RI=0;
		RxdByte=SBUF;
		SBUF=RxdByte;
	}
	if(TI){
		TI=0;
	}
}
