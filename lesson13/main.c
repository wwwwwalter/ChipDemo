#include <reg52.h>
#define freq 24000000
extern void InitLcd();
extern void LcdShowStr(unsigned char x,unsigned char y,unsigned char *str,unsigned char len);

bit flag500ms=0;

unsigned char T0RH=0;
unsigned char T0RL=0;
unsigned char code str1[]="0123456789abcdefghijklmnopqrstuvwxyz";
unsigned char code str2[]="0123456789abcdefghijklmnopqrstuvwxyz";

void ConfigTimer0(unsigned int ms);


void main(void){
	unsigned char i;
	unsigned char index=0;
	unsigned char pdata bufMove1[16+sizeof(str1)+16];
	unsigned char pdata bufMove2[16+sizeof(str2)+16];
	EA=1;
	ConfigTimer0(10);
	InitLcd();
	for(i=0;i<16;++i){
		bufMove1[i]=' ';
		bufMove2[i]=' ';
	}
	for(i=0;i<(sizeof(str1)-1);i++){
		bufMove1[16+i]=str1[i];
		bufMove2[16+i]=str2[i];
	}
	for(i=(16+sizeof(str1)-1);i<sizeof(bufMove1);i++){
		bufMove1[i]=' ';
		bufMove2[i]=' ';
	}
	
	while(1){
		if(flag500ms){
			flag500ms=0;
			LcdShowStr(0,0,bufMove1+index,16);
			LcdShowStr(0,1,bufMove2+index,16);
			index++;
			if(index>=(16+sizeof(str1)-1)){
				index=0;
			}			
		}
	}
}

void ConfigTimer0(unsigned int ms){
	unsigned long tmp;
	tmp=freq/12/1000*ms;
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

void InterruptTimer0() interrupt 1{
	static unsigned char i=0;
	TH0=T0RH;
	TL0=T0RL;
	i++;
	if(i>=50){
		i=0;
		flag500ms=1;
	}
}
