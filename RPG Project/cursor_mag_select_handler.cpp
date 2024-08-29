#include "cursor_mag_select_handler.h"
#include "input_handler.h"
#include "battle_cursor.h"
#include "game.h"

void CursorMagicSelectHandler::setup(Game* game, BattleCursor* cursor)
{
	basicSetup(game, cursor);
}

void CursorMagicSelectHandler::update()
{
	updateMagicSelectInput();
}

void CursorMagicSelectHandler::turnOn()
{
	cursor->index = cursor->memory_index;
	if (cursor->index % 2 != 0)
	{
		flip = true;
	}
	setCursor();
}

void CursorMagicSelectHandler::updateMagicSelectInput()
{
	bool vertical_click = false;
	bool horizontal_click = false;
	bool is_odd = false;
	cursor->move_down = false;
	cursor->move_up = false;
	updateInput(vertical_click, horizontal_click, is_odd);
	fixCursorVisuals(vertical_click, horizontal_click, is_odd);
}

void CursorMagicSelectHandler::updateInput(bool& vertical_click, bool& horizontal_click, bool& is_odd)
{
	cursor->pressing = false;
	if (input->findKey("x"))
	{
		cursor->return_input = true;
	}
	if (input->findKey("up"))
	{
		cursor->index = (cursor->index - 2) % (int)cursor->options.size();

		if (cursor->index < 0)
		{
			int index_base = (int)cursor->options.size();
			int correction_factor = cursor->index;

			if (index_base % 2 != 0)
			{
				correction_factor % 2 == 0 ? correction_factor = -1 : correction_factor = -2;
			}
			cursor->index = index_base + correction_factor;
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
	else if (input->findKey("z"))
	{
		cursor->go_back = true;
	}
}

void CursorMagicSelectHandler::fixCursorVisuals(bool& vertical_click, bool& horizontal_click, bool& is_odd)
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
		setCursor();
	}

	if (flip == true)
	{
		cursor->x_factor = (float)TILE * 5;
	}
	else
	{
		cursor->x_factor = 0;
	}
	if (vertical_click == true || horizontal_click == true)
	{
		cursor->sound_player.playSound(cursor->click);
	}
}

void CursorMagicSelectHandler::setCursor()
{
	cursor->y_factor = (float)(30 * (visual_index));
}

void CursorMagicSelectHandler::moveDown()
{
	visual_index += 2;
	if (visual_index > 5)
	{
		visual_index -= 2;
	}
	if (cursor->index >= (int)cursor->options.size() - 2)
	{
		visual_index = 4;
	}
	if (cursor->index <= 1)
	{
		visual_index = 0;
	}
}

void CursorMagicSelectHandler::moveUp()
{
	visual_index -= 2;
	if (visual_index < 0)
	{
		visual_index += 2;
	}
	if (cursor->index >= cursor->options.size() - 2)
	{
		visual_index = 4;
	}
}
