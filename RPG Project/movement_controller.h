#pragma once
#include <raylib.h>
#include <cmath>
#include "controller.h"
#include "path_finding.h"

using namespace std;

class Unit;
class Cam;

class MovementController : public Controller
{
public:
	//Setup
	void setup(Unit* unit);
	void update() override;
	void setCamera(Cam* camera);

	//Movement
	void move(vector<int> direction, float speed = 0, bool ignore_map_rect = false);
	float move_speed;
	float tile_speed;
	float og_move_speed;

	//Location
	void setMoveSpeed(int speed);
	void setLocation(Vector2 offset);
	void updateMapRect(vector<float> world_offset);

	//Path Finding
	PathFinder path_finder;

	//Logic
	bool moving = false;
	bool onTile = true;
	bool fixing = false;
	bool canRun = false;
	Rectangle newLocation;
	string last_direction;

	//Path
	void findPath(Vector2 destination, int speed = 16);

	//Checkers
	bool isPathDone();
private:
	//Unit
	Unit* unit;
	Cam* camera;
	bool isParty = false;

	//Setters
	void setLastMove();
	void setRunning();
	void addMoveSpeed(float speed, string type, int direction);
	
	//Update
	void otherUnitMode();
	void updateOtherUnitMove();
	void partyMode();
	void updateMapMode();
	void updatePartyMapMode();
	void updatePartyMove();

	//Location
	void fixLocation();
	void updateTrueOffset(Vector2 offset);

	//Data
	vector<float> total_moved_true = {0.f, 0.f};
	vector<float> total_moved_rect = {0.f, 0.f};
};
