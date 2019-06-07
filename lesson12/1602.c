#include <reg52.h>
#define LCD_DB P0
sbit LCD_RS=P2^6;
sbit LCD_RW=P2^5;
sbit LCD_E=P2^7;

void InitLcd();
void LcdWaitReady();
void LcdWriteCmd(unsigned char cmd);
void LcdWriteDat(unsigned char dat);
void LcdSetCursor(unsigned char x,unsigned char y);
void LcdShowStr(unsigned char x,unsigned char y,unsigned char *str);


void main(void){
	unsigned char str[]="0123456789abcde";
	InitLcd();
	LcdShowStr(0,0,str);
	while(1);
}

void InitLcd(){
	LcdWriteCmd(0x38);
	LcdWriteCmd(0x0f);//0000 1111
	LcdWriteCmd(0x06);//0000 0110
	LcdWriteCmd(0x01);
}

void LcdWaitReady(){
	unsigned char sta;
	LCD_DB=0xff;
	LCD_RS=0;
	LCD_RW=1;
	do{
		
		LCD_E=1;
		sta=LCD_DB;
		LCD_E=0;
	}while(sta&0x80);
}

void LcdWriteCmd(unsigned char cmd){
	LcdWaitReady();
	LCD_RS=0;
	LCD_RW=0;
	LCD_DB=cmd;
	LCD_E=1;
	LCD_E=0;
}

void LcdWriteDat(unsigned char dat){
	LcdWaitReady();
	LCD_RS=1;
	LCD_RW=0;
	LCD_DB=dat;
	LCD_E=1;
	LCD_E=0;
}

void LcdSetCursor(unsigned char x,unsigned char y){
	unsigned char addr;
	if(y==0){
		addr=0x00+x;
	}else{
		addr=0x40+x;
	}
	LcdWriteCmd(addr|0x80);
}

void LcdShowStr(unsigned char x,unsigned char y,unsigned char *str){
	LcdSetCursor(x,y);
	while(*str!='\0'){
		unsigned int i=0;
		for(i=0;i<30000;++i);
		LcdWriteDat(*str);
		str++;
	}
}