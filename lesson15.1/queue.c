#include <reg52.h>

#define QUEUELEN 10

typedef struct Queue{
	unsigned char dat[QUEUELEN];
	unsigned char front;
	unsigned char rear;
}MsgQueue;

MsgQueue msg;

bit IsEmptyQueue();
bit IsFullQueue();
void InitQueue();
unsigned char DeleteQueue();
void EnterQueue(unsigned char byte);


void InitQueue(){
	msg.front=0;
	msg.rear=0;
}

bit IsEmptyQueue(){
	return msg.front==msg.rear;
}

bit IsFullQueue(){
	return (msg.rear+1)%QUEUELEN==msg.front;
}

void EnterQueue(unsigned char byte){
	if(IsFullQueue()){
		;
	}
	else{
		if(msg.rear==QUEUELEN){
			msg.rear=0;
		}		
		msg.dat[msg.rear++]=byte;
	}
}

unsigned char DeleteQueue(){
	unsigned char temp=0;
	if(IsEmptyQueue()){
		return 0xFF;
	}
	if(msg.front==QUEUELEN)
	{
		msg.front=0;
	}
	temp=msg.dat[msg.front];
	msg.dat[msg.front++]=0x00;
	return temp;
}




