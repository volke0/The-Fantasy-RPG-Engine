#include "door_obj_condition.h"
#include "game.h"
#include "level.h"

void DoorObjCondition::setup(Json::Value data, Level* level, EventController* event_controller)
{
	MapObjectCondition::setup(data, level, event_controller);
	parseData();
}

void DoorObjCondition::parseData()
{
	MapObjectCondition::parseData();
	if (data.isMember("Locked"))
	{
		bDoorLocked = data["Locked"].asBool();
	}
	if (data.isMember("Key"))
	{
		if (data["Key"]["type"].asString() == "event")
		{
			doorkey.bRequiresEvent = true;
			doorkey.trigger_flag = data["Key"]["trigger_flag"].asString();
		}
		else if (data["Key"]["type"].asString() == "item")
		{
			doorkey.bRequiresItem = true;
			doorkey.item_name = data["Key"]["key_item"].asString();
		}
	}
	addToBlockers();
}

void DoorObjCondition::addToBlockers()
{
	Rectangle blocker = { area_rect.x * TILE_SIZE, area_rect.y * TILE_SIZE, area_rect.width, area_rect.height };
	blocker.y += (float)TILE_SIZE;
	vector<Rectangle> blocker_list = { blocker };
	level->createBlockerSet(tile_change_name, blocker_list, layer_pos);
}


void DoorObjCondition::update()
{
	updateAreaRect();
	updateDoor();
	if (event_link != "" && trigger_event)
	{
		RunEvent();
	}
}

void DoorObjCondition::draw()
{
}

void DoorObjCondition::ResetDoor()
{
	level->undoTileChange(tile_change_name);
	level->AddBlockerSet(tile_change_name);
}

void DoorObjCondition::updateDoor()
{
	if (collision_handler->checkIfUnitFacesArea(party, area_rect))
	{
		if (!level->isTileChangeActive(tile_change_name))
		{
			if (!bDoorLocked)
			{
				TriggerDoor();
			}
			else
			{
				DoorInput();
			}
		}
	}
	TimerUpdate();
}

void DoorObjCondition::TriggerDoor()
{
	level->setTileChange(tile_change_name);
	bRemoveBlocker = true;
	timer = 0;
	if (sfx_name != "")
	{
		level->sound_player.playSound(sfx_name);
	}
	if (event_link != "")
	{
		trigger_event = true;
	}
}

void DoorObjCondition::DoorInput()
{
	if (bCanInput)
	{
		if (game->input.findKey("x") && !party->running)
		{
			if (!CheckIfHasKey())
			{
				level->setTextBox("notification");
				level->setText("Locked[A]");
			}
			else
			{
				bDoorLocked = false;
			}
			bCanInput = false;
			timer = 0;
		}
	}
}

bool DoorObjCondition::CheckIfHasKey()
{
	if (doorkey.bRequiresEvent)
	{
		if (level->GetFlag(doorkey.trigger_flag))
		{
			return true;
		}
	}
	if (doorkey.bRequiresItem)
	{
		if (party->CheckIfItemExists(doorkey.item_name))
		{
			return true;
		}
	}
	return false;
}

void DoorObjCondition::TimerUpdate()
{
	if (!game->scene_manager.busy)
	{
		if (timer > 5)
		{
			bCanInput = true;
			if (bRemoveBlocker)
			{
				bRemoveBlocker = false;
				level->RemoveBlockerSet(tile_change_name);
			}
		}
		else
		{
			timer++;
		}
	}
}
