#include "item_confirm_status_menu.h"
#include "menu_cursor.h"
#include "game.h"
#include "party.h"
#include "player_status_menu.h"
#include "apply_item_menu.h"

void ItemConfirmStatusMenu::setup(Game* game, PlayerStatusMenu* player_status_menu)
{
	vis_x = 12.5f * TILE + 12.f;
	vis_y = 6.0f * TILE + 20.0f;
	max_vis_index = 15;
	vis_spacing = 25.f;
	box_pos = { 11.5f * TILE + 14.f, 5.f * TILE };
	ConfirmStatusMenu::setup(game, player_status_menu);

	string item_name = party->getItem(item_pos)["name"].asString();
	confirm_message = "Do you want to use~" + item_name + "?";
	confirm_pos = { box_pos.x + 12.f, box_pos.y + 12.f };
}

void ItemConfirmStatusMenu::update()
{
	ConfirmStatusMenu::update();
	if (cursor->return_input)
	{
		cursor->return_input = false;
		if (cursor->selected_option == "Yes")
		{
			sound_player.playSound("click");
			shared_ptr<ApplyItemMenu> apply_item_menu = make_shared<ApplyItemMenu>();
			apply_item_menu->setup(game, player_status_menu, item_pos);
			PushMenu(apply_item_menu);
		}
		if (cursor->selected_option == "No")
		{
			sound_player.playSound("click2");
			scene_manager->pop(true);
		}
	}
}

void ItemConfirmStatusMenu::draw()
{
	ConfirmStatusMenu::draw();
}

void ItemConfirmStatusMenu::setItem(int item_pos)
{
	this->item_pos = item_pos;
}
