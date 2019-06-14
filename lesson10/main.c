#include <reg52.h>

sbit PWMOUT=P0^0;

unsigned char HighRH=0;
unsigned char HighRL=0;
unsigned char LowRH=0;
unsigned char LowRL=0;
unsigned char T1RH=0;
unsigned char T1RL=0;
unsigned long PeriodCnt=0;

void ConfigTimer1(unsigned int ms);
void ConfigPWM(unsigned int fr,unsigned int dc);
void AdjustDutyCycle(unsigned char dc);

void main(){	
	EA=1;
	ConfigPWM(100,10);
	ConfigTimer1(1);
	while(1);
}

void ConfigPWM(unsigned int fr,unsigned int dc){
	unsigned int high;
	unsigned int low;	
	
	PeriodCnt=24576000/12/fr;
	high=(PeriodCnt*dc)/100;
	low=PeriodCnt-high;
	high=65536-high+12;
	low=65536-low+12;
	
	HighRH=(unsigned char)(high>>8);
	HighRL=(unsigned char)high;
	LowRH=(unsigned char)(low>>8);
	LowRL=(unsigned char)low;
	
	TMOD&=0xf0;
	TMOD|=0x01;
	TH0=HighRH;
	TL0=HighRL;
	ET0=1;
	TR0=1;
	PWMOUT=1;
}

void InterruptTimer0() interrupt 1{
	if(PWMOUT==1){
		TH0=LowRH;
		TL0=LowRL;
		PWMOUT=0;
	}else{
		TH0=HighRH;
		TL0=HighRL;
		PWMOUT=1;
	}
}

void AdjustDutyCycle(unsigned char dc){
	unsigned int high;
	unsigned int low;
	high=(PeriodCnt*dc)/100;
	low=PeriodCnt-high;
	high=65536-high+12;
	low=65536-low+12;
	
	HighRH=(unsigned char)(high>>8);
	HighRL=(unsigned char)high;
	LowRH=(unsigned char)(low>>8);
	LowRL=(unsigned char)low;
}

void ConfigTimer1(unsigned int ms){
	unsigned long tmp;
	tmp=24576000/12/1000*ms;
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
	static unsigned int i=0;
	static bit dir=0;
	static unsigned char index=0;
	unsigned char code table[13]={
		5, 18, 30, 41, 51, 60, 68, 75, 81, 86, 90, 93, 95
	};
	
	TH1=T1RH;
	TL1=T1RL;
	
	i++;
	if(i>=50){
		i=0;
		AdjustDutyCycle(table[index]);
		if(dir==0){
			index++;
			if(index>=12){
				dir=1;
			}
		}else{
			index--;
			if(index==0){
				dir=0;
			}
		}
	}	
}