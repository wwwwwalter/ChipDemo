#include <reg52.h>
#include <intrins.h>

sbit IO_18B20=P3^7;
sbit TURN=P2^0;

unsigned char code LedCharYin[] = {
	0x3f, 0x06, 0x5b, 0x4f,
  	0x66, 0x6d, 0x7d, 0x07,
  	0x7f, 0x6f, 0x77, 0x7c,
  	0x39, 0x5e, 0x79, 0x71,
	0x40,0x00
};
unsigned char LedBuff[8] = { 	
	0xff,0xff,0xff,0xff,
	0x00,0x00,0x00,0x00
};
unsigned char out[] = {
	0x00,
	0x00,0x00,
	0x00,0x00,0x00,0x00
};



bit Get18B20Ack();
bit Start18B20();
bit Get18B20Temp(int *temp);
void Write18B20(unsigned char dat);
unsigned char Read18B20();
void Delay10us(unsigned char t);
void ConfigTimer0(unsigned char ms);
void ConfigTimer1(unsigned char ms);
void convert(unsigned char *out);

unsigned char T0RH=0;
unsigned char T0RL=0;
unsigned char T1RH=0;
unsigned char T1RL=0;

bit flag=0;
int temp;
unsigned char intT;
unsigned int decT;


int main(void){   	 
   EA=1;
   Start18B20();
   _nop_();
   _nop_();
   ConfigTimer0(1);
   ConfigTimer1(1000);
   PT0=1;

   while(1){
		if(flag){
		 	flag=0;
			convert(out);
			LedBuff[6]=LedCharYin[out[6]];				
			LedBuff[5]=LedCharYin[out[5]];
			LedBuff[4]=LedCharYin[out[4]];
			LedBuff[4]|=0x80;
			LedBuff[3]=LedCharYin[out[3]];
			LedBuff[2]=LedCharYin[out[2]];
			LedBuff[1]=LedCharYin[out[1]];
			LedBuff[0]=LedCharYin[out[0]];
		}
   }
   return 0;
}

void convert(unsigned char *out){
	unsigned char i = 0;
	
	bit sig = temp>>15&0x01;	
	intT=temp>>4&0x7f;
	decT=temp&0x0f;

	decT=decT*10000/16;
	out[i++]=decT%10;
	out[i++]=decT/10%10;
	out[i++]=decT/100%10;
	out[i++]=decT/1000%10;

	out[i++]=intT%10;
	out[i++]=intT/10%10;

	if(sig==0){out[i]=17;}
	else{out[i]=16;}

	if(intT>=25){TURN=0;}
	else{TURN=1;}
}


void ConfigTimer0(unsigned char ms){
 	unsigned long tmp;
	tmp=12000000/12/1000*ms;
	tmp=65536-tmp;
	tmp+=12;
	T0RH=(unsigned char)(tmp>>8);
	T0RL=(unsigned char)tmp;
	TMOD&=0xf0;
	TMOD|=0x01;
	TH0=T0RH;
	TL0=T0RL;
	ET0=1;
	TR0=1;
}
void LedScan(){
 	static unsigned char i=0;
	P0=0x00;
	P1=(P1&0xf8)|~i;
	P0=LedBuff[i];
	if(i<sizeof(LedBuff)-1){
	 	i++;
	}else{
	 	i=0;
	}
}

void InterruptTimer0() interrupt 1{ 	
	TH0=T0RH;
	TL0=T0RL;
	LedScan();
}

void ConfigTimer1(unsigned char ms){
 	unsigned long tmp;
	tmp=12000000/12/1000*ms;
	tmp=65536-tmp;
	tmp+=12;
	T1RH=(unsigned char)(tmp>>8);
	T1RL=(unsigned char)tmp;
	TMOD&=0x0f;
	TMOD|=0x10;
	TH1=T1RH;
	TL1=T1RL;
	ET1=1;
	TR1=1;
}

void InterruptTimer1() interrupt 3{
 	TH1=T1RH;
	TL1=T1RL;
	Get18B20Temp(&temp);
	Start18B20();
	flag=1;
}




void Delay10us(unsigned char t){
 	do{
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
	}while(--t);
}

bit Get18B20Ack(){
	bit ack;
	EA=0;
	IO_18B20=0;
	Delay10us(50);
	IO_18B20=1;
	Delay10us(6);
	ack=IO_18B20;
	while(!IO_18B20);
	EA=1;
	return ack;
}

void Write18B20(unsigned char dat){
 	unsigned char mask;
	EA=0;
	for(mask=0x01;mask!=0;mask<<=1){
	 	IO_18B20 = 0;
		_nop_();
		_nop_();
		if((mask&dat)==0){
			IO_18B20=0;
		}else{
			IO_18B20=1;
		}
		Delay10us(6);
		IO_18B20=1;
	}
	EA=1;
}

unsigned char Read18B20(){
	unsigned char dat;
	unsigned char mask;

	EA=0;
	for(mask=0x01;mask!=0;mask<<=1){
		IO_18B20 = 0;
		_nop_();
		_nop_();
		IO_18B20= 1;
		_nop_();
		_nop_();
		if(!IO_18B20){
		 	dat&=~mask;
		}else{
			dat|=mask;
		}
		Delay10us(6);
	}
	EA=1;
	return dat;
}				    

bit Start18B20(){
 	bit ack;
	ack=Get18B20Ack();
	if(ack==0){
	 	Write18B20(0xcc);
		Write18B20(0x44);
	}
	return ~ack;
}
bit Get18B20Temp(int *temp){
 	bit ack;
	unsigned char LSB,MSB;
	ack=Get18B20Ack();

	if(ack==0){
	 	Write18B20(0xcc);
		Write18B20(0xBE);
		LSB=Read18B20();
		MSB=Read18B20();
		*temp=((int)MSB<<8)+LSB;
	}
	return ~ack;
}