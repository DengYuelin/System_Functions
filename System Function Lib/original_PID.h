#pragma once

class PID
{
public:
	PID(const float k_p, const float k_i, const float k_d);
	~PID();
	//function
	float PID_control(const float reference, const float sensor);
	void reset(const float k_p, const float k_i, const float k_d);
	void clearpid();
	//data
	float error_1;
	float error;
	float error_2;
	float u;
	float u_1;
	float kp;
	float ki;
	float kd;
};


class LPF
{
public:
	LPF(const float a);
	~LPF();
	//function
	unsigned int lpf(const unsigned int x);
	//data
	float a;
	unsigned int y;
	unsigned int y1;
	int timer;
};

