#include "camera.h"
#include "map.h"
#include "party.h"
#include "level.h"
#include "input_handler.h"

void Cam::update()
{
	map_rect.width = (float)TILE;
	map_rect.height = (float)TILE;
	changeMode();
	playerUpdate();
	updateCAM1();
	done_moving = true;
	//cout << "CAM MAP X: " << map_rect.x/TILE << " Y: " << map_rect.y/TILE << endl;
	//cout << "CAM TRUE X: " << true_offset[0] / TILE  << "  Y: " << true_offset[1] / TILE << endl;
	//cout << "WORLD X: " << map->world_offset[0] / TILE << " WORLD Y: " << map->world_offset[1] / TILE << endl;
}

void Cam::changeMode()
{
	if (party == nullptr)
	{
		followPlayer = false;
	}
}

void Cam::setupPartyCamera(Party* party)
{
	this->party = party;
	followPlayer = true;
	//cout << "CAMERA PARTY NAME: " << this->party->name << endl;
}

void Cam::moveCamera(vector<int> direction)
{
	this->direction = direction;
	goal[0] = map->world_offset[0] + (float)direction[0] * TILE;
	goal[1] = map->world_offset[1] + (float)direction[1] * TILE;
	//cout << "GOAL X: " << goal[0] / TILE << " Y: " << goal[1] / TILE << endl;
}

void Cam::autoCameraLocation(Vector2 location, int speed, bool bWarpToLocation)
{
	gotoLocation(location, speed, bWarpToLocation);
}

void Cam::autoCameraUnit(Unit* unit, int speed, bool bWarpToLocation)
{
	//cout << unit->name << endl;
	Vector2 location = { unit->true_offset[0] / (float)TILE, unit->true_offset[1] / (float)TILE };
	if (unit->name == "party")
	{
		autoCamOnParty = true;
	}
	gotoLocation(location, speed, bWarpToLocation);
}

void Cam::setUpCamera(Level* level, Input* input)
{
	this->level = level;
	this->map = level->map.get();
	this->input = input;
	CalculateMaxWorldOffsets();
	//map_rect = { 0, 0, (float)TILE, (float)TILE };
}

void Cam::setCameraSpeed()
{
	cam_speed = 1 / party->movement_controller.move_speed;
	tile_speed = (float)cam_speed * TILE;
}

void Cam::CalculateMaxWorldOffsets()
{
	float max_map_size_x = (float)(map->size.x * TILE);
	float max_map_size_y = (float)(map->size.y * TILE);

	float max_world_x = -1 * (max_map_size_x - SCREEN_WIDTH);
	float max_world_y = -1 * (max_map_size_y - SCREEN_HEIGHT);

	/*cout << "BEFORE:  WORLD X: " << map->world_offset[0] / TILE << " WORLD Y: " << map->world_offset[1] / TILE << endl;*/

	//X
	if (map->world_offset[0] < max_world_x)
	{
		map->world_offset[0] = max_world_x;
		level->AdjustUnitPos(Vector2{ 1.f * TILE, 0.f });
	}
	if (map->world_offset[0] > 0)
	{
		map->world_offset[0] = 0;
		level->AdjustUnitPos(Vector2{ -1.f * TILE, 0.f });
	}

	//Y
	if (map->world_offset[1] < max_world_y)
	{
		map->world_offset[1] = max_world_y;
		level->AdjustUnitPos(Vector2{ 0.f, 1.f * TILE });
	}
	if (map->world_offset[1] > 0)
	{
		map->world_offset[1] = 0;
		level->AdjustUnitPos(Vector2{ 0.f, -1.f * TILE });
	}

	max_world_offsets[0] = max_world_x;
	max_world_offsets[1] = max_world_y;

	/*cout << "AFTER:  WORLD X: " << map->world_offset[0] / TILE << " WORLD Y: " << map->world_offset[1] / TILE << endl;*/
}



//Main Functionality
void Cam::playerUpdate()
{
	if (party != nullptr && cam1_mode == false)
	{
		setCameraSpeed();
		updateMap();
	}
}

void Cam::keepInMap()
{

}

void Cam::updateMap()
{
	if (updatingMap == true)
	{
		fixWorldOffset();
	}
}

void Cam::fixWorldOffset()
{
	if (direction[0] != 0 || direction[1] != 0)
	{
		if (map->world_offset[0] != goal[0] || map->world_offset[1] != goal[1])
		{
			updateLocation(direction, tile_speed, true);
			fixing_world_offsets = true;
		}
		else
		{
			direction = { 0, 0 };
			fixing_world_offsets = false;
		}
	}
}




void Cam::updateLocation(vector<int> direction, float dist, bool ignore_party)
{
	vector<float> distance = { 0, 0 };
	distance[0] = direction[0] * dist;
	distance[1] = direction[1] * dist;
	if (party != nullptr && ignore_party == false)
	{
		party->map_rect.x += (distance[0]);
		party->map_rect.y += (distance[1]);
	}
	map->setWorldOffset(distance);
}


void Cam::gotoLocation(Vector2 location, int speed, bool bWarpToLocation)
{
	//cout << speed << endl;
	autoCamDone = false;
	updatingMap = false;
	followPlayer = false;
	targetLocation = location;
	cam_speed = (float)speed;
	cam1_mode = true;
	this->bWarpToLocation = bWarpToLocation;
	//followPlayer = false;
	
	//cout << "MAP RECT X: " << map_rect.x / TILE << " Y: " << map_rect.y / TILE << endl;
	//cout << "WORLD OFFSET X: " << map->world_offset[0] / TILE << " Y: " << map->world_offset[1] / TILE << endl;
	//cout << "True Offset X: " << true_offset[0] << " Y: " << true_offset[1] << endl;
	float x = location.x * TILE;
	float y = location.y * TILE;
	calculateMoveData(x, y);
}

void Cam::calculateMoveData(float x, float y)
{
	map->world_offset[0] = round(map->world_offset[0] / TILE) * TILE;
	map->world_offset[1] = round(map->world_offset[1] / TILE) * TILE;
	//The X direction
	if (x / TILE >= 9 && x / TILE <= map->size.x - 9)
	{
		float diff_x = x - (float)(9 * TILE);
		auto_world_offset[0] = map->world_offset[0] + diff_x;
	}
	else if (x / TILE < 9)
	{
		auto_world_offset[0] = map->world_offset[0];
	}
	else if (x / TILE > map->size.x - 9)
	{
		float diff_x = (float)(map->size.x * TILE) - SCREEN_WIDTH;
		auto_world_offset[0] = map->world_offset[0] + diff_x;
	}

	//The Y direction
	if (y / TILE >= 7 && y / TILE <= map->size.y - 7)
	{
		float diff_y = y - (float)(6 * TILE);
		auto_world_offset[1] = map->world_offset[1] + diff_y;
	}
	else if (y / TILE < 6)
	{
		auto_world_offset[1] = map->world_offset[1];
	}
	else if (y / TILE > map->size.y - 6)
	{
		float diff_y = float(map->size.y * TILE) - SCREEN_HEIGHT;
		auto_world_offset[1] = map->world_offset[1] + diff_y;
	}

	auto_world_offset[0] = round(auto_world_offset[0] / TILE) * TILE;
	auto_world_offset[1] = round(auto_world_offset[1] / TILE) * TILE;
	/*cout << "WORLD OFFSET TO AIM FOR IS X: " << auto_world_offset[0] / TILE << " Y: " << auto_world_offset[1] / TILE << endl;
	cout << "MAX MAP SIZE X: " << map->size.x << " MAX MAP SIZE Y: " << map->size.y << endl;*/
}


void Cam::updateCAM1()
{
	if (cam1_mode == true)
	{
		moveCAM1();
		if (onLocation == true)
		{
			true_offset[0] = targetLocation.x * TILE;
			true_offset[1] = targetLocation.y * TILE;
			map_rect.x = (true_offset[0] + map->world_offset[0]);
			map_rect.y = (true_offset[1] + map->world_offset[1]);
			onLocation = false;
			cam1_mode = false;
			if (autoCamOnParty == true)
			{
				followPlayer = true;
				autoCamOnParty = false;
			}
			direction = { 0, 0 };
			fixing_world_offsets = false;
			updatingMap = true;
			autoCamDone = true;
		}
	}
}

void Cam::moveCAM1()
{
	float tile = ((float)1 / cam_speed) * TILE;
	int direction_x = 0;
	int direction_y = 0;

	if (auto_world_offset[0] > 0)
	{
		direction_x = -1;
	}
	else if (auto_world_offset[0] < 0)
	{
		direction_x = 1;
	}
	if (auto_world_offset[1] > 0)
	{
		direction_y = -1;
	}
	else if (auto_world_offset[1] < 0)
	{
		direction_y = 1;
	}

	if (!bWarpToLocation)
	{
		if (auto_world_offset[0] != 0 && auto_world_offset[1] != 0)
		{
			auto_world_offset[0] += ((float)direction_x * tile);
			auto_world_offset[1] += ((float)direction_y * tile);
			updateLocation(vector<int>{direction_x, direction_y}, tile);
		}
		else if (auto_world_offset[0] != 0 && auto_world_offset[1] == 0)
		{
			auto_world_offset[0] += ((float)direction_x * tile);
			updateLocation(vector<int>{direction_x, 0}, tile);
		}
		else if (auto_world_offset[0] == 0 && auto_world_offset[1] != 0)
		{
			auto_world_offset[1] += ((float)direction_y * tile);
			updateLocation(vector<int>{0, direction_y}, tile);
		}
	}
	else
	{
		float tile_x = abs(auto_world_offset[0]);
		float tile_y = abs(auto_world_offset[1]);
		updateLocation(vector<int>{ direction_x, 0 }, tile_x);
		updateLocation(vector<int>{ 0, direction_y }, tile_y);
		auto_world_offset[0] += direction_x * tile_x;
		auto_world_offset[1] += direction_y * tile_y;
		bWarpToLocation = false;
	}
	
	if (auto_world_offset[0] == 0 && auto_world_offset[1] == 0)
	{
		onLocation = true;
	}
}


void Cam::isDoneMoving()
{
	if (party != nullptr)
	{
		if (posXDone == true && negXDone == true)
		{
			if (posYDone == true && negYDone == true)
			{
				done_moving = true;
			}
			else
			{
				done_moving = false;
			}
		}
		else
		{
			done_moving = false;
		}
	}
	/*cout << "WORLD X: " << map->world_offset[0] / TILE << " WORLD Y: " << map->world_offset[1] / TILE << endl;*/
}
