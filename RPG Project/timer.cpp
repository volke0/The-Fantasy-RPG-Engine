#include "timer.h"

void Timer::STAL(float time)
{
	timer = time;
	this->time = 0;
	wait = true;
}

void Timer::tick()
{
	if (time <= timer)
	{
		time += fps;
	}
	else
	{
		wait = false;
	}
}
