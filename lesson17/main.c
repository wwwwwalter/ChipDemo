#include <reg52.h>

bit flag300ms=1;
unsigned char T0RH=0;
unsigned char T0RL=0;

void ConfigTimer0(unsigned int ms);
unsigned char GetADCValue(unsigned char chn);
void ValueToString(unsigned char *str,unsigned char val);


extern void LcdInit();
extern void LcdShowStr(unsigned char x,unsigned char y,unsigned char *str);
extern void LcdAreaClear(unsigned char x,unsigned char y,unsigned char len);
extern void I2CStart();
extern void I2CStop();
extern unsigned char I2CReadACK();
extern unsigned char I2CReadNAK();
extern bit I2CWrite(unsigned char dat);

void main(){
	unsigned char val;
	unsigned char str[10];
	EA=1;
	ConfigTimer0(10);
	LcdInit();
	LcdShowStr(0,0,"AIN0 AIN1 AIN2");
	
	while(1){
		if(flag300ms){
			flag300ms=0;
			LcdAreaClear(0,1,16);
			val=GetADCValue(0);
			ValueToString(str,val);
			LcdShowStr(0,1,str);
			val=GetADCValue(1);
			ValueToString(str,val);
			LcdShowStr(6,1,str);
			val=GetADCValue(3);
			ValueToString(str,val);
			LcdShowStr(12,1,str);
		}
	}
}

void ConfigTimer0(unsigned int ms){
	unsigned char tmp;
	tmp=24576000/12/1000*ms;
	tmp=65536-tmp;
	tmp+=33;
	T0RH=(unsigned char)(tmp>>8);
	T0RL=(unsigned char)tmp;
	TMOD&=0xf0;
	TMOD|=0x01;
	TH0=T0RH;
	TL0=T0RL;
	ET0=1;
	TR0=1;
}

void InterruptTimer0() interrupt 1{
	static unsigned char i=0;
	TH0=T0RH;
	TL0=T0RL;
	i++;
	if(i>=30){
		i=0;
		flag300ms=1;
	}
}

void ValueToString(unsigned char *str,unsigned char val){
	val=val*25/255;
	str[0]=(val/10)+'0';
	str[1]='.';
	str[2]=(val%10)+'0';
	str[3]='V';
	str[4]='\0';
}

unsigned char GetADCValue(unsigned char chn){
	unsigned char val;
	I2CStart();
	if(!I2CWrite(0x48<<1)){
		I2CStop();
		return 0;
	}
	
	I2CWrite(0x40|chn);
	I2CStart();
	I2CWrite((0x40<<1)|0x01);
	I2CReadACK();
	val=I2CReadNAK();
	I2CStop();
	return val;
}