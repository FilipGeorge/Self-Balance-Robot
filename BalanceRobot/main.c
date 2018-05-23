#include <avr/io.h>
#define F_CPU 16000000
#include <util/delay.h>
#include <string.h>

#include "mpu6050/mpu6050registers.h"
#include "mpu6050/mpu6050.h"
#include "i2chw/i2cmaster.h"
#include "usart/usart.h"
#include "motors/motors.h"
#include "pid/pid.h"

#define Dev24C02 0xD0

#define MIN_SPEED 45


/* Call PID controller */
ISR(TIMER1_COMPA_vect)
{
	/* TODO PID */
	
	int angle;
	double ay, nan, output;
	char buffer[30];
	memset(buffer, 0, 30);
	
	mpu6050_getConvData(&nan, &ay, &nan, &nan, &nan, &nan);
	angle = asin(ay) * 180 / M_PI;
	
	output = compute(angle);
	
	
	
	
	if(output == 0) {
		
		move_stop();
		
	} else if(output > 0) {
		
		if(0xFF - MIN_SPEED - output < 0) {
			move_forward(0xFF);
			output = 0xFF;
		} else {
			move_forward(output + MIN_SPEED);
			output = output + MIN_SPEED;
		}
	} else {
		
		output = - output;
		
		if(0xFF - MIN_SPEED - output < 0) {
			move_backward(0xFF);
			output = 0xFF;
			} else {
			move_backward(output + MIN_SPEED);
			output = output + MIN_SPEED;
		}
	}
	
	sprintf(buffer, "input: %i, output: %.3f\n", angle, output);
	USART0_print(buffer);
}

/* Configure timer to generate interrupt every 100 Mhz */
void timer1_init() {
	
	/* CTC mode */
	TCCR1B |= (1 << WGM12);
	
	/* Prescaler clk / 1024 */
	TCCR1B |= (1 << CS10) | (1 << CS12);
	
	/* Enable output compare A interrupt */
	TIMSK1 |= (1 << OCIE1A);
	
	/* 100Hz */
	OCR1A = 0x9C;
	
}

void time2_init() {
	
	//TCCR2A |= (1 << COM2A0);
	TCCR2A |= (1 << COM2A1);
	
	//TCCR2B |= (1 << WGM22);
	TCCR2A |= (1 << WGM00) | (1 << WGM01);
	
	TCCR2B |= (1 << CS20);
	
	DDRD |= (1 << PD7); // output pin for pwm
}

void init_robot() {
	motor_init();
	sei();
	mpu6050_init();
	timer1_init();
	USART0_init();
}

int main(void)
{
	init_robot();
	while (1) 
    {
		//USART0_print("merge\n");
	}
	
	
}

