#pragma once
#include <raylib.h>
#include <string>
#include <iostream>
#include <json/json.h>
#include <cstdlib>

using namespace std;

class Maps;
class Level;
class EventController;
class CollisionHandler;
class EventManager;
class NPC;
class Party;
class Game;

class Condition
{
public:
	//Setup
	virtual void setup(Json::Value data, Level* level, EventController* event_controller);
	virtual void parseData();

	virtual void update();
	virtual void draw();

	//Update methods
	void RunEvent();
	virtual void TimerUpdate();

	//Getters
	vector<float> getWorldOffset();

	//Setters
	void ResetCondition();

	//Event
	string event_link;
	int next_level_id;
	void push_level_portal(int num);

	//Flags
	string Flag;
	string condition_flag = "";
	string global_condition_flag = "";
	bool bHasConditionFlag = false;
	bool bHasGlobalConditionFlag = false;
	bool event_read = false;

	//Portal stuff
	Vector2 exitCoords;
	string direction = "up";
	Vector2 getExitCoords();
	string getDirection();

	//Helpers
	string GetUnitName(Json::Value line);

	//Logic
	bool done = false;
	bool trigger_event = false;
	bool bTextRunning = false;
	bool bCanInput = true;
	int timer = 0;

	//Data
	string type;
	Json::Value data;
	Maps* map;
	Level* level;
	CollisionHandler* collision_handler;
	EventController* event_controller;
	EventManager* event_manager;
	Game* game;
	Party* party;
	int next_portal_id;
};
