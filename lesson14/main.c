#include <reg52.h>
#include <intrins.h>
#define I2CDelay() {_nop_();_nop_();_nop_();_nop_();}

sbit SCL=P2^1;
sbit SDA=P2^0;
void I2CStart();
void I2CStop();
bit I2CWrite(unsigned char dat);
bit I2CAddressing(unsigned char addr);

extern void LcdInit();
extern void LcdShowStr(unsigned char x,unsigned char y,unsigned char *str);

void main(void){
	bit ack;
	unsigned char str[10];
	LcdInit();
	ack=I2CAddressing(0x50);
	str[0]='5';
	str[1]='0';
	str[2]=':';
	str[3]=(unsigned char)ack+'0';
	str[4]='\0';
	LcdShowStr(0,0,str);
	ack=I2CAddressing(0x62);
	str[0]='6';
	str[1]='2';
	str[2]=':';
	str[3]=(unsigned char)ack+'0';
	str[4]='\0';
	LcdShowStr(8,0,str);
	while(1);
}

void I2CStart(){
	SDA=1;
	SCL=1;
	I2CDelay();
	SDA=0;
	I2CDelay();
	SCL=0;	
}

void I2CStop(){
	SCL=0;
	SDA=0;
	I2CDelay();
	SCL=1;
	I2CDelay();
	SDA=1;
	I2CDelay();
}

bit I2CWrite(unsigned char dat){
	bit ack;
	unsigned char mask;
	for(mask=0x80;mask!=0;mask>>=1){
		if((mask&dat)==0){
			SDA=0;
		}else{
			SDA=1;
		}
		I2CDelay();		
		SCL=1;
		I2CDelay();
		SCL=0;
	}
	SDA=1;
	I2CDelay();
	SCL=1;
	ack=SDA;
	I2CDelay();
	SCL=0;
	return ack;	
}

bit I2CAddressing(unsigned char addr){
	bit ack;
	I2CStart();
	ack=I2CWrite(addr<<1);
	I2CStop();
	return ack;
}