#include "scene_menu.h"
#include "game.h"
#include "menu_cursor.h"

void SceneMenu::setup(Game* game)
{
	Scene::setup(game);
	input->changeToMenuMode();
	setupParty(game);
	type = "scene_menu";
}

void SceneMenu::draw()
{
}

void SceneMenu::update()
{
}

void SceneMenu::setupParty(Game* game)
{
	this->party = &game->party;
}

void SceneMenu::GetPreviousCursorPosition(MenuCursor& cursor)
{
	if (pushedScene)
	{
		if (memory_index > cursor.max_index)
		{
			memory_index = cursor.max_index;
			memory_cursor_vis_index = cursor.max_vis_index;
		}
		cursor.index = memory_index;
		vis_index = memory_vis_index;
		cursor.vis_index = memory_cursor_vis_index;
	}
}


void SceneMenu::SetCursor(MenuCursor& cursor, vector<string> options, float vis_x, float vis_y, float vis_spacing, int max_vis_index, bool horizontal)
{
	vis_option_pos = { vis_x, vis_y };
	this->vis_x = vis_x;
	this->vis_y = vis_y;
	this->vis_spacing = vis_spacing;
	this->max_vis_index = max_vis_index;
	cursor.setCursor(vis_option_pos, options, horizontal, max_vis_index);
	cursor.changeVisSpacing(vis_spacing);
}

void SceneMenu::RememberCursorPosition(MenuCursor& cursor)
{
	memory_index = cursor.index;
	memory_vis_index = vis_index;
	memory_cursor_vis_index = cursor.vis_index;
}

void SceneMenu::PushMenu(shared_ptr<Scene> scene, bool isInstant, int speed, float stall_time)
{
	pushedScene = true;
	scene_manager->push(scene, isInstant, speed, stall_time);
}

void SceneMenu::PopMenu(MenuCursor& cursor, bool isInstant, int pop_amt, int speed, float stall_time)
{
	RememberCursorPosition(cursor);
	scene_manager->pop(isInstant, pop_amt, speed, stall_time);
}

void SceneMenu::SetVisIndexToTheMax(MenuCursor& cursor)
{
	cursor.max_index = (int)cursor.options.size() - 1;
	if (cursor.index >= cursor.max_index)
	{
		if ((int)cursor.options.size() - 1 >= cursor.max_vis_index)
		{
			int num = ((int)cursor.options.size() - 1) - cursor.max_vis_index;
			vis_index = num;
		}
	}
}

void SceneMenu::UpdateCursor(MenuCursor& cursor)
{
	cursor.update();
	party->update();
	SetVisIndexToTheMax(cursor);
	if (!game->scene_manager.busy)
	{
		if (!pushedScene)
		{
			RememberCursorPosition(cursor);
		}
		GetPreviousCursorPosition(cursor);
		pushedScene = false;
		updateVisibleOptions(cursor);
		sound_player.update();
	}
}

void SceneMenu::updateVisibleOptions(MenuCursor& cursor)
{
	if (cursor.move_down == true)
	{
		if (cursor.index > cursor.max_vis_index + vis_index)
		{
			if (cursor.vis_index >= cursor.max_vis_index)
			{
				vis_index += 1;
			}
		}
		if (cursor.index <= 0)
		{
			vis_index = 0;
		}
	}
	else if (cursor.move_up == true)
	{
		if (cursor.index < vis_index)
		{
			if (cursor.vis_index <= 0)
			{
				vis_index -= 1;
			}
		}
		SetVisIndexToTheMax(cursor);
		if (vis_index < 0)
		{
			vis_index = 0;
		}
	}
}

void SceneMenu::drawBackground()
{

}
