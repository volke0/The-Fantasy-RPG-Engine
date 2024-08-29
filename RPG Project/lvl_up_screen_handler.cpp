#include "lvl_up_screen_handler.h"
#include "lvl_up_screen.h"
#include "party.h"
#include "unit.h"
#include "timer.h"
#include "game.h"

void LvlUpScreenHandler::setup(Game* game, LvLUpScreen* lvl_screen)
{
	this->game = game;
	this->lvl_screen = lvl_screen;
	this->party = lvl_screen->party;
	this->unit = lvl_screen->unit;
	this->timer = &lvl_screen->timer;
	growth_calculator.setup(game, unit, lvl_screen->num_of_lvl);
}

void LvlUpScreenHandler::update()
{
	pullUnitStats();
	startLvlUp();
	if (timer->wait == false)
	{
		if (lvl_screen->start == false)
		{
			growStats();
			endLvlUp();
		}
	}
	checkIfDone();
}

void LvlUpScreenHandler::growStats()
{
	if (lvl_screen->grow_stats == true)
	{
		addStats();
	}
	if (lvl_screen->grow_stats == false)
	{
		lvl_screen->grown_stats = growth_calculator.calculateStats();
		lvl_screen->show_grown_stats = true;
		lvl_screen->grow_stats = true;
	}
}

void LvlUpScreenHandler::addStats()
{
	if (lvl_screen->grow_stats_done == false)
	{
		unit->max_hp += lvl_screen->grown_stats["max_hp"];
		unit->hp += lvl_screen->grown_stats["max_hp"];
		unit->max_mp += lvl_screen->grown_stats["max_mp"];
		unit->mp += lvl_screen->grown_stats["max_mp"];
		unit->str += lvl_screen->grown_stats["str"];
		unit->mag += lvl_screen->grown_stats["mag"];
		unit->skl += lvl_screen->grown_stats["skl"];
		unit->spd += lvl_screen->grown_stats["spd"];
		unit->lck += lvl_screen->grown_stats["lck"];
		unit->def += lvl_screen->grown_stats["def"];
		unit->res += lvl_screen->grown_stats["res"];

		//Base Stat growth
		unit->base_stats["max_hp"] = unit->base_stats["max_hp"].asInt() + lvl_screen->grown_stats["max_hp"];
		unit->base_stats["hp"] = unit->base_stats["hp"].asInt() + lvl_screen->grown_stats["max_hp"];
		unit->base_stats["max_mp"] = unit->base_stats["max_mp"].asInt() + lvl_screen->grown_stats["max_mp"];
		unit->base_stats["mp"] = unit->base_stats["mp"].asInt() + lvl_screen->grown_stats["max_mp"];
		unit->base_stats["str"] = unit->base_stats["str"].asInt() + lvl_screen->grown_stats["str"];
		unit->base_stats["mag"] = unit->base_stats["mag"].asInt() + lvl_screen->grown_stats["mag"];
		unit->base_stats["skl"] = unit->base_stats["skl"].asInt() + lvl_screen->grown_stats["skl"];
		unit->base_stats["spd"] = unit->base_stats["spd"].asInt() + lvl_screen->grown_stats["spd"];
		unit->base_stats["lck"] = unit->base_stats["lck"].asInt() + lvl_screen->grown_stats["lck"];
		unit->base_stats["def"] = unit->base_stats["def"].asInt() + lvl_screen->grown_stats["def"];
		unit->base_stats["res"] = unit->base_stats["res"].asInt() + lvl_screen->grown_stats["res"];

		lvl_screen->grow_stats_done = true;
	}
}

void LvlUpScreenHandler::pullUnitStats()
{
	lvl_screen->unit_stats = {
					{"max_hp", unit->base_stats["max_hp"].asInt()},
					{"max_mp",  unit->base_stats["max_mp"].asInt()},
					{"str",  unit->base_stats["str"].asInt()},
					{"mag",  unit->base_stats["mag"].asInt()},
					{"skl",  unit->base_stats["skl"].asInt()},
					{"spd",  unit->base_stats["spd"].asInt()},
					{"lck",  unit->base_stats["lck"].asInt()},
					{"def",  unit->base_stats["def"].asInt()},
					{"res",  unit->base_stats["res"].asInt()},
										};
}

void LvlUpScreenHandler::startLvlUp()
{
	if (lvl_screen->start == true)
	{
		timer->STAL(45);
		lvl_screen->start = false;
	}
}

void LvlUpScreenHandler::checkIfDone()
{
	if (lvl_screen->grow_stats_done == true)
	{
		if (lvl_screen->done_updating == false)
		{
			timer->STAL(125);
			lvl_screen->done_updating = true;
		}
	}
}

void LvlUpScreenHandler::endLvlUp()
{
	if (lvl_screen->done_updating == true)
	{
		lvl_screen->done = true;
	}
}
