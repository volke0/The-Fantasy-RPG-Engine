#pragma once
#include <raylib.h>

using namespace std;

class Fades
{
public:
	void in(int frames);
	void out(int frames);

	void update();
	void draw();

	//Logic Variables
	bool fadein_done = false;
	bool fadeout_done = false;
	bool done = true;
private:
	//Color
	Color alpha_color = { 0, 0, 0, 255 };

	//Logic Variables
	bool start = true;
	bool IN = false;
	bool OUT = false;

	//Alpha
	float alpha = 0;
	int fps;
	float percent = 0.0f;
};
