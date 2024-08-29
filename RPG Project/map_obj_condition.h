#pragma once
#include <raylib.h>
#include "condition.h"

using namespace std;

class Maps;
class Level;
class EventController;
class CollisionHandler;
class EventManager;
class NPC;
class Game;

class MapObjectCondition : public Condition
{
public:
	//Setup
	virtual void setup(Json::Value data, Level* level, EventController* event_controller) override;
	virtual void parseData() override;

	virtual void update() override;
	virtual void draw() override;

	//Update methods
	void updateAreaRect();

	//Data
	int layer_pos = 0;
	string tile_change_name = "";
	Vector2 tile_location = { 0.f, 0.f };
	string sfx_name = "";
	Rectangle area_rect;
	Rectangle og_area_rect;
};