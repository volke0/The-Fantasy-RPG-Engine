#include "battle_menu.h"
#include "cursor.h"
#include "game.h"
#include "battle_controller.h"

void BattleMenu::basicSetup(Game* game, Party* party, BattleController* battle_controller, BattleMenuController* menu_controller)
{
	this->game = game;
	this->party = party;
	this->menu_controller = menu_controller;
	this->battle_controller = battle_controller;
	this->cursor = &battle_controller->cursor;
}
