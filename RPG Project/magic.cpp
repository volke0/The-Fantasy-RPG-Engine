#include "magic.h"
#include "unit.h"
#include "game.h"

void Magic::setup(Game* game, Unit* unit, Unit* target, Json::Value script)
{
	basicSetup(game, unit, script);
	setUnit(unit);
	setTarget(target);
	setupMagic();
}

void Magic::ApplyEffect(float amount, Unit* user, Unit* target, bool hit, bool bMultiTarget)
{
	effect->ApplyEffect((int)amount, user, target, hit, bMultiTarget);
}

void Magic::setupMagic()
{
	initializeValues();
	setupAnimation();
	effect = &magic_effect;
	static_cast<MagicEffect*>(effect)->setup(game, this);
}


void Magic::initializeValues()
{
	setBasicValues();
	if (script.isMember("type"))
	{
		mag_type = script["type"].asString();
		if (mag_type == "healing")
		{
			can_heal = true;
		}
	}
	if (script.isMember("elem_type"))
	{
		elem_type = script["elem_type"].asString();
	}
	if (script.isMember("heal_type"))
	{
		heal_type = script["heal_type"].asString();
	}
	if (script.isMember("power"))
	{
		power = script["power"].asInt();
	}
	if (script.isMember("hit"))
	{
		if (script["hit"].asString() == "---")
		{
			hit = 255;
		}
		else
		{
			hit = script["hit"].asInt();
		}
	}
	if (script.isMember("crit"))
	{
		crit = script["crit"].asInt();
	}
	if (script.isMember("target_type"))
	{
		target_type = script["target_type"].asString();
	}
	if (script.isMember("descr"))
	{
		description = script["descr"].asString();
	}
	if (script.isMember("cost"))
	{
		cost = script["cost"].asInt();
	}
	if (script.isMember("cost_type"))
	{
		cost_type = script["cost_type"].asString();
	}
	basicInitialization();
}

void Magic::setBasicValues()
{
	type = "magic";
	if (script.isMember("name"))
	{
		ingame_name = script["name"].asString();
	}
	if (script.isMember("c_name"))
	{
		name = script["c_name"].asString();
	}
	if (script.isMember("animation"))
	{
		animation_name = script["animation"].asString();
	}
}

void Magic::setupAnimation()
{
	anim.setup(game, game->vfx_anim_database.vfx_anims[animation_name]);
}
