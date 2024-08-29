#include "basic_menu_cursor_handler.h"
#include "game.h"
#include "menu_cursor.h"

void BasicMenuCursorHandler::basicSetup(Game* game, MenuCursor* cursor)
{
	this->game = game;
	this->cursor = cursor;
	this->input = cursor->input;
}

void BasicMenuCursorHandler::update()
{

}
