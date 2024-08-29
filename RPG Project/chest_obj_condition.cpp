#include "chest_obj_condition.h"
#include "party.h"
#include "game.h"
#include "level.h"

void ChestObjCondition::setup(Json::Value data, Level* level, EventController* event_controller)
{
	MapObjectCondition::setup(data, level, event_controller);
	parseData();
}

void ChestObjCondition::parseData()
{
	MapObjectCondition::parseData();
	if (data.isMember("Contents"))
	{
		contents = data["Contents"];
	}
}



void ChestObjCondition::update()
{
	updateAreaRect();
	if (collision_handler->checkIfUnitFacesArea(party, area_rect))
	{
		if (game->input.findKey("x") && !party->running)
		{
			updateChest();
		}
	}
	if (event_link != "" && trigger_event)
	{
		RunEvent();
	}
}

void ChestObjCondition::draw()
{
}


void ChestObjCondition::updateChest()
{
	if (!level->isTileChangeActive(tile_change_name))
	{
		level->setTileChange(tile_change_name);
		if (sfx_name != "")
		{
			level->sound_player.playSound(sfx_name);
		}
		GiveChestContents();
		if (event_link != "")
		{
			trigger_event = true;
		}
	}
}

void ChestObjCondition::GiveChestContents()
{
	if ((int)contents.size() > 0)
	{
		level->setTextBox("notification");
		string item_name = contents["item"].asString();
		int amount = contents["amount"].asInt();
		string notification_text = "";
		if (item_name != "nothing")
		{
			string true_item_name = game->item_database.item_scripts[item_name]["name"].asString();
			notification_text = "You obtained a " + true_item_name + " x" + to_string(amount) + "[A]";
			party->addToInventory(item_name, amount);
		}
		else
		{
			notification_text = "--Nothing--[A]";
		}
		level->setText(notification_text);
		bTextRunning = true;
	}
}
