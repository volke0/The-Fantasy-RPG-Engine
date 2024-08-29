#include "menu_cursor_manager.h"
#include "menu_cursor.h"
#include "input_handler.h"
#include "cursor.h"

void MenuCursorManager::setup(Game* game, MenuCursor* cursor)
{
	basicSetup(game);
	this->cursor = cursor;
	setupAllHandlers();
}

void MenuCursorManager::setupAllHandlers()
{
	general_handler.setup(game, cursor);
	magic_handler.setup(game, cursor);
}

void MenuCursorManager::update()
{
	if (cursor->magic_mode)
	{
		magic_handler.update();
	}
	else
	{
		general_handler.update();
	}
}