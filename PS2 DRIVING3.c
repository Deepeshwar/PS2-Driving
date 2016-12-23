
#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "USART_32.h"
#include "PS2.h"

enum {select, leftStick, rightStick, start, up, right, down, left}; //3rd byte
enum {leftFront2, rightFront2, leftFront1, rightFront1, triangle_up, circle_right, cross_down, square_left}; // 4th byte

enum left;

int isPressed(uint8_t dataByte, uint8_t dataBit) {
	return ((dataByte & (1 << dataBit)) ? 1 : 0);
}

uint8_t x,y;

int num;
void check_x(int num);

int main(void) {
	USART_Init(12);
	USART_TransmitString("Up and running");
	_delay_ms(50);
	init_PS2();
	_delay_ms(2000);
	
	uint8_t x,y;
	
	DDRC|=0xFF;
	DDRD|=0xFF;
	
    while (1) {
	
		scan_PS2();
		
		x=~ data_array[3];
		y=~ data_array[4];
		
	   if(isPressed(x, right)) {
			//sharp right
			PORTC |=(1<<PINC1)|(1<<PINC2)|(1<<PINC3);
			USART_Transmitchar('r');
			check_x(x);
			//return;
		}
		else if(isPressed(x, left)) {
			//sharp left 
			PORTC|=(1<<PINC0)|(1<<PINC1)|(1<<PINC3);
			USART_Transmitchar('l');
			check_x(x);
			//return;
		}
		else if (isPressed(y,triangle_up)) {
			//forward	
			PORTC|=(1<<PINC1)|(1<<PINC3);
			USART_Transmitchar('T');
			check_x(x);
			//return;
		}
		else if (isPressed(y,square_left)) {	
			//left turn
			PORTC|=(1<<PINC3);
			USART_Transmitchar('L');
			check_x(x);
			//return;
		}
		else if (isPressed(y,circle_right)) {
			//right turn
			PORTC|=(1<<PINC1);
			USART_Transmitchar('R');
			check_x(x);
			//return;
		}
		else if (isPressed(y,cross_down)) {
			//backward
			PORTC|=(1<<PINC0)|(1<<PINC1)|(1<<PINC2)|(1<<PINC3);
			USART_Transmitchar('D');
			check_x(x);
		}
		else
		{
			PORTD &=0x00;
			PORTC &=0x00;
		   	USART_TransmitString("ku6ni");
			USART_Transmitchar(0x0A);
			check_x(x);
		}
		
		USART_Transmitchar(0x0D);
		
	 }
}





void check_x(int num)
{
	if(isPressed(num,up)) {
		//up
		PORTD |=(1<<PIND2)|(1<<PIND3);
		PORTC |=(1<<PINC4)|(1<<PINC5);
		USART_Transmitchar('u');
		//return;
	}
	else if(isPressed(num,down)) {
		//down
		PORTD |=1<<PIND3;
		PORTD &=~(1<<PIND2);
		PORTC|=1<<PINC5;
		PORTC &=~(1<<PINC4);
		USART_Transmitchar('d');
		//return;
	}
	else
	{
		PORTC&=~(1<<PINC5);
		PORTD&=~(1<<PIND3);
		USART_TransmitString("n0thr0tle");
	}
	
}