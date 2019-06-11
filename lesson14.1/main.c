#include <reg52.h>

extern void LcdInit();
extern void LcdShowStr(unsigned x,unsigned y,unsigned char *str);
extern void I2CStart();
extern void I2CStop();
extern unsigned char I2CReadNAK();
extern bit I2CWrite(unsigned char addr);

unsigned char EEReadByte(unsigned char addr);
void EEWriteByte(unsigned char addr,unsigned char dat);

void main(void){
	unsigned char dat;
	unsigned char str[10];
	LcdInit();
	dat=EEReadByte(0x03);
	str[0]=(dat/100)+'0';
	str[1]=(dat/10%10)+'0';
	str[2]=(dat%10)+'0';
	str[3]='\0';
	LcdShowStr(0,0,str);
	dat++;
	EEWriteByte(0x03,dat);
	while(1);
}

unsigned char EEReadByte(unsigned char addr){
	unsigned char dat;
	I2CStart();
	I2CWrite(0x50<<1);
	I2CWrite(addr);
	I2CStart();
	I2CWrite((0x50<<1)|0x01);
	dat=I2CReadNAK();
	I2CStop();
	return dat;
}

void EEWriteByte(unsigned char addr,unsigned char dat){
	I2CStart();
	I2CWrite(0x50<<1);
	I2CWrite(addr);
	I2CWrite(dat);
	I2CStop();
}