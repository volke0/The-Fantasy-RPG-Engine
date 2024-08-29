#pragma once
#include <raylib.h>
#include "handler.h"

using namespace std;

class Game;
class Unit;
class NPC;
class Battle;

class CalculationHandler : public Handler
{
public:
	void basicSetup(Game* game, Battle* battle);

	//Getters
	void getHitCalculation(Unit* user, Unit* target);
	void getAttackDamageCalculation(Unit* user, Unit* target, bool crit);

	//Setters
	virtual void applyDamage(Unit* user, vector<NPC*> targets, int target_index);
	void applyAttackDamage(Unit* target, int target_index);

	//Hit
	int hit_pct = 0;
	bool hit = false;
	int crit_pct = 0;
	void pushHit(float hit_rate);
	void pushCrit(float crit_rate);

	//Data
	Game* game;
	Battle* battle;
	Unit* user;

	//Rng
	int rng_frame = 0;
private:
	
};
