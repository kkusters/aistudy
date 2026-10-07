// CH_KEY.H

#ifndef _CH_KEY_H
#define _CH_KEY_H

extern unsigned char key;
extern unsigned char key_func;
extern int key_inc;

#define LEFT     0x11
#define RIGHT    0x12
#define UP	     0x13 
#define DOWN     0x14
#define OK       0x15
#define PLUS_MIN 0x16
// let op F1 tot en met F6 moeten groter zijn dan '0' tm '9' (0x30-0x39)
#define F1		 0x40
#define F2		 0x41
#define F3       0x42 
#define F4		 0x43
#define F5		 0x44
#define F6		 0x45
#define PREV   0x50
#define NEXT   0x51

void Key_Control(void);
void Key_Init(void);

#endif


