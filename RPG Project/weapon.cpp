#include "weapon.h"
#include <cstdlib>
#include <cmath>
#include "unit.h"
#include "game.h"


void Weapon::setup(Game* game, Unit* unit, Json::Value script)
{
	Equipment::setup(game, unit, script);
	setupWeapon();
}

void Weapon::setupWeapon()
{
	if (script.isMember("weapon_type"))
	{
		weapon_type = script["weapon_type"].asString();
	}
	if (script.isMember("is_mag_weapon"))
	{
		is_mag_weapon = script["is_mag_weapon"].asBool();
	}
}
