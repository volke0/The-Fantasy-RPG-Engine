#include "Party.h"
#include "map.h"
#include "input_handler.h"
#include "game.h"
#include "vars.h"

//Setup
void Party::setup(Game* game, Input* input)
{
	this->input = input;
	name = "party";
	type = "player_party";
	PartyBase::setup(game);
}


void Party::addUnitToParty(int unit_id)
{
	shared_ptr<NPC> unit = make_shared<NPC>();
	unit->party_pos = (int)party.size();
	party.push_back(unit);
	unit->setup(game, unit_id);
	unit->party = this;
	SetVisibleParty();
}

void Party::AddSilver(int amount)
{
	slv += amount;
}

void Party::SubSilver(int amount)
{
	slv -= amount;
	if (slv < 0)
	{
		slv = 0;
	}
}

void Party::SetFormation()
{
	formation.SetFormation();
}

void Party::SetupCombatants(unordered_map<string, shared_ptr<NPC>>& combatants, BattleController* battle_controller)
{
	int num = 1;
	for (auto& unit : party)
	{
		if (combatants.find(unit->name) != combatants.end())
		{
			num++;
			string name = unit->name + "_" + to_string(num);
			unit->setBattleMode();
			unit->combat_name = name;
			combatants[name] = unit;
		}
		else
		{
			unit->setBattleMode();
			unit->combat_name = unit->name;
			combatants[unit->name] = unit;
		}
	}
	SetFormation();
}


//Basic Methods
void Party::draw()
{
	PartyBase::draw();
}

void Party::update()
{
	PartyBase::update();
}


//Menu stuff
void Party::updateMenuMode()
{
}