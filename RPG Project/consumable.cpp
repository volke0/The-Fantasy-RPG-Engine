#include "consumable.h"
#include "game.h"
#include "unit.h"

void Consumable::setup(Game* game, Unit* unit, Unit* target, Json::Value script)
{
	basicSetup(game, unit, script);
	setUnit(unit);
	setTarget(target);
	setupConsumable();
}

void Consumable::setupConsumable()
{
	initializeConsumable();
	effect = &item_effect;
	static_cast<ItemEffect*>(effect)->setup(game, this);
}

void Consumable::initializeConsumable()
{
	type = "consumable";
	basicInitialization();
	if (script.isMember("name"))
	{
		ingame_name = script["name"].asString();
	}
	if (script.isMember("c_name"))
	{
		name = script["c_name"].asString();
	}
	if (script.isMember("inventory_usable"))
	{
		inventory_usable = script["inventory_usable"].asBool();
	}
	if (script.isMember("sub_type"))
	{
		consume_type = script["sub_type"].asString();
	}
	if (script.isMember("heal_type"))
	{
		heal_type = script["heal_type"].asString();
	}
	if (script.isMember("amount"))
	{
		amount = script["amount"].asFloat();
	}
	setupGraphics();
}
