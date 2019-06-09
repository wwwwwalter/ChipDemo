#include <reg52.h>
#define speed 200

sbit PHA=P0^0;
sbit PHB=P0^1;

sbit A1=P0^2;
sbit A2=P0^3;
sbit B1=P0^4;
sbit B2=P0^5;
//http://www.elecfans.com/d/586969.html

//PHA=0 A-  PHA=1 A+
//PHB=0 B-  PHB=1 B+
//A1=A2=0 max
//A1=A2=1 min
//B1=B2=0 max
//B1=B2=1 min


void delay(unsigned int time);
void ax(bit dir,bit on);
void bx(bit dir,bit on);

void main(void){
	ax(0,0);delay(speed);//b
	bx(1,0);delay(speed);//x
	ax(1,0);delay(speed);//n
	bx(0,0);delay(speed);//d
	
}

void ax(bit dir,bit on){
	PHA=dir;
	A1=1;
	A2=0;
}

void bx(bit dir,bit on){
	PHB=dir;
	B1=on;
	B2=on;
}

//10us
void delay(unsigned long time){
	while(time--);
}