#include "menu_magic_cursor_handler.h"
#include "input_handler.h"
#include "menu_cursor.h"
#include "game.h"

void MenuMagicCursorHandler::setup(Game* game, MenuCursor* cursor)
{
	basicSetup(game, cursor);
}

void MenuMagicCursorHandler::update()
{
	bool vertical_click = false;
	bool horizontal_click = false;
	bool is_odd = false;
	refreshMaxIndex();
	if (game->scene_manager.busy == false)
	{
		updateInput(vertical_click, horizontal_click, is_odd);
	}
	updateVisibleIndex(vertical_click, horizontal_click, is_odd);
}

void MenuMagicCursorHandler::updateInput(bool& vertical_click, bool& horizontal_click, bool& is_odd)
{
	cursor->move_down = false;
	cursor->move_up = false;
	cursor->pressing = false;
	if (input->findKey("up"))
	{
		cursor->index = (cursor->index - 2) % (int)cursor->options.size();
		if (cursor->index < 0)
		{
			if (cursor->index % 2 == 0)
			{
				cursor->index = (int)cursor->options.size() - 1;
			}
			else
			{
				cursor->index = (int)cursor->options.size() - 2;
			}
		}
		cursor->move_up = true;
		cursor->pressing = true;
		vertical_click = true;
	}
	else if (input->findKey("down"))
	{
		cursor->index = cursor->index + 2;
		if (cursor->index > (int)cursor->options.size() - 1)
		{
			if (cursor->index % 2 == 0)
			{
				cursor->index = 0;
			}
			else
			{
				cursor->index = 1;
			}
		}
		cursor->move_down = true;
		cursor->pressing = true;
		vertical_click = true;
	}
	else if (input->findKey("right") || input->findKey("left"))
	{
		if (cursor->index % 2 == 0)
		{
			if (cursor->index + 1 < (int)cursor->options.size())
			{
				cursor->index = (cursor->index + 1) % (int)cursor->options.size();
				flip = true;
			}
		}
		else
		{
			cursor->index = (cursor->index - 1) % (int)cursor->options.size();
			flip = false;
		}
		cursor->pressing = true;
		horizontal_click = true;
	}
	sendInput();
}

void MenuMagicCursorHandler::refreshMaxIndex()
{
	cursor->max_index = (int)cursor->options.size() - 1;
}

void MenuMagicCursorHandler::updateVisibleIndex(bool& vertical_click, bool& horizontal_click, bool& is_odd)
{
	if (cursor->index % 2 != 0)
	{
		is_odd = true;
	}
	if (vertical_click == true)
	{
		if (cursor->move_up == true)
		{
			moveUp();
		}
		else if (cursor->move_down == true)
		{
			moveDown();
		}
		//setCursor();
	}
	if (horizontal_click == true)
	{
		if (flip == true)
		{
			cursor->x_factor = (float)TILE * 5;
		}
		else
		{
			cursor->x_factor = 0;
		}
	}
	if (vertical_click == true || horizontal_click == true)
	{
		cursor->sound_player.playSound(cursor->click);
	}
}

void MenuMagicCursorHandler::sendInput()
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

void MenuMagicCursorHandler::moveDown()
{
	cursor->vis_index += 2;
	if (cursor->vis_index > cursor->max_vis_index)
	{
		cursor->vis_index -= 2;
	}
	if (cursor->index >= (int)cursor->options.size() - 1)
	{
		cursor->vis_index = cursor->max_vis_index;
	}
	if (cursor->index <= 1)
	{
		cursor->vis_index = 0;
	}
}

void MenuMagicCursorHandler::moveUp()
{
	cursor->vis_index -= 2;
	if (cursor->vis_index < 0)
	{
		cursor->vis_index += 2;
	}
	if (cursor->index >= cursor->options.size() - 2)
	{
		int num = cursor->max_vis_index;
		if (cursor->max_index != cursor->index)
		{
			num -= 2;
		}
		cursor->vis_index = num;
	}
}
