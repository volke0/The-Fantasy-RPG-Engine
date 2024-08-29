#include "shield.h"

void Shield::setup(Game* game, Unit* unit, Json::Value script)
{
	Equipment::setup(game, unit, script);
	SetupShield();
}

void Shield::SetupShield()
{
	if (script.isMember("shield_type"))
	{
		shield_type = script["shield_type"].asString();
	}
}
