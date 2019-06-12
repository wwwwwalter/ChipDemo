#include <reg52.h>

extern void LcdInit();
extern void LcdShowStr(unsigned char x,unsigned char y,unsigned char *str);
extern void E2Read(unsigned char *buf,unsigned char addr,unsigned char len);
extern void E2Write(unsigned char *buf,unsigned char addr,unsigned char len);

void MemToStr(unsigned char *str,unsigned char *src,unsigned char len);

void main(void){
	unsigned char i;
	unsigned char buf[5];
	unsigned char str[20];	
	LcdInit();	
	for(i=0;i<sizeof(buf);i++){
		buf[i]=buf[i]+1+i;
	}
	E2Write(buf,0x0e,sizeof(buf));
	
	E2Read(buf,0x0e,sizeof(buf));
	MemToStr(str,buf,sizeof(buf));
	LcdShowStr(0,0,str);
	while(1);
}

void MemToStr(unsigned char *str,unsigned char *src,unsigned char len){
	unsigned char tmp;
	while(len--){
		tmp=*src>>4;
		if(tmp<=9){
			*str++=tmp+'0';
		}else{
			*str++=tmp-10+'A';
		}
		
		tmp=*src&0x0f;
		if(tmp<=9){
			*str++=tmp+'0';
		}else{
			*str++=tmp-10+'A';
		}
		*str++=' ';
		src++;
	}
}