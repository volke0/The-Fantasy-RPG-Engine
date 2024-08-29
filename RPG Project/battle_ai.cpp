#include "battle_ai.h"
#include <cstdlib>
#include <time.h>
#include "lib/cpp_assert.hpp"
#include "battle_controller.h"
#include "party.h"
#include "equipment_base.h"
#include "npc.h"
#include "magic.h"
#include "game.h"

void BattleAI::setup(Game* game, Unit* user)
{
	basic_setup(game, user);
}

void BattleAI::setupAI(BattleController* battle_controller, string ai_script_path, PartyBase* user_party)
{
	this->user_party = user_party;
	setBattleController(battle_controller);
	if (ai_script_path != "")
	{
		setupAIScript("battle", ai_script_path);
	}
	else
	{
		bPlayDefaultAI = true;
	}
	GetTriggers();
	ParseAIBehavior();
	ParseMetaData();
	GetSelectors();
}

void BattleAI::setBattleController(BattleController* battle_controller)
{
	this->battle_controller = battle_controller;
}

void BattleAI::ParseMetaData()
{
	string type = meta_data["type"].asString();

	if (type == "default")
	{
		bPlayDefaultAI = true;
	}

	if (type == "selector")
	{
		string target_type = meta_data["targets"].asString();

		if (target_type == "multi")
		{
			bMultiTarget = true;
		}
		if (target_type == "enemies")
		{
			bEnemiesOnly = true;
		}
		if (target_type == "allies")
		{
			bAlliesOnly = true;
		}
	}
}

void BattleAI::GetSelectors()
{
	if (bPlayDefaultAI) return;
	for (int index = 0; index < (int)selectors.size(); index++)
	{
		selector_line = selectors[index];

		//Modes
		if (selector_line.isMember("modes"))
		{
			float attackPercent = selector_line["modes"]["attack"].asFloat();
			float magicPercent = selector_line["modes"]["magic"].asFloat();
			float itemPercent = selector_line["modes"]["item"].asFloat();

			mode_percents["Attack"] = attackPercent;
			mode_percents["Magic"] = magicPercent;
			mode_percents["Item"] = itemPercent;
		}

		//Targets
		if (selector_line.isMember("target_mode"))
		{
			string ai_mode = selector_line["target_mode"].asString();
			SetAIType(ai_mode);
		}

		//Magic
		if (selector_line.isMember("magic"))
		{
			for (auto& key : selector_line["magic"].getMemberNames())
			{
				magic_percents[key] = selector_line["magic"][key].asFloat();
			}
		}

		//Item
		if (selector_line.isMember("item"))
		{
			for (auto& key : selector_line["item"].getMemberNames())
			{
				item_percents[key] = selector_line["item"][key].asFloat();
			}
		}

		//TODO: Add triggers for selectors, e.g. {"triggers": {"hp<": 0.60, "move": "rock_hit", "repeat": false}}
		//TODO: Add magic target preferences for each type of magic
	}
}

void BattleAI::GetTriggers()
{
	Json::Value triggers = ai_script["triggers"];

	if ((int)triggers.size() == 0) return;
	for (int i = 0; i < (int)triggers.size(); i++)
	{
		Json::Value trigger_line = triggers[i];
		//cout << trigger_line << endl;
		string hp_trigger_type = "";

		if (trigger_line.isMember("hp<"))
		{
			hp_trigger_type = "hp<";
		}

		if (trigger_line.isMember("hp>"))
		{
			hp_trigger_type = "hp>";
		}

		if (trigger_line.isMember("hp="))
		{
			hp_trigger_type = "hp=";
		}

		if (trigger_line.isMember("hp<="))
		{
			hp_trigger_type = "hp<=";
		}

		if (trigger_line.isMember("hp>="))
		{
			hp_trigger_type = "hp>=";
		}

		if (trigger_line.isMember("hp!="))
		{
			hp_trigger_type = "hp!=";
		}

		if (hp_trigger_type != "")
		{
			float hp_pct = trigger_line[hp_trigger_type].asFloat();
			hp_thresholds[hp_trigger_type].push_back(hp_pct);

			string threshold_command_name = hp_trigger_type + "-" + to_string(hp_pct);
			threshold_commands[threshold_command_name] = trigger_line;
		}
	}
}



void BattleAI::calculateDecision()
{
	input.clear();
	target_names.clear();

	generateAlliesEnemies();
	RunBehavior();
	if (!bPlayDefaultAI)
	{
		ExecuteCommands();
	}
	else
	{
		//---Implement Generic AI here if script is unintialized or if "normal" is inputted for pick---//
		mode = mode_list[mode_index];
		input["main_option"] = mode;
		input["user"] = user_name;
		getTargetSelection();

		pushBattle();
	}
}

void BattleAI::generateAlliesEnemies()
{
	allies.clear();
	allies.shrink_to_fit();
	enemies.clear();
	enemies.shrink_to_fit();
	ally_names.clear();
	ally_names.shrink_to_fit();
	enemy_names.clear();
	enemy_names.shrink_to_fit();
	target_stats.clear();

	user_index = user_type[user->type];
	string ally = unit_types[user_index][0];
	string enemy = unit_types[user_index][1];

	int index = 0;
	int ally_index = 0;
	bool bResetPredictedValues = false;
	for (auto& [name, unit] : battle_controller->combatants)
	{
		if (unit->type == ally)
		{
			allies.push_back(unit.get());
			ally_names.push_back(name);
			if (name == user->combat_name)
			{
				if (ally_index == 0)
				{
					bResetPredictedValues = true;
				}
			}
			if (bResetPredictedValues)
			{
				unit->predicted_hp = INT_MAX;
				unit->predicted_mp = INT_MAX;

				//Reset predicted item usage amounts
				for (auto& item_wrapper : user->party->item_inventory.inventory)
				{
					item_wrapper["predicted_usage"] = item_wrapper["amount"].asInt();
				}
			}
			ally_index++;
		}
		else if (unit->type == enemy)
		{
			enemies.push_back(unit.get());
			enemy_names.push_back(name);
		}
		if (unit.get() == user)
		{
			user_name = name;
			user_unique_id = index;
		}
		index++;
	}
}

void BattleAI::RunBehavior()
{
	magic_name = GetCommand(magic_percents);  //Gets the magic name from the list
	Evaluate();
	CheckTriggers();
}

void BattleAI::Evaluate()
{
	EvaluateAllies();
	EvaluateEnemies();
}

void BattleAI::CheckTriggers()
{
	trigger_commands.clear();
	trigger_commands.shrink_to_fit();

	float hp_pct = 100 * ((float)user->hp / (float)user->max_hp);
	for (auto [hp_indicator, threshold_pcts] : hp_thresholds)
	{
		for (auto& threshold : threshold_pcts)
		{
			float threshold_pct = 100 * threshold;
			string command_name = hp_indicator + "-" + to_string(threshold);

			if (hp_indicator == "hp<")
			{
				if (hp_pct < threshold_pct) trigger_commands.push_back(command_name);
			}
			if (hp_indicator == "hp>")
			{
				if (hp_pct > threshold_pct) trigger_commands.push_back(command_name);
			}
			if (hp_indicator == "hp=")
			{
				if (hp_pct == threshold_pct) trigger_commands.push_back(command_name);
			}
			if (hp_indicator == "hp<=")
			{
				if (hp_pct <= threshold_pct) trigger_commands.push_back(command_name);
			}
			if (hp_indicator == "hp>=")
			{
				if (hp_pct >= threshold_pct) trigger_commands.push_back(command_name);
			}
			if (hp_indicator == "hp!=")
			{
				if (hp_pct != threshold_pct) trigger_commands.push_back(command_name);
			}

			trigger_commands.size() > 0 ? bGotoTriggerReact = true : bGotoTriggerReact = false;
		}
	}
}

void BattleAI::EvaluateAllies()
{
	int lowest_hp_pct = 101;
	int lowest_mp_pct = 101;
	int max_hp_count = 0;
	int max_mp_count = 0;

	string best_target = "";

	for (auto& ally : allies)
	{
		int hp = ally->predicted_hp == INT_MAX ? ally->hp : ally->predicted_hp;
		int mp = ally->predicted_mp == INT_MAX ? ally->mp : ally->predicted_mp;
		int hp_pct = (int)(100 * (float)hp / (float)ally->max_hp);
		int mp_pct = (int)(100 * (float)mp / (float)ally->max_mp);

		target_stats[ally->combat_name]["hp_pct"] = hp_pct;
		target_stats[ally->combat_name]["mp_pct"] = mp_pct;

		if (hp_pct >= 99)
		{
			max_hp_count++;
		}
		if (mp_pct >= 99)
		{
			max_mp_count++;
		}

		if (hp_pct < lowest_hp_pct)
		{
			lowest_hp_pct = hp_pct;
			best_allies["hp_pct"] = ally->combat_name;
		}
		if (mp_pct < lowest_mp_pct)
		{
			lowest_mp_pct = mp_pct;
			best_allies["mp_pct"] = ally->combat_name;
		}
	}
	max_hp_count == (int)allies.size() ? bEveryAllyMaxHP = true : bEveryAllyMaxHP = false;
	max_mp_count == (int)allies.size() ? bEveryAllyMaxMP = true : bEveryAllyMaxMP = false;
}

void BattleAI::EvaluateEnemies()
{
	int best_atk_dmg = 0;
	int best_mag_dmg = 0;
	int best_hit = 0;

	string best_target = "";

	for (auto& enemy : enemies)
	{
		Magic temp_magic;
		temp_magic.setup(game, user, enemy, game->GetMagicScript(magic_name));

		int atk_dmg = (int)static_cast<NPC*>(user)->GetAttackDamage(enemy);
		int mag_dmg = (int)temp_magic.GetDamage(user, enemy, false);
		int hit_rate = user->hit - enemy->avoid;



		target_stats[enemy->combat_name]["atk"] = atk_dmg;
		target_stats[enemy->combat_name]["mag"] = mag_dmg;
		target_stats[enemy->combat_name]["hit"] = hit_rate;

		if (atk_dmg > best_atk_dmg)
		{
			best_atk_dmg = atk_dmg;
			best_enemies["atk"] = enemy->combat_name;
		}
		if (mag_dmg > best_mag_dmg)
		{
			best_mag_dmg = mag_dmg;
			best_enemies["mag"] = enemy->combat_name;
		}
		if (hit_rate > best_hit)
		{
			best_hit = hit_rate;
			best_enemies["hit"] = enemy->combat_name;
		}
	}
}



void BattleAI::ExecuteCommands()
{
	RunTriggerCommands();
	mode = GetCommand(mode_percents);
	item_name = GetCommand(item_percents);

	input["main_option"] = mode;
	input["user"] = user_name;
	pushBattle();
}

void BattleAI::RunTriggerCommands()
{
	if (!bGotoTriggerReact) return;

	for (auto& trigger : trigger_commands)
	{
		auto& command = threshold_commands[trigger];
		SetCommand(command);
	}
}

void BattleAI::SetCommand(Json::Value& command)
{
	actions.clear();
	actions = {};
	bool done = command["done"].asBool();
	bool repeat = command["repeat"].asBool();
	if (done && !repeat) return;

	string ai_mode = command["chg_ai"].asString();

	if (ai_mode != "")
	{
		SetAIType(ai_mode);
		/*cout << "SWITCHED AI MODE TO ---> " << ai_mode << endl;*/
	}
	if (command.isMember("actions"))
	{
		actions = command["actions"];
	}

	command["done"] = true;
}


void BattleAI::getTargetSelection()
{
	//---Add different ways of determining targets later (Such as if the user wants to heal his allies)---//
	current_selection.clear();
	current_selection.shrink_to_fit();
	target_names.clear();
	target_names.shrink_to_fit();
	bool bGotoSingleTargetSelection = true;

	string magic_type = game->GetMagicScript(magic_name)["type"].asString();
	bool bHealingMagic = magic_type == "healing" ? true : false;

	if (mode == "Attack")
	{
		current_selection = enemies;
	}
	if (mode == "Magic")
	{
		//TODO: Switch to allies if it's healing magic for Smart Mode
		bHealingMagic ? current_selection = allies : current_selection = enemies;

		if (bEnemiesOnly) current_selection = enemies; //This overrides healing magic ai
		if (bAlliesOnly) current_selection = allies; //This overrides damage magic ai

		if (bMultiTarget)
		{
			for (auto& unit : current_selection)
			{
				target_names.push_back(unit->combat_name);
			}
			bGotoSingleTargetSelection = false;
		}
	}
	if (mode == "Item")
	{
		current_selection = allies;
	}

	if (bEnemiesOnly) current_selection = enemies;

	if (bAlliesOnly) current_selection = allies;

	if (!bGotoSingleTargetSelection) return;

	if (bGotoSingleTargetSelection)
	{
		string best_target = "";

		if (bRandomTarget)
		{
			target_index = getRandomTarget();
			target_names.push_back(current_selection[target_index]->combat_name);
			return;
		}
		if (bMagicMode)
		{
			mode = "Magic";
			target_index = getRandomTarget();
			target_names.push_back(current_selection[target_index]->combat_name);
			return;
		}
		if (bHealingMode)
		{
			string heal_type = "";
			if (bEveryAllyMaxHP && bEveryAllyMaxMP || bEnemiesOnly)
			{
				target_index = getRandomTarget();
				target_names.push_back(current_selection[target_index]->combat_name);
				return;
			}

			string best_hp_heal_target = best_allies["hp_pct"];
			string best_mp_heal_target = best_allies["mp_pct"];

			if (target_stats[best_hp_heal_target]["hp_pct"] <= 100 || target_stats[best_hp_heal_target]["hp_pct"] <= target_stats[best_mp_heal_target]["mp_pct"])
			{
				heal_type = "hp";
			}
			else
			{
				if (target_stats[best_mp_heal_target]["mp_pct"] <= 100)
				{
					heal_type = "mp";
				}
			}

			best_target = HealingLogic(best_hp_heal_target, heal_type);

			if (best_target == "" || heal_type == "")
			{
				current_selection = enemies;
				mode = GetCommand(mode_percents);
				magic_name = GetCommand(magic_percents);
				item_name = GetCommand(item_percents);
				target_index = getRandomTarget();
				target_names.push_back(current_selection[target_index]->combat_name);
				return;
			}
		}

		SmartSelection(best_target);
		if (bMostDamageSelection)
		{
			string best_target_atk = best_enemies["atk"];
			string best_target_mag = best_enemies["mag"];

			target_stats[best_target_atk]["atk"] > target_stats[best_target_mag]["mag"] ? mode = "Attack" : mode = "Magic";  //TODO: Implement Special Mode calculations later...
			if (mode == "Attack")
			{
				best_target = best_target_atk;
			}
			if (mode == "Magic")
			{
				best_target = best_target_mag;
			}
			if (mode == "Special")
			{
				string best_atk_target = best_enemies["atk"];
				string best_mag_target = best_enemies["mag"];

				if (target_stats[best_atk_target]["atk"] > target_stats[best_mag_target]["mag"])
				{
					best_target = best_enemies["atk"];
				}
				if (target_stats[best_atk_target]["mag"] > target_stats[best_mag_target]["atk"])
				{
					best_target = best_enemies["mag"];
				}
			}
		}
		if (bPlayDefaultAI)
		{
			target_index = getRandomTarget();
			target_names.push_back(current_selection[target_index]->combat_name);
			return;
		}
		target_names.push_back(best_target);
	}
}





void BattleAI::pushBattle()
{
	if ((int)actions.size() != 0)
	{
		SetActionChain();
		return;
	}
	getTargetSelection();
	setInput();
	setAppropiateReadyStatus();
	cout << input << endl;
	battle_controller->battle_queue_manager.pushBattle(game, input);
}

void BattleAI::SetActionChain()
{
	for (int i = 0; i < (int)actions.size(); i++)
	{
		Json::Value action = actions[i];
		unordered_map<string, string> mode_map = { {"attack", "Attack"}, {"magic", "Magic"}, {"item", "Item"} , {"", ""} };

		string mode_name = action["mode"].asString();

		//Check to see if the mode is a valid one
		string error_msg = "Error in Battle AI: " + mode_name + " is not a valid mode." + " Please enter a valid mode: attack, magic, item or just an empty string.  Remember it is case sensitive.";
		log_assert((mode_map.find(mode_name) == mode_map.end()), error_msg);

		mode = mode_map[action["mode"].asString()];
		magic_name = action["magic_name"].asString();
		if (mode == "Magic" && magic_name == "")
		{
			magic_name = GetRandomMagic();
		}
		item_name = action["item_name"].asString();
		string target_ai_type = action["target"].asString();
		SetAIType(target_ai_type);

		getTargetSelection();

		if (mode != "")
		{
			input["main_option"] = mode;
			input["user"] = user_name;

			setInput();
			setAppropiateReadyStatus();
			cout << "ACTION INPUT ==> " << input << endl;
			battle_controller->battle_queue_manager.pushBattle(game, input);
		}
	}
	actions.clear();
	actions = {};
}


void BattleAI::setInput()
{
	int index = 0;
	Json::Value target_list;

	for (string target_name : target_names)
	{
		target_list[index] = target_name;
		index++;
	}

	input["targets"] = target_list;
	input["magic"] = magic_name;

	if (mode == "Item")
	{
		string citem_name = user_party->item_inventory.getItemName(item_name);
		int inv_index = user_party->item_inventory.getItemID(citem_name);
		input["item_selection"] = inv_index;
	}
}

void BattleAI::setAppropiateReadyStatus()
{
	user->btl_con_controller.setCondition("ready");
	if (mode == "Attack")
	{
		user->btl_con_controller.setCondition("atk_wait");
	}
	else if (mode == "Special")
	{
		//---For later implementation---//
	}
	else if (mode == "Magic")
	{
		user->btl_con_controller.setCondition("mag_wait");
	}
	else if (mode == "Item")
	{
		user->btl_con_controller.setCondition("item_wait");
	}
	user->btl_con_controller.eraseCondition("idle");
}

bool BattleAI::isTargetDead(int target)
{
	if (!isPartyWiped())
	{
		if (current_selection[target]->dead)
		{
			return true;
		}
	}
	return false;
}

bool BattleAI::isPartyWiped()
{
	int counter = 0;
	for (auto& unit : battle_controller->party->party)
	{
		if (!unit->dead)
		{
			counter++;
		}
	}

	if (counter != 0)
	{
		return false;
	}
	return true;
}

int BattleAI::GetRng(int range)
{	
	return game->GetRNG(rng_frame, range);
}

string BattleAI::GetCommand(unordered_map<string, float> command_pcts)
{
	int rng_pct = GetRng();

	unordered_map<string, float> commands = {};
	string command = "";

	float greatest_value = 0.f;
	string greatest_value_command = "";

	for (auto& [key, value] : command_pcts)
	{
		float value_pct = value * 100.f;
		if (rng_pct <= (int)value_pct)
		{
			commands[key] = value;
		}
		if (value > greatest_value)
		{
			greatest_value = value;
			greatest_value_command = key;
		}
	}

	if ((int)commands.size() == 0)
	{
		command = greatest_value_command;
	}
	else
	{
		float least_value = 2.f;
		string least_value_command = "";

		for (auto& pair : commands)
		{
			if (pair.second < least_value && pair.second > 0.f)
			{
				least_value = pair.second;
				command = pair.first;
			}
		}
	}
	return command;
}

string BattleAI::GetRandomMagic()
{
	string temp_magic_name = game->magic_database.master_list[0];  //To default it is set to the first item in the magic master list; just in case you don't have any magic set on the unit.
	if ((int)user->magic_options.size() > 0)
	{
		int index = GetRng((int)user->magic_options.size());
		temp_magic_name = user->magic_options[index];
	}
	return temp_magic_name;
}


int BattleAI::getRandomTarget()
{
	int target = 0;
	do 
	{
		target = GetRng((int)current_selection.size());
	} while (isTargetDead(target));
	
	return target;
}


void BattleAI::SmartSelection(string& best_target)
{
	if (bSmartSelection)
	{
		string best_hit_target = best_enemies["hit"];
		string best_dmg_target = "";
		string best_hp_heal_target = best_allies["hp_pct"];
		string best_mp_heal_target = best_allies["mp_pct"];
		string target_mode = "";

		bool bTriggerEmergencyHeal = false;
		bool bTriggerMPHeal = false;
		bool bSkipDamagePart = false;
		bool bCanUseMagic = user->mp >= game->GetMagicScript(magic_name)["cost"].asInt() ? true : false;

		//Refactor later for including Specials
		user->atk > user->mag ? mode = "Attack" : mode = "Magic";

		if (target_stats[best_hp_heal_target]["hp_pct"] <= 30)
		{
			bTriggerEmergencyHeal = true;
		}
		else
		{
			//TODO: UnComment later when implementing the MP Heal AI logic
			if (target_stats[best_hp_heal_target]["mp_pct"] <= 25)
			{
				bTriggerMPHeal = true;
			}
		}

		if (bTriggerEmergencyHeal)
		{
			best_target = HealingLogic(best_hp_heal_target, "hp");
			if (best_target != "")
			{
				bSkipDamagePart = true;
			}
		}

		if (bTriggerMPHeal)
		{
			best_target = HealingLogic(best_mp_heal_target, "mp");
			if (best_target != "")
			{
				bSkipDamagePart = true;
			}
		}

		if (!bSkipDamagePart)
		{
			if (mode == "Attack" || !bCanUseMagic)
			{
				best_dmg_target = best_enemies["atk"];
				target_mode = "atk";
			}
			if (mode == "Magic")
			{
				best_dmg_target = best_enemies["mag"];
				target_mode = "mag";
			}

			if (best_hit_target != best_dmg_target)
			{
				int hit_diff = target_stats[best_hit_target]["hit"] - target_stats[best_dmg_target]["hit"];
				int dmg_diff = target_stats[best_hit_target][target_mode] - target_stats[best_dmg_target][target_mode];

				if (hit_diff > 10 && target_stats[best_hit_target][target_mode] != 0)
				{
					best_target = best_hit_target;
				}
				else
				{
					best_target = best_dmg_target;
				}
			}
			else
			{
				best_target = best_hit_target;
			}
		}
	}
}

string BattleAI::HealingLogic(string best_heal_target, string heal_type)
{
	string best_target = "";
	int best_magic_healing_power = 0;
	int best_item_healing_power = 0;
	//Item check
	if ((int)user_party->item_inventory.inventory.size() > 0)
	{
		for (auto& item_wrapper : user->party->item_inventory.inventory)
		{
			auto& item = item_wrapper["item"];
			string name = item["c_name"].asString();
			string type = item["type"].asString();
			string sub_type = item["sub_type"].asString();

			if (sub_type == "heal")
			{
				string item_heal_type = item["heal_type"].asString();
				int heal_amount = item["amount"].asInt();

				if (item_heal_type != heal_type) continue;

				if (heal_amount > best_item_healing_power && item_wrapper["predicted_usage"].asInt() != 0)
				{
					best_item_healing_power = heal_amount;
					item_name = name;
					item_wrapper["predicted_usage"] = item_wrapper["amount"].asInt() - 1;
				}
			}
		}
	}

	//Magic check
	if ((int)user->magic_options.size() > 0 && user->mp > 0)
	{
		for (auto& option : user->magic_options)
		{
			Magic temp_magic;
			temp_magic.setup(game, user, user, game->GetMagicScript(option));

			string magic_type = temp_magic.mag_type;
			int magic_cost = temp_magic.cost;

			if (magic_type == "healing" && user->mp > magic_cost)
			{
				string magic_healing_type = temp_magic.heal_type;

				if (magic_healing_type != heal_type) continue;

				int healing_power = (int)temp_magic.GetDamage(user, user, false);
				if (healing_power > best_magic_healing_power)
				{
					best_magic_healing_power = healing_power;
					magic_name = option;
					break;
				}
			}
		}
	}

	if (best_magic_healing_power > 0 && best_magic_healing_power > best_item_healing_power)
	{
		best_target = best_heal_target;
		battle_controller->combatants[best_target]->predicted_hp = best_magic_healing_power + battle_controller->combatants[best_target]->hp;
		mode = "Magic";
	}
	else if (best_item_healing_power > 0 && best_item_healing_power > best_magic_healing_power)
	{
		best_target = best_heal_target;
		battle_controller->combatants[best_target]->predicted_hp = best_item_healing_power + battle_controller->combatants[best_target]->hp;
		mode = "Item";
	}
	return best_target;
}

void BattleAI::SetAIType(string ai_mode)
{
	ai_mode == "heal" ? bHealingMode = true : bHealingMode = false;
	ai_mode == "magic" ? bMagicMode = true : bMagicMode = false;
	ai_mode == "random" ? bRandomTarget = true : bRandomTarget = false;
	ai_mode == "damage" ? bMostDamageSelection = true : bMostDamageSelection = false;
	ai_mode == "smart" ? bSmartSelection = true : bSmartSelection = false;

	if (ai_mode == "")
	{
		bRandomTarget = true;
		bMagicMode = false;
		bHealingMode = false;
		bSmartSelection = false;
		bMostDamageSelection = false;
	}
}
