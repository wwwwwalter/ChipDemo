#include <reg52.h>


sbit Key0 = P2^0;
sbit Key1 = P2^1;
sbit Key2 = P2^2;
sbit Key3 = P2^3;
sbit Key4 = P2^4;
sbit Key5 = P2^5;
sbit Key6 = P2^6;
sbit Key7 = P2^7;

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


/*void delay(){
 	unsigned int i = 1000;
	while(i--);
}*/

bit KeySta = 1;
void main(void){


	bit backup = 1;
	bit keybuf = 1;
	unsigned char cnt= 0;
	P0 = LedCharYang[0];
	P2 = 0xFF; 

	/*while(1){	
	   if(Key0==0){P0 = LedCharYang[0];}
	   if(Key1==0){P0 = LedCharYang[1];}
	   if(Key2==0){P0 = LedCharYang[2];}
	   if(Key3==0){P0 = LedCharYang[3];}
	   if(Key4==0){P0 = LedCharYang[4];}
	   if(Key5==0){P0 = LedCharYang[5];}
	   if(Key6==0){P0 = LedCharYang[6];}
	   if(Key7==0){P0 = LedCharYang[7];}
	   }*/

	

	/*while(1){
		keybuf = Key0;
		if(keybuf!=backup){
		 	delay();//µÈ´ýKey0ÎÈ¶¨
			if(keybuf==Key0){
				if(backup==0){
				 	cnt++;
					if(cnt>=10){
					 	cnt = 0;
					}
					P0=LedCharYang[cnt];
				}
				backup = keybuf;	
			}
		}	
	}*/



	EA=1;
	TMOD = 0x01;
	TH0 = 0xf8;
	TL0 = 0xcd;
	ET0= 1;
	TR0 = 1;

	while(1){
	 	if(KeySta!=backup){
		 	if(backup==0){
			 	cnt++;
				if(cnt>=10){
				 	cnt=0;
				}
				P0 = LedCharYang[cnt];
			}						  
			backup=KeySta;
		}
	}
}

void InterruptTimer() interrupt 1{
 	static unsigned char keybuf = 0xff;

	TH0 = 0xf8;
	TL0 = 0xcd;

	keybuf = (keybuf<<1)|Key0;

	if(keybuf==0x00){
	 	KeySta = 0;
	}else if(keybuf==0xff){
	 	KeySta = 1;
	}else{
	 	;
	}
}