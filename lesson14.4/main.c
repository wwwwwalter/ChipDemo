#include <reg52.h>

extern void LcdInit();
extern void LcdShowStr(unsigned char x,unsigned char y,unsigned char *str);
extern void I2C_Start();
extern void I2C_Stop();
extern unsigned char I2C_SendByte(unsigned char dat,unsigned char ack);
extern unsigned char I2C_ReadByte();
	
unsigned char E2Read(unsigned char addr);
void E2Write(unsigned char addr,unsigned char dat);
void MemToStr(unsigned char *str,unsigned char *src,unsigned char len);

void main(void){
	unsigned char num0=0xab;
	unsigned char ret[4];
	unsigned char str[10];
	LcdInit();
	E2Write(2,num0);
	ret[0] = E2Read(2);
	MemToStr(str,ret,sizeof(ret));
	LcdShowStr(0,0,str);
	while(1);
}

unsigned char E2Read(unsigned char addr){
	unsigned char num;
	do{
		
	
		I2C_Start();
		if(I2C_SendByte(0xa0,1)){
			break;
		}		
	}while(1);
	
	I2C_SendByte(addr,1);
	I2C_Start();
	I2C_SendByte(0xa1,1);
	num=I2C_ReadByte();
	I2C_Stop();
	return num;
}

void E2Write(unsigned char addr,unsigned char dat){
	do{
		
		
		I2C_Start();
		if(I2C_SendByte(0xa0,1)){
			break;
		}
	}while(1);
	
	I2C_SendByte(addr,1);
	I2C_SendByte(dat,0);
	I2C_Stop();
}

void MemToStr(unsigned char *str,unsigned char *src,unsigned char len){
	unsigned char tmp;
	while(len--){
		tmp=*src>>4;
		if(tmp<=9){
			*str++=tmp+'0';
		}else{
			*str++=tmp-10+'A';
		}
		
		tmp=*src&0x0f;
		if(tmp<=9){
			*str++=tmp+'0';
		}else{
			*str++=tmp-10+'A';
		}
		*str++=' ';
		src++;
	}
}