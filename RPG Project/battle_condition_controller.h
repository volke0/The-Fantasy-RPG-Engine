#pragma once
#include <raylib.h>
#include "unit_condition_controller.h"

using namespace std;

class Game;
class Unit;

class BattleConditionController : public UnitConditionController
{
public:
	void setup(Game* game, Unit* unit);
	void update() override;

	//Setters
	void resetToIdle();
private:

	//Dead
	void checkIfDead();

	//Debug
	void viewAllConditions();
};
