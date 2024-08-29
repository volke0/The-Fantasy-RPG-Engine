#include "fade.h"
#include "vars.h"
#include <math.h>
#include <iostream>

void Fades::in(int frames)
{
	if (start == true)
	{
		if (alpha == 255)
		{
			fadein_done = true;
		}
		else
		{
			start = false;
			fadein_done = false;
			fps = frames;
			IN = true;
		}
	}
}

void Fades::out(int frames)
{
	if (start == true)
	{
		start = false;
		fadeout_done = false;
		fps = frames;
		alpha = 255;
		OUT = true;
	}
}

void Fades::update()
{
	//check if fade isn't running
	if (fadein_done == true && fadeout_done == true)
	{
		done = true;
	}
	else
	{
		done = false;
	}
	if (IN == true)
	{
		if (alpha <= 255)
		{
			alpha += (float)fps;
		}
		else
		{
			IN = false;
			fadein_done = true;
			alpha = 255;
			start = true;
		}
	}
	else if (OUT == true)
	{
		if (alpha > 0)
		{
			alpha -= (float)fps;
		}
		else
		{
			OUT = false;
			fadeout_done = true;
			start = true;
		}
	}
	percent = alpha / 255.0f;
}

void Fades::draw()
{
	Color color = ColorAlpha(alpha_color, percent);
	DrawRectangle(0, 0, 17 * TILE, 13 * TILE, color);
}
