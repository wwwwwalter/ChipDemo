#include <reg52.h>
#ifndef freq
#define freq 24000000
#endif

extern void UartAction(unsigned char *buf,unsigned char len);

bit flagFrame=0;
bit flagTxd=0;
unsigned char cntRxd=0;
unsigned char pdata bufRxd[64];

void UartDriver();
void ConfigUART(unsigned int baud);
void UartRxMonitor(unsigned char ms);
void UartWrite(unsigned char *buf,unsigned char len);
unsigned char UartRead(unsigned char *buf,unsigned char len);

void ConfigUART(unsigned int baud){
	SCON=0x50;
	TMOD&=0x0f;
	TMOD|=0x20;
	PCON |=0x80;
	TH1=256-(freq/12/16)/baud;
	TL1=TH1;
	ET1=0;
	ES=1;
	TR1=1;
}

void InterruptUART() interrupt 4{
	if(RI){
		RI=0;
		if(cntRxd<sizeof(bufRxd)){
			bufRxd[cntRxd++]=SBUF;
		}
	}
	if(TI){
		TI=0;
		flagTxd=1;
	}
}

void UartWrite(unsigned char *buf,unsigned char len){
	while(len--){
		flagTxd=0;
		SBUF=*buf++;
		while(!flagTxd);
	}
}	

unsigned char UartRead(unsigned char *buf,unsigned char len){
	unsigned char i;
	if(len>cntRxd){
		len=cntRxd;
	}
	for(i=0;i<len;++i){
		*buf++=bufRxd[i];
	}
	cntRxd=0;
	return len;
}

void UartRxMonitor(unsigned char ms){
	static unsigned char cntbkp=0;
	static unsigned char idletmr=0;
	
	if(cntRxd>0){
		if(cntbkp!=cntRxd){
			cntbkp=cntRxd;
			idletmr=0;
		}else{
			if(idletmr<30){
				idletmr+=ms;
				if(idletmr>=30){
					flagFrame=1;
				}
			}
		}
	}else{
		cntbkp=0;
	}
}

void UartDriver(){
	unsigned char len;
	unsigned char pdata buf[40];
	if(flagFrame){
		flagFrame=0;
		len=UartRead(buf,sizeof(buf));
		UartAction(buf,len);
	}
}