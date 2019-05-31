#include <reg52.h>
#include <intrins.h>

#define uchar unsigned char
#define uint unsigned int
#define ulong unsigned long

sbit SDI = P3^4;
sbit LCK = P3^5;
sbit SCK = P3^6;

void HC595SendData(uchar data0,uchar data1,uchar data2,uchar data3){
	uchar i = 0;
	SCK=1;
	LCK=1;  	
	for(i = 0;i<8;++i){
	 	SDI=data0&0x01;
		data0>>=1;
		SCK=0;
		_nop_();
		_nop_();
		SCK=1;
		_nop_();
		_nop_();
	}
	for(i = 0;i<8;++i){
	 	SDI=data1&0x01;
		data1>>=1;
		SCK=0;
		_nop_();
		_nop_();
		SCK=1;
		_nop_();
		_nop_();
	}
		for(i = 0;i<8;++i){
	 	SDI=data2&0x01;
		data2>>=1;
		SCK=0;
		_nop_();
		_nop_();
		SCK=1;
		_nop_();
		_nop_();
	}
		for(i = 0;i<8;++i){
	 	SDI=data3&0x01;
		data3>>=1;
		SCK=0;
		_nop_();
		_nop_();
		SCK=1;
		_nop_();
		_nop_();
	}
	LCK=0;
	_nop_();
	_nop_();
	LCK=1;
	_nop_();
	_nop_();	 

}





	  	
void main(void){



	
 	while(1){
	 	 HC595SendData(0x00,0x00,0x01,0x80);
		 
		 HC595SendData(0xFE,0x7F,0xff,0xff);
		 
	}
}