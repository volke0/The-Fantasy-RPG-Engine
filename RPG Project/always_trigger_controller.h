#pragma once
#include <raylib.h>
#include "condition_controller.h"
#include "always_trigger_condition.h"

using namespace std;

class Level;
class Cam;

class AlwaysTriggerEventController : public ConditionController
{
public:
	//Setup
	virtual void setup(Json::Value data, Level* level, EventController* event_controller) override;

	//Update methods
	virtual void update() override;
	virtual void draw() override;
private:
	//Setup
	virtual void setupAllConditions() override;

	//Conditions
	vector<AlwaysTriggerCondition> always_conditions = {};

	//Update Methods
	virtual void updateAllConditions() override;
};