#include <reg52.h>
#ifndef freq
#define freq 24000000
#endif

extern void UartDriver();
extern void ConfigUART(unsigned int baud);
extern void UartRxMonitor(unsigned char ms);
extern void UartWrite(unsigned char *buf,unsigned char len);
extern void LcdInit();
extern void LcdShowStr(unsigned char x,unsigned char y,unsigned char *str);
extern void LcdAreaClear(unsigned char x,unsigned char y,unsigned char len);

sbit BUZZ=P3^2;
bit flagBuzzOn=0;
unsigned char T0RH=0;
unsigned char T0RL=0;

void ConfigTimer0(unsigned int ms);
void UartAction(unsigned char *buf,unsigned char len);
bit CmpMemory(unsigned char *ptr1,unsigned char *ptr2,unsigned char len);


void main(void){
	EA=1;
	ConfigTimer0(1);
	ConfigUART(9600);
	LcdInit();
	while(1){
		UartDriver();
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
	TH0=T0RH;
	TL0=T0RL;
	if(flagBuzzOn){
		BUZZ=~BUZZ;
	}else{
		BUZZ=1;
	}
	UartRxMonitor(1);
}

bit CmpMemory(unsigned char *ptr1,unsigned char *ptr2,unsigned char len){
	while(len--){
		if(*ptr1++!=*ptr2++){
			return 0;
		}
	}
	return 1;
}

void UartAction(unsigned char *buf,unsigned char len){
	unsigned char i;
	unsigned char code cmd0[]="buzz on";
	unsigned char code cmd1[]="buzz off";
	unsigned char code cmd2[]="showstr";
	unsigned char code cmdLen[]={sizeof(cmd0)-1,sizeof(cmd1)-1,sizeof(cmd2)-1};
	//unsigned char code *cmdPtr[]={&cmd0[0],&cmd1[0],&cmd2[0]};
	unsigned char code *cmdPtr[]={cmd0,cmd1,cmd2};
	for(i=0;i<sizeof(cmdLen);++i){
		if(len>=cmdLen[i]){
			if(CmpMemory(buf,cmdPtr[i],cmdLen[i])){
				break;
			}
		}
	}
	switch(i){
		case 0:flagBuzzOn=1;break;
		case 1:flagBuzzOn=0;break;
		case 2:buf=buf+cmdLen[2];len=len-cmdLen[2];break;
		default:UartWrite("bad command.\r\n",sizeof("bad command.\r\n")-1);break;
	}
	
	buf[len]='\0';
	LcdShowStr(0,0,buf);
	i=len;
	if(i<16){
		LcdAreaClear(i,0,16-i);
	}
	
	buf[len++]='\r';
	buf[len++]='\n';
	UartWrite(buf,len);	
}
