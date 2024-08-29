#pragma once
#include <raylib.h>
#include "item.h"
#include "item_effect.h"

using namespace std;

class Game;
class Unit;

class Consumable : public Item
{
public:
	void setup(Game* game, Unit* unit, Unit* target, Json::Value script);

	//Effect
	ItemEffect item_effect;

	//Values
	string consume_type;
	float amount = 0.f;
	bool inventory_usable = true;
private:
	//Setup
	void setupConsumable();
	void initializeConsumable();
};
