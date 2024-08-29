#pragma once
#include <raylib.h>
#include "condition_controller.h"
#include "map_obj_condition.h"

using namespace std;

class Level;
class Cam;
class EventController;

class MapObjectController : public ConditionController
{
public:
	//Setup
	virtual void setup(Json::Value data, Level* level, EventController* event_controller) override;

	virtual void update() override;
	virtual void draw() override;

	//Setters
	void ResetAllDoors();
private:
	//Setup
	virtual void setupAllConditions() override;

	//Conditions
	vector<shared_ptr<MapObjectCondition>> obj_conditions;

	//Update Methods
	virtual void updateAllConditions() override;
};