/*
 * motors.h
 *
 * Created: 5/18/2018 8:47:10 PM
 *  Author: Filip Manole
 */ 


#ifndef MOTORS_H_
#define MOTORS_H_

#define IN_A1 PA0
#define IN_B1 PA1
#define IN_A2 PA2
#define IN_B2 PA3

#define PMW1 PB3
#define PMW2 PB4

#include <avr/io.h>

void motor_init(void);
void move_forward(uint8_t ocr);
void move_backward(uint8_t ocr);
void move_stop();

#endif /* MOTORS_H_ */