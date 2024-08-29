#pragma once
#include <raylib.h>
#include <memory>
#include <fstream>
#include <string>
#include <unordered_map>
#include <iostream>
#include <json/json.h>
#include <memory>
#include "controller.h"
#include "event.h"

using namespace std;

class Party;
class Game;
class Maps;
class Level;
class Event;
class AreaEventController;
class TalkEventController;
class MapObjectController;
class AlwaysTriggerEventController;

class EventController : public Controller
{
public:
	void update() override;
	void draw();

	//Setup
	void setup(Json::Value script, Level* level);

	//Data
	string name;

	//Events
	unordered_map<string, Event> events;
	string current_event = "OpeningEvent";

	//Update
	void AddPartyToAllEvents();

	//Getters
	bool isEventDone(string name, string type = "global");

	//Setters
	void setLevel(Level* level);
	void ResetAllDoors();
	void runEvent(string event_name);
	void EndEvent(string event_name);
	void runEvent(string event_name, string previous_event, int previous_index);
	void runEventNoReset(string event_name);

	//Logic
	bool busy = false;
	bool runningEvent = false;
private:
	//Script
	Json::Value main_script;
	Json::Value opening_script;

	//Controllers
	shared_ptr<AreaEventController> area_controller;
	shared_ptr<TalkEventController> talk_controller;
	shared_ptr<MapObjectController> map_obj_controller;
	shared_ptr<AlwaysTriggerEventController> always_event_controller;
	void updateAllControllers();

	//Setup
	void makeAllEvents();
	void setupEventScript();
	void setupEvent(string name, Json::Value script);
	void setupAllControllers();

	//Others
	void checkIfBusy();
	void resetEvent(string event_name);
};
