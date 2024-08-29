#pragma once
#include <raylib.h>
#include <iostream>

using namespace std;

class Timer
{
public:
	bool wait = false;
	float fps = 1.0f;
	float time = 0.0f;
	float timer = 0;

	void STAL(float time);
	void tick();
};
