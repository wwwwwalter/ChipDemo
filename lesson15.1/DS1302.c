#include <reg52.h>

sbit CE=P3^5;
sbit CLK=P3^6;
sbit IO=P1^3;


typedef struct sTime{
	unsigned int year;
	unsigned char mon;
	unsigned char day;
	unsigned char hour;
	unsigned char min;
	unsigned char sec;
	unsigned char week;
}Time;



void DS1302ByteWrite(unsigned char dat){
	unsigned char mask;
	for(mask=0x01;mask!=0;mask<<=1){
	 	if((mask&dat)!=0){
		 	IO=1;
		}else{
		 	IO=0;
		}
		CLK=1;
		CLK=0;
	}
	IO=1;
}

unsigned char DS1302ByteRead(){
	unsigned char mask;
	unsigned char dat=0;
	for(mask=0x01;mask!=0;mask<<=1){
	   if(IO!=0){
			dat|=mask;
	   }
	   CLK=1;
	   CLK=0;
	}
	return dat;
}

void DS1302SingleWrite(unsigned char reg,unsigned char dat){
	CE=1;
	DS1302ByteWrite((reg<<1)|0x80);
	DS1302ByteWrite(dat);
	CE=0;
}

unsigned char DS1302SingleRead(unsigned char reg){
	unsigned char dat;
	CE=1;
	DS1302ByteWrite((reg<<1)|0x81);
	dat=DS1302ByteRead();
	CE=0;
	return dat;
}

void DS1302BurstWrite(unsigned char *dat){
	unsigned char i;
	CE=1;
	DS1302ByteWrite(0xBE);
	for(i=0;i<8;++i){
	 	DS1302ByteWrite(dat[i]);
	}
	CE=0;
}

void DS1302BurstRead(unsigned char *dat){
	unsigned char i;
	CE=1;
	DS1302ByteWrite(0xBF);
	for(i=0;i<8;++i){
	 	dat[i]=DS1302ByteRead();
	}							
	CE=0;
}
void GetRealTime(Time *time){
	unsigned char buf[8];
	DS1302BurstRead(buf);
	time->year=buf[6]+0x2000;
	time->mon=buf[4];
	time->day=buf[3];
	time->hour=buf[2];
	time->min=buf[1];
	time->sec=buf[0];
	time->week=buf[5];
}

void SetRealTime(Time *time){
	unsigned char buf[8];
	buf[7]=0;
	buf[6]=time->year;
	buf[5]=time->week;
	buf[4]=time->mon;
	buf[3]=time->day;
	buf[2]=time->hour;
	buf[1]=time->min;
	buf[0]=time->sec;
	DS1302BurstWrite(buf);
}
void InitDS1302(){
 	unsigned char dat;
	Time code InitTime = {0x2019,0x06,0x05,0x11,0x55,0x00,0x03};
	CE=0;
	CLK=0;
	dat=DS1302SingleRead(0);
	if((dat&0x80)!=0){
	 	DS1302SingleWrite(7,0x00);
		SetRealTime(&InitTime);
	}
}



