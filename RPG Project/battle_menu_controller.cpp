#include "battle_menu_controller.h"
#include "unit.h"
#include "party.h"
#include "game.h"
#include "battle_controller.h"

BattleMenuController::~BattleMenuController()
{
	//sound_player.unloadSFX();
}

void BattleMenuController::setup(Game* game, Party* party, unordered_map<string, shared_ptr<NPC>> combatants, BattleController* battle_controller)
{
	this->game = game;
	this->party = party;
	this->combatants = combatants;
	this->battle_controller = battle_controller;
	this->cursor = &battle_controller->cursor;
	this->sound_player.setup(game);
	setupAllMenus();
	state_controller.setup(game, this);
}

void BattleMenuController::setupAllMenus()
{
	battle_ui.setup(game, party, battle_controller, this);
	player_menu.setup(game, party, battle_controller, this);
	unit_select.setup(game, party, battle_controller, this);
	exp_menu.setup(game, party, battle_controller, this);
	magic_menu.setup(game, party, battle_controller, this);
	item_menu.setup(game, party, battle_controller, this);
}



void BattleMenuController::update()
{
	timer.tick();
	sound_player.update();
	state_controller.update();
	battle_ui.update();
	if (timer.wait == false)
	{
		updateAllMenus();
	}
}

void BattleMenuController::updateAllMenus()
{
	if (on == true)
	{
		if (displayPlayerMenu == true)
		{
			player_menu.update();
		}
		if (displayMagicMenu == true)
		{
			magic_menu.update();
		}
		if (displayItemMenu)
		{
			item_menu.update();
		}
		if (displayUnitSelection == true)
		{
			unit_select.refreshCombatants();
			unit_select.update();
		}
		if (displayExpMenu == true && !exiting)
		{
			exp_menu.update();
		}
	}
	if (exiting)
	{
		exp_menu.unloadPartyFaces();
		player_menu.deleteAnimations();
	}
}


void BattleMenuController::draw()
{
	battle_ui.draw();
	if (timer.wait == false)
	{
		drawAllMenus();
	}
}

void BattleMenuController::drawAllMenus()
{
	if (on == true)
	{
		if (displayPlayerMenu == true)
		{
			player_menu.draw();
		}
		if (displayMagicMenu == true)
		{
			magic_menu.draw();
		}
		if (displayItemMenu)
		{
			item_menu.draw();
		}
		if (displayExpMenu == true && !exiting)
		{
			exp_menu.draw();
		}
	}
}



void BattleMenuController::turnOn()
{
	pushState("on");
	pushState("player_menu");
	changeUnit();
}

void BattleMenuController::startEnemyTurn()
{
	state_controller.clearStates();
	battle_controller->state_controller.pushState("enemy_turn");
}

void BattleMenuController::playSound(string sfx)
{
	sound_player.playSound(sfx);
}

void BattleMenuController::turnUnitSelectOn()
{
	pushState("unit_select");
	unit_select.turnOn();
}

void BattleMenuController::turnMagicMenuOn()
{
	pushState("magic_menu");
	magic_menu.turnOn();
}

void BattleMenuController::turnItemMenuOn()
{
	pushState("item_menu");
	item_menu.turnOn();
}

void BattleMenuController::changeFocus(string focus)
{
	this->focus = focus;
}



void BattleMenuController::changeUnit()
{
	if (battle_controller->game_over) return;

	getPartyMembers();
	player_index = 0;
	for (auto& unit : party_members)
	{
		if (unit->btl_con_controller.findCondition("ready") || unit->btl_con_controller.findCondition("defend"))
		{
			player_index++;
		}
		else if (unit->dead == true)
		{
			player_index++;
		}
		else
		{
			break;
		}
	}
	if (player_index > party_members.size() - 1)
	{
		player_index = 0;
		startEnemyTurn();
	}
	else
	{
		clearStates();
		pushState("on");
		pushState("player_menu");
		player_menu.turnPlayerMenuOn();
	}
	current_unit = party_members[player_index];
	current_unit_name = party_member_names[player_index];
}

void BattleMenuController::GotoPreviousUnit()
{
	if (battle_controller->game_over) return;
	if (player_index <= 0)
	{
		playSound("click2"); 
		return;
	}

	//Decrement to the previous character in the party
	player_index--;

	clearStates();
	pushState("on");
	pushState("player_menu");
	player_menu.turnPlayerMenuOn();

	current_unit = party_members[player_index];
	current_unit_name = party_member_names[player_index];

	//Reset the current character in the party
	current_unit->btl_logic_controller.EndAttackMode();
	current_unit->btl_logic_controller.endMagicMode();
	current_unit->btl_logic_controller.endItemMode();


	//Remove the previous battle
	battle_controller->battle_queue_manager.RemovePreviousBattle();
}



void BattleMenuController::pushState(string key)
{
	state_controller.pushState(key);
}

void BattleMenuController::eraseState(string key)
{
	if (key == "unit_select")
	{
		state_controller.eraseUnitSelectState();
	}
	state_controller.eraseState(key);
}

void BattleMenuController::clearStates()
{
	state_controller.states.clear();
	state_controller.states.shrink_to_fit();
}

void BattleMenuController::refreshCombatants(unordered_map<string, shared_ptr<NPC>> combatants)
{
	this->combatants = combatants;
}

bool BattleMenuController::checkFocus(string key)
{
	if (focus == key)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void BattleMenuController::getPartyMembers()
{
	party_members .clear();
	party_member_names .clear();
	for (auto& [name, unit] : combatants)
	{
		for (auto& party_unit : party->party)
		{
			if (unit == party_unit)
			{
				party_members.push_back(unit.get());
				party_member_names.push_back(name);
			}
		}
	}
}
