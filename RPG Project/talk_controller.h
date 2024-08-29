#pragma once
#include <raylib.h>
#include "condition_controller.h"
#include "talk_condition.h"

using namespace std;

class Level;
class Cam;
class EventController;

class TalkEventController : public ConditionController
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
	vector<TalkCondition> talk_conditions = {};

	//Update Methods
	virtual void updateAllConditions() override;
};

