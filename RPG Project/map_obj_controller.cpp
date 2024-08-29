#include "map_obj_controller.h"
#include "level.h"
#include "game.h"
#include "chest_obj_condition.h"
#include "door_obj_condition.h"

void MapObjectController::setup(Json::Value data, Level* level, EventController* event_controller)
{
	ConditionController::setup(data, level, event_controller);
	setupAllConditions();
}

void MapObjectController::setupAllConditions()
{
	for (int index = 0; index < (int)data.size(); index++)
	{
		if (data[index].isMember("Type"))
		{
			if (data[index]["Type"].asString() == "Chest")
			{
				shared_ptr<ChestObjCondition> map_condition = make_shared<ChestObjCondition>();
				map_condition->setup(data[index], level, event_controller);
				obj_conditions.push_back(map_condition);
			}
			if (data[index]["Type"].asString() == "Door")
			{
				shared_ptr<DoorObjCondition> door_condition = make_shared<DoorObjCondition>();
				door_condition->setup(data[index], level, event_controller);
				obj_conditions.push_back(door_condition);
			}
		}
	}
}


void MapObjectController::update()
{
	if (level->cameFromPortal == false)
	{
		updateAllConditions();
	}
}

void MapObjectController::draw()
{
	for (auto& condition : obj_conditions)
	{
		condition->draw();
	}
}

void MapObjectController::ResetAllDoors()
{
	for (auto& condition : obj_conditions)
	{
		if (condition->type == "Door")
		{
			static_cast<DoorObjCondition*>(condition.get())->ResetDoor();
		}
	}
}

void MapObjectController::updateAllConditions()
{
	for (auto& condition : obj_conditions)
	{
		condition->update();
	}
}
