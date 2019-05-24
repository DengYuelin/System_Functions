#include "original_PID.h"

PID::PID(const float k_p, const float k_i, const float k_d)
{
	this->kp = k_p;
	this->ki = k_i;
	this->kd = k_d;
	this->q0 = this->kp + this->ki + this->kd;
	this->q1 = -this->kp - 2 * this->kd;
	this->q2 = this->kd;
	this->error_last = 0;
	this->error = 0;
	this->B_last = 0;
	this->B = 0;
	this->u = 0;
}

PID::~PID()
{
}

float PID::PID_control(const float reference, const float sensor)
{
	this->error = reference - sensor;
	this->u = this->q0*this->error + this->B_last;
	this->B = this->u + this->q1*this->error + this->q2*this->error_last;

	//update the B and error
	this->error_last = this->error;
	this->B_last = this->B;

	return this->u;
}
