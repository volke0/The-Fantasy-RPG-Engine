#include "area_condition.h"
#include "map.h"
#include "collision_handler.h"
#include "scene_manager.h"
#include "Party.h"
#include "event_controller.h"
#include "vars.h"
#include "level_database.h"
#include "game.h"
#include "unit.h"
#include "level.h"

void AreaCondition::setup(Json::Value data, Level* level, EventController* event_controller)
{
	Condition::setup(data, level, event_controller);
	parseData();
}

void AreaCondition::parseData()
{	
	Condition::parseData();
	type = data["Type"].asString();
	PortalID = data["Portal_ID"].asInt();
	setupAreaRect();
}

void AreaCondition::setupAreaRect()
{
	x1 = data["Range"]["x1"].asInt();
	y1 = data["Range"]["y1"].asInt();
	x2 = data["Range"]["x2"].asInt();
	y2 = data["Range"]["y2"].asInt();
	float x = (float)abs(x1 - x2) + 1;
	float y = (float)abs(y1 - y2);
	if (x == 0)
	{
		x = 1;
	}
	if (y == 0)
	{
		y = 1;
	}
	float width = x * TILE;
	float height = y * TILE;
	area_rect = { (float)(x1 * TILE), (float)(y1 * TILE), width, height };
	og_area_rect = area_rect;
}

void AreaCondition::update()
{
	updateAreaRect();
	checkIfInRange();
}

void AreaCondition::draw()
{
	//Test drawing
	DrawRectangle((int)area_rect.x, (int)area_rect.y, (int)area_rect.width, (int)area_rect.height, RED);
}

void AreaCondition::checkIfInRange()
{
	for (auto& unit : level->unit_list)
	{
		if (unit->name == "party")
		{
			if (collision_handler->checkCollision(area_rect, unit))
			{
				if (!unit->movement_controller.moving && !unit->movement_controller.fixing)
				{
					if (done == false)
					{
						Portal(unit);
						InRange(unit);
					}
				}
			}
			else
			{
				event_read = false;
				done = false;
			}
			
		}
	}
}

void AreaCondition::updateAreaRect()
{
	vector<float> world_offset = getWorldOffset();
	area_rect.x = og_area_rect.x + world_offset[0];
	area_rect.y = og_area_rect.y + world_offset[1];
}

void AreaCondition::Portal(Unit* unit)
{
	//cout << "X: " << area_rect.x/TILE << " Y: " <<  area_rect.y/TILE << endl;
	if (type == "Portal")
	{
		if ((int)event_link.size() > 0)
		{
			if (level->flag_sys->getFlag(Flag, level->name) == false)
			{
				level->flag_sys->ENUT(Flag, level->name);
				event_controller->runEvent(event_link);
			}
			else if (event_controller->isEventDone(event_link, level->name) == true && event_controller->runningEvent == false)
			{
				if (level->flag_sys->getFlag(Flag, level->name) == true)
				{
					if (Flag == "0")
					{
						level->flag_sys->ENUF(Flag, level->name);
					}
					push_level_portal(next_level_id);
					done = true;
				}
			}
		}
		else
		{
			push_level_portal(next_level_id);
			done = true;
		}
	}
}

void AreaCondition::InRange(Unit* unit)
{
	if (type == "InRange")
	{
		RunEvent();
	}
}

int AreaCondition::getPortalID()
{
	return PortalID;
}