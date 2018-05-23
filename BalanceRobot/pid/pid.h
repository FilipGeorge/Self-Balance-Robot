/*
 * pid.h
 *
 * Created: 5/18/2018 10:53:29 PM
 *  Author: Filip Manole
 */ 

/* unele vor trebui sa fie volatile */

#ifndef PID_H_
#define PID_H_

#define SAMPLE_TIME	1/100 // seconds
#define SETPOINT	0

#define Kp	4
#define Ki	3
#define Kd	1

double compute(int input);

#endif /* PID_H_ */