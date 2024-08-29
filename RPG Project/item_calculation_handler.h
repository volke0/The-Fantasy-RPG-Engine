#pragma once
#include <raylib.h>
#include "calculation_handler.h"

using namespace std;

class Game;
class Unit;
class NPC;
class Battle;
class Consumable;

class ItemCalculationHandler : public CalculationHandler
{
public:
	void setup(Game* game, Battle* battle);
	void setItem(Consumable* item);

	//Getters
	void getItemHitCalculation(Unit* user, Unit* target);
	void getItemAttackCalculation(Unit* user, Unit* target, bool crit, Consumable* item);

	//Setters
	void applyDamage(Unit* user, vector<NPC*> targets, int target_index) override;

	//Data
	Consumable* item;
};