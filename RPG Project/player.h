#pragma once
#include <raylib.h>
#include <string>
#include <iostream>
#include "unit.h"

using namespace std;

class Player : public Unit
{
public:
	//Setup

	//Basic Methods
	void draw() override;
	void update() override;

	//Modes

	//Rectangles
	
private:
	//Draw Methods
	void drawMapMode();
	void drawBattleMode();
};