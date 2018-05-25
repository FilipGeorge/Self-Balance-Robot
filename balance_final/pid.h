/*
 * pid.h
 *
 * Created: 5/25/2018 12:06:04 AM
 *  Author: Filip Manole
 */ 


#ifndef PID_H_
#define PID_H_


#define SAMPLE_TIME	1/300 // seconds
#define SETPOINT	0

#define Kp	13
#define Ki	5
#define Kd	7

double compute(int input);


#endif /* PID_H_ */