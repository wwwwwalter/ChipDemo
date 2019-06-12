#include <reg52.h>

typedef struct msg{
	unsigned char cmd;
	unsigned char len;
	unsigned char *cmdPtr;
}Msg;


unsigned long freq=24576000;
unsigned char T0RH=0;
unsigned char T0RL=0;

void InitShowStr();
void ConfigTimer0(unsigned int ms);
void UartAction(unsigned char *buf,unsigned char len);
void TrimString16(unsigned char *out,unsigned char *in);
bit CmpMemory(unsigned char *ptr1,unsigned char *ptr2,unsigned char len);


extern void LcdInit();
extern void LcdShowStr(unsigned char x,unsigned char y,unsigned char *str);
void LcdAreaClear(unsigned char x,unsigned char y,unsigned char len);
extern void UartDriver();
extern void ConfigUART(unsigned long baud);
extern void UartRxMonitor(unsigned char ms);
extern void UartWrite(unsigned char *buf,unsigned char len);
extern void E2Read(unsigned char *buf,unsigned char addr,unsigned char len);
extern void E2Write(unsigned char *buf,unsigned char addr,unsigned char len);

void main(){

	EA=1;
	ConfigTimer0(1);
	ConfigUART(9600);
	LcdInit();
	InitShowStr();
	while(1){
		UartDriver();
	}
}

void InitShowStr(){
	unsigned char str[17];
	str[16]='\0';
	E2Read(str,0x20,16);
	LcdShowStr(0,0,str);
	E2Read(str,0x40,16);
	LcdShowStr(0,1,str);
}

bit CmpMemory(unsigned char *ptr1,unsigned char *ptr2,unsigned char len){
	while(len--){
		if(*ptr1++!=*ptr2++){
			return 0;
		}
	}
	return 1;
}


void TrimString16(unsigned char *out,unsigned char *in){
	unsigned char i=0;
	while(*in!='\0'){
		*out++=*in++;
		i++;
		if(i>=16){
			break;
		}
	}
	for(;i<16;i++){
		*out='\0';
	}
}

void ConfigTimer0(unsigned int ms){
	unsigned long tmp;
	tmp=freq/12/1000*ms;
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

void InterruptTimer() interrupt 1{
	TH0=T0RH;
	TL0=T0RL;
	UartRxMonitor(1);
}



void UartAction(unsigned char *buf,unsigned char len){
	unsigned char i;
	unsigned char str[17];
	unsigned char code cmd0[]="showstr1 ";
	unsigned char code cmd1[]="showstr2 ";
	unsigned char code errStr[]="bad commamd.\n";
	
	unsigned char code cmdLen[]={
		sizeof(cmd0)-1,
		sizeof(cmd1)-1,
	};
	
	unsigned char code *cmdPtr[]={
		cmd0,
		cmd1,
	};
	
	for(i=0;i<sizeof(cmdLen);i++){
		if(len>=cmdLen[i]){
			if(CmpMemory(buf,cmdPtr[i],cmdLen[i])){
				break;
			}
		}
	}
	
	switch(i){
		case 0:			
			buf[len]='\0';
			TrimString16(str,buf+cmdLen[0]);
			LcdAreaClear(0,0,16);
			LcdShowStr(0,0,str);			
			E2Write(str,0x20,sizeof(str));
			break;
		case 1:
			buf[len]='\0';
			TrimString16(str,buf+cmdLen[1]);
			LcdAreaClear(0,1,16);
			LcdShowStr(0,1,str);
			E2Write(str,0x40,sizeof(str));
			break;
		default:			
			UartWrite(errStr,sizeof(errStr)-1);
			return;
	}	
}


