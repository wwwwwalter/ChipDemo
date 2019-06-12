#include <reg52.h>
typedef unsigned char uchar;

extern void I2CStart();
extern void I2CStop();
extern unsigned char I2CReadACK();
extern unsigned char I2CReadNAK();
extern bit I2CWrite(unsigned char dat);

void E2Read(unsigned char *buf,unsigned char addr,unsigned char len);
void E2Write(unsigned char *buf,unsigned char addr,unsigned char len);

void E2Read(unsigned char *buf,unsigned char addr,unsigned char len){
	do{
		I2CStart();
		if(I2CWrite(0x50<<1)){
			break;
		}
	}while(1);
	
	I2CWrite(addr);
	I2CStart();
	I2CWrite((0x50<<1)|0x01);
	while(len>1){
		*buf++=I2CReadACK();
		len--;
	}
	*buf=I2CReadNAK();
	I2CStop();
}

void E2Write(unsigned char *buf,unsigned char addr,unsigned char len){
	while(len>0){
		do{
			I2CStart();
			if(I2CWrite(0x50<<1)){
				break;
			}
			I2CStop();
		}while(1);
		
		I2CWrite(addr);
		while(len>0){
			I2CWrite(*buf++);
			len--;
			addr++;
			if((addr&0x0F)==0){
				break;
			}
		}
		I2CStop();
	}
}
