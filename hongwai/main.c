#include <reg52.h>

sbit OUT=P3^7;
sbit ACT=P3^2;

void main(void){
	while(1){
		if(OUT){
			ACT=1;
		}else{
			ACT=0;
		}
	}
	
	
}