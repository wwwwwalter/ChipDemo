#include <reg52.h>
sbit ADDR0 = P1^0;
sbit ADDR1 = P1^1;
sbit ADDR2 = P1^2;

unsigned char code LedCharYin[] = {
	0x3f, 0x06, 0x5b, 0x4f,
  	0x66, 0x6d, 0x7d, 0x07,
  	0x7f, 0x6f, 0x77, 0x7c,
  	0x39, 0x5e, 0x79, 0x71
};

unsigned char code LedCharYang[] = {
	0xC0, 0xF9, 0xA4, 0xB0, 
  	0x99, 0x92, 0x82, 0xF8,
	0x80, 0x90, 0x88, 0x83, 
	0xC6, 0xA1, 0x86, 0x8E	
};

unsigned char LedBuff[8] = {
	0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff
};

unsigned int cnt = 0;
unsigned long sec = 0;
unsigned char i = 0;

void main()
{	
	//ÖÐ¶Ï
	EA = 1;	
	ET0 = 1;		  
	//¼ÆÊ±Æ÷
	TMOD = 0x01;
	TH0 = 0xFC;//1ms
	TL0 = 0x1A;
	TR0 = 1;

	while (1){
		if(cnt % 100==0){
		 	P0 = LedCharYang[cnt/100%10];
		}
		if(cnt>=1000){
			cnt = 0;
			sec++;
			LedBuff[0] = LedCharYin[sec%10];
			LedBuff[1] = LedCharYin[sec/10%10];
			LedBuff[2] = LedCharYin[sec/100%10];
			LedBuff[3] = LedCharYin[sec/1000%10];
			LedBuff[4] = LedCharYin[sec/10000%10];
			LedBuff[5] = LedCharYin[sec/100000%10];
			LedBuff[6] = LedCharYin[sec/1000000%10];
			LedBuff[7] = LedCharYin[sec/10000000%10];							
		}
	}
}

void InterruptTimer() interrupt 1{
	TH0 = 0xFC;
	TL0 = 0x1A;
	cnt++;

	P2 = 0x00;
	switch(i){
		case 0: ADDR2 = 1;ADDR1 = 1;ADDR0 = 1;i++;P2=LedBuff[0];break;
		case 1:	ADDR2 = 1;ADDR1 = 1;ADDR0 = 0;i++;P2=LedBuff[1];break;
		case 2:	ADDR2 = 1;ADDR1 = 0;ADDR0 = 1;i++;P2=LedBuff[2];break;
		case 3:	ADDR2 = 1;ADDR1 = 0;ADDR0 = 0;i++;P2=LedBuff[3];break;
		case 4:	ADDR2 = 0;ADDR1 = 1;ADDR0 = 1;i++;P2=LedBuff[4];break;
		case 5:	ADDR2 = 0;ADDR1 = 1;ADDR0 = 0;i++;P2=LedBuff[5];break;
		case 6:	ADDR2 = 0;ADDR1 = 0;ADDR0 = 1;i++;P2=LedBuff[6];break;
		case 7: ADDR2 = 0;ADDR1 = 0;ADDR0 = 0;i=0;P2=LedBuff[7];break;
		default:break;											  
	}
}