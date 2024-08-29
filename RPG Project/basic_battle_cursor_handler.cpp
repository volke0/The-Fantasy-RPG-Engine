#include "basic_battle_cursor_handler.h"
#include "battle_cursor.h"

void BasicBattleCursorHandler::basicSetup(Game* game, BattleCursor* cursor)
{
	this->game = game;
	this->cursor = cursor;
	this->input = cursor->input;
}

void BasicBattleCursorHandler::update()
{

}
