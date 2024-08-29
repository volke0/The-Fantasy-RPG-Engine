#pragma once
#include <raylib.h>
#include "calculation_handler.h"

using namespace std;

class Game;
class Unit;
class NPC;
class Battle;
class Magic;

class MagicCalculationHandler : public CalculationHandler
{
public:
	void setup(Game* game, Battle* battle);
	void setMagic(Magic* magic);

	//Getters
	void getMagHitCalculation(Unit* user, Unit* target);
	void getMagAttackCalculation(Unit* user, Unit* target, bool crit, Magic* magic, bool bMultiTargeting);

	//Setters
	void applyDamage(Unit* user, vector<NPC*> targets, int target_index);

	//Data
	Magic* magic;
};
