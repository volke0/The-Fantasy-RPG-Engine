#include "exp_menu_handler.h"
#include "battle_controller.h"
#include "unit.h"
#include "party.h"
#include "game.h"
#include "exp_menu.h"

void ExpMenuHandler::setup(Game* game, Party* party, ExpMenu* exp_menu)
{
	this->game = game;
	this->party = party;
	this->exp_menu = exp_menu;
}

void ExpMenuHandler::update()
{
	prepareExpRatio();
	prepareMemory();
	skipExpAnimations();
	if (timer.wait == false)
	{
		checkIfDone();
		updateSilver();
		updateLevel();
		updateExp();
	}
	timer.tick();
}

void ExpMenuHandler::draw()
{
	if (timer.wait == false)
	{
		drawLevel();
	}
}

void ExpMenuHandler::prepareMemory()
{
	if (start == true)
	{
		timer.STAL(65);
		calculateExpPerUnit();
		for (auto& unit : party->party)
		{
			float exp_per_tic = (float)unit->until_next_level * 0.01f;
			if (exp_per_tic > 0)
			{
				exp_per_tic = 1;
			}
			exp_menu->value_memory[unit->name]["orig_lvl"] = unit->lvl;
			exp_menu->value_memory[unit->name]["exp"] = (int)exp_menu->exp_per_unit;
			exp_menu->value_memory[unit->name]["exp_per_tic"] = (int)exp_per_tic;
			exp_menu->value_memory[unit->name]["exp_for_next_lvl"] = unit->until_next_level;
		}
		start = false;
	}
}

void ExpMenuHandler::calculateExpPerUnit()
{
	int num = 0;
	for (auto& unit : party->party)
	{
		if (!unit->dead)
		{
			num++;
		}
	}
	if (num != 0)
	{
		exp_menu->exp_per_unit = round((float)exp_menu->battle_controller->total_exp / (float)num);
	}
	else
	{
		exp_menu->exp_per_unit = 0;
	}
}

void ExpMenuHandler::prepareExpRatio()
{
	for (auto& unit : party->party)
	{
		float base_exp = (float)unit->exp - (float)unit->base_exp;
		float required_exp = (float)unit->until_next_level - (float)unit->base_exp;
		exp_menu->exp_ratios[unit->name] = base_exp / required_exp;
		if (exp_menu->exp_ratios[unit->name] > 1)
		{
			exp_menu->exp_ratios[unit->name] = 1;
		}
	}
}

void ExpMenuHandler::updateExp()
{
	if (exp_done == false)
	{
		for (auto& unit : party->party)
		{
			addExp(unit.get());
			updateExpRatio(unit.get());
		}
		checkIfExpDone();
	}
}

void ExpMenuHandler::addExp(Unit* unit)
{
	if (exp_menu->value_memory[unit->name]["exp"] == 0 || skip == true)
	{
		if (ready_for_lvl_up == true && exp_menu->value_memory[unit->name]["exp"] == 0)
		{
			exp_menu->value_memory[unit->name]["done_with_exp"] = 1;
		}
		addExpToUnit(unit, "exp");
		exp_menu->value_memory[unit->name]["exp"] -= exp_menu->value_memory[unit->name]["exp"];
	}
	else if (exp_menu->value_memory[unit->name]["exp"] - exp_menu->value_memory[unit->name]["exp_per_tic"] >= 0)
	{
		exp_menu->value_memory[unit->name]["exp"] -= exp_menu->value_memory[unit->name]["exp_per_tic"];
		addExpToUnit(unit, "exp_per_tic");
	}
	else if (exp_menu->value_memory[unit->name]["exp"] - exp_menu->value_memory[unit->name]["exp_per_tic"] < 0)
	{
		addExpToUnit(unit, "exp");
		exp_menu->value_memory[unit->name]["exp"] -= exp_menu->value_memory[unit->name]["exp"];
	}
}

void ExpMenuHandler::updateExpRatio(Unit* unit)
{
	int current_exp = unit->exp;
	//cout << unit->name << " CURRENT EXP: " << current_exp << endl;
	//cout << unit->name << " EXP UNTIL NEXT LEVEL: " << unit->until_next_level << endl;
	if (current_exp > unit->until_next_level)
	{
		if (unit->lvl != 99)
		{
			unit->lvl++;
			level_up_amount[unit->name] += 1;
			need_to_lvl_up = true;
			lvl_up_screen_on = true;
			needs_generation = true;
		}
		else
		{
			exp_menu->value_memory[unit->name]["done_with_exp"] = 1;
		}
		exp_menu->value_memory[unit->name]["check"] += 1;
	}
	if (unit->dead)
	{
		level_up_amount[unit->name] = 0;
		exp_menu->value_memory[unit->name]["check"] += 1;
	}

	if (unit->exp < unit->unit_class.exp_chart["EXP"][unit->lvl]["exp"].asInt())
	{
		ready_for_lvl_up = true;
	}
	else
	{
		ready_for_lvl_up = false;
	}
}

void ExpMenuHandler::checkIfExpDone()
{
	int tally = 0;
	for (auto& unit : party->party)
	{
		if (exp_menu->value_memory[unit->name]["done_with_exp"] == 1)
		{
			if (exp_menu->value_memory[unit->name]["check"] >= level_up_amount[unit->name])
			{
				tally += 1;
			}
		}
	}
	if (tally == party->party.size())
	{
		exp_done = true;
	}
	if (exp_done == true)
	{
		if (need_to_lvl_up == true)
		{
			generateLvLUpScreens();
			timer.STAL(35);
		}
		if (lvl_up_screen_on == false)
		{
			timer.STAL(35);
			lvl_done = true;
		}
		getLvlUpScreenPosition();
	}
}

void ExpMenuHandler::addExpToUnit(Unit* unit, string exp_type)
{
	if (!unit->dead)
	{
		unit->exp += exp_menu->value_memory[unit->name][exp_type];
	}
}

void ExpMenuHandler::skipExpAnimations()
{
	if (game->input.findKey("x"))
	{
		skip = true;
	}
}


void ExpMenuHandler::updateLevel()
{
	if (lvl_up_screen_on == true)
	{
		if ((int)lvl_up_screens.size() > 0)
		{
			lvl_up_screens[lvl_up_pos]->update();
		}
		checkIfLevelUpDone();
	}
}

void ExpMenuHandler::checkIfLevelUpDone()
{
	if (needs_generation == false)
	{
		if ((int)lvl_up_screens.size() > 0)
		{
			if (lvl_up_screens[lvl_up_pos]->done == true)
			{
				lvl_up_screens.erase(lvl_up_screens.begin() + lvl_up_pos);
				getLvlUpScreenPosition();
				timer.STAL(35);
			}
		}
		else
		{
			lvl_done = true;
			lvl_up_screen_on = false;
			timer.STAL(35);
		}
	}
}

void ExpMenuHandler::updateSilver()
{
	if (lvl_done && exp_done)
	{
		if (exp_menu->battle_controller->total_silver > 0)
		{
			int silver_to_reduce = 1;
			exp_menu->battle_controller->total_silver -= silver_to_reduce;
			exp_menu->total_silver_color = WHITE;
			party->slv += silver_to_reduce;
			exp_menu->sfx_player.playSound("silver_get", .5f);
		}
		else
		{
			slv_done = true;
			timer.STAL(55);
		}
		if (skip && exp_menu->battle_controller->total_silver > 0)
		{
			int silver_to_reduce = exp_menu->battle_controller->total_silver;
			exp_menu->battle_controller->total_silver -= silver_to_reduce;
			exp_menu->total_silver_color = WHITE;
			party->slv += silver_to_reduce;
			exp_menu->sfx_player.playSound("silver_get");
		}
	}
}

void ExpMenuHandler::drawLevel()
{
	if (lvl_up_screen_on == true)
	{
		if ((int)lvl_up_screens.size() > 0)
		{
			lvl_up_screens[lvl_up_pos]->draw();
		}
	}
}


void ExpMenuHandler::checkIfDone()
{
	if (exp_done == true && lvl_done == true && slv_done == true)
	{
		exp_menu->battle_controller->state_controller.pushState("battle_end");
	}
}


void ExpMenuHandler::getLvlUpScreenPosition()
{
	for (int i = 0; i < (int)party->party.size(); i++)
	{
		int index = 0;
		bool done = false;
		auto& unit = party->party[i];
		while (true)
		{
			if (index >= lvl_up_screens.size())
			{
				break;
			}
			if (unit->name == lvl_up_screens[index]->unit->name)
			{
				lvl_up_pos = index;
				done = true;
				break;
			}
			index++;
		}
		if (done == true)
		{
			break;
		}
	}
}

void ExpMenuHandler::generateLvLUpScreens()
{
	for (auto& [name, lvl_num] : level_up_amount)
	{
		for (auto& unit : party->party)
		{
			if (unit->name == name)
			{
				if (!unit->dead && lvl_num > 0)
				{
					shared_ptr<LvLUpScreen> lvl_up_screen = make_shared<LvLUpScreen>();
					lvl_up_screen->setup(game, party, unit.get(), lvl_num, exp_menu->battle_controller, exp_menu->menu_controller);
					lvl_up_screens.push_back(lvl_up_screen);
				}
			}
		}
	}
	needs_generation = false;
}
