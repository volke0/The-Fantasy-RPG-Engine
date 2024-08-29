#pragma once
#include <raylib.h>
#include "map_obj_condition.h"

using namespace std;

class Maps;
class Level;
class EventController;
class CollisionHandler;
class EventManager;
class NPC;
class Game;

struct DoorKey
{
	bool bRequiresEvent = false;
	bool bRequiresItem = false;
	string trigger_flag = "";
	string item_name = "";
};

class DoorObjCondition : public MapObjectCondition
{
public:
	//Setup
	virtual void setup(Json::Value data, Level* level, EventController* event_controller) override;

	virtual void update() override;
	virtual void draw() override;

	//Setters
	void ResetDoor();
private:
	//Setup
	virtual void parseData() override;
	void addToBlockers();

	//Update Methods
	void updateDoor();
	void TriggerDoor();
	void DoorInput();
	virtual void TimerUpdate() override;

	//Getters
	bool CheckIfHasKey();

	//Data
	bool bDoorLocked = false;
	bool bRemoveBlocker = false;
	DoorKey doorkey;
};
