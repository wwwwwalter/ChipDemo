#include <reg52.h>

unsigned char code SinWave[] = {
	127, 152, 176, 198, 217, 233, 245, 252,
    255, 252, 245, 233, 217, 198, 176, 152,
    127, 102, 78, 56, 37, 21, 9, 2,
    0, 2, 9, 21, 37, 56, 78, 102
};

unsigned char code TriWave[]={
	 0, 16, 32, 48, 64, 80, 96, 112,
    128, 144, 160, 176, 192, 208, 224, 240,
    255, 240, 224, 208, 192, 176, 160, 144,
    128, 112, 96, 80, 64, 48, 32, 16
};

unsigned char code SawWave[]={
	0, 8, 16, 24, 32, 40, 48, 56,
    64, 72, 80, 88, 96, 104, 112, 120,
    128, 136, 144, 152, 160, 168, 176, 184,
    192, 200, 208, 216, 224, 232, 240, 248
};


unsigned char code *pWave;
unsigned char T0RH=0;
unsigned char T0RL=0;
unsigned char T1RH=0;
unsigned char T1RL=0;

void ConfigTimer0(unsigned int ms);
void SetWaveFreq(unsigned char freq);
void SetWaveFreq(unsigned char freq);
void KeyAction(unsigned char keycode);
void SetDACOut(unsigned char val);

extern void KeyScan();
extern void KeyGet();
extern void I2CStart();
extern void I2CStop();
extern bit I2CWrite(unsigned char dat);



void main(){
	EA=1;
	ConfigTimer0(1);
	pWave=SinWave;
	SetWaveFreq(10);
	while(1){
		KeyGet();
	}
}

void KeyAction(unsigned char keycode){
	static unsigned char i=0;
	if(keycode==0x26){
		if(i==0){
			i=1;
			pWave=TriWave;
		}else if(i==1){
			i=2;
			pWave=SawWave;
		}else{
			i=0;
			pWave=SinWave;
		}
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

void SetWaveFreq(unsigned char freq){
	unsigned long tmp;
	tmp=24576000/12/1000*freq;
	tmp=65536-tmp;
	tmp+=33;
	T1RH=(unsigned char)(tmp>>8);
	T1RL=(unsigned char)tmp;
	TMOD&=0x0f;
	TMOD|=0x10;
	TH1=T1RH;
	TL1=T1RL;
	ET1=1;
	PT1=1;
	TR1=1;
}

void ConfigTimer0(unsigned int ms){
	unsigned long tmp;
	tmp=24576000/12/1000*ms;
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

void InterruptTimer1() interrupt 3{
	static unsigned char i=0;
	TH1=T1RH;
	TL1=T1RL;
	SetDACOut(pWave[i]);
	i++;
	if(i>=32){
		i=0;
	}
}

