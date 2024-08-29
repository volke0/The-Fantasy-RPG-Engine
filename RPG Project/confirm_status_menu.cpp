#include "confirm_status_menu.h"
#include "game.h"
#include "unit.h"
#include "menu_cursor.h"
#include "party.h"

void ConfirmStatusMenu::setup(Game* game, PlayerStatusMenu* player_status_menu)
{
	basicSetup(game);
	setupParty(game);
	setupPlayerStatusMenuVariables(player_status_menu);
	cursor->index = 0;
	cursor->vis_index = 0;
	SetCursor(*cursor, options, vis_x, vis_y, vis_spacing, max_vis_index);
}

void ConfirmStatusMenu::update()
{
	SetCursor(*cursor, options, vis_x, vis_y, vis_spacing, max_vis_index);
	UpdateCursor(*cursor);
	if (scene_manager->busy == false)
	{
		if (cursor->go_back)
		{
			cursor->go_back = false;
			sound_player.playSound("click2");
			scene_manager->pop(true);
		}
	}
}

void ConfirmStatusMenu::draw()
{
	scene_manager->getLastScene()->draw();
	auto& item_select_box = game->GFX.UI["item_status_yes_or_no"];
	Rectangle select_source_rect = { 0, 0, (float)(item_select_box.width), (float)(item_select_box.height) };
	Rectangle select_dest_rect = { box_pos.x, box_pos.y, (float)select_source_rect.width * SCALE, (float)select_source_rect.height * SCALE };

	DrawTexturePro(item_select_box, select_source_rect, select_dest_rect, Vector2{ 0, 0 }, 0.0f, WHITE);
	drawOptions();
	drawConfirmMessage();
}

void ConfirmStatusMenu::drawConfirmMessage()
{
	vector<string> confirm_msg;
	boost::split(confirm_msg, confirm_message, boost::is_any_of("~"));
	int index = 0;
	for (auto& text : confirm_msg)
	{
		Vector2 pos = { confirm_pos.x, confirm_pos.y + (index * 23.f) };
		drawText(pos, text, fontSize);
		index++;
	}
}

void ConfirmStatusMenu::drawOptions()
{
	int index = 0;
	for (auto& option : options)
	{
		Vector2 pos = { vis_option_pos.x , vis_option_pos.y + (index) * (vis_spacing) };
		drawText(pos, option, 23.f);
		index++;
	}
	if (!pushedScene)
	{
		cursor->draw();
	}
}
