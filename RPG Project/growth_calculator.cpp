#include "growth_calculator.h"
#include <cstdlib>
#include "game.h"
#include "unit.h"
#include "unit_class.h"

void GrowthCalculator::setup(Game* game, Unit* unit, int num_of_lvls)
{
	this->game = game;
	this->unit = unit;
	this->num_of_lvls = num_of_lvls;
}

unordered_map<string, int> GrowthCalculator::calculateStats()
{
	og_lvl = unit->lvl - num_of_lvls;
	int lvl = 0;
	int hp = 0;
	int mp = 0;
	int str = 0;
	int mag = 0;
	int skl = 0;
	int spd = 0;
	int lck = 0;
	int def = 0;
	int res = 0;

	for (int i = 1; i <= num_of_lvls; i++)
	{
		hp += unit->unit_class.growth_chart["hp"][og_lvl]["hp_gain"].asInt();
		mp += unit->unit_class.growth_chart["mp"][og_lvl]["mp_gain"].asInt();
		str += calculateRandomStatGain("str");
		mag += calculateRandomStatGain("mag");
		skl += calculateRandomStatGain("skl");
		spd += calculateRandomStatGain("spd");
		lck += calculateRandomStatGain("lck");
		def += calculateRandomStatGain("def");
		res += calculateRandomStatGain("res");
		grown_stats = {
					{"max_hp", hp},
					{"max_mp", mp},
					{"str", str},
					{"mag", mag},
					{"skl", skl},
					{"spd", spd},
					{"lck", lck},
					{"def", def},
					{"res", res},
		};
		og_lvl++;
		lvl++;
	}
	/*cout << "LVL+ " << lvl << " HP+ " << hp + unit->hp << " MP+ " << mp + unit->mp << " STR+ " << str + unit->str << " MAG+ " << mag + unit->mag << " SKL+ ";
	cout << skl + unit->skl << " SPD+ " << spd + unit->spd;
	cout << " LCK+ " << lck + unit->lck << " DEF+ " << def + unit->def << " RES+ " << res + unit->res << endl;*/
	return grown_stats;
}

int GrowthCalculator::calculateRandomStatGain(string stat_name)
{
	int max_stat = unit->unit_class.max_stats[stat_name].asInt();
	int current_stat = findCurrentStat(stat_name);
	int diff = max_stat - (current_stat + grown_stats[stat_name]);
	int avg_gain = (int)round((float)(diff / (100 - og_lvl)));
	int stat = 0;
	int rng = (int)time(NULL) + rng_frame;
	rng_frame++;
	srand((unsigned)rng);
	
	if (og_lvl < 30)
	{
		int range = (int)(ceil((float)avg_gain * 0.70f));
		int stat = 0 + (rand() % range);
		//cout << stat_name << ": " << stat << endl;
		return stat;
	}
	if (og_lvl < 60)
	{
		int range = (int)(ceil((float)avg_gain * 0.90f));
		int stat = 0 + (rand() % range);
		return stat;
	}
	if (og_lvl < 80)
	{
		int range = (1 + abs((int)avg_gain));
		int stat = (int)(abs((ceil((float)avg_gain * 0.20f))) + (rand() % range));
		return stat;
	}
	if (og_lvl < 100)
	{
		int range = (5 + abs((int)avg_gain));
		int stat = (int)(abs((ceil((float)range * 0.35f))) + (rand() % range));
		return stat;
	}
	/*if (og_lvl == 98)
	{
		int range = 1 + (int)(ceil((float)avg_gain * 1.65f));
		int stat = range;
		return stat;
	}*/
	return stat;
}

int GrowthCalculator::findCurrentStat(string stat_name)
{
	if (stat_name == "str")
	{
		return unit->base_stats["str"].asInt();
	}
	if (stat_name == "mag")
	{
		return unit->base_stats["mag"].asInt();
	}
	if (stat_name == "skl")
	{
		return unit->base_stats["skl"].asInt();
	}
	if (stat_name == "spd")
	{
		return unit->base_stats["spd"].asInt();
	}
	if (stat_name == "lck")
	{
		return unit->base_stats["lck"].asInt();
	}
	if (stat_name == "def")
	{
		return unit->base_stats["def"].asInt();
	}
	if (stat_name == "res")
	{
		return unit->base_stats["res"].asInt();
	}
	return 0;
}
