#include "cursor_handler.h"
#include "battle_cursor.h"
#include "input_handler.h"
#include "game.h"

void CursorHandler::setup(Game* game, BattleCursor* cursor)
{
	basicSetup(game, cursor);
}

void CursorHandler::update()
{
	refreshMaxIndex();
	updateInput();
	//seeAllOptions();
}

void CursorHandler::refreshMaxIndex()
{
	cursor->max_index = (int)cursor->options.size() - 1;
}

void CursorHandler::updateInput()
{
	cursor->move_down = false;
	cursor->move_up = false;
	if (input->findKey("up"))
	{
		if (cursor->index == 0)
		{
			cursor->index = (int)cursor->options.size() - 1;
			cursor->vis_index = cursor->max_vis_index;
		}
		else
		{
			cursor->index = (cursor->index - 1) % (int)cursor->options.size();
			cursor->vis_index -= 1;
		}
		cursor->move_up = true;
		cursor->sound_player.playSound(cursor->click);
	}
	if (input->findKey("down"))
	{
		cursor->index = (cursor->index + 1) % (int)cursor->options.size();
		cursor->vis_index += 1;
		cursor->sound_player.playSound(cursor->click);
		cursor->move_down = true;
	}
	sendInput();
	updateVisibleIndex();
}

void CursorHandler::updateVisibleIndex()
{
	if (cursor->index > cursor->max_vis_index)
	{
		if (cursor->vis_index > cursor->max_vis_index)
		{
			cursor->vis_index = cursor->max_vis_index;
		}
	}
	if (cursor->vis_index < 0)
	{
		cursor->vis_index += 1;
	}

	if (cursor->index <= 0)
	{
		cursor->vis_index = 0;
	}
}

void CursorHandler::sendInput()
{
	if (input->findKey("x"))
	{
		if (cursor->return_input == false)
		{
			cursor->return_input = true;
			cursor->selected_option = cursor->options[cursor->index];
		}
	}
	if (input->findKey("z"))
	{
		cursor->go_back = true;
	}
}

void CursorHandler::seeAllOptions()
{
	for (string option : cursor->options)
	{
		if (option == "")
		{
			option = "...";
		}
		cout << option << endl;
	}
}
