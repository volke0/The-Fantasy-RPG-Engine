#include "equipment_base.h"
#include "game.h"
#include "unit.h"

void Equipment::setup(Game* game, Unit* unit, Json::Value script)
{
	basicSetup(game, unit, script);
	initializeItem();
	setupStats();
}

void Equipment::setupStats()
{
	type = script["type"].asString();
	sub_type = script["sub_type"].asString();
	if (script.isMember("stats"))
	{
		Json::Value stats = script["stats"];
		if (stats.isMember("attack"))
		{
			attack = stats["attack"].asInt();
		}
		if (stats.isMember("mag_attack"))
		{
			mag_attack = stats["mag_attack"].asInt();
		}
		if (stats.isMember("atk_speed"))
		{
			atk_speed = stats["atk_speed"].asInt();
		}
		if (stats.isMember("hit"))
		{
			hit = stats["hit"].asInt();
		}
		if (stats.isMember("avoid"))
		{
			avoid = stats["avoid"].asInt();
		}
		if (stats.isMember("block"))
		{
			block = stats["block"].asInt();
		}
		if (stats.isMember("mag_def"))
		{
			mag_defense = stats["mag_def"].asInt();
		}
		if (stats.isMember("crit"))
		{
			crit = stats["crit"].asInt();
		}
		if (stats.isMember("crit_dmg"))
		{
			crit_dmg = stats["crit_dmg"].asInt();
		}
		if (stats.isMember("weight"))
		{
			weight = stats["weight"].asInt();
		}
	}
	if (script.isMember("bonus_stats"))
	{
		Json::Value stats = script["bonus_stats"];
		if (stats.isMember("strength"))
		{
			str = stats["strength"].asInt();
		}
		if (stats.isMember("magic"))
		{
			mag = stats["magic"].asInt();
		}
		if (stats.isMember("skill"))
		{
			skl = stats["skill"].asInt();
		}
		if (stats.isMember("speed"))
		{
			spd = stats["speed"].asInt();
		}
		if (stats.isMember("luck"))
		{
			lck = stats["luck"].asInt();
		}
		if (stats.isMember("defense"))
		{
			def = stats["defense"].asInt();
		}
		if (stats.isMember("resistance"))
		{
			res = stats["resistance"].asInt();
		}
		if (stats.isMember("condition"))
		{
			con = stats["condition"].asInt();
		}
	}
	if (script.isMember("special_lock"))
	{
		special_lock = script["special_lock"];
	}
}
