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

class TalkCondition : public Condition
{
public:
	//Setup
	virtual void setup(Json::Value data, Level* level, EventController* event_controller) override;

	virtual void update() override;
	virtual void draw() override;
private:
	//Setup
	virtual void parseData() override;

	//Update methods
	void attemptToGetUnit();
	void CheckIfTalkValid();

	//Getter
	NPC* getUnit();

	//Data
	string unit_name = "";
	NPC* unit;

	//Logic
	bool loaded_unit = false;
};
