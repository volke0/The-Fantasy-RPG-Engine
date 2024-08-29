#include "area_controller.h"
#include "event_controller.h"
#include "area_condition.h"
#include "party.h"
#include "game.h"
#include "level.h"
#include "camera.h"
#include "vars.h"

void AreaEventController::setup(Json::Value data, Level* level, EventController* event_controller)
{
	ConditionController::setup(data, level, event_controller);
	setupAllConditions();
}

void AreaEventController::setupAllConditions()
{
	for (int index = 0; index < (int)data.size(); index++)
	{
		AreaCondition condition;
		condition.setup(data[index], level, event_controller);
		area_conditions.push_back(condition);
	}
}


void AreaEventController::update()
{
	if (level->cameFromPortal == false)
	{
		updateAllConditions();
	}
	updatePartyLocation();
}

void AreaEventController::draw()
{
	for (auto& condition : area_conditions)
	{
		condition.draw();
	}
}

void AreaEventController::ResetAllPortalConditions()
{
	for (auto& condition : area_conditions)
	{
		if (condition.type == "Portal")
		{
			condition.ResetCondition();
		}
	}
}

void AreaEventController::updateAllConditions()
{
	for (auto& condition : area_conditions)
	{
		condition.update();
	}
}

void AreaEventController::updatePartyLocation()
{
	if (level->cameFromPortal == true && (int)area_conditions.size() > 0)
	{
		findPortalCondition();
		Vector2 exit_coords = area_conditions[event_id].getExitCoords();
		if (!camera->autoCamDone && !camera->cam1_mode)
		{
			//cout << "EXIT X: " << area_conditions[event_id].getExitCoords().x/TILE << " EXIT Y: " << area_conditions[event_id].getExitCoords().y/TILE << endl;
			party = &game->party;
			party->setLevel(level, map);
			camera->setupPartyCamera(party);
			level->setParty(party);
			level->party->setDirection(area_conditions[event_id].getDirection());
			camera->autoCameraLocation(exit_coords, 16, true);
		}
		if (!camera->autoCamDone) return;
		exit_coords.x = exit_coords.x * TILE;
		exit_coords.y = exit_coords.y * TILE;
		level->party->movement_controller.setLocation(exit_coords);
		event_controller->ResetAllDoors();
		ResetAllPortalConditions();
		camera->autoCamDone = false;
		level->cameFromPortal = false;
	}
}

void AreaEventController::findPortalCondition()
{
	int index = 0;
	int id = 0;
	while (index < area_conditions.size())
	{
		if (area_conditions[index].type == "Portal")
		{
			if (area_conditions[index].getPortalID() == level->nextPortalID)
			{
				event_id = index;
				break;
			}
		}
		index++;
	}
}

