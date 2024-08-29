#include "menu_cursor_handler.h"
#include "input_handler.h"
#include "menu_cursor.h"
#include "game.h"

void MenuCursorHandler::setup(Game* game, MenuCursor* cursor)
{
	basicSetup(game, cursor);
}

void MenuCursorHandler::update()
{
	refreshMaxIndex();
	if (game->scene_manager.busy == false)
	{
		updateInput();
	}
	updateVisibleIndex();
	//seeAllOptions();
}

void MenuCursorHandler::refreshMaxIndex()
{
	cursor->max_index = (int)cursor->options.size() - 1;
}

void MenuCursorHandler::updateInput()
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
}

void MenuCursorHandler::updateVisibleIndex()
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

void MenuCursorHandler::sendInput()
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

void MenuCursorHandler::seeAllOptions()
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
