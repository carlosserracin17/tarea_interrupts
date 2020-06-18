/*
 * Tarea_interrupts.c
 *
 * Created: 06/17/2020 1:10:49 p. m.
 * Author : cesh_
 */ 

#define F_CPU 2000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

//#define int_btn PORTB6 //se inabilito el interruptor del puerto B6
#define int_btn1 PORTA2
//#define led_blue PORTB0 // se inabilito el led azul del puerto B0
#define led_green PORTA3
#define led_yellow PORTA5
#define led_red PORTA4
#define pot PORTA1
volatile unsigned int analogResult = 0;



int main(void)
{
DDRA |= (1<<led_green)|(1<<led_yellow)|(1<<led_red);   //Set the Data Direction Register to output (PA3 = yellow led , PA6 = green led, PA4= red led)
DDRA &= ~((1<<pot)|(1<<int_btn1));	//Set the Data Direction Register for the POT to input

//DDRB |= (1<<led_blue); //set as output
//DDRB &= ~(1<<int_btn); //set as input and enable pull-up resistor on btn

ADMUX =
(0 << REFS1) |  (0 << REFS0) |   // Sets ref. voltage to VCC
(0 << ADLAR) |				     // 0: right adjust, 1: left adjust
(0 << MUX4) | (0 << MUX3) | (0 << MUX2)  |	(0 << MUX1)  |	(1 << MUX0);   // ADC1 working


ADCSRA =
(1 << ADEN)  |     // Enable ADC
(1 << ADSC)  |     // Start Conversion
(1 << ADATE)  |     // Enable Auto trigger
(0 << ADIF)  |     // ADC interrupt flag
(1 << ADIE)  |     //ADC Interrupt Enable
(1 << ADPS2) | (1 << ADPS1) |  (1 << ADPS0);  // set pre-scaler to 128

ADCSRB =
(0<<ADTS2)|		//Free running mode bit 2
(0<<ADTS1)|		//Free running mode bit 1
(0<<ADTS0);		//Free running mode bit 0

GIMSK = (1<<INT1);  //habilita la interrupcion externa INT1 (solo se puede habilitar 1 a la vez INT0 o INT1)

MCUCR = (0<<ISC01) | (1<<ISC00); //cualquier cambio logico en INT0 o INT1 genera una solicitud de interrupcion


sei();

while (1)
 {
	if(analogResult<=200)
	{
		PORTA|=(1<<led_yellow);
		PORTA&=~(1<<led_green);
		_delay_ms(100);
	}
	else if(analogResult>=800)
	{
		PORTA&=~(1<<led_yellow);
		PORTA|=(1<<led_green);
		_delay_ms(100);
	}
	else
	{
		PORTA &=~((1<<led_green)|(1<<led_yellow));
	}
 }
}


ISR(ADC_vect) //interrupt function
{
	//The ADC generates a 10-bit result
	//which can be found in the ADC Result Registers, ADCH and ADCL
	unsigned int binary_weighted_voltage_low = ADCL; //Read 8 low bits first
	unsigned int binary_weighted_voltage_high = ((unsigned int)(ADCH << 8)); //Read 2 high bits
	analogResult = binary_weighted_voltage_low | binary_weighted_voltage_high;
}

/*
ISR(INT0_vect)
{
	if((PINB & (1<<int_btn))== 0){
		PORTB |= (1<<led_blue); //when btn is pressed, int get in
	}
	else{
		PORTB &= ~(1<<led_blue); //when btn is released, we got a logical change again, int get in again and reset led to off
	}
}
*/

ISR(INT1_vect)
{
	if((PINA & (1<<int_btn1))== 0){
		PORTA |= (1<<led_red); //when btn is pressed, int get in
	}
	else{
		PORTA &= ~(1<<led_red); //when btn is released, we got a logical change again, int get in again and reset led to off
	}
}
