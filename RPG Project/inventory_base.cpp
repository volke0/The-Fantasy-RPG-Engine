#include "inventory_base.h"
#include "game.h"

void InventoryBase::basicSetup(Game* game)
{
	this->game = game;
}

bool InventoryBase::checkIfExists(string obj_name, int& inv_index, vector<int> indices_to_skip)
{
	bool bSkipIndexActive = false;
	if (indices_to_skip.size() > 0)
	{
		bSkipIndexActive = true;
	}
	int index = 0;
	for (auto& item : inventory)
	{
		string item_name = item["name"].asString();
		if (item_name == obj_name)
		{
			if (bSkipIndexActive)
			{
				inv_index = index;
				return true;
			}
			else
			{
				if (find(indices_to_skip.begin(), indices_to_skip.end(), index) != indices_to_skip.end())
				{
					inv_index = index;
					return true;
				}

				if (indices_to_skip.size() == 0)
				{
					inv_index = index;
					return true;
				}
			}
		}
		index++;
	}
	return false;
}

bool InventoryBase::checkIfExists(string obj_name)
{
	for (auto& item : inventory)
	{
		if (item["name"].asString() == obj_name)
		{
			return true;
		}
	}
	return false;
}

bool InventoryBase::checkIfExists(int inv_index)
{
	int count = 0;
	for (auto& item : inventory)
	{
		if (count == inv_index)
		{
			return true;
		}
		count++;
	}
	return false;
}

int InventoryBase::checkAmount(string obj_name)
{
	int inv_index = 0;
	if (checkIfExists(obj_name, inv_index))
	{
		return inventory[inv_index]["amount"].asInt();
	}
	return 0;
}

string InventoryBase::getItemName(string obj_name)
{
	Json::Value obj = game->item_database.item_scripts[obj_name];
	string name = obj["name"].asString();

	return name;
}

int InventoryBase::getItemID(string obj_name)
{
	int index = 0;
	int item_index = 0;
	for (auto& item : inventory)
	{
		string inv_item_name = item["name"].asString();
		if (inv_item_name == obj_name)
		{
			item_index = index;
			break;
		}
		index++;
	}
	return item_index;
}

void InventoryBase::addToInventory(Json::Value obj, int amount)
{
	int inv_index = 0;
	if (checkIfExists(obj["name"].asString(), inv_index))
	{
		if (obj["stackable"].asBool() == true)
		{
			int item_amt = inventory[inv_index]["amount"].asInt();
			item_amt += amount;
			inventory[inv_index]["amount"] = item_amt;
		}
		else
		{
			for (int i = amount; i > 0; i--)
			{
				makeAddItem(obj, 1);
			}
		}
	}
	else
	{
		if (obj["stackable"].asBool() == true)
		{
			makeAddItem(obj, amount);
		}
		else
		{
			for (int i = amount; i > 0; i--)
			{
				makeAddItem(obj, 1);
			}
		}
	}
	
}

void InventoryBase::subFromInventory(int inv_index, int amount)
{
	if (checkIfExists(inv_index))
	{
		int item_amt = inventory[inv_index]["amount"].asInt();
		item_amt -= amount;
		if (item_amt <= 0)
		{
			inventory.erase(inventory.begin() + inv_index);
			/*cout << "ERASED!" << endl;*/
		}
		else
		{
			inventory[inv_index]["amount"] = item_amt;
			/*cout << "NEW AMOUNT: " << inventory[inv_index]["amount"].asInt() << endl;*/
		}
	}
}

void InventoryBase::subFromInventory(string obj_name, int amount, vector<int> indices_to_skip)
{
	int inv_index = 0;
	if (checkIfExists(obj_name, inv_index, indices_to_skip))
	{
		int item_amt = inventory[inv_index]["amount"].asInt();
		item_amt -= amount;
		if (item_amt <= 0)
		{
			inventory.erase(inventory.begin() + inv_index);
			/*cout << "ERASED!" << endl;*/
		}
		else
		{
			inventory[inv_index]["amount"] = item_amt;
			/*cout << "NEW AMOUNT: " << inventory[inv_index]["amount"].asInt() << endl;*/
		}
	}
}

void InventoryBase::makeAddItem(Json::Value obj, int amount)
{
	Json::Value temp_obj;
	temp_obj["name"] = obj["name"];
	temp_obj["type"] = obj["type"];
	temp_obj["amount"] = amount;
	temp_obj["item"] = obj;
	temp_obj["predicted_usage"] = amount;  //Used only for AI predictions at the moment
	temp_obj["unit_id"] = -1;
	inventory.push_back(temp_obj);
}
