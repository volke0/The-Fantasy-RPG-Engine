#pragma once
#include <raylib.h>
#include "condition_controller.h"
#include "area_condition.h"

using namespace std;

class Level;
class Cam;
class EventController;

class AreaEventController : public ConditionController
{
public:
	//Setup
	virtual void setup(Json::Value data, Level* level, EventController* event_controller) override;

	//Update methods
	virtual void update() override;
	virtual void draw() override;

	//Setters
	void ResetAllPortalConditions();
private:
	//Setup
	virtual void setupAllConditions() override;

	//Conditions
	vector<AreaCondition> area_conditions;

	//Update Methods
	virtual void updateAllConditions() override;
	virtual void updatePartyLocation() override;

	//Portal
	void findPortalCondition();
};
