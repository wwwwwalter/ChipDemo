#include <reg52.h>
unsigned char code LedCharYin[] = {
	0x3f, 0x06, 0x5b, 0x4f,
  	0x66, 0x6d, 0x7d, 0x07,
  	0x7f, 0x6f, 0x77, 0x7c,
  	0x39, 0x5e, 0x79, 0x71
};

unsigned char LedBuff[8] = {
 	0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00
};
//interrupt
bit flag200ms=0;
unsigned char T0RH=0;
unsigned char T0RL=0;
void ConfigTimer0(unsigned int ms);
//DS1302
sbit CE=P3^5;
sbit CLK=P3^6;
sbit IO=P3^7;

void InitDS1302();
void DS1302ByteWrite(unsigned char dat);
unsigned char DS1302ByteRead();
unsigned char DS1302SingleRead(unsigned char reg);
void DS1302SingleWrite(unsigned char reg,unsigned char dat);
void DS1302BurstRead(unsigned char *dat);
void DS1302BurstWrite(unsigned char *dat);
//Lcd
extern void InitLcd1602();
extern void LcdShowStr(unsigned char x,unsigned char y,unsigned char *str);


void main(void){
	
	unsigned char psec=0xAA;//1 010 1010
	unsigned char time[8];
	//unsigned char str[12];

	EA=1;
	ConfigTimer0(1);
	InitDS1302();
	//InitLcd1602();

	while(1){
		if(flag200ms){
			flag200ms=0;
			DS1302BurstRead(time);
			if(psec!=time[0]){

				LedBuff[0]=LedCharYin[time[0]&0x0f];
				LedBuff[1]=LedCharYin[time[0]>>4&0x07];			

				LedBuff[2]=LedCharYin[time[1]&0x0f];
				LedBuff[2]|=0x80;
				LedBuff[3]=LedCharYin[time[1]>>4&0x07];

				LedBuff[4]=LedCharYin[time[2]&0x0f];
				LedBuff[4]|=0x80;
				LedBuff[5]=LedCharYin[time[2]>>4&0x03];

				LedBuff[6]=LedCharYin[time[3]&0x0f];
				LedBuff[6]|=0x80;
				LedBuff[7]=LedCharYin[time[3]>>4&0x03];


				psec=time[0];	
			}
		}
	} 	
}

void ConfigTimer0(unsigned int ms){
 	unsigned long tmp;
	tmp=12000000/12/1000*ms;
	tmp=65536-tmp;
	tmp+=12;
	T0RH=(unsigned char)(tmp>>8);
	T0RL=(unsigned char)tmp;
	TMOD&=0xf0;
	TMOD|=0x01;
	TH0=T0RH;
	TL0=T0RL;
	ET0=1;
	TR0=1;
}

void LedScan(){
 	static unsigned char i=0;
	P0=0x00;
	P1=(P1&0xF8)|~i;
	P0=LedBuff[i];
	if(i<sizeof(LedBuff)-1){
	 	i++;
	}else{
	 	i=0;
	}
}

void InterruptTimer0() interrupt 1{
 	static unsigned char i=0;
	TH0=T0RH;
	TL0=T0RL;
	LedScan();
	i++;
	if(i>=200){
	 	i=0;
		flag200ms=1;
	}
}

void InitDS1302(){
 	unsigned char i;
	unsigned char code InitTime[]={
		0x00,0x30,0x09,0x02,0x06,0x07,0x19,
		0x00	
	};
	CE=0;
	CLK=0;
	i=DS1302SingleRead(0);
	if((i&0x80)!=0){
		DS1302SingleWrite(7,0x00);
		DS1302BurstWrite(InitTime);
	}
	/*
	else{
	   DS1302SingleWrite(7,0x00);
		for(i=0;i<7;++i){
		 	DS1302SingleWrite(i,InitTime[i]);
		}
	}
	*/	
}

void DS1302ByteWrite(unsigned char dat){
 	unsigned char mask;
	for(mask=0x01;mask!=0;mask<<=1){
	 	if((mask&dat)!=0){
			IO=1;
		}else{
		 	IO=0;
		}
		CLK=1;
		CLK=0;
	}
	IO=1;
}

unsigned char DS1302ByteRead(){
 	unsigned char mask;
	unsigned char dat=0;
	for(mask=0x01;mask!=0;mask<<=1){
	 	if(IO!=0){
		 	dat|=mask;
		}
		CLK=1;
		CLK=0;
	}
	return dat;
}

unsigned char DS1302SingleRead(unsigned char reg){
   unsigned char dat;
   CE=1;
   DS1302ByteWrite((reg<<1)|0x81);
   dat=DS1302ByteRead();
   CE=0;
   return dat;
}
void DS1302SingleWrite(unsigned char reg,unsigned char dat){
 	CE=1;
	DS1302ByteWrite((reg<<1)|0x80);
	DS1302ByteWrite(dat);
	CE=0;
}

void DS1302BurstWrite(unsigned char *dat){
	unsigned char i;
	CE=1;
	DS1302ByteWrite(0xBE);
	for(i=0;i<8;++i){
	 	DS1302ByteWrite(dat[i]);
	}
	CE=0;
}

void DS1302BurstRead(unsigned char *dat){
 	unsigned char i;
	CE=1;
	DS1302ByteWrite(0xBF);
	for(i=0;i<8;++i){
	 	dat[i]=DS1302ByteRead();
	}
	CE=0;
}

	