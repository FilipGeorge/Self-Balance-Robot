/*
 * pid.cpp
 *
 * Created: 5/25/2018 12:06:48 AM
 *  Author: Filip Manole
 */

#include "pid.h"


double compute(int input)
{
	
	static int lastErr = 0;
	static int errSum = 0;
	
	int error = SETPOINT - input;
	errSum += error;
	int dErr = (error - lastErr);
	
	/*Remember some variables for next time*/
	lastErr = error;
	
	/*Compute PID Output*/
	return Kp * error + Ki * errSum * SAMPLE_TIME + Kd * dErr / SAMPLE_TIME;
	
}
