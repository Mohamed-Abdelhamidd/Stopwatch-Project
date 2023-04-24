#include <avr/io.h>
#include <avr/interrupt.h>

void seg1 (void);
void seg2 (void);
void seg3 (void);
void seg4 (void);
void seg5 (void);
void seg6 (void);

unsigned char segm1 = 0;
unsigned char segm2 = 0;
unsigned char segm3 = 0;
unsigned char segm4 = 0;
unsigned char segm5 = 0;
unsigned char segm6 = 0;

//////////////////////////Interrupt initialize/////////////
void Interrupts_Init (void){
	MCUCR = (1<<ISC01) | (1<<ISC11) | (1<<ISC10);
	GICR = (1<<INT0) | (1<<INT1) | (1<<INT2);
	MCUCSR &= ~(1<<ISC2);
	SREG |= (1<<7);
}

////////////////////////////Timer initialize////////////////
void Timer1_Init_CTC (void)
{
	TCCR1A = (1<<FOC1A) | (1<<FOC1B);
	TCCR1B = (1<<WGM12) | (1<<CS10) | (1<<CS11);
	TCNT1 = 0;
	OCR1A = 15624;         /*prescaler 64*/
	TIMSK = (1<<OCIE1A);


}
///////////////////////INT0/////////////////////////////////
ISR (INT0_vect){
	segm1 = 0;
	segm2 = 0;
	segm3 = 0;
	segm4 = 0;
	segm5 = 0;
	segm6 = 0;
}

//////////////////////////INT1/////////////////////////////
ISR (INT1_vect){
	TCCR1B &=  ~(1<<CS10) & ~(1<<CS11);
}

////////////////////////////INT2///////////////////////////
ISR (INT2_vect){
	TCCR1B |= (1<<CS10) | (1<<CS11);
}

/////////////////////////Timer ISR/////////////////////////
ISR (TIMER1_COMPA_vect){

	OCR1A = 15624;
	segm1++;
	if (segm1 == 10){
		segm2++;
	}
	if (segm2 == 6){
		segm3++;
	}
	if (segm3 == 10){
		segm4++;
	}
	if (segm4 == 6){
		segm5++;
	}
	if (segm5 == 10){
		segm6++;
	}
}

/////////////////////////////main////////////////////////////
int main (void) {

	DDRC |= 0x0F;
	DDRA  |= 0x3F;
	DDRD &= ~(1<<2) & ~(1<<3);
	DDRB &= ~(1<<2);

	PORTB |= (1<<2);
	PORTD |= (1<<2);
	PORTA |= (0xFF>>2);

	Interrupts_Init ();

	Timer1_Init_CTC ();

	for (;;){

		seg1 ();
		seg2 ();
		seg3 ();
		seg4 ();
		seg5 ();
		seg6 ();

	}
}

//////////////////////segments///////////////////////////
void seg1 (void){
	segm1 %= 10;
	PORTA = (1<<0);
	PORTC = segm1;
}

void seg2 (void){
	segm2 %= 6;
	PORTA = (1<<1);
	PORTC = segm2;
}

void seg3 (void){
	segm3 %= 10;
	PORTA = (1<<2);
	PORTC = segm3;
}

void seg4 (void){
	segm4 %= 6;
	PORTA = (1<<3);
	PORTC = segm4;
}

void seg5 (void){
	if (segm6 == 1){
		segm5 %= 10;
	}
	else if (segm6 == 2){
		segm5 %= 4;
	}
	PORTA = (1<<4);
	PORTC = segm5;
}

void seg6 (void){
	segm6 %= 3;
	PORTA = (1<<5);
	PORTC = segm6;
}
