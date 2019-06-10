#include <reg52.h>
#include <intrins.h>

sbit DIR=P1^0;
bit flagFrame=0;
bit flagTxd=0;
unsigned char cntRxd=0;
unsigned char pdata bufRxd[64];

void UartDriver();
void Delay10us(unsigned char t);
void ConfigUART(unsigned int baud);
void UartRxMonitor(unsigned char ms);
void UartWrite(unsigned char *buf,unsigned char len);
unsigned char UartRead(unsigned char *buf,unsigned char len);
extern void UartAction(unsigned char *buf,unsigned char len);

void ConfigUART(unsigned int baud){
	DIR=0;
	SCON=0x50;
	TMOD&=0xf0;
	TH1=256-(24576000/12/32)/baud;
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

void Delay10us(unsigned char t){
	do{
		_nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
	}while(--t);
}

void UartWrite(unsigned char *buf,unsigned char len){
	DIR=1;
	while(len--){
		flagTxd=0;
		SBUF=*buf++;
		while(!flagTxd);
	}
	Delay10us(5);
	DIR=0;
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
	static unsigned char idletime=0;
	if(cntRxd>0){
		if(cntbkp!=cntRxd){
			cntbkp=cntRxd;
			idletime=0;
		}else{
			if(idletime<30){
				idletime+=ms;
				if(idletime>=30){
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
		len=UartRead(buf,sizeof(buf)-2);
		UartAction(buf,len);
	}
}
