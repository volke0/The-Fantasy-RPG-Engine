#include "formation_status_menu.h"
#include "menu_cursor.h"
#include "game.h"
#include "unit.h"
#include "player_status_menu.h"

void FormationStatusMenu::setup(Game* game, PlayerStatusMenu* player_status_menu)
{
	ApplyAbilityMenu::setup(game, player_status_menu);
	player_menu_pos = scene_manager->getLastScenePos() + 1;
}

void FormationStatusMenu::update()
{
	ApplyAbilityMenu::update();
	if (cursor->go_back)
	{
		sound_player.playSound("click2");
		if (!selectedFirstUnit)
		{
			scene_manager->pop(true);
		}
		else
		{
			selectedFirstUnit = false;
			cursor->index = unit_num_1;
			cursor->vis_index = unit_num_1;
		}
		cursor->go_back = false;
	}
	if (cursor->return_input)
	{
		cursor->return_input = false;
		swapPartyMembers();
		sound_player.playSound("click");
	}
	visuallyUpdateCursor();
}

void FormationStatusMenu::draw()
{
	ApplyAbilityMenu::draw();
	if (selectedFirstUnit)
	{
		if (flickerOn)
		{
			float x = cursor->dest.x + cursor->x_factor;
			float y = cursor->dest.y + (float)(vis_spacing * GetUnitNumIndex(unit_num_1));
			Rectangle new_dest = { x, y, cursor->dest.width, cursor->dest.height };
			DrawTexturePro(cursor->cursor, cursor->src, new_dest, Vector2{ 0, 0 }, 0.0f, WHITE);
			flickerOn = false;
		}
		else
		{
			flickerOn = true;
		}
	}
}

void FormationStatusMenu::swapPartyMembers()
{
	if (!selectedFirstUnit)
	{
		unit_num_1 = stoi(cursor->selected_option);
		selectedFirstUnit = true;
	}
	else
	{
		party->formation.SwapMembers(unit_num_1, stoi(cursor->selected_option));
		swap(player_status_menu->faces[GetUnitNumIndex(unit_num_1)], player_status_menu->faces[(GetUnitNumIndex(stoi(cursor->selected_option)))]);
		selectedFirstUnit = false;
		scene_manager->pop(true);
	}
}

void FormationStatusMenu::visuallyUpdateCursor()
{
	if (selectedFirstUnit)
	{
		if (cursor->index == GetUnitNumIndex(unit_num_1))
		{
			cursor->index++;
			cursor->vis_index++;
			if (cursor->index > (int)options.size() - 1)
			{
				cursor->index = 0;
				cursor->vis_index = 0;
			}
		}
	}
}
