#include "movement_controller.h"
#include "unit.h"
#include "vars.h"
#include "input_handler.h"
#include "camera.h"
#include "level.h"
#include "map.h"
#include "scene_manager.h"
#include "party.h"

void MovementController::setup(Unit* unit)
{
	this->unit = unit;
	og_move_speed = 12;
	move_speed = og_move_speed;
	tile_speed = (1 / (float)move_speed);
	newLocation = { 0, 0, (float)TILE, (float)TILE };
	if (unit->name == "party")
	{
		party = static_cast<Party*>(unit);
		isParty = true;
	}
}

void MovementController::update()
{
	if (unit->map_mode == true)
	{
		updateMapMode();
		/*cout << "Unit: " << unit->name << " TRUE OFFSET: " << unit->true_offset[0] / TILE << ", " << unit->true_offset[1] / TILE << endl;
		cout << "Unit: " << unit->name << " Map Rect: " << unit->map_rect.x / TILE << ", " << unit->map_rect.y / TILE << endl;*/
	}
}

void MovementController::setCamera(Cam* camera)
{
	this->camera = camera;
}

void MovementController::updateMapMode()
{
	unit->move_list.clear();
	unit->move_list.shrink_to_fit();
	path_finder.update();
	if (fixing == false && camera->fixing_world_offsets == false)
	{
		partyMode();
		otherUnitMode();
		setLastMove();
		if (moving)
		{
			unit->currentlyMoving = true;
		}
		else if (!moving)
		{
			unit->currentlyMoving = false;
		}
	}
	fixLocation();
}


void MovementController::partyMode()
{
	if (isParty == true)
	{
		if (!level->scene_manager->busy)
		{
			updatePartyMapMode();
		}
		updatePartyMove();
		setRunning();
	}
}

void MovementController::setLastMove()
{
	if ((int)unit->move_list.size() > 0)
	{
		unit->last_move = unit->move_list[0];
	}
}

void MovementController::otherUnitMode()
{
	if (isParty == false)
	{
		updateOtherUnitMove();
	}
}



void MovementController::updatePartyMapMode()
{
	if (party->input->findKey("x"))
	{
		move_speed = og_move_speed / 2;
		tile_speed = (1 / (float)move_speed);
	}
	else
	{
		move_speed = og_move_speed;
		tile_speed = (1 / (float)move_speed);
	}
	if (party->input->findKey("up"))
	{
		party->move_list.push_back("up");
	}
	else if (party->input->findKey("down"))
	{
		party->move_list.push_back("down");
	}
	else if (party->input->findKey("right"))
	{
		party->move_list.push_back("right");
	}
	else if (party->input->findKey("left"))
	{
		party->move_list.push_back("left");
	}
}

void MovementController::updatePartyMove()
{
	if (unit->move_list.size() > 0)
	{
		if (unit->move_list[0] == "up")
		{
			newLocation.x = unit->map_rect.x;
			newLocation.y = unit->map_rect.y - TILE;
			if (level->collision_handler.checkCollisionWithObjects(newLocation) != true)
			{
				if (unit->map_rect.y / (float)TILE < 3 && map->world_offset[1] != 0)
				{
					camera->moveCamera(vector<int> {0, 1});
					move(vector<int> {0, -1}, tile_speed, true);
				}
				else
				{
					if (unit->true_offset[1] != 0)
					{
						move(vector<int> {0, -1});
					}
				}
			}
		}
		if (unit->move_list[0] == "down")
		{
			newLocation.x = unit->map_rect.x;
			newLocation.y = unit->map_rect.y + TILE;
			if (level->collision_handler.checkCollisionWithObjects(newLocation) != true)
			{
				if (unit->true_offset[1] / (float)TILE < map->size.y - 3)
				{
					if (unit->map_rect.y / (float)TILE >= (float)(SCREEN_HEIGHT / TILE) - 3)
					{
						camera->moveCamera(vector<int> {0, -1});
						move(vector<int> {0, 1}, tile_speed, true);
					}
					else
					{
						move(vector<int> {0, 1});
					}
				}
				else
				{
					if (unit->true_offset[1] / (float)TILE != map->size.y - 1)
					{
						move(vector<int> {0, 1});
					}
				}
			}
		}
		if (unit->move_list[0] == "right")
		{
			newLocation.x = unit->map_rect.x + TILE;
			newLocation.y = unit->map_rect.y;
			if (level->collision_handler.checkCollisionWithObjects(newLocation) != true)
			{
				if (unit->true_offset[0] / (float)TILE < map->size.x - 3)
				{
					if (unit->map_rect.x / (float)TILE >= (float)(SCREEN_WIDTH / TILE) - 3)
					{
						camera->moveCamera(vector<int> {-1, 0});
						move(vector<int> {1, 0}, tile_speed, true);
					}
					else
					{
						move(vector<int> {1, 0});
					}
				}
				else
				{
					if (unit->true_offset[0] / (float)TILE != map->size.x - 1)
					{
						move(vector<int> {1, 0});
					}
				}
			}
		}
		if (unit->move_list[0] == "left")
		{
			newLocation.x = unit->map_rect.x - TILE;
			newLocation.y = unit->map_rect.y;
			if (level->collision_handler.checkCollisionWithObjects(newLocation) != true)
			{
				if (unit->map_rect.x / (float)TILE < 3 && map->world_offset[0] != 0)
				{
					camera->moveCamera(vector<int> {1, 0});
					move(vector<int> {-1, 0}, tile_speed, true);
				}
				else
				{
					if (unit->true_offset[0] != 0)
					{
						move(vector<int> {-1, 0});
					}
				}
			}
		}
	}
}

void MovementController::updateOtherUnitMove()
{
	if (unit->move_list.size() > 0)
	{
		if (unit->move_list[0] == "up")
		{
			newLocation.x = unit->map_rect.x;
			newLocation.y = unit->map_rect.y - TILE;
			if (level->collision_handler.checkCollisionWithObjects(newLocation) != true)
			{
				move(vector<int> {0, -1});
			}
		}
		if (unit->move_list[0] == "down")
		{
			newLocation.x = unit->map_rect.x;
			newLocation.y = unit->map_rect.y + TILE;
			if (level->collision_handler.checkCollisionWithObjects(newLocation) != true)
			{
				move(vector<int> {0, 1});
			}
		}
		if (unit->move_list[0] == "right")
		{
			newLocation.x = unit->map_rect.x + TILE;
			newLocation.y = unit->map_rect.y;
			if (level->collision_handler.checkCollisionWithObjects(newLocation) != true)
			{
				move(vector<int> {1, 0});
			}
		}
		if (unit->move_list[0] == "left")
		{
			newLocation.x = unit->map_rect.x - TILE;
			newLocation.y = unit->map_rect.y;
			if (level->collision_handler.checkCollisionWithObjects(newLocation) != true)
			{
				move(vector<int> {-1, 0});
			}
		}
	}
}


void MovementController::move(vector<int> direction, float speed, bool ignore_map_rect)
{
	float t_speed = 0;
	if (speed == 0)
	{
		t_speed = tile_speed;
	}
	else
	{
		t_speed = speed;
	}
	unit->true_offset[0] += (float)direction[0] * ((float)t_speed * TILE);
	unit->true_offset[1] += (float)direction[1] * ((float)t_speed * TILE);
	
	if (ignore_map_rect == false)
	{
		unit->map_rect.x += (float)direction[0] * ((float)t_speed * TILE);
		unit->map_rect.y += (float)direction[1] * ((float)t_speed * TILE);
	}
	unit->direction = direction;
	moving = true;
}

void MovementController::fixLocation()
{
	if (moving == true)
	{
		if (fmod(unit->map_rect.x, (float)TILE) != 0)
		{
			unit->map_rect.x += (float)unit->direction[0] * ((float)tile_speed * TILE);
			fixing = true;
		}
		if (fmod(unit->map_rect.y, (float)TILE) != 0)
		{
			unit->map_rect.y += (float)unit->direction[1] * ((float)tile_speed * TILE);
			fixing = true;
		}

		//True Offset Fixing
		if (fmod(unit->true_offset[0], (float)TILE) != 0)
		{
			unit->true_offset[0] += (float)unit->direction[0] * ((float)tile_speed * TILE);
			fixing = true;
		}
		if (fmod(unit->true_offset[1], (float)TILE) != 0)
		{
			unit->true_offset[1] += (float)unit->direction[1] * ((float)tile_speed * TILE);
			fixing = true;
		}
		

		if (fmod(unit->true_offset[0], (float)TILE) == 0 && fmod(unit->true_offset[1], (float)TILE) == 0)
		{
			if (fmod(unit->map_rect.x, (float)TILE) != 0 || fmod(unit->map_rect.y, (float)TILE) != 0)
			{
				unit->map_rect.x = round(unit->map_rect.x / (float)TILE) * (float)TILE;
				unit->map_rect.y = round(unit->map_rect.y / (float)TILE) * (float)TILE;
				unit->true_offset[0] = round(unit->true_offset[0] / (float)TILE) * (float)TILE;
				unit->true_offset[1] = round(unit->true_offset[1] / (float)TILE) * (float)TILE;
			}
			setLastMove();
			unit->move_list.clear();
			unit->move_list.shrink_to_fit();
			unit->direction = { 0, 0 };
			fixing = false;
			moving = false;
		}
	}
}

void MovementController::addMoveSpeed(float speed, string type, int direction)
{
	if (type == "rect")
	{
		total_moved_rect[direction] += speed;
	}
	if (type == "true")
	{
		total_moved_true[direction] += speed;
		if (total_moved_true[direction] > 48.f)
		{
			total_moved_true[direction] = 0.f;
		}
	}
}


void MovementController::setRunning()
{
	if (party->input->getIfKeyPressedDown(KEY_X))
	{
		if (moving)
		{
			party->running = true;
		}
	}
	else
	{
		party->running = false;
	}
}




void MovementController::setLocation(Vector2 offset)
{
	updateTrueOffset(offset);
}

void MovementController::updateMapRect(vector<float> world_offset)
{
	unit->map_rect.x += world_offset[0];
	unit->map_rect.y += world_offset[1];
}

void MovementController::setMoveSpeed(int speed)
{
	if (speed % 2 == 0)
	{
		move_speed = (float)speed;
		tile_speed = (1 / (float)move_speed);
	}
	else
	{
		move_speed = (float)speed + 1;
		tile_speed = (1 / (float)move_speed);
	}
}

void MovementController::findPath(Vector2 destination, int speed)
{
	path_finder.findPath(unit, map, destination, speed);
}

bool MovementController::isPathDone()
{
	return path_finder.done;
}

void MovementController::updateTrueOffset(Vector2 offset)
{
	if (unit->true_offset.size() == 0)
	{
		unit->true_offset = { 0, 0 };
	}
	unit->map_rect.x = offset.x + map->world_offset[0];
	unit->map_rect.y = offset.y + map->world_offset[1];
	unit->true_offset[0] = offset.x;
	unit->true_offset[1] = offset.y;
}
