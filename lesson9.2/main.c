#include <reg52.h>
#define HEART 24000000

sbit BUZZ=P0^0;

unsigned int code NoteFreq[]={
	523, 587, 659, 698, 784, 880, 988,
    1047, 1175, 1319, 1397, 1568, 1760, 1976
};

unsigned int code NoteReload[] = { 
    65536 - (HEART/12) / (523*2), //1
    65536 - (HEART/12) / (587*2), //2
    65536 - (HEART/12) / (659*2), //3
    65536 - (HEART/12) / (698*2), //4
    65536 - (HEART/12) / (784*2), //5
    65536 - (HEART/12) / (880*2), //6
    65536 - (HEART/12) / (988*2), //7
    65536 - (HEART/12) / (1047*2), //1
    65536 - (HEART/12) / (1175*2), //2
    65536 - (HEART/12) / (1319*2), //3
    65536 - (HEART/12) / (1397*2), //4
    65536 - (HEART/12) / (1568*2), //5
    65536 - (HEART/12) / (1760*2), //6
    65536 - (HEART/12) / (1976*2), //7
};


bit enable =1;
bit tmflag =0;
unsigned char T0RH=0xff;
unsigned char T0RL=0x00;
void ConfigTimer0();
void PlayMusic();

void main(void){
	unsigned int i;
	EA=1;
	ConfigTimer0();
	while(1){
		PlayMusic();
		for(i=0;i<40000;i++);
	}
}

void ConfigTimer0(){
	TH0=T0RH;
	TL0=T0RL;
	TMOD&=0xf0;
	TMOD|=0x01;	
	ET0=1;
	TR0=1;
}

void InterruptTimer0() interrupt 1{
	TH0=T0RH;
	TL0=T0RL;
	tmflag=1;
	if(enable){
		BUZZ=~BUZZ;
	}else{
		BUZZ=1;
	}
}

void PlayMusic(){
	unsigned char beat;
	unsigned char note;
	unsigned int time=0;
	unsigned int beatTime=0;
	unsigned int soundTime=0;
	unsigned char code TwoTigerNote[]={
		1,   2,   3, 1,    1,   2,   3, 1,     3, 4, 5,   3, 4, 5,
		5,6, 5,4, 3, 1,    5,6, 5,4, 3, 1,     1, 5, 1,   1, 5, 1,
	};
	unsigned char code TwotTigerBeat[]={
		4,   4,   4, 4,   4,   4,   4, 4,    4, 4, 8,   4, 4, 8,
		3,1, 3,1, 4, 4,   3,1, 3,1, 4, 4,    4, 4, 8,   4, 4, 8,
	};
	for(beat=0;beat<sizeof(TwoTigerNote);){
		while(!tmflag);
		tmflag=0;
		if(time==0){
			note=TwoTigerNote[beat]-1;
			T0RH=NoteReload[note]>>8;
			T0RL=NoteReload[note];
			beatTime=(TwotTigerBeat[beat]*NoteFreq[note])>>2;
			soundTime=beatTime-(beatTime>>2);
			enable=1;
			time++;
		}else{
			if(time>=beatTime){
				time=0;
				beat++;
			}else{
				time++;
				if(time==soundTime){
					enable=0;
				}
			}
		}
	}
}