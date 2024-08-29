#include "stat_handler.h"
#include "unit.h"
#include "item.h"
#include "weapon.h"
#include "unit_class.h"
#include "game.h"

void StatHandler::setup(Game* game, Unit* unit)
{
	this->game = game;
	this->unit = unit;
	this->type = "stats";
}


void StatHandler::update()
{
	checkIfDead();
	checkIfRevived();
	refreshClassStats();
	refreshBasicStats();
	refreshCombatStats();
}

void StatHandler::refreshClassStats()
{
	if (unit->class_initialized == true)
	{
		unit_class = &unit->unit_class;
		refreshLvlExpStats();
	}
}

void StatHandler::refreshBasicStats()
{
	if (unit->hp > unit->max_hp)
	{
		unit->hp = unit->max_hp;
	}
	if (unit->mp > unit->max_mp)
	{
		unit->mp = unit->max_mp;
	}
	//You must manually resest the bonus stats if you modify them
	unit->str = unit->base_stats["str"].asInt() + unit->bonus_str;
	unit->mag = unit->base_stats["mag"].asInt() + unit->bonus_mag;
	unit->skl = unit->base_stats["skl"].asInt() + unit->bonus_skl;
	unit->spd = unit->base_stats["spd"].asInt() + unit->bonus_spd;
	unit->lck = unit->base_stats["lck"].asInt() + unit->bonus_lck;
	unit->def = unit->base_stats["def"].asInt() + unit->bonus_def;
	unit->res = unit->base_stats["res"].asInt() + unit->bonus_res;
	unit->con = unit->base_stats["con"].asInt() + unit->bonus_con;
}

void StatHandler::refreshCombatStats()
{
	burden = unit->con - unit->weight;
	if (burden > 0)
	{
		burden = 0;
	}

	unit->atk = unit->str + unit->bonus_atk;
	unit->mag_atk = unit->mag + unit->bonus_mag_atk;

	unit->base_atk_spd = unit->spd;
	unit->atk_spd = unit->base_atk_spd + unit->bonus_atk_spd + burden;

	unit->base_hit = (unit->skl * 2) + (int)(unit->lck / 2);
	unit->hit = unit->base_hit + unit->bonus_hit;

	unit->base_avoid = unit->atk_spd + unit->lck;
	unit->avoid = unit->base_avoid + unit->bonus_avoid;

	unit->block = unit->def + unit->bonus_block;
	unit->mag_defense = unit->res + unit->bonus_mag_defense;

	unit->base_crit = (int)round(((float)unit->skl / 2.0f));
	unit->crit = unit->base_crit + unit->bonus_crit;
	unit->crit_dmg = unit->base_crit_dmg + unit->bonus_crit_dmg;
}

void StatHandler::checkIfDead()
{
	if (unit->hp <= 0)
	{
		unit->hp = 0;
		unit->state_controller.pushState("dead");
	}
	else
	{
		unit->state_controller.eraseState("dead");
	}
}

void StatHandler::checkIfRevived()
{
	if (unit->btl_con_controller.findCondition("revived"))
	{
		unit->btl_con_controller.eraseCondition("revived");
		unit->btl_con_controller.setCondition("idle");
	}
}

void StatHandler::refreshLvlExpStats()
{
	if (unit_class->exp_chart_initialized == true)
	{
		int current_lvl = unit->lvl - 1;
		int next_lvl = unit->lvl;
		int base_exp = unit_class->exp_chart["EXP"][current_lvl]["exp"].asInt();
		int exp_needed = unit_class->exp_chart["EXP"][next_lvl]["exp"].asInt();
		unit->base_exp = base_exp;
		unit->until_next_level = exp_needed;
	}
}
