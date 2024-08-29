#include "armor.h"

void Armor::setup(Game* game, Unit* unit, Json::Value script)
{
	Equipment::setup(game, unit, script);
	SetupArmor();
}

void Armor::SetupArmor()
{
	if (script.isMember("armor_type"))
	{
		armor_type = script["armor_type"].asString();
	}
}
