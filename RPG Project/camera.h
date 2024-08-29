#pragma once
#include <raylib.h>
#include <vector>
#include <iostream>
#include "vars.h"

using namespace std;

class Maps;
class Party;
class Unit;
class Input;
class Level;

class Cam
{
public:
	void update();
	void setUpCamera(Level* level, Input* input);
	void changeMode();
	void setupPartyCamera(Party* party);

	//Move Camera
	void moveCamera(vector<int> direction);

	//Logic
	bool done_moving = false;
	bool updatingMap = true;
	bool followPlayer = true;

	//Auto Camera
	void autoCameraLocation(Vector2 location, int speed, bool bWarpToLocation = false);
	void autoCameraUnit(Unit* unit, int speed, bool bWarpToLocation = false);
	bool cam1_mode = false;
	bool autoCamDone = false;

	//Fixing
	bool fixing_world_offsets = false;
private:
	//Rect
	Rectangle map_rect = { 0, 0, (float)TILE, (float)TILE };

	//Direction
	vector<int> direction = { 0, 0 };

	//Camera Speed
	float cam_speed = 1;
	float tile_speed = 48;
	void setCameraSpeed();

	//Methods
	void playerUpdate();
	void keepInMap();
	void updateCAM1();
	void updateMap();
	void fixWorldOffset();
	void updateLocation(vector<int> direction, float dist, bool ignore_party = false);

	//Auto Camera
	void gotoLocation(Vector2 location, int speed = 16, bool bWarpToLocation = false);
	void calculateMoveData(float x, float y);
	void moveCAM1();
	Vector2 targetLocation;
	vector<float> auto_world_offset = { 0.0f, 0.0f };
	bool autoCamOnParty = false;
	bool onLocation = false;

	//Map
	Maps* map;
	void CalculateMaxWorldOffsets();

	//Party
	Party* party;

	//Level
	Level* level;

	//Input
	Input* input;

	//Offsets
	vector<float> true_offset = { 0.f, 0.f };
	vector<float> goal = { 0.f, 0.f };
	vector<float> max_world_offsets = { 0.f, 0.f };

	//Logic
	bool moved = false;
	bool posXDone = false;
	bool negXDone = false;
	bool posYDone = false;
	bool negYDone = false;
	bool bWarpToLocation = false;
	void isDoneMoving();
};
