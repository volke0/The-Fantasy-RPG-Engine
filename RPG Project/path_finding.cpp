#include "path_finding.h"
#include "map.h"
#include "level.h"
#include "unit.h"
#include "vars.h"

void PathFinder::findPath(Unit* unit, Maps* Map, Vector2 destination, int speed)
{
	this->unit = unit;
	this->level = unit->level;
	this->Map = Map;
	this->destination = destination;
	this->unit->movement_controller.setMoveSpeed(speed);
	done = false;
	path_ready = false;
	path_index = 0;
	this->unit->followingPath = true;

	setupCoordinates();
	makeAreaGrid();
	findNearestNeighbors();
	AStar();
	//cout << "START X: " << start_coords.x / TILE << " START Y: " << start_coords.y / TILE << endl;
	//cout << "DESTINATION X: " << destination.x << " DESTINATION Y: " << destination.y << endl;
	//cout << "DIST X: " << traveling_offset.x / TILE << " DIST Y: " << traveling_offset.y / TILE << endl;
}

void PathFinder::update()
{
	if (unit != nullptr)
	{
		walkPath();
	}
}

void PathFinder::walkPath()
{
	if (unit->followingPath == true)
	{
		if (path_ready == true)
		{
			Rectangle tile = { path[path_index].x, path[path_index].y, (float)TILE, (float)TILE };
			//cout << "UNIT: " << unit->true_offset[0] / (float)TILE << "," << unit->true_offset[1] / (float)TILE << endl;
			//cout << "TILE: " << tile.x / (float)TILE << "," << tile.y / (float)TILE << endl;
			if (unit->movement_controller.moving == false)
			{
				if (unit->true_offset[0] == tile.x && unit->true_offset[1] == tile.y)
				{
					if (path_index < path.size() - 1)
					{
						path_index++;
					}
					else
					{
						path_ready = false;
						unit->followingPath = false;
						done = true;
					}
				}
				else
				{
					if (unit->true_offset[0] != tile.x && unit->true_offset[1] == tile.y)
					{
						if (tile.x > unit->true_offset[0])
						{
							unit->move_list.push_back("right");
						}
						if (tile.x < unit->true_offset[0])
						{
							unit->move_list.push_back("left");
						}
					}
					if (unit->true_offset[1] != tile.y && unit->true_offset[0] == tile.x)
					{
						if (tile.y > unit->true_offset[1])
						{
							unit->move_list.push_back("down");
						}
						if (tile.y < unit->true_offset[1])
						{
							unit->move_list.push_back("up");
						}
					}
				}
			}
		}
	}
}



void PathFinder::draw()
{
	if (Map != nullptr)
	{
		//cout << Map->world_offset[0] / TILE << endl;
	}
	for (Rectangle rect : area_rects)
	{
		rect.x += Map->world_offset[0];
		rect.y += Map->world_offset[1];
		DrawRectangle((int)rect.x, (int)rect.y, TILE, TILE, GREEN);
	}
	drawPath();
}

void PathFinder::setupCoordinates()
{
	start_coords = { (unit->true_offset[0]), (unit->true_offset[1]) };
}

void PathFinder::makeAreaGrid()
{
	area_rects.clear();
	area_rects.shrink_to_fit();
	for (int i = 0; i < (int)Map->size.x; i++)
	{
		for (int k = 0; k < (int)Map->size.y; k++)
		{
			float x = ((float)i * TILE);
			float y = ((float)k * TILE);

			Rectangle temp_rect = { (float)x, (float)y, (float)TILE, (float)TILE };
			Rectangle compare_rect = { temp_rect.x + Map->world_offset[0], temp_rect.y + Map->world_offset[1], (float)TILE, (float)TILE };
			if (level->collision_handler.checkCollisionWithObjects(compare_rect) == false)
			{
				//cout << "X: " << x / TILE << " Y: " << y / TILE << endl;
				area_rects.push_back(temp_rect);
			}
		}
	}
	//cout << "AREA RECT SIZE: " << area_rects.size() << endl;
}


void PathFinder::findNearestNeighbors()
{
	neighbors.clear();
	for (Rectangle tile : area_rects)
	{
		string tile_name = to_string((int)tile.x / TILE) + "," + to_string((int)tile.y / TILE);
		neighbors[tile_name] = {};

		//Neighbor tiles
		Rectangle right = { tile.x + (float)TILE, tile.y, (float)TILE, (float)TILE };
		Rectangle left = { tile.x - (float)TILE, tile.y, (float)TILE, (float)TILE };
		Rectangle up = { tile.x, tile.y - (float)TILE, (float)TILE, (float)TILE };
		Rectangle down = { tile.x, tile.y + (float)TILE, (float)TILE, (float)TILE };

		//neighbors[tile_name].push_back(tile);
		if (findIfTileInAreaRects(right))
		{
			neighbors[tile_name].push_back(right);
		}
		if (findIfTileInAreaRects(left))
		{
			neighbors[tile_name].push_back(left);
		}
		if (findIfTileInAreaRects(up))
		{
			neighbors[tile_name].push_back(up);
		}
		if (findIfTileInAreaRects(down))
		{
			neighbors[tile_name].push_back(down);
		}
		//printNearestNeighbors(tile_name);
	}
}


//------------A* star--------------//
float PathFinder::h(Rectangle start_pt, Rectangle end_pt)
{
	float x1 = start_pt.x;
	float x2 = end_pt.x;
	float y1 = start_pt.x;
	float y2 = start_pt.y;
	return abs(x1 - x2) + abs(y1 - y2);
}

void PathFinder::generateGScores(unordered_map<string, float>& g_scores)
{
	for (Rectangle tile : area_rects)
	{
		string tile_name = to_string((int)tile.x / TILE) + "," + to_string((int)tile.y / TILE);
		g_scores[tile_name] = INFINITY;
	}
}

void PathFinder::generateFScores(unordered_map<string, float>& f_scores)
{
	for (Rectangle tile : area_rects)
	{
		string tile_name = to_string((int)tile.x / TILE) + "," + to_string((int)tile.y / TILE);
		f_scores[tile_name] = INFINITY;
	}
}

void PathFinder::AStar()
{
	int count = 0;
	string start_name = to_string((int)start_coords.x / TILE) + "," + to_string((int)start_coords.y / TILE);
	string end_name = to_string((int)destination.x) + "," + to_string((int)destination.y);
	Rectangle start_rect = { (float)start_coords.x, (float)start_coords.y, (float)TILE, (float)TILE };
	Rectangle end_rect = { (float)destination.x * (float)TILE, (float)destination.y * (float)TILE, (float)TILE, (float)TILE };

	//cout << "START: " << start_name << endl;
	//cout << "END: " << end_name << endl;

	vector<tuple<int, int, Rectangle>> open_set;
	tuple node = {0, count, start_rect };
	path.clear();
	path.shrink_to_fit();

	open_set.push_back(node);

	unordered_map<string, float> g_scores = {};
	unordered_map<string, float> f_scores = {};
	generateGScores(g_scores);
	generateFScores(f_scores);
	g_scores[start_name] = 0;
	f_scores[start_name] = h(start_rect, end_rect);

	while (open_set.size() > 0)
	{
		int current_index = findLowesetFScoreTile(open_set);
		Rectangle current_tile = get<2>(open_set[current_index]);
		string current_node = to_string((int)current_tile.x / TILE) + "," + to_string((int)current_tile.y / TILE);

		if (current_tile.x == end_rect.x && current_tile.y == end_rect.y)
		{
			path.push_back(current_tile);
			path_ready = true;
			cout << "DONE!" << endl;
			break;
		}

		//Erase the current tile from the open set
		open_set.erase(open_set.begin() + current_index);

		
		for (auto& neighbor : neighbors[current_node])
		{
			string current_neighbor = to_string((int)neighbor.x / TILE) + "," + to_string((int)neighbor.y / TILE);
			float temp_g_score = g_scores[current_node] + 1;
			if (temp_g_score < g_scores[current_neighbor])
			{
				addToCameFrom(current_tile, path);
				g_scores[current_neighbor] = temp_g_score;
				f_scores[current_neighbor] = temp_g_score + h(neighbor, end_rect);
				if (foundInOpenSet(current_neighbor, open_set) == false)
				{
					count++;
					tuple temp_node = { (int)f_scores[current_neighbor], count, neighbor };
					open_set.push_back(temp_node);
				}
			}
		}
	}
}

void PathFinder::generatePath(vector<Rectangle> came_from)
{
	for (auto& tile : came_from)
	{
		string name = to_string((int)tile.x / TILE) + "," + to_string((int)tile.y / TILE);
		cout << name << endl;
	}
}

void PathFinder::drawPath()
{
	if (path.size() > 0)
	{
		for (Rectangle tile : path)
		{
			tile.x += Map->world_offset[0];
			tile.y += Map->world_offset[1];
			DrawRectangle((int)tile.x, (int)tile.y, TILE, TILE, DARKPURPLE);
		}
	}	
}



//-----Others-----//
void PathFinder::addToCameFrom(Rectangle& tile, vector<Rectangle>& came_from)
{
	if (came_from.size() > 0)
	{
		bool has_tile_already = false;
		for (auto& temp_tile : came_from)
		{
			if (tile.x == temp_tile.x && tile.y == temp_tile.y)
			{
				has_tile_already = true;
			}
		}
		if (has_tile_already == false)
		{
			came_from.push_back(tile);
		}
	}
	else
	{
		came_from.push_back(tile);
	}
}

bool PathFinder::foundInOpenSet(string key, vector<tuple<int, int, Rectangle>> open_set)
{
	for (int index = 0; index < open_set.size(); index++)
	{
		Rectangle current_tile = get<2>(open_set[index]);
		string current_name = to_string((int)current_tile.x / TILE) + "," + to_string((int)current_tile.y / TILE);
		if (current_name == key)
		{
			return true;
		}
	}
	return false;
}

int PathFinder::findLowesetFScoreTile(vector<tuple<int, int, Rectangle>> open_set)
{
	int lowest_f_score = 99999999;
	int lowest_index = 0;
	for (int index = 0; index < open_set.size(); index++)
	{
		auto& container = open_set[index];
		if (get<0>(container) < lowest_f_score)
		{
			lowest_f_score = get<0>(container);
			lowest_index = index;
		}
	}
	return lowest_index;
}

void PathFinder::printNearestNeighbors(string tile_name)
{
	cout << tile_name << "'s neighbors: ";
	for (auto& tile : neighbors[tile_name])
	{
		cout << to_string((int)tile.x / TILE) + "," + to_string((int)tile.y / TILE) << " ";
	}
	cout << endl;
}

bool PathFinder::findIfTileInAreaRects(Rectangle tile)
{
	for (auto& temp_tile : area_rects)
	{
		if (temp_tile.x == tile.x && temp_tile.y == tile.y)
		{
			return true;
		}
	}
	return false;
}
