#include "battle_cursor_manager.h"
#include "battle_cursor.h"
#include "cursor.h"

void BattleCursorManager::setup(Game* game, BattleCursor* cursor)
{
	basicSetup(game);
	this->cursor = cursor;
	setupAllHandlers();
}

void BattleCursorManager::setupAllHandlers()
{
	general_handler.setup(game, cursor);
	unit_select_handler.setup(game, cursor);
	mag_select_handler.setup(game, cursor);
}


void BattleCursorManager::update()
{
	if (cursor->unit_mode == true)
	{
		unit_select_handler.update();
		general_handler.update();
	}
	else
	{
		if (cursor->magic_mode == true)
		{
			mag_select_handler.update();
		}
		else
		{
			general_handler.update();
		}
	}
}


