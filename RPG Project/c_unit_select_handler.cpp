#include "c_unit_select_handler.h"
#include "input_handler.h"
#include "game.h"
#include "battle_cursor.h"

void CursorUnitSelectHandler::setup(Game* game, BattleCursor* cursor)
{
	basicSetup(game, cursor);
}

void CursorUnitSelectHandler::update()
{
	updateUnitSelectInput();
}

void CursorUnitSelectHandler::updateUnitSelectInput()
{
	if (input->findKey("right"))
	{
		if (cursor->switch_to_allies == true)
		{
			cursor->index = (cursor->index + 1) % (int)cursor->options.size();
		}
		else if (cursor->switch_to_enemies == true)
		{
			cursor->index = cursor->index - 1;
			if (cursor->index < 0)
			{
				cursor->index = 0;
				cursor->switch_to_allies = true;
				cursor->switch_to_enemies = false;
			}
		}
		cursor->sound_player.playSound(cursor->click);
	}
	if (input->findKey("left"))
	{
		if (cursor->switch_to_allies == true)
		{
			cursor->index = cursor->index - 1;
			if (cursor->index < 0)
			{
				cursor->index = 0;
				cursor->switch_to_allies = false;
				cursor->switch_to_enemies = true;
			}
		}
		else if (cursor->switch_to_enemies == true)
		{
			cursor->index = (cursor->index + 1) % (int)cursor->options.size();
		}
		cursor->sound_player.playSound(cursor->click);
	}
}
