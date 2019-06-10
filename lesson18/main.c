#include <reg52.h>

unsigned char T0RH=0;
unsigned char T0RL=0;

void ConfigTimer0(unsigned int ms);
extern void UartDriver();
extern void ConfigUART(unsigned int baud);
extern void UartRxMonitor(unsigned char ms);
extern void UartWrite(unsigned char *buf,unsigned char len);

void main(void){
	EA=1;
	ConfigTimer0(1);
	ConfigUART(9600);
	while(1){
		UartDriver();
	}		
}

void ConfigTimer0(unsigned int ms){
	unsigned long tmp;
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
	TH0=T0RH;
	TL0=T0RL;
	UartRxMonitor(1);
}

void UartAction(unsigned char *buf,unsigned char len){
	buf[len++]='\r';
	buf[len++]='\n';
	UartWrite(buf,len);
}

