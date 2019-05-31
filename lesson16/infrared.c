#include <reg52.h>

sbit IR_INPUT=P3^3;
bit irflag=0;
unsigned char ircode[4];

void InitInfrared(){  
   	IR_INPUT=1;
	TMOD&=0x0f;
	TMOD|=0x10;
	TR1=0;
	ET1=0;
	IT1=1;
	EX1=1;
}

unsigned int GetHighTime(){
	TH1=0;
	TL1=0;
	TR1=1;
	while(IR_INPUT){
	 	if(TH1>=0x47){
		 	break;
		}
	}
	TR1=0;
	return (TH1*256+TL1);			  
}

unsigned int GetLowTime(){
   TH1=0;
   TL1=0;
   TR1=1;
   while(!IR_INPUT){
	 	if(TH1>=0x47){
		 	break;
		}
	}
   TR1=0;
   return (TH1*256+TL1);
}


void EXINT1_ISR() interrupt 2{	
	unsigned char i,j;
	unsigned char byt;
	unsigned int time;
	 P0=0x06;
	time = GetLowTime();
	if((time<8500)||(time>9500)){
	 	IE1=0;
		return;
	}
	
	time=GetHighTime();
	if((time<4000)||(time>5000)){
		IE1=0;
		return;
	} 
	
	for(i=0;i<4;++i){
	 	for(j=0;j<8;++j){
		 	time=GetLowTime();
			if((time<340)||(time>780)){
			 	IE1=0;
				return;
			}
			time=GetHighTime();
			if((time>340)&&(time<780)){
			 	byt>>=1;
			}else if((time>1460)&&(time<1900)){
			 	byt>>=1;
				byt|=0x80;
			}else{
			 	IE1=0;
				return;
			}
		}
		ircode[i]=byt;
	}
	irflag=1;
	IE1=0;
}




