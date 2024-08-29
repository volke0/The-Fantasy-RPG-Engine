#include "battle.h"
#include <cstdlib>
#include "game.h"
#include "battle_manager.h"
#include "item.h"
#include "consumable.h"
#include "party_base.h"
#include "unit.h"

Battle::~Battle()
{

}

void Battle::setup(Game* game, BattleManager* battle_manager, Json::Value input, unordered_map<string, shared_ptr<NPC>> combatants, int position)
{
	this->game = game;
	this->battle_manager = battle_manager;
	this->input = input;
	this->combatants = combatants;
	this->position = position;
	btl_calculator.basicSetup(game, this);
	mag_calculator.setup(game, this);
	item_calculator.setup(game, this);
	prepareBattle();
}


void Battle::play()
{
	//cout << "PLAYING BATTLE #" << position << "..." << endl;
	btl_display_handler.updateBattle();
	if (btl_display_handler.done == true)
	{
		done = true;
	}
	if (user->dead == true)
	{
		done = true;
	}
	if (btl_display_handler.wrap_up == true)
	{
		battle_manager->wrap_up = true;
	}
}

void Battle::draw()
{
	btl_display_handler.drawBattle();
}


void Battle::updateCombatants(unordered_map<string, shared_ptr<NPC>> combatants)
{
	this->combatants = combatants;
}


//Preparations
void Battle::prepareBattle()
{
	mode = input["main_option"].asString();
	//cout << mode << endl;
	prepareUnits();
	CalculateDamage();
}

void Battle::prepareUnits()
{
	user_name = input["user"].asString();
	user = combatants[user_name].get();
	party = user->party;
	target_names = input["targets"];
	bMultiTarget = input["bMultiTarget"].asBool();

	for (int i = 0; i < (int)target_names.size(); i++)
	{
		string name = target_names[i].asString();
		targets.push_back(combatants[name].get());
		unit_targets.push_back(combatants[name].get());
	}
	for (int i = 0; i < (int)input["other_targets"].size(); i++)
	{
		string name = input["other_targets"][i].asString();
		other_targets.push_back(combatants[name].get());
	}
	//showTargets();
}

void Battle::CalculateDamage()
{
	if (mode == "Attack")
	{
		for (int i = 0; i < targets.size(); i++)
		{
			auto& target = targets[i];
			btl_calculator.getHitCalculation(user, target);
			/*cout << "Hit Rate: " << hit_rates[i] << "%" << endl;
			cout << "Target Avoid: " << target->avoid << "%" << endl;
			cout << "Crit Rate: " << crit_rates[i] << "%" << endl;*/
			if (hit[i] == true)
			{
				btl_calculator.getAttackDamageCalculation(user, target, crit[i]);
				//cout << "Damage: " << dmg[i] << endl;
			}
		}
	}
	if (mode == "Magic")
	{
		magic_input = input["magic"].asString();
		magic_type = input["magic_type"].asString();

		for (int i = 0; i < targets.size(); i++)
		{
			auto& target = targets[i];
			shared_ptr<Magic> magic = make_shared<Magic>();
			magic->setup(game, user, target, game->GetMagicScript(magic_input));

			if (!magic->CheckIfEffectApplicable(unit_targets))
			{
				bCantUseMagic = true;
			}
			magic_list.push_back(magic);
			mag_calculator.setMagic(magic.get());
			mag_calculator.getMagHitCalculation(user, target);
			if (hit[i] == true)
			{
				mag_calculator.getMagAttackCalculation(user, target, crit[i], magic.get(), bMultiTarget);
			}
		}
		btl_display_handler.setMagicList(magic_list);
		//showMagic();
	}
	if (mode == "Item")
	{
		int item_index = input["item_selection"].asInt();
		for (int i = 0; i < targets.size(); i++)
		{
			auto& target = targets[i];
			shared_ptr<Consumable> item = make_shared<Consumable>();
			if (!party->item_inventory.checkIfExists(item_index))
			{
				bCantUseMagic = true;
				break;
			}
			auto& item_script = party->item_inventory.inventory[item_index]["item"];
			item->setup(game, user, target, item_script);
			item_list.push_back(item);
			item_calculator.setItem(item.get());
			item_calculator.getItemHitCalculation(user, target);
			if (hit[i] == true)
			{
				item_calculator.getItemAttackCalculation(user, target, crit[i], item.get());
			}
		}
		btl_display_handler.setItemList(item_list, item_index);
	}
	btl_display_handler.setup(game, this, user, targets);
	if (bCantUseMagic)
	{
		btl_display_handler.EndBattle();
	}
}


//Debug
void Battle::showTargets()
{
	for (auto& unit : targets)
	{
		cout << "TARGET: ";
		cout << unit->name << endl;
	}
}

void Battle::showMagic()
{
	for (auto& magic : magic_list)
	{
		cout << "magic: " << magic->ingame_name << endl;
	}
}
