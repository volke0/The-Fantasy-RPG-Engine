#pragma once
#include <raylib.h>
#include "handler.h"

using namespace std;

class Game;
class Unit;
class UnitClass;

class StatHandler : public Handler
{
public:
	void setup(Game* game, Unit* unit);
	void update() override;
private:
	//Data
	Game* game;
	Unit* unit;
	UnitClass* unit_class;

	//Stats
	int burden = 0;
	int weapon_atk = 0;
	int weapon_mag = 0;
	int weapon_hit = 0;
	int weapon_weight = 0;
	int weapon_avoid = 0;
	int weapon_crit = 0;

	//Update
	void refreshClassStats();
	void refreshBasicStats();
	void refreshCombatStats();
	void checkIfDead();
	void checkIfRevived();

	//Update Class Stats
	void refreshLvlExpStats();
};