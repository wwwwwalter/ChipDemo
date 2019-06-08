#include <reg52.h>

//in
sbit Col0 = P1^4;
sbit Col1 = P1^5;
sbit Col2 = P1^6;
sbit Col3 = P1^7;
//out
sbit Row0 = P1^0;
sbit Row1 = P1^1;
sbit Row2 = P1^2;
sbit Row3 = P1^3;

unsigned char KeySta[4][4] = {
	{1,1,1,1},
	{1,1,1,1},
	{1,1,1,1},
	{1,1,1,1}
};

unsigned char code KeyCodeMap[4][4]={
	{'1', '2', '3', 0x26},
	{'4', '5', '6', 0x28},
	{'7', '8', '9', 0x25},
	{'0', 0x1B, 0x0D, 0x27} 
};


extern void KeyAction(unsigned char keycode);

void KeyGet(){
	unsigned char i,j;
	static unsigned char backup[4][4]={
		{1,1,1,1},
		{1,1,1,1},
		{1,1,1,1},
		{1,1,1,1}
	};
	
	for(i=0;i<4;++i){
		for(j=0;j<4;++j){
			if(backup[i][j]!=KeySta[i][j]){
				if(backup[i][j]!=0){					
					
					KeyAction(KeyCodeMap[i][j]);
				}
				backup[i][j]=KeySta[i][j];
			}
		}
	}	
}

void KeyScan(){
	unsigned char i;
	static unsigned char keyout=0;
	static unsigned char keybuf[4][4] = {
	 	{0xff,0xff,0xff,0xff},
		{0xff,0xff,0xff,0xff},
		{0xff,0xff,0xff,0xff},
		{0xff,0xff,0xff,0xff}
	};
	
	keybuf[keyout][0]=(keybuf[keyout][0]<<1)|Col0;
	keybuf[keyout][1]=(keybuf[keyout][1]<<1)|Col1;
	keybuf[keyout][2]=(keybuf[keyout][2]<<1)|Col2;
	keybuf[keyout][3]=(keybuf[keyout][3]<<1)|Col3;
	
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
		default:break;
	}
	
	keyout++;
	keyout=keyout&0x03;
}
