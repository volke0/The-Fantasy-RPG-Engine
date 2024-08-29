#include "condition.h"
#include "level.h"
#include "event_controller.h"
#include "game.h"

void Condition::setup(Json::Value data, Level* level, EventController* event_controller)
{
	this->data = data;
	this->level = level;
	this->map = level->map.get();
	this->collision_handler = &level->collision_handler;
	this->event_controller = event_controller;
	this->game = level->game;
	this->party = &game->party;
	this->event_manager = &game->scene_manager.event_manager;
}

void Condition::parseData()
{
	if (data.isMember("Event"))
	{
		event_link = data["Event"].asString();
	}
	if (data.isMember("Completion_Flag"))
	{
		Flag = data["Completion_Flag"].asString();
		//cout << "AREA FLAG: " << Flag << endl;
	}
	if (data.isMember("Condition_Flag"))
	{
		condition_flag = data["Condition_Flag"].asString();
		bHasConditionFlag = true;
	}
	if (!bHasConditionFlag)
	{
		if (data.isMember("Global_Con_Flag"))
		{
			global_condition_flag = data["Global_Con_Flag"].asString();
			bHasGlobalConditionFlag = true;
		}
	}
	if (data.isMember("Map"))
	{
		next_level_id = data["Map"].asInt();
	}
	if (data.isMember("NextPortal"))
	{
		next_portal_id = data["NextPortal"].asInt();
	}
	if (data.isMember("Exit"))
	{
		exitCoords.x = data["Exit"]["x"].asFloat();
		exitCoords.y = data["Exit"]["y"].asFloat();
	}
	if (data.isMember("Direction"))
	{
		direction = data["Direction"].asString();
	}
}

void Condition::update()
{
}

void Condition::draw()
{
}

void Condition::RunEvent()
{
	if (!bTextRunning)
	{
		if ((int)event_link.size() > 0)
		{
			if (level->flag_sys->getFlag(Flag, level->name) == false)
			{
				if (condition_flag != "")
				{
					if (level->flag_sys->getFlag(condition_flag, level->name) == true)
					{
						level->flag_sys->ENUT(Flag, level->name);
						event_controller->runEvent(event_link);
					}
				}
				else
				{
					level->flag_sys->ENUT(Flag, level->name);
					event_controller->runEvent(event_link);
				}
			}
			if (event_controller->isEventDone(event_link, level->name) == true)
			{
				if (level->flag_sys->getFlag(Flag, level->name) == true)
				{
					if (Flag == "0")
					{
						level->flag_sys->ENUF(Flag, level->name);
					}
					done = true;
					trigger_event = false;
				}
			}
		}
		else
		{
			done = true;
			trigger_event = false;
		}
	}
	else
	{
		if (level->isTextDone())
		{
			bTextRunning = false;
		}
	}
}

void Condition::TimerUpdate()
{
}

vector<float> Condition::getWorldOffset()
{
	return level->map->world_offset;
}

void Condition::ResetCondition()
{
	done = false;
}

void Condition::push_level_portal(int num)
{
	game->push_level_portal(num, next_portal_id);
}

Vector2 Condition::getExitCoords()
{
	return exitCoords;
}

string Condition::getDirection()
{
	return direction;
}

string Condition::GetUnitName(Json::Value line)
{
	if (line.isString() && line.asString() == "party")
	{
		return "party";
	}
	int unit_id = line.asInt();
	return game->GetUnitName(unit_id);
}
