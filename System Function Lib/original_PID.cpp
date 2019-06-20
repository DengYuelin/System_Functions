#include "original_PID.h"

PID::PID(const float k_p, const float k_i, const float k_d)
{
	this->kp = k_p;
	this->ki = k_i;
	this->kd = k_d;
	this->error_1 = 0;
	this->error_2 = 0;
	this->error = 0;
	this->u = 0;
	this->u_1 = 0;
}

PID::~PID()
{
}

float PID::PID_control(const float reference, const float sensor)
{
	error = reference - sensor;
	if ((error > 0 && u < 0) || (error < 0 && u> 0))
		u = u_1 + kp * (error - error_1) + ki * error + kd * (error - 2 * error_1 + error_2);
	else
		u = u_1 + kp * (error - error_1) + kd * (error - 2 * error_1 + error_2);

	
	//update
	error_2 = error_1;
	error_1 = error;
	u_1 = u;


	//dead zone
	if (error <= 0.1 && error >= -0.1)
		return 1800;
	else
	{
		if (this->u < 0)
		{
			return 1550 + this->u;
		}
		else if (this->u > 0)
		{
			return 2460 + this->u;
		}
	}
	
}

void PID::reset(const float k_p, const float k_i, const float k_d)
{
	this->kp = k_p;
	this->ki = k_i;
	this->kd = k_d;
	this->error_1 = 0;
	this->error_2 = 0;
	this->error = 0;
	this->u = 0;
	this->u_1 = 0;
}

void PID::clearpid()
{
	this->error_1 = 0;
	this->error_2 = 0;
	this->error = 0;
	this->u = 0;
	this->u_1 = 0;
}

LPF::LPF(const float a)
{
	this->a = a;
	this->y = 0;
	this->y1 = 0;
	this->timer = 0;
}

LPF::~LPF()
{
}

unsigned int LPF::lpf(const unsigned int x)
{
	if (this->timer == 0)
	{
		this->y1 = x;
		this->timer++;
	}
	this->y = (unsigned int)(a * y1 + (1 - a)*x);
	this->y1 = this->y;
	return y;
}
