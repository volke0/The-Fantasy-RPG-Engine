#pragma once
#include <raylib.h>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <string>
#include <algorithm>
#include <queue>

using namespace std;

class Unit;
class Maps;
class Level;
 
class PathFinder
{
public:
	void findPath(Unit* unit, Maps* Map, Vector2 destination, int speed = 16);
	bool done = false;

	//Update
	void update();

	//Test
	void draw();
private:
	//Setup
	void setupCoordinates();

	//Unit
	Unit* unit;

	//Map
	Maps* Map;

	//Level
	Level* level;

	//Path
	vector<Rectangle> path;
	int path_index = 0;
	bool path_ready = false;

	//Coordinates
	Vector2 start_coords;
	Vector2 destination;

	//Rectangles
	vector<Rectangle> area_rects;
	unordered_map<string, vector<Rectangle>> neighbors;

	//Update
	void walkPath();

	//A* algorithm
	float h(Rectangle start_pt, Rectangle end_pt);
	void generateGScores(unordered_map<string, float>& g_scores);
	void generateFScores(unordered_map<string, float>& f_scores);
	void AStar();
	void generatePath(vector<Rectangle> came_from);
	void drawPath();

	//Neighbors
	void findNearestNeighbors();
	void printNearestNeighbors(string tile_name);

	//Misc.
	void makeAreaGrid();
	void addToCameFrom(Rectangle& tile, vector<Rectangle>& came_from);
	bool foundInOpenSet(string key, vector<tuple<int, int, Rectangle>> open_set);
	int findLowesetFScoreTile(vector<tuple<int, int, Rectangle>> open_set);
	bool findIfTileInAreaRects(Rectangle tile);
};
