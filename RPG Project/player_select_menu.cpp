#include "player_select_menu.h"
#include "game.h"
#include "party.h"
#include "unit.h"
#include "ability_status_menu.h"
#include "equip_status_menu.h"
#include "status_info_menu.h"
#include "menu_cursor.h"

void PlayerSelectMenu::setup(Game* game, PlayerStatusMenu* player_status_menu, string menu_to_push)
{
	ApplyAbilityMenu::setup(game, player_status_menu);
	this->menu_to_push = menu_to_push;
	player_menu_pos = scene_manager->getLastScenePos() + 1;
}

void PlayerSelectMenu::update()
{
	ApplyAbilityMenu::update();
	if (cursor->go_back)
	{
		sound_player.playSound("click2");
		scene_manager->pop(true);
		cursor->go_back = false;
	}
	if (cursor->return_input)
	{
		cursor->return_input = false;
		if (!party->party[stoi(cursor->selected_option)]->dead)
		{
			sound_player.playSound("click");
			loadScene();
		}
		else
		{
			sound_player.playSound("cantclick");
		}
	}
}

void PlayerSelectMenu::draw()
{
	ApplyAbilityMenu::draw();
}


void PlayerSelectMenu::loadScene()
{
	if (menu_to_push == "ability_status_menu")
	{
		shared_ptr<AbilityStatusMenu> ability_menu = make_shared<AbilityStatusMenu>();
		ability_menu->setup(game, player_status_menu);
		ability_menu->unit_num = stoi(cursor->selected_option);
		PushMenu(ability_menu);
	}
	if (menu_to_push == "equipment_status_menu")
	{
		shared_ptr<EquipStatusMenu> equip_menu = make_shared<EquipStatusMenu>();
		equip_menu->setup(game, player_status_menu);
		equip_menu->unit_num = stoi(cursor->selected_option);
		PushMenu(equip_menu);
	}
	if (menu_to_push == "status_info_menu")
	{
		shared_ptr<StatusInfoMenu> status_menu = make_shared<StatusInfoMenu>();
		status_menu->setup(game, player_status_menu, stoi(cursor->selected_option));
		PushMenu(status_menu);
	}
}
