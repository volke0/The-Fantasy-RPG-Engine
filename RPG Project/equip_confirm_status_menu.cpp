#include "equip_confirm_status_menu.h"
#include "menu_cursor.h"
#include "equipment_base.h"
#include "game.h"

void EquipConfirmStatusMenu::setup(Game* game, PlayerStatusMenu* player_status_menu, Unit* user, string slot, Json::Value item, int item_num)
{
	vis_x = 12.5f * TILE + 12.f;
	vis_y = 6.0f * TILE + 20.0f;
	max_vis_index = 15;
	vis_spacing = 25.f;
	box_pos = { 11.5f * TILE + 14.f, 5.f * TILE };
	ConfirmStatusMenu::setup(game, player_status_menu);
	this->user = user;
	this->slot = slot;
	this->item = item;
	this->item_num = item_num;
	FindMemberId();
	SetupConfirmMessage();
	confirm_pos = { box_pos.x + 12.f, box_pos.y + 12.f };
	fontSize = 20.f;
}

void EquipConfirmStatusMenu::FindMemberId()
{
	Unit* equipped_unit = party->item_inventory.GetEquippedUnit(item_num);
	if (equipped_unit)
	{
		string temp_member_id = "";
		for (auto& item_id : equipped_unit->equipped_items)
		{
			temp_member_id = equipped_unit->name + "_" + to_string(item_id);
			if (temp_member_id == item["equip_id"].asString())
			{
				member_id = temp_member_id;
				if (equipped_unit == user)
				{
					member_inv_id = item_id;
					bEquippedUnit = true;
				}
				break;
			}
		}
	}
}

void EquipConfirmStatusMenu::SetupConfirmMessage()
{
	Unit* equipped_unit = party->item_inventory.GetEquippedUnit(item_num);
	confirm_message = "Are you sure you~want to equip?";
	if (bEquippedUnit)
	{
		if (equipped_unit->equip_system.GetEquipment(member_inv_id) != nullptr)
		{
			if (equipped_unit->equip_system.GetEquipment(member_inv_id)->slot == slot)
			{
				confirm_message = "Are you sure you~want to unequip?";
			}
		}
	}
}


void EquipConfirmStatusMenu::update()
{
	ConfirmStatusMenu::update();
	if (cursor->return_input)
	{
		cursor->return_input = false;
		if (cursor->selected_option == "Yes")
		{
			Unit* equipped_unit = party->item_inventory.GetEquippedUnit(item_num);
			if (equipped_unit == user)
			{
				if (user->equip_system.GetEquipment(member_inv_id)->slot == slot)
				{
					user->equip_system.UnEquip(slot);
				}
				else
				{
					equipped_unit->equip_system.UnEquip(equipped_unit->equip_system.GetEquipment(member_inv_id)->slot);
					if (user->equip_system.GetEquipment(slot) != nullptr)
					{
						user->equip_system.UnEquip(slot);
					}
					user->equip_system.Equip(slot, item["item"], item_num);
				}
			}
			else
			{
				if (equipped_unit != nullptr)
				{
					auto& equip = equipped_unit->equip_system.GetEquipment(item_num);
					equipped_unit->equip_system.UnEquip(equip->slot);
				}
				if (user->equip_system.GetEquipment(slot) != nullptr)
				{
					user->equip_system.UnEquip(slot);
				}
				user->equip_system.Equip(slot, item["item"], item_num);
			}
			sound_player.playSound("click");
			scene_manager->pop(true, 2);
		}
		if (cursor->selected_option == "No")
		{
			sound_player.playSound("click2");
			scene_manager->pop(true);
		}
	}
}

void EquipConfirmStatusMenu::draw()
{
	ConfirmStatusMenu::draw();
}


