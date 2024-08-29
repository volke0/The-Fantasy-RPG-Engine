#include "map_obj_condition.h"
#include "level.h"
#include "party.h"
#include "game.h"

void MapObjectCondition::setup(Json::Value data, Level* level, EventController* event_controller)
{
	Condition::setup(data, level, event_controller);
}

void MapObjectCondition::parseData()
{
	Condition::parseData();
	type = data["Type"].asString();
	if (data.isMember("TileChange"))
	{
		tile_change_name = data["TileChange"].asString();
	}
	if (data.isMember("location"))
	{
		tile_location.x = data["location"]["x"].asFloat();
		tile_location.y = data["location"]["y"].asFloat();
	}
	if (data.isMember("layer"))
	{
		layer_pos = data["layer"].asInt();
	}
	if (data.isMember("sfx"))
	{
		sfx_name = data["sfx"].asString();
	}
	area_rect = { tile_location.x, tile_location.y, (float)TILE_SIZE, (float)TILE_SIZE };
	og_area_rect = area_rect;
}


void MapObjectCondition::update()
{
	
}

void MapObjectCondition::draw()
{
}

void MapObjectCondition::updateAreaRect()
{
	vector<float> world_offset = getWorldOffset();
	area_rect.x = og_area_rect.x * TILE;
	area_rect.y = (og_area_rect.y * TILE);
	area_rect.width = (float)TILE;
	area_rect.height = (float)TILE;
	area_rect.x = area_rect.x + world_offset[0];
	area_rect.y = area_rect.y + world_offset[1];
}
