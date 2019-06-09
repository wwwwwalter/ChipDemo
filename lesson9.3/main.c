#include <reg52.h>
#define heart 24576000
#define LEN 28
sbit buzz=P1^0;

unsigned char count=0;
unsigned char T0RH=0;
unsigned char T0RL=0;
float code NoteFreq[] = {
    523.251, 587.33, 659.255, 698.456, 783.991, 880, 987.767,
    1046.502, 1174.659, 1318.52, 1396.913, 1567.982, 1760, 1975.533,
	2093.004, 2349.318, 2637.02, 2793.826, 3135.437, 3520, 3951.066,
	4186.009, 4698.636, 5274.04, 5587.652, 6270.874, 7040, 7902.132
};
unsigned int NoteReload[LEN];
void PlayMusic(unsigned int freq);
void reload(unsigned int *noteReload,float *noteFreq,unsigned char len);



void main(void){
	unsigned int i=0;
	reload(NoteReload,NoteFreq,LEN);
	
	EA=1;
	TH0=0xff;
	TL0=0xfe;
	TMOD&=0xf0;
	TMOD|=0x01;
	ET0=1;
	TR0=1;
	
	while(1){
		PlayMusic(NoteReload[count++]);
		for(i=0;i<40000;i++);
		if(count>=LEN){
			count=0;
		}
	}
}

void PlayMusic(unsigned int noteReload){
	T0RH=(unsigned char)(noteReload>>8);
	T0RL=(unsigned char)noteReload;
}

void InterruptTimer0() interrupt 1{
	TH0=T0RH;
	TL0=T0RL;
	buzz=~buzz;
}

void reload(unsigned int *noteReload,float *noteFreq,unsigned char len){
	unsigned char i=0;
	for(i=0;i<len;i++){
		noteReload[i]=65536-heart/12/(noteFreq[i]*2);
	}
}