#pragma once
#include <raylib.h>
#include <string>
#include <unordered_map>
#include <memory>
#include <json/json.h>
#include "database.h"

using namespace std;

class Game;
class LevelDatabase;
class EventController;
class Level;

class EventManager : public Database
{
public:
	void setup(Game* game) override;
	void update();
	void draw();

	//Setters
	void runEvent(string controller, string event_name);
	void EndEvent(string controller, string event_name);
	void InitController(Level* level, bool bUseCurrentController = false);
	void SetController(string controller_name, Level* level);

	//Checkers
	bool checkIfBusy();

	//Database
	unordered_map<string, shared_ptr<EventController>> controllers;
private:
	//Setters
	void setLevel(Level* level, bool bUseCurrentController = false);

	//Data
	LevelDatabase* level_database;
	string current_controller;

	//Json
	Json::Reader reader;
};