#include "collision_handler.h"
#include "Party.h"
#include "npc.h"
#include "map.h"
#include "level.h"
#include "vars.h"

void CollisionHandler::setup(unordered_map<int, unordered_map<string, vector<Rectangle>>> all_layer_blockers, Level* level)
{
	this->all_layer_blockers = all_layer_blockers;
	this->level = level;
	this->type = "collision";
}

bool CollisionHandler::checkCollision(Rectangle area_rect, Unit* unit)
{
	return CheckCollisionRecs(area_rect, unit->map_rect);
}

bool CollisionHandler::checkCollisionWithObjects(Rectangle& rect)
{
	if (checkIfCollidesWithBlocker(rect) == true)
	{
		return true;
	}
	return false;
}


bool CollisionHandler::checkIfCollidesWithBlocker(Rectangle rect)
{
	vector<string> accepted_blocker_sets = level->GetAcceptedBlockerSets();
	for (auto& [name, blockers] : layer_blockers)
	{
		if (find(accepted_blocker_sets.begin(), accepted_blocker_sets.end(), name) != accepted_blocker_sets.end())
		{
			for (auto blocker : blockers)
			{
				setBlockerScale(blocker);
				if (CheckCollisionRecs(blocker, rect))
				{
					return true;
				}
			}
		}
	}
	return false;
}

void CollisionHandler::setBlockerScale(Rectangle& blocker)
{
	blocker.x = blocker.x * (float)SCALE;
	blocker.x += world_offset[0];
	blocker.y = (blocker.y * (float)SCALE) - TILE;
	blocker.y += world_offset[1];
	blocker.width = blocker.width * (float)SCALE;
	blocker.height = blocker.height * (float)SCALE;
}

string CollisionHandler::getRelativeDirectionToUnit(Rectangle rect, NPC* unit)
{
	Rectangle left_rect = { rect.x - TILE, rect.y, rect.width, rect.height };
	Rectangle right_rect = { rect.x + TILE, rect.y, rect.width, rect.height };
	Rectangle up_rect = { rect.x, rect.y - TILE, rect.width, rect.height };
	Rectangle down_rect = { rect.x, rect.y + TILE, rect.width, rect.height };
	if (CheckCollisionRecs(left_rect, unit->map_rect))
	{
		return "left";
	}
	else if (CheckCollisionRecs(right_rect, unit->map_rect))
	{
		return "right";
	}
	else if (CheckCollisionRecs(up_rect, unit->map_rect))
	{
		return "up";
	}
	else if (CheckCollisionRecs(down_rect, unit->map_rect))
	{
		return "down";
	}
	return "none";
}

bool CollisionHandler::checkIfPartyFacesUnit(NPC* unit)
{
	Rectangle unit_rect = unit->map_rect;
	if (!party) return false;
	string area_next_to = getRelativeDirectionToUnit(unit_rect, party);
	if (area_next_to == "left" && party->getDirection() == "right")
	{
		return true;
	}
	if (area_next_to == "right" && party->getDirection() == "left")
	{
		return true;
	}
	if (area_next_to == "up" && party->getDirection() == "down")
	{
		return true;
	}
	if (area_next_to == "down" && party->getDirection() == "up")
	{
		return true;
	}
	return false;
}

bool CollisionHandler::checkIfUnitFacesArea(NPC* unit, Rectangle area)
{
	string area_next_to = getRelativeDirectionToUnit(area, unit);
	if (area_next_to == "left" && unit->getDirection() == "right")
	{
		return true;
	}
	if (area_next_to == "right" && unit->getDirection() == "left")
	{
		return true;
	}
	if (area_next_to == "up" && unit->getDirection() == "down")
	{
		return true;
	}
	if (area_next_to == "down" && unit->getDirection() == "up")
	{
		return true;
	}
	return false;
}

void CollisionHandler::setLayer(int num)
{
	layer = num;
	setBlockers(num);
}

void CollisionHandler::setBlockers(int layer_pos)
{
	if (all_layer_blockers.find(layer_pos) != all_layer_blockers.end())
	{
		layer_blockers = all_layer_blockers[layer_pos];
	}
}

void CollisionHandler::updateAllLayerBlockers(unordered_map<int, unordered_map<string, vector<Rectangle>>> all_layer_blockers)
{
	this->all_layer_blockers = all_layer_blockers;
}


void CollisionHandler::update()
{
	updateWorldOffset();
	updateUnits();
}

void CollisionHandler::draw()
{
	//testDraw();
}


void CollisionHandler::updateUnits()
{
	this->party = level->party;
}

void CollisionHandler::updateWorldOffset()
{
	this->world_offset = level->map->world_offset;
}

//Test Methods
void CollisionHandler::testDraw()
{
	if (layer_blockers.size() > 0)
	{
		vector<string> accepted_blocker_sets = level->GetAcceptedBlockerSets();
		for (auto& [name, blockers] : layer_blockers)
		{
			if (find(accepted_blocker_sets.begin(), accepted_blocker_sets.end(), name) != accepted_blocker_sets.end())
			{
				for (auto blocker : blockers)
				{
					setBlockerScale(blocker);
					DrawRectangle((int)blocker.x, (int)blocker.y, (int)blocker.width, (int)blocker.width, PINK);
				}
			}
		}
	}
}
