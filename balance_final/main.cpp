/*
 * balance_final.cpp
 *
 * Created: 5/24/2018 6:34:17 PM
 * Author : Filip Manole
 */ 

#include <avr/io.h>
#include <string.h>
#include <stdio.h>

#include "bno055.h"
#include "motors.h"
#include "pid.h"
#include "usart0.h"

#define MIN_ANGLE 1
#define MAX_ANGLE 90
#define OFFSET 1

BNO055 gyro;

/* Call PID controller */
ISR(TIMER1_COMPA_vect)
{
	int angle;
	double output;
	char buffer[30];
	memset(buffer, 0, 30);
	
	imu::Vector<3> euler;
	euler = gyro.getVector(BNO055::VECTOR_EULER);
	angle = euler.z() - OFFSET;
	
	//sprintf(buffer, "input: %i\n", angle);
	//USART0_print(buffer);
	
	if(abs(angle) < MIN_ANGLE || abs(angle) > MAX_ANGLE)
		move_stop();
	else {
		output = compute(angle);
		
		//sprintf(buffer, "input: %i, output: %.3f\n", angle, output);
		//USART0_print(buffer);
		
		if(output > 0) {
			
			if(output - 0xFF > 0)
				output = 0xFF;
			
			if(abs(output) > 45)
				move_backward(output);
		} else {
			
			if(-output - 0xFF > 0)
				output = -0xFF;
			
			if(abs(output) > 45)
				move_forward(-output);
		}
		
	}
}

/* Configure timer to generate interrupt every 100 Mhz */
void timer1_init() {
	
	/* CTC mode */
	TCCR1B |= (1 << WGM12);
	
	/* Prescaler clk / 1024 */
	TCCR1B |= (1 << CS10) | (1 << CS12);
	
	/* Enable output compare A interrupt */
	TIMSK1 |= (1 << OCIE1A);
	
	/* 200Hz */
	//OCR1A = 0x4E;
	
	OCR1A = 0x34;
	
	/* 100Hz */
	//OCR1A = 0x9C;
}

void init_robot() {
	sei();
	gyro.begin();
	motor_init();
	timer1_init();
	USART0_init();
}

int main(void)
{
    init_robot();
	
    while (1) 
    {	
    }
}

