#include "item_inventory.h"
#include "consumable.h"
#include "party_base.h"

void ItemInventory::setup(Game* game, PartyBase* party)
{
	basicSetup(game);
	this->party = party;
}

void ItemInventory::SetEquippedUnit(int item_num, Unit* unit)
{
	string id = unit->name + "_" + to_string(item_num);
	inventory[item_num]["equipped"] = true;
	inventory[item_num]["equip_id"] = id;
	inventory[item_num]["unit_id"] = unit->id;
	for (auto& member : party->party)
	{
		if (member.get() == unit)
		{
			member->equipped_items.push_back(item_num);
		}
	}
}

void ItemInventory::RemoveEquippedUnit(int item_num)
{
	string member_id = "";
	for (auto& member : party->party)
	{
		if (find(member->equipped_items.begin(), member->equipped_items.end(), item_num) != member->equipped_items.end())
		{
			member->equipped_items.erase(find(member->equipped_items.begin(), member->equipped_items.end(), item_num));
			member->equipped_items.shrink_to_fit();
			member_id = member->name + "_" + to_string(item_num);
		}
	}
	for (auto& item : inventory)
	{
		string id = item["equip_id"].asString();
		if (id == member_id)
		{
			item["equipped"] = false;
			item["equip_id"] = "";
			item["unit_id"] = -1;
		}
	}
}

Unit* ItemInventory::GetEquippedUnit(int item_num)
{
	string id = inventory[item_num]["equip_id"].asString();
	for (auto& member : party->party)
	{
		for (auto& item : member->equipped_items)
		{
			string member_id = member->name + "_" + to_string(item);
			if (member_id == id)
			{
				return member.get();
			}
		}
	}
	return nullptr;
}
