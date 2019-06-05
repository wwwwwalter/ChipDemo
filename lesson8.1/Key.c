#include <reg52.h>
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
//in
sbit Col0 = P2^4;
sbit Col1 = P2^5;
sbit Col2 = P2^6;
sbit Col3 = P2^7;
//out
sbit Row0 = P2^0;
sbit Row1 = P2^1;
sbit Row2 = P2^2;
sbit Row3 = P2^3;



unsigned char KeySta[4][4] = {
 {1,1,1,1},
 {1,1,1,1},
 {1,1,1,1},
 {1,1,1,1}
};
/*
void main(void){
	P0 = 0x00;

	Row0 = 0;
	Row1 = 0;
	Row2 = 0;
	Row3 = 0;
			 Col0 = 1;Col1 = 1;Col2 = 1;Col3 = 1; 




	while(1){
		if(Col0==0){P0 = LedCharYang[0];}
		else if(Col1==0){P0=LedCharYang[1];}
		else if(Col2==0){P0=LedCharYang[2];}
		else if(Col3==0){P0=LedCharYang[3];}				 	

	}
}	*/


void main(void){
 	unsigned char i,j;
	unsigned char backup[4][4] = {
		{1,1,1,1},
 		{1,1,1,1},
		{1,1,1,1},
 		{1,1,1,1}
	};

	EA		= 1;
	TMOD	= 0x01;
	TH0 	= 0xfc;
	TL0 	= 0x67;
	ET0 	= 1;
	TR0 	= 1;

	while(1){
	 	for(i = 0;i<4;++i){
		 	for(j = 0;j<4;++j){
			 	if(backup[i][j]!=KeySta[i][j]){
				 	if(backup[i][j]!=0){
					 	P0=LedCharYang[i*4+j];
					}
					backup[i][j] = KeySta[i][j];
				}
			}
		}										
	}											
}

void InterruptTimer() interrupt 1{
	unsigned char i;
	static unsigned char keyout=0;
	static unsigned char keybuf[4][4] = {
	 	{0xff,0xff,0xff,0xff},
		{0xff,0xff,0xff,0xff},
		{0xff,0xff,0xff,0xff},
		{0xff,0xff,0xff,0xff}
	};

	TH0=0xfc;
	TL0=0x67;

	keybuf[keyout][0] = (keybuf[keyout][0]<<1)|Col0;
	keybuf[keyout][1] = (keybuf[keyout][1]<<1)|Col1;
	keybuf[keyout][2] = (keybuf[keyout][2]<<1)|Col2;
	keybuf[keyout][3] = (keybuf[keyout][3]<<1)|Col3;

	for(i = 0;i<4;++i){
	 	if((keybuf[keyout][i]&0x0f)==0x00){
		 	KeySta[keyout][i] = 0;
		}else if((keybuf[keyout][i]&0x0f)==0x0f){
		 	KeySta[keyout][i] = 1;
		}else{
		 	;
		}
	}
	


	switch(keyout){
	 	case 0:Row0=1;Row1=0;break;
		case 1:Row1=1;Row2=0;break;
		case 2:Row2=1;Row3=0;break;
		case 3:Row3=1;Row0=0;break;
		default: break;
	}

	keyout++;					
	//0000 0000
	//0000 0001
	//0000 0010
	//0000 0011
	//0000 0100
	keyout = keyout&0x03;//0000 0011

	
}
  

