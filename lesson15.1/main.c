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
extern void InitQueue();
extern unsigned char DeleteQueue();
extern unsigned char flagUart;


void ConfigTimer0(unsigned int ms);
void ConfigUART(unsigned int baud);
void LoadTime(Time *time);
void FlashTime(Time *time);
void KeyAction(unsigned char keycode);
void timeToHexString(Time *time,unsigned char *str);
void EnterTimeSet();
void ExitTimeSet(bit save);
void IncSetTime();
void DecSetTime();
void LeftShift();
void RightShift();
unsigned char IncBcdHigh(unsigned char bcd);
unsigned char IncBcdLow(unsigned char bcd);
unsigned char DecBcdHigh(unsigned char bcd);
unsigned char DecBcdLow(unsigned char bcd);


bit flag200ms=1;
bit flag1s=1;
Time time = {0};   
unsigned char T0RH=0;
unsigned char T0RL=0;
unsigned long code freq=24000000;
unsigned char setIndex=0;




void main(){	
	unsigned char backsec=0xAA;
	unsigned char str[9]={'\0'};
	unsigned char command=0;

    EA=1;
    InitQueue();
	InitDS1302();
	ConfigTimer0(1);
	ConfigUART(9600);


    while(1){
		KeyGet();
		if(flagUart){
			flagUart=0;
			command=DeleteQueue();			
			KeyAction(command);
		}	
		if(setIndex==0){
			if(flag200ms){
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
		}else{
			if(flag200ms){				
				flag200ms=0;
				FlashTime(&time);
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

void FlashTime(Time *time){
	static bit flash=0;
	flash=!flash;
	if(!flash){
		LedLoad(setIndex,17);
	}else{
		LoadTime(time);
	}	
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
	
	if(setIndex==0){
		if(keycode==0x0d){
			EnterTimeSet();
		}else{
			return;
		}
	}else{
		if((keycode>='0')&&(keycode<='9')){		
			LedLoad(0,0);		
		}else if(keycode==0x26){
			IncSetTime();
		}else if(keycode==0x28){
			DecSetTime();
		}else if(keycode==0x25){
			LeftShift();
		}else if(keycode==0x27){
			RightShift();
		}else if(keycode==0x0d){				
			ExitTimeSet(1);			
		}else if(keycode==0x1b){
			ExitTimeSet(0);
		}else{
			return;
		}
	}
}

void EnterTimeSet(){
	setIndex=7;	
}

void ExitTimeSet(bit save){
	setIndex=0;
	if(save){
		SetRealTime(&time);
	}
}

void LeftShift(){
	if(setIndex>=7){
		setIndex=2;
	}else{
		setIndex++;
	}
}

void RightShift(){
	if(setIndex<=2){
		setIndex=7;
	}else{
		setIndex--;
	}
}

unsigned char IncBcdHigh(unsigned char bcd){
	if((bcd&0xf0)<0x90){
		bcd+=0x10;
	}else{
		bcd&=0x0f;
	}
	return bcd;
}
unsigned char IncBcdLow(unsigned char bcd){
	if((bcd&0x0f)<0x09){
		bcd+=0x01;
	}else{
		bcd&=0xf0;
	}
	return bcd;
}
unsigned char DecBcdHigh(unsigned char bcd){
	if((bcd&0xf0)>0x00){
		bcd-=0x10;
	}else{
		bcd|=0x90;
	}
	return bcd;
}
unsigned char DecBcdLow(unsigned char bcd){
	if((bcd&0x0f)>0x00){
		bcd-=0x01;
	}else{
		bcd|=-0x09;
	}
	return bcd;
}

void IncSetTime(){
	LedLoad(0,setIndex);
	switch(setIndex){
		case 7:time.hour=IncBcdHigh(time.hour);break;
		case 6:time.hour=IncBcdLow(time.hour);break;
		case 5:time.min=IncBcdHigh(time.min);break;
		case 4:time.min=IncBcdLow(time.min);break;
		case 3:time.sec=IncBcdHigh(time.sec);break;
		case 2:time.sec=IncBcdLow(time.sec);break;
		default:break;		
	}
	LoadTime(&time);
}

void DecSetTime(){
	LedLoad(0,setIndex);
	switch(setIndex){
		case 7:time.hour=DecBcdHigh(time.hour);break;
		case 6:time.hour=DecBcdLow(time.hour);break;
		case 5:time.min=DecBcdHigh(time.min);break;
		case 4:time.min=DecBcdLow(time.min);break;
		case 3:time.sec=DecBcdHigh(time.sec);break;
		case 2:time.sec=DecBcdLow(time.sec);break;
		default:break;
	}
	LoadTime(&time);
}



