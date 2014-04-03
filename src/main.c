#include <salvo.h>
#include <p18f4550.h>
#include <delays.h>
#undef OSC
 
/*  PIC Configuratings */
#pragma config FOSC = HS
#pragma config WDT = OFF
#pragma config PBADEN = OFF
#pragma config LVP = OFF
#pragma config PWRT = ON
#pragma config DEBUG= OFF
 
void Motor_Run(void);
void Input_Read(void);

_OSLabel (Motor_Run1)
_OSLabel (Input_Read1)

unsigned char left=0;
unsigned char right=0;

void Motor_Run(void)
{
	for(;;){
		if (left==1){
			PORTDbits.RD6 = 1;
			PORTDbits.RD5=0;
			PORTDbits.RD7=1;
			//left=left-1;
		}
		else if (right==1){
			PORTDbits.RD6=0;
			PORTDbits.RD5=1;
			PORTDbits.RD7=1;
		}
		else {
			PORTDbits.RD6=0;
			PORTDbits.RD5=0;
			PORTDbits.RD7=0;
		}
		OS_Yield(Motor_Run1);
	}
}

void Input_Read(void)
{
	for(;;)
	{
		if(PORTDbits.RD4==0){
			left=1;
			right=0;
		}
		else if (PORTDbits.RD3==0){
			left=0;
			right=1;
		}
		else {
			left=0;
			right=0;
		}
		OS_Yield(Input_Read1);
	}
}

void main(void)
{
	PORTD=0;
	TRISD=0b00011000;

	OSInit();
	OSCreateTask(Motor_Run,OSTCBP(1),10);
	OSCreateTask(Input_Read,OSTCBP(2),10);
	while(1)OSSched();
}