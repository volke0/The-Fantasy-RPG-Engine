#include "sub_menu.h"
#include "menu_cursor.h"
#include "game.h"
#include "party.h"

void SubMenu::setup(Game* game)
{
	SceneMenu::setup(game);
	type = "menu";
}

void SubMenu::draw()
{
	DrawOptions();
}

void SubMenu::update()
{
	if (first_loaded)
	{
		this->cursor->index = 0;
		this->cursor->vis_index = 0;
		first_loaded = false;
	}
	SetCursor(*cursor, options, vis_x, vis_y, vis_spacing, max_vis_index, horizontal);
	UpdateCursor(*cursor);
}

int SubMenu::GetUnitNumIndex(int unit_num)
{
	int index = 0;
	for (auto& unit_pos : party->visible_party)
	{
		if (unit_num == unit_pos)
		{
			break;
		}
		index++;
	}
	return index;
}

void SubMenu::SetUnitNum(bool bIncrementUp)
{
	int index = 0;
	for (auto& unit_pos : party->visible_party)
	{
		if (unit_num == unit_pos)
		{
			break;
		}
		index++;
	}

	if (bIncrementUp)
	{
		index++;
	}
	else
	{
		index--;
	}

	if (index > (int)party->visible_party.size() - 1)
	{
		index = 0;
	}
	if (index < 0)
	{
		index = (int)party->visible_party.size() - 1;
	}
	unit_num = party->visible_party[index];
}


void SubMenu::DrawOptions()
{
	int num = 0;
	vector<string> vis_options = MakeVisibleOptions();
	for (auto& option : vis_options)
	{
		Vector2 pos = { vis_option_pos.x, vis_option_pos.y + ((float)num * vis_spacing) };
		drawText(pos, option, font_size, option_color);
		num++;
	}
}

vector<string> SubMenu::MakeVisibleOptions()
{
	vector<string> temp_options;
	int max_index = vis_index + max_vis_index + 1;

	if (max_index > (int)options.size())
	{
		max_index = (int)options.size();
	}
	for (int i = vis_index; i < max_index; i++)
	{
		temp_options.push_back(options[i]);
	}
	return temp_options;
}
