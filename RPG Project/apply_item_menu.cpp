#include "apply_item_menu.h"
#include "game.h"
#include "party.h"
#include "menu_cursor.h"
#include "unit.h"
#include "consumable.h"
#include "item_status_menu.h"

void ApplyItemMenu::setup(Game* game, PlayerStatusMenu* player_status_menu, int item_pos)
{
	ApplyAbilityMenu::setup(game, player_status_menu);
	player_menu_pos = scene_manager->getLastScenePos() - 1;
	this->item_pos = item_pos;
}

void ApplyItemMenu::update()
{
	ApplyAbilityMenu::update();
	if (cursor->go_back)
	{
		sound_player.playSound("click2");
		cursor->go_back = false;
		popToItemSelectMenu();
	}
	if (cursor->return_input)
	{
		cursor->return_input = false;

		auto& unit = party->party[stoi(cursor->selected_option)];
		auto& item_script = party->getItem(item_pos)["item"];
		auto& item_inventory = party->item_inventory;
		string item_name = party->getItem(item_pos)["name"].asString();

		Consumable item;
		item.setup(game, unit.get(), unit.get(), item_script);

		if (item.CheckIfEffectApplicable())
		{
			sound_player.playSound("healing_sound2");
			item.CheckAndApplyEffect();
			if (item_inventory.checkAmount(item.ingame_name) == 1)
			{
				item_inventory.subFromInventory(item.ingame_name);
				popToItemSelectMenu();
			}
			else
			{
				item_inventory.subFromInventory(item.ingame_name);
			}
		}
		else
		{
			sound_player.playSound("cantclick");
		}
	}
}

void ApplyItemMenu::draw()
{
	ApplyAbilityMenu::draw();
}


void ApplyItemMenu::applyItemEffect(Unit* unit, Consumable* item)
{
	
}

void ApplyItemMenu::popToItemSelectMenu()
{
	static_cast<ItemStatusMenu*>(scene_manager->getScene(scene_manager->getLastScenePos() - 1).get())->makeOptions();
	scene_manager->pop(false, 2);
}
