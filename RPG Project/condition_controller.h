#pragma once
#include <raylib.h>
#include <string>
#include <json/json.h>
#include <iostream>
#include <vector>
#include "controller.h"

using namespace std;

class Level;
class Cam;
class EventController;

class ConditionController : public Controller
{
public:
	//Setup
	virtual void setup(Json::Value data, Level* level, EventController* event_controller);
	virtual void setupAllConditions();

	//Update methods
	virtual void update() override;
	virtual void updateAllConditions();
	virtual void updatePartyLocation();

	//Draw methods
	virtual void draw();

	//Event Controller
	EventController* event_controller;

	//Portal
	int event_id;
	int map_id;

	//Data
	Json::Value data;
	Level* level;
	Cam* camera;
};