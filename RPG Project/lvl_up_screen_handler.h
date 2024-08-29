#pragma once
#include <raylib.h>
#include <iostream>
#include "growth_calculator.h"
#include "handler.h"

using namespace std;

class Game;
class Party;
class Unit;
class LvLUpScreen;
class Timer;

class LvlUpScreenHandler : public Handler
{
public:
	void setup(Game* game, LvLUpScreen* lvl_screen);
	void update();
private:
	//Update
	void growStats();
	void addStats();
	void pullUnitStats();

	void startLvlUp();
	void checkIfDone();
	void endLvlUp();

	GrowthCalculator growth_calculator;

	//Data
	Game* game;
	LvLUpScreen* lvl_screen;
	Party* party;
	Unit* unit;
	Timer* timer;
};