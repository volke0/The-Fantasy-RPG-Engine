#include "foot_gear.h"

void FootGear::setup(Game* game, Unit* unit, Json::Value script)
{
	Equipment::setup(game, unit, script);
	SetupFootGear();
}

void FootGear::SetupFootGear()
{
	if (script.isMember("foot_type"))
	{
		foot_gear_type = script["foot_type"].asString();
	}
}
