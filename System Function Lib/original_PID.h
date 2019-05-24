#pragma once

class PID
{
public:
	PID(const float k_p, const float k_i, const float k_d);
	~PID();
	//function
	float PID_control(const float reference, const float sensor);
	//data
	float q0;
	float q1;
	float q2;
	float B_last;
	float B;
	float error_last;
	float error;
	float u;
	float kp;
	float ki;
	float kd;
};

