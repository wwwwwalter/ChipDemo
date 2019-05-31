#include <reg52.h>

unsigned char code LedCharYang[] = {
	0xC0, 0xF9, 0xA4, 0xB0, 
  	0x99, 0x92, 0x82, 0xF8,
	0x80, 0x90, 0x88, 0x83, 
	0xC6, 0xA1, 0x86, 0x8E	
};

sbit PIN_RXD=P3^0;
sbit PIN_TXD=P3^1;

bit RxdOrTxd = 0;
bit RxdEnd = 0;
bit TxdEnd = 0;

unsigned char RxdBuf = 0;
unsigned char TxdBuf = 0;

void ConfigUART(unsigned int baud);
void StartTXD(unsigned char dat);
void StartRXD();

void main(void){
 	EA=1;
	ConfigUART(9600);
	while(1){
			
	 	while(PIN_RXD);
		StartRXD();
		while(!RxdEnd);
		P0=LedCharYang[RxdBuf];
		StartTXD(RxdBuf);
		while(!TxdEnd);
	
	/*
		unsigned char i;
		unsigned int j;
		for(i = 0;i<16;++i){
		 	StartTXD(i);
			while(!TxdEnd);
			P0 = LedCharYang[i];
			for(j = 0;j<30000;j++);
		}
	*/

	}
}

void ConfigUART(unsigned int band){
 	TMOD &=0xf0;//force
	TMOD |=0x02;//add
	TH0	  =	256-(12000000/12)/band;	 //256-96=160~256
}

void StartRXD(){
 	TL0=256-((256-TH0)>>1);
	ET0=1;
	TR0=1;
	RxdEnd=0;
	RxdOrTxd=0;
}

void StartTXD(unsigned char dat){
	TxdBuf = dat;
	TL0=TH0;
	ET0=1;
	TR0=1;
	PIN_TXD=0;
	TxdEnd=0;
	RxdOrTxd=1;
}

void InterruptTimer() interrupt 1{
 	static unsigned char cnt = 0;
	if(RxdOrTxd){
	 	cnt++;
		if(cnt<=8){
		 	PIN_TXD=TxdBuf&0x01;
			TxdBuf>>=1;
		}else if(cnt==9){
		 	PIN_TXD=1;
			cnt=0;
			TR0=0;
			TxdEnd=1;
		}
	}else{
		if(cnt==0){
		 	if(!PIN_RXD){
			 	RxdBuf=0;
				cnt++;
			}else{
		 		TR0=0;
			}
		}else if(cnt<=8){
		 	RxdBuf>>=1;
			if(PIN_RXD){
			 	RxdBuf |=0x80;
			}
			cnt++;
		}else{
			cnt=0;
			TR0=0;
			if(PIN_RXD){
			 	RxdEnd=1;
			}
		}
	}
}

