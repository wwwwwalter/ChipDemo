#include <reg52.h>

typedef struct sTime{
	unsigned int year;
	unsigned char mon;
	unsigned char day;
	unsigned char hour;
	unsigned char min;
	unsigned char sec;
	unsigned char week;
}Time;

extern void InitDS1302();
extern void GetRealTime(Time *time);
extern void SetRealTime(Time *time);
extern void LedScan();
extern void LedLoad(unsigned char index,unsigned char dat);
extern void KeyGet();
extern void KeyScan();
extern void ConfigTimer1(unsigned int ms);
extern void uart_send_string(unsigned char *str);
extern void uart_send_hex_string(unsigned char *str,unsigned int len);

void ConfigTimer0(unsigned int ms);
void ConfigUART(unsigned int baud);
void LoadTime(Time *time);
void KeyAction(unsigned char keycode);
void timeToHexString(Time *time,unsigned char *str);


bit flag200ms=1;
bit flag1s=1;
unsigned char T0RH=0;
unsigned char T0RL=0;
unsigned long code freq=24000000;
unsigned char settime=0;
unsigned char uartRecvByte=0;
unsigned char recvflag=0;



void main(){
   unsigned char backsec=0xAA;
   unsigned char str[9]={'\0'};
   Time time = {0};   

   EA=1;
   ConfigTimer0(1);
   ConfigUART(9600);
   InitDS1302();

   while(1){
	   KeyGet();	   
	   if(flag200ms&&(settime==0)){
		   flag200ms=0;
		   GetRealTime(&time);		   
		   if(time.sec!=backsec){
			 	LoadTime(&time);				
				backsec=time.sec;
		   }
	   }
	   if(flag1s){
		   flag1s=0;
		   timeToHexString(&time,str);
		   uart_send_hex_string(str,8);
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
	PT0=1; 
}
/*
void ConfigTimer1(unsigned int ms){
	unsigned long tmp;
	tmp=freq/12/1000*ms;
	tmp=65536-tmp;
	tmp+=12;
	T1RH=(unsigned char)(tmp>>8);
	T1RL=(unsigned char)tmp;
	TMOD&=0x0f;
	TMOD|=0x10;
	TH1=T1RH;
	TL1=T1RL;
	ET1=1;
	TR1=1;
}
void InterruptTimer1() interrupt 3{	
	TH1=T1RH;
	TL1=T1RL;
	KeyScan();	
}*/


void InterruptTimer0() interrupt 1{
	static unsigned char i = 0;	
	static unsigned int j = 0;
	TH0=T0RH;
	TL0=T0RL;	
	LedScan();
	KeyScan();	
	i++;
	if(i>=200){
	 	i=0;
		flag200ms=1;		
	}
	j++;
	if(j>=1000){
		j=0;
		flag1s=1;
	}
}


void LoadTime(Time *time){	
	LedLoad(2,time->sec&0x0f);
	LedLoad(3,time->sec>>4&0x07);
	LedLoad(4,time->min&0x0f);
	LedLoad(5,time->min>>4&0x07);
	LedLoad(6,time->hour&0x0f);
	LedLoad(7,time->hour>>4&0x03);
}

void timeToHexString(Time *time,unsigned char *str){
	str[0]=time->year>>8;
	str[1]=time->year&0xff;
	str[2]=time->mon;
	str[3]=time->day;
	str[4]=time->hour;
	str[5]=time->min;
	str[6]=time->sec;
	str[7]=time->week;	
}

void KeyAction(unsigned char keycode){
	if((keycode>='0')&&(keycode<='9')){		
		LedLoad(0,0);		
		uart_send_string("hello world:123456789");
	}else if(keycode==0x26){
		LedLoad(0,1);
	}else if(keycode==0x28){
		LedLoad(0,2);
	}else if(keycode==0x25){
		LedLoad(0,3);
	}else if(keycode==0x27){
		LedLoad(0,4);
	}else if(keycode==0x0d){
		LedLoad(0,5);
		uart_send_string("setting or confirm");
	}else if(keycode==0x1b){
		LedLoad(0,14);
	}else{
		;
	}
}


