#define LCD_DB P0

sbit LCD_RS=P2^6;
sbit LCD_RW=P2^5;
sbit LCD_E=P2^7;


void LcdInit();
void LcdWaitReady();
void LcdWriteCmd(unsigned char cmd);
void LcdWriteDat(unsigned char dat);
void LcdSetCursor(unsigned char x,unsigned char y);
void LcdShowStr(unsigned char x,unsigned char y,unsigned char *str);