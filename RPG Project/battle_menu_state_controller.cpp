#include "battle_menu_state_controller.h"
#include "battle_menu_controller.h"
#include "battle_cursor.h"
#include "player_menu.h"

void BattleMenuStateController::setup(Game* game, BattleMenuController* menu_controller)
{
	basicSetup(game);
	this->menu_controller = menu_controller;
}


void BattleMenuStateController::update()
{
	turnOn();
	updateMenus();
}

void BattleMenuStateController::eraseUnitSelectState()
{
	eraseState("unit_select");
	if (findState("magic_menu"))
	{
		menu_controller->magic_menu.turnOn();
	}
	if (findState("item_menu"))
	{
		menu_controller->item_menu.turnOn();
	}
}

void BattleMenuStateController::turnOn()
{
	if (findState("on"))
	{
		menu_controller->on = true;
	}
	else
	{
		menu_controller->on = false;
	}
}

void BattleMenuStateController::updateMenus()
{
	if (states.size() > 0)
	{
		updatePlayerMenu();
		updateMagicMenu();
		updateItemMenu();
		updateUnitSelectMenu();
		updateExpScreen();
	}
	else
	{
		menu_controller->cursor->on = false;
	}
}

void BattleMenuStateController::updatePlayerMenu()
{
	if (findState("player_menu"))
	{
		menu_controller->displayPlayerMenu = true;
		menu_controller->displayItemMenu = false;
		menu_controller->displayMagicMenu = false;
		menu_controller->displaySpecialMenu = false;
		menu_controller->cursor->on = true;
		menu_controller->changeFocus("player_menu");
	}
	else
	{
		menu_controller->displayPlayerMenu = false;
		menu_controller->displayItemMenu = false;
		menu_controller->displayMagicMenu = false;
		menu_controller->displaySpecialMenu = false;
	}
}

void BattleMenuStateController::updateMagicMenu()
{
	if (findState("magic_menu"))
	{
		menu_controller->displayItemMenu = false;
		menu_controller->displayMagicMenu = true;
		menu_controller->displaySpecialMenu = false;
		menu_controller->changeFocus("magic_menu");
	}
	else
	{
		menu_controller->displayMagicMenu = false;
	}
}

void BattleMenuStateController::updateItemMenu()
{
	if (findState("item_menu"))
	{
		menu_controller->displayItemMenu = true;
		menu_controller->displayMagicMenu = false;
		menu_controller->displaySpecialMenu = false;
		menu_controller->changeFocus("item_menu");
	}
	else
	{
		menu_controller->displayItemMenu = false;
	}
}

void BattleMenuStateController::updateUnitSelectMenu()
{
	if (findState("unit_select"))
	{
		menu_controller->displayUnitSelection = true;
		this->menu_controller->changeFocus("unit_select");
	}
	else
	{
		menu_controller->displayUnitSelection = false;
	}
}

void BattleMenuStateController::updateExpScreen()
{
	if (findState("exp_menu"))
	{
		menu_controller->changeFocus("exp_menu");
		menu_controller->displayExpMenu = true;
		menu_controller->displayPlayerMenu = false;
		menu_controller->displayItemMenu = false;
		menu_controller->displayMagicMenu = false;
		menu_controller->displaySpecialMenu = false;
		menu_controller->cursor->on = false;
	}
}
