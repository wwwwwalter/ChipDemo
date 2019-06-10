#include <reg52.h>

sbit BUZZ=P1^1;
bit flagBuzzOn = 0;
unsigned char T0RH=0;
unsigned char T0RL=0;
unsigned char regGroup[5];

void ConfigTimer0(unsigned int ms);
void UartAction(unsigned char *buf,unsigned char len);
void Deal0x03(unsigned char *buf,unsigned char len);
void Deal0x06(unsigned char *buf,unsigned char len);
void Deal0x80(unsigned char *buf,unsigned char len);

extern void LcdInit();
extern void UartDriver();
extern void ConfigUART(unsigned int baud);
extern void UartRxMonitor(unsigned char ms);
extern void UartWrite(unsigned char *buf,unsigned char len);
extern unsigned int GetCRC16(unsigned char *ptr,unsigned char len);
extern void LcdShowStr(unsigned char x,unsigned char y,unsigned char *str);

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
	tmp=24576000/12/1000*ms;
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
void UartAction(unsigned char *buf,unsigned char len){	
	
	unsigned int crc;
	unsigned char crch,crcl;
	
	if(buf[0]!=0x01){
		return;
	}
	
	crc=GetCRC16(buf,len-2);
	crch=crc>>8;
	crcl=crc&0xff;
	if((buf[len-2]!=crch)||(buf[len-1]!=crcl)){
		return;
	}
	
	switch(buf[1]){
		case 0x03:
			Deal0x03(buf,len);
			break;
		case 0x06:
			Deal0x06(buf,len);
			break;
		default:
			Deal0x80(buf,len);
			break;
	}
	crc=GetCRC16(buf,len);
	buf[len++]=crc>>8;
	buf[len++]=crc&0xff;
	UartWrite(buf,len);
	
}

void Deal0x03(unsigned char *buf,unsigned char len){
	unsigned char i;
	unsigned char cnt;
	
	if((buf[2]==0x00)&&(buf[3]<=0x05)){
		if(buf[3]<=0x04){
			i=buf[3];
			cnt=buf[5];
			buf[2]=cnt*2;
			len=3;
			while(cnt--){
				buf[len++]=0x00;
				buf[len++]=regGroup[i++];
			}
		}else{
			buf[2]=2;
			buf[3]=0x00;
			buf[4]=flagBuzzOn;
			len=5;
		}
	}else{
		buf[1]=buf[1]|0x80;
		buf[2]=0x02;
		len=3;
	}
}

void Deal0x06(unsigned char *buf,unsigned char len){	
	unsigned char i;
	unsigned char cnt;
	unsigned char str[4];
	
	if((buf[2]==0x00)&&(buf[3]<=0x05)){
		if(buf[3]<=0x04){
			i=buf[3];
			regGroup[i]=buf[5];
			cnt=regGroup[i]>>4;//high
			if(cnt>=0x0A){
				str[0]=cnt-0x0A+'A';
			}else{
				str[0]=cnt+'0';
			}
			cnt=regGroup[i]&0x0f;//low
			if(cnt>=0x0A){
				str[1]=cnt-0x0A+'A';
			}else{
				str[1]=cnt+'0';
			}
			str[2]='\0';
			LcdShowStr(i*3,0,str);
		}else{
			flagBuzzOn=(bit)buf[5];
		}
		len-=2;
	}else{
		buf[1]=buf[1]|0x80;
		buf[2]=0x02;
		len=3;
	}
}

void Deal0x80(unsigned char *buf,unsigned char len){
	buf[1]|=0x80;
	buf[2]=0x01;
	len=3;
}

