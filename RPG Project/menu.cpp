#include "menu.h"
#include "battle_controller.h"
#include "game.h"

void Menu::basicSetup(Game* game, Party* party, BattleController* battle_controller, BattleMenuController* menu_controller)
{
	this->game = game;
	this->party = party;
	this->menu_controller = menu_controller;
	this->battle_controller = battle_controller;
	this->cursor = &battle_controller->cursor;
}

void Menu::update()
{
}

void Menu::draw()
{
}

void Menu::drawText(Vector2 pos, string text, float fontSize, Color color)
{
	Vector2 drop_shadow = { pos.x + dropshadow_offset, pos.y + dropshadow_offset };
	DrawTextEx(game->GFX.font, text.c_str(), drop_shadow, fontSize, 0.0f, BLACK);
	DrawTextEx(game->GFX.font, text.c_str(), pos, fontSize, 0.0f, color);
}
