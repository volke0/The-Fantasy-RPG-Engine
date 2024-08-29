#include "unit_select.h"
#include "party.h"
#include "game.h"
#include "npc.h"
#include "vfx_animation.h"
#include "battle_cursor.h"
#include "battle_controller.h"
#include "consumable.h"
#include "magic.h"
#include "vars.h"

void UnitSelectMenu::setup(Game* game, Party* party, BattleController* battle_controller, BattleMenuController* menu_controller)
{
	basicSetup(game, party, battle_controller, menu_controller);
	refreshCombatants();
	refreshAllyList();
}


void UnitSelectMenu::update()
{
	if (menu_controller->checkFocus("unit_select"))
	{
		refreshCombatants();
		cursor->setCursor(current_selection, combatants);
		if (cursor->FindKey("a"))
		{
			SwitchToMultiTarget();
			cursor->SetMultiTargetMode(bMultiTarget);
		}
		if (cursor->return_input)
		{
			string current_name = current_selection[cursor->index];
			checkIfAttack(current_name);
			checkIfSpecial(current_name);
			checkIfMagic(current_name);
			checkIfItem(current_name);
		}
		if (cursor->go_back == true)
		{
			cursor->go_back = false;
			menu_controller->eraseState("unit_select");
			bMultiTarget = false;
			cursor->SetMultiTargetMode(bMultiTarget);
			menu_controller->playSound("click2");
		}
	}
}

void UnitSelectMenu::sendInput()
{
	cursor->return_input = false;
	menu_controller->playSound("click");
	Json::Value target_list;
	Json::Value other_targets;

	AddTargetsToList(target_list, other_targets);

	menu_controller->input_history["targets"] = target_list;
	menu_controller->input_history["other_targets"] = other_targets;
	menu_controller->input_history["bMultiTarget"] = bMultiTarget;
	//cout << "Option: " << menu_controller->input_history["main_option"];
	//cout << " >> " << "Target: " << menu_controller->input_history["targets"] << endl;
	//cout << "INPUT HISTORY: " << menu_controller->input_history << endl;
	playerReady();
}

void UnitSelectMenu::AddTargetsToList(Json::Value& target_list, Json::Value& other_targets)
{
	if (bMultiTarget)
	{
		int index = 0;
		for (auto& option : current_selection)
		{
			target_list[index] = option;
			index++;
		}
	}
	else
	{
		target_list[0] = cursor->selected_option;

		int index = 0;
		for (auto& name : current_selection)
		{
			other_targets[index] = name;
			index++;
		}
	}
}

void UnitSelectMenu::SwitchToMultiTarget()
{
	if (menu_controller->input_history["main_option"] == "Attack" || (menu_controller->input_history["main_option"] == "Item"))
	{
		bMultiTarget = false;
		menu_controller->playSound("cantclick");
		return;
	}

	!bMultiTarget ? bMultiTarget = true : bMultiTarget = false;
	menu_controller->playSound("click");
}

void UnitSelectMenu::cancelInput()
{
	cursor->return_input = false;
	menu_controller->playSound("cantclick");
}


void UnitSelectMenu::refreshCombatants()
{
	this->combatants = menu_controller->combatants;
	refreshEnemyList();

	current_selection.clear();
	if (cursor->switch_to_enemies == true)
	{
		current_selection = enemy_names;
	}
	if (cursor->switch_to_allies == true)
	{
		current_selection = ally_names;
	}
	if (enemies.size() == 0)
	{
		menu_controller->eraseState("unit_select");
		current_selection = ally_names;
		cursor->switch_to_allies = true;
	}
}

void UnitSelectMenu::refreshEnemyList()
{
	enemies.clear();
	enemy_names.clear();
	for (auto& [name, unit] : combatants)
	{
		if (unit->type == "enemy")
		{
			enemies.push_back(unit.get());
			enemy_names.push_back(name);
		}
	}
}


void UnitSelectMenu::refreshAllyList()
{
	allies.clear();
	ally_names.clear();
	for (auto& [name, unit] : combatants)
	{
		if (unit->type == "playable")
		{
			allies.push_back(unit.get());
			ally_names.push_back(name);
		}
	}
}

void UnitSelectMenu::setAppropiatePlayerReadyStatus()
{
	menu_controller->current_unit->btl_con_controller.setCondition("ready");
	if (menu_controller->input_history["main_option"] == "Attack")
	{
		menu_controller->current_unit->btl_con_controller.setCondition("atk_wait");
	}
	else if (menu_controller->input_history["main_option"] == "Magic")
	{
		menu_controller->current_unit->btl_con_controller.setCondition("mag_wait");
	}
	else if (menu_controller->input_history["main_option"] == "Item")
	{
		menu_controller->current_unit->btl_con_controller.setCondition("item_wait");
	}
	menu_controller->current_unit->btl_con_controller.eraseCondition("idle");
}

void UnitSelectMenu::playerReady()
{
	auto& input_history = menu_controller->input_history;
	battle_controller->battle_queue_manager.pushBattle(game, input_history);
	setAppropiatePlayerReadyStatus();
	//cout << "PUSHED READY FOR " << menu_controller->current_unit->name << endl;

	menu_controller->changeUnit();
	bMultiTarget = false;
}

void UnitSelectMenu::checkIfAttack(string current_name)
{
	if (menu_controller->input_history["main_option"].asString() == "Attack")
	{
		if (combatants[current_name]->name != menu_controller->current_unit->name)
		{
			!combatants[current_name]->dead ? sendInput() : cancelInput();
		}
		else
		{
			cancelInput();
		}
	}
}

void UnitSelectMenu::checkIfSpecial(string current_name)
{
	//TODO: Implement Specials later... for now just cancel the input.
	if (menu_controller->input_history["main_option"].asString() == "Special")
	{
		cancelInput();
	}
}

void UnitSelectMenu::checkIfMagic(string current_name)
{
	if (menu_controller->input_history["main_option"].asString() == "Magic")
	{
		string name = menu_controller->input_history["magic"].asString();
		Json::Value magic_script = game->magic_database.magic_scripts[name];

		Magic magic;
		magic.setup(game, menu_controller->current_unit, combatants[cursor->selected_option].get(), magic_script);

		vector<Unit*> selected_targets = {};

		if (bMultiTarget)
		{
			for (auto& target_name : current_selection)
			{
				selected_targets.push_back(combatants[target_name].get());
			}
		}
		bool bCanSendInput = false;

		bMultiTarget ? bCanSendInput = magic.CheckIfEffectApplicable(selected_targets) : bCanSendInput = magic.CheckIfEffectApplicable();

		if (bCanSendInput)
		{
			sendInput();
		}
		else
		{
			cancelInput();
		}
	}
}

void UnitSelectMenu::checkIfItem(string current_name)
{
	if (menu_controller->input_history["main_option"].asString() == "Item")
	{
		int item_index = menu_controller->input_history["item_selection"].asInt();
		Json::Value item_script = party->item_inventory.inventory[item_index]["item"];
		Consumable item;
		item.setup(game, menu_controller->current_unit, combatants[cursor->selected_option].get(), item_script);
		if (item.CheckIfEffectApplicable())
		{
			sendInput();
		}
		else
		{
			cancelInput();
		}
	}
}

void UnitSelectMenu::turnOn()
{
	refreshCombatants();
	if (cursor->switch_to_enemies == true)
	{
		current_selection = enemy_names;
	}
	if (cursor->switch_to_allies == true)
	{
		current_selection = ally_names;
	}
	cursor->index = 0;
	cursor->setCursor(current_selection, combatants);
}
