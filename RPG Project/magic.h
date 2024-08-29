#pragma once
#include <raylib.h>
#include "item.h"
#include "magic_effect.h"

using namespace std;

class Game;
class Unit;

class Magic : public Item
{
public:
	void setup(Game* game, Unit* unit, Unit* target, Json::Value script);

	//Setters
	virtual void ApplyEffect(float amount, Unit* user, Unit* target, bool hit = true, bool bMultiTarget = false);

	//Effect
	MagicEffect magic_effect;

	//Data
	string mag_type;
	string elem_type;
	int power = 0;
	int hit = 0;
	int crit = 0;
	string target_type;
	string description;
	int cost;
	string cost_type;
	bool pct_hp_dmg = false;
	float pct_hp_amount = 0;
private:
	//Setup
	void setupMagic();
	void initializeValues();
	void setBasicValues();
	void setupAnimation();
};