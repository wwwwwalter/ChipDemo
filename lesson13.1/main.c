#include <reg52.h>
#define freq 24000000
unsigned char T0RH=0;
unsigned char T0RL=0;
unsigned char step=0;
unsigned char oprt=0;
signed long num1=0;
signed long num2=0;
signed long result=0;

extern void LcdInit();
extern void LcdShowStr(unsigned char x,unsigned char y,unsigned char *str);
extern void LcdAreaClear(unsigned char x,unsigned char y,unsigned char len);
extern void LcdFullClear();
extern void KeyGet();
extern void KeyScan();

void ConfigTimer0(unsigned int ms);
void KeyAction(unsigned char keycode);
void NumKeyAction(unsigned char n);
void OprtKeyAction(unsigned char type);
void GetResult();
void Reset();
void ShowOprt(unsigned char y,unsigned char type);
unsigned char LongToString(unsigned char *str,signed long dat);

void main(void){
	EA=1;
	ConfigTimer0(1);
	LcdInit();
	LcdShowStr(15,1,"0");
	while(1){
		KeyGet();
	}
}

void ConfigTimer0(unsigned int ms){
	unsigned long tmp;
	tmp=freq/12/1000*ms;
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
void InterruptTimer0() interrupt 1{
	TH0=T0RH;
	TL0=T0RL;
	KeyScan();
}

void KeyAction(unsigned char keycode){
	if((keycode>='0')&&(keycode<='9')){
		NumKeyAction(keycode-'0');
	}else if(keycode==0x26){
		OprtKeyAction(0);
	}else if(keycode==0x28){
		OprtKeyAction(1);
	}else if(keycode==0x25){
		OprtKeyAction(2);
	}else if(keycode==0x27){
		OprtKeyAction(3);
	}else if(keycode==0x0d){
		GetResult();
	}else if(keycode==0x1b){
		Reset();
		LcdShowStr(15,1,"0");
	}
}

void NumKeyAction(unsigned char n){
	unsigned char len;
	unsigned char str[12];
	if(step>1){
		Reset();
	}
	if(step==0){
		num1=num1*10+n;
		len=LongToString(str,num1);
		LcdShowStr(16-len,1,str);
	}else{
		num2=num2*10+n;
		len=LongToString(str,num2);
		LcdShowStr(16-len,1,str);
	}
}

void OprtKeyAction(unsigned char type){
	unsigned char len;
	unsigned char str[12];
	if(step==0){
		len=LongToString(str,num1);
		LcdAreaClear(0,0,16-len);
		LcdShowStr(16-len,0,str);
		ShowOprt(1,type);
		LcdAreaClear(1,1,14);
		LcdShowStr(15,1,"0");
		oprt=type;
		step=1; 
	}
}

unsigned char LongToString(unsigned char *str,signed long dat){
	signed char i=0;
	unsigned char len=0;
	unsigned char buf[12];
	if(dat<0){
		dat=-dat;
		*str++='-';
		len++;
	}	
	do{
		buf[i++]=dat%10;
		dat/=10;
	}while(dat>0);	
	len+=i;
	while(i--){
		*str++=buf[i]+'0';
	}
	*str='\0';
	return len;
}

void GetResult(){
	unsigned char len;
	unsigned char str[12];
	if(step==1){
		step=2;
		switch(oprt){
			case 0:result=num1+num2;break;
			case 1:result=num1-num2;break;
			case 2:result=num1*num2;break;
			case 3:result=num1/num2;break;
			default:break;
		}
	}
	len=LongToString(str,num2);
	ShowOprt(0,oprt);
	LcdAreaClear(1,0,16-1-len);
	LcdShowStr(16-len,0,str);
	len=LongToString(str,result);
	LcdShowStr(0,1,"=");
	LcdAreaClear(1,1,16-1-len);
	LcdShowStr(16-len,1,str);
}

void Reset(){
	num1=0;
	num2=0;
	step=0;
	LcdFullClear();
}

void ShowOprt(unsigned char y,unsigned char type){
	switch(type){
		case 0:LcdShowStr(0,y,"+");break;
		case 1:LcdShowStr(0,y,"-");break;
		case 2:LcdShowStr(0,y,"*");break;
		case 3:LcdShowStr(0,y,"/");break;
		default:break;
	}
}