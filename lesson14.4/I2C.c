#include <reg52.h>

sbit I2C_SCL=P2^1;
sbit I2C_SDA=P2^0;

void I2C_Delay10us();
void I2C_Start();
void I2C_Stop();
unsigned char I2C_SendByte(unsigned char dat,unsigned char ack);
unsigned char I2C_ReadByte();


void I2C_Delay10us(){
	unsigned char a;
	unsigned char b;
	for(b=1;b>0;b--){
		for(a=2;a>0;a--);
	}
}
void I2C_Start(){
	I2C_SDA=1;
	I2C_Delay10us();
	I2C_SCL=1;
	I2C_Delay10us();
	I2C_SDA=0;
	I2C_Delay10us();
	I2C_SCL=0;
	I2C_Delay10us();
}
void I2C_Stop(){
	I2C_SDA=0;
	I2C_Delay10us();
	I2C_SCL=1;
	I2C_Delay10us();	
	I2C_SDA=1;
	I2C_Delay10us();
}
unsigned char I2C_SendByte(unsigned char dat,unsigned char ack){
	unsigned char a;
	unsigned char b;
	for(a=0;a<8;a++){
		I2C_SDA=dat>>7;
		dat=dat<<1;
		I2C_Delay10us();
		I2C_SCL=1;
		I2C_Delay10us();
		I2C_SCL=0;
		I2C_Delay10us();
	}
	I2C_SDA=1;
	I2C_Delay10us();
	I2C_SCL=1;
	while(I2C_SDA&&(ack==1)){
		b++;
		if(b>200){
			I2C_SCL=0;
			I2C_Delay10us();
			return 0;
		}
	}
	I2C_SCL=0;
	I2C_Delay10us();
	return 1;
}
unsigned char I2C_ReadByte(){
	unsigned char a;	
	unsigned char dat;
	I2C_SDA=1;
	I2C_Delay10us();
	for(a=0;a<8;a++){
		I2C_SCL=1;
		I2C_Delay10us();
		dat<<=1;
		dat|=I2C_SDA;
		I2C_Delay10us();
		I2C_SCL=0;
		I2C_Delay10us();
	}
	return dat;
}