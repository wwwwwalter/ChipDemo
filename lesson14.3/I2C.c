#include <reg52.h>
#include <intrins.h>
#define I2CDelay() {unsigned char a,b;for(b=1;b>0;b--){for(a=2;a>0;a--);}}

sbit I2C_SCL=P2^1;
sbit I2C_SDA=P2^0;

void I2CStart();
void I2CStop();
unsigned char I2CReadNAK();
unsigned char I2CReadACK();
bit I2CWrite(unsigned char dat);

extern void LcdShowStr(unsigned char x,unsigned char y,unsigned char *str);
extern void MemToStr(unsigned char *str,unsigned char *src,unsigned char len);

void I2CStart(){
	I2C_SDA=1;
	I2CDelay();
	I2C_SCL=1;
	I2CDelay();
	I2C_SDA=0;
	I2CDelay();
	I2C_SCL=0;
	I2CDelay();
}

void I2CStop(){
	I2C_SCL=0;
	I2CDelay();
	I2C_SDA=0;
	I2CDelay();
	I2C_SCL=1;
	I2CDelay();
	I2C_SDA=1;
	I2CDelay();
}

bit I2CWrite(unsigned char dat){
	bit ack;
	unsigned char mask;
	
	for(mask==0x80;mask!=0;mask>>=1){
		if((mask&dat)==0){
			I2C_SDA=0;
		}else{
			I2C_SDA=1;
		}
		I2CDelay();
		I2C_SCL=1;
		I2CDelay();
		I2C_SCL=0;
	}
	I2C_SDA=1;
	I2CDelay();
	I2C_SCL=1;
	I2CDelay();
	while(I2C_SDA){
		static unsigned char count=0;
		unsigned long i=0;
		unsigned char str[4];
		for(i=0;i<30000;++i);
		count++;
		MemToStr(str,&count,1);
		LcdShowStr(0,1,str);
	}
	ack=I2C_SDA;
	I2CDelay();
	I2C_SCL=0;
	return (~ack);	
}

unsigned char I2CReadNAK(){
	unsigned char mask;
	unsigned char dat;
	I2C_SDA=1;
	for(mask=0x80;mask!=0;mask>>=1){
		I2CDelay();
		I2C_SCL=1;
		if(I2C_SDA==0){
			dat&=~mask;
		}else{
			dat|=mask;
		}
		I2CDelay();
		I2C_SCL=0;
	}
	I2C_SDA=1;
	I2CDelay();
	I2C_SCL=1;
	I2CDelay();
	I2C_SCL=0;
	return dat;
}

unsigned char I2CReadACK(){
	unsigned char mask;
	unsigned char dat;
	I2C_SDA=1;
	for(mask=0x80;mask!=0;mask>>=1){
		I2CDelay();
		I2C_SCL=1;
		if(I2C_SDA==0){
			dat&=~mask;
		}else{
			dat|=mask;
		}
		I2CDelay();
		I2C_SCL=0;
	}
	I2C_SDA=0;
	I2CDelay();
	I2C_SCL=1;
	I2CDelay();
	I2C_SCL=0;
	return dat;
}
