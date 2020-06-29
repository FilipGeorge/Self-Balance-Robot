/*
 * motors.cpp
 *
 * Created: 5/24/2018 8:38:29 PM
 *  Author: Filip Manole
 */

#include "motors.h"

/* Configure motor TIMER0 */
void motor_init(void){

	/* Fast PWM */
	TCCR0A |= (1 << WGM01) | (1 << WGM00);
	
	/* Non inverting mode */
	TCCR0A |= (1 << COM0A1) | (1 << COM0B1);
	
	/* No prescaling */
	TCCR0B |= (1 << CS00);

	/* Setting PWM pins as output */
	DDRB |= (1 << PMW1) | (1 << PMW2);
	
	/* At the begging the motors are stopped */
	DDRA |= (1 << IN_A1) | (1 << IN_B1) | (1<< IN_A2) | (1 << IN_B2);

}

void move_forward(uint8_t ocr) {
	
	OCR0A = ocr;
	OCR0B = ocr;
	
	PORTA |= (1 << IN_A1) | (1 << IN_B2);
	PORTA &= ~(1 << IN_B1) & ~(1 << IN_A2);
}

void move_backward(uint8_t ocr) {
	
	OCR0A = ocr;
	OCR0B = ocr;
	
	PORTA &= ~(1 << IN_A1) & ~(1 << IN_B2);
	PORTA |= (1 << IN_B1) | (1 << IN_A2);
}

void move_stop() {
	PORTA |= (1 << IN_A1) | (1 << IN_B1) | (1 << IN_A2) | (1 << IN_B2);
}