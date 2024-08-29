#pragma once
#include <raylib.h>
#include "condition.h"

using namespace std;

class Maps;
class Level;
class EventController;
class CollisionHandler;
class EventManager;
class Unit;
class Game;

class AreaCondition : public Condition
{
public:
	//Setup
	virtual void setup(Json::Value data, Level* level, EventController* event_controller) override;

	virtual void update() override;
	virtual void draw() override;
	
	//Area
	int getPortalID();
private:
	//Setup
	virtual void parseData() override;
	void setupAreaRect();

	//Data
	int PortalID;

	//Checkers
	void checkIfInRange();

	//Update Methods
	void updateAreaRect();

	//Ranged Events
	void Portal(Unit* unit);
	void InRange(Unit* unit);

	//Rectangles
	Rectangle og_area_rect;
	Rectangle area_rect;

	//Location Values
	int x1;
	int y1;
	int x2;
	int y2;
};
