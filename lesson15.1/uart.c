#include <reg52.h>

unsigned char T1RH=0;
unsigned char T1RL=0;
unsigned char flagUart;

extern unsigned long code freq;
extern void LedLoad(unsigned char index,unsigned char dat);
extern EnterQueue(unsigned char byte);

void send_byte(unsigned char byte);
void uart_send_string(unsigned char *str);
void uart_send_hex_string(unsigned char *str,unsigned int len);



void ConfigUART(unsigned int baud){
	SCON=0x50;
	TMOD&=0x0f;
	TMOD|=0x20;
	PCON|=0x80;
	TH1=256-(freq/12/16)/baud;
	TL1=TH1;
	ET1=0;
	ES=1;
	TR1=1;
}

void InterrptUART() interrupt 4{	
	if(RI){		
		RI=0;  
		EnterQueue(SBUF);		
		flagUart=1;				
	}	
	if(TI);	
}

void uart_send_hex_string(unsigned char *p,unsigned int len){
	for(len;len>0;len--){
		send_byte(*p);
		p++;
	}
}

void uart_send_string(unsigned char *p){
	while(*p!='\0'){
		send_byte(*p);
		p++;
	}
}
void send_byte(unsigned char byte){
	SBUF=byte;
	while(!TI);
	TI=0;
}
