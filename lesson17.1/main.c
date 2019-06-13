#include <reg52.h>

unsigned char T0RH=0;
unsigned char T0RL=0;


extern void KeyScan();
extern void KeyGet();
extern void I2CStart();
extern void I2CStop();
extern bit I2CWrite(unsigned char dat);

void ConfigTimer0(unsigned int ms);
void SetDACOut(unsigned char val);
void KeyAction(unsigned char keycode);

void main(void){
	EA=1;
	ConfigTimer0(1);
	while(1){
		KeyGet();
	}
}

void SetDACOut(unsigned char val){
	I2CStart();
	if(!I2CWrite(0x48<<1)){
		I2CStop();
		return;
	}
	I2CWrite(0x40);
	I2CWrite(val);
	I2CStop();
}

void KeyAction(unsigned char keycode){
	static unsigned char volt=0;
	if(keycode==0x26){
		if(volt<25){
			volt++;
			SetDACOut(volt*255/25);
		}
	}else if(keycode==0x28){
		if(volt>0){
			volt--;
			SetDACOut(volt*255/25);
		}
	}
}

void ConfigTimer0(unsigned int ms){
	unsigned long tmp;
	tmp=24576000/12*ms/1000;
	tmp=65536-tmp;
	tmp+=28;
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
	KeyScan();
}