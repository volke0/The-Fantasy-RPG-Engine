#include "head_gear.h"

void HeadGear::setup(Game* game, Unit* unit, Json::Value script)
{
	Equipment::setup(game, unit, script);
	SetupHeadGear();
}

void HeadGear::SetupHeadGear()
{
	if (script.isMember("head_type"))
	{
		head_gear_type = script["head_type"].asString();
	}
}
