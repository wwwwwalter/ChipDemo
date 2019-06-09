#include <reg52.h>
#define speed 1000000

sbit PHA=P1^0;
sbit PHB=P1^1;

sbit A1=P1^2;
sbit A2=P1^3;
sbit B1=P1^4;
sbit B2=P1^5;
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
	ax(0,0);bx(1,1);delay(speed);//b
	ax(0,0);bx(1,0);delay(speed);//xb
	ax(1,1);bx(1,0);delay(speed);//x
	ax(1,0);bx(1,0);delay(speed);//xn
	ax(1,0);bx(0,1);delay(speed);//n
	ax(1,0);bx(0,0);delay(speed);//dn
	ax(0,1);bx(0,0);delay(speed);//d
	ax(0,0);bx(0,0);delay(speed);//db
}

void ax(bit dir,bit on){
	PHA=dir;
	A1=on;
	A2=on;
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