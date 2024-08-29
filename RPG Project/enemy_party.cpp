#include "enemy_party.h"
#include "npc.h"
#include "vars.h"

void EnemyParty::setup(Game* game, Json::Value party_script)
{
	this->party_script = party_script;
	name = "enemy_party";
	type = "enemy_party";
	PartyBase::setup(game);
	setupParty();
}

void EnemyParty::setupParty()
{
	Json::Value slots = party_script["slots"];
	for (int i = 0; i < (int)slots.size(); i++)
	{
		shared_ptr<NPC> enemy = make_shared<NPC>();
		int unit_id = slots[i]["id"].asInt();
		enemy->setup(game, unit_id);
		enemy->party = this;
		party.push_back(enemy);
	}
	formation_name = party_script["formation"].asString();
	SetupInventory();
}

void EnemyParty::SetupInventory()
{
	if (party_script.isMember("inventory"))
	{
		auto& inventory_script = party_script["inventory"];
		for (int i = 0; i < (int)inventory_script.size(); i++)
		{
			string item_name = inventory_script[i]["item"].asString();
			int amount = inventory_script[i]["amount"].asInt();
			addToInventory(item_name, amount);
		}
	}
}


void EnemyParty::update()
{
	PartyBase::update();
}

void EnemyParty::draw()
{
	PartyBase::draw();
}


void EnemyParty::SetFormation()
{
	formation.SetFormation(formation_name);
}

void EnemyParty::SetupCombatants(unordered_map<string, shared_ptr<NPC>>& combatants, vector<shared_ptr<NPC>>& enemies, BattleController* battle_controller)
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
			//unit->anim_controller.changeAnimation("entrance");
			unit->setBattleMode();
			unit->combat_name = unit->name;
			combatants[unit->name] = unit;
		}
		unit->battle_ai.setupAI(battle_controller, unit->battle_ai_script_name, this);
		enemies.push_back(unit);
	}
	SetFormation();
}


