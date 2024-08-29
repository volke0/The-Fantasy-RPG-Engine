#pragma once
#include <raylib.h>
#include <unordered_map>
#include <memory>
#include <string>
#include <iostream>
#include <json/json.h>
#include <algorithm>
#include "unit_block_handler.h"

using namespace std;

class Party;
class Unit;
class Game;
class Level;
class Cam;
class Flags;
class Maps;
class Fades;
class Timer;
class EventController;
class MusicPlayer;
class VFXAnimation;

class Event
{
public:
	//Setup
	void setup(string name, Json::Value script, EventController* event_controller, Level* level);
	void play();
	void initialize();

	//Update
	void update();
	void reset();

	//Getters
	Party* getParty();
	Level* GetLevel();
	Maps* GetMap();

	//Setters
	void setPreviousEvent(string previous_event, int previous_index);
	void setLevel(Level* level);
	void setParty(Party* party);
	void SetUnit(Unit* unit);
	void SetTarget(Unit* target);

	//Script
	Json::Value script;
	string name;

	//Logic
	bool done = false;

	//Data
	unordered_map<string, shared_ptr<Unit>> temp_units;
private:
	//Data
	Game* game;
	EventController* event_controller;
	Party* party;
	Maps* map;
	Level* level;
	Fades* fade;
	Timer* timer;
	Flags* flag_sys;
	Cam* camera;
	MusicPlayer* music_player;

	//Functions
	void STAL();
	void FADE();
	void LOAD1();
	void LOAD4();
	void TEXTSHOW();
	//---Party
	void LOAD_PARTY();
	void AUTPARTY();
	void ADDITEM();
	void REMOVE_ITEM();
	void GIVE_MONEY();
	void REMOVE_MONEY();
	//---Music
	void MUSC();
	void MUSFI();
	void MUSNF();
	void CHG_BGM();
	void RETURN_TO_BGM();
	//--Level
	void CHANGE_LEVEL();
	//---Map
	void TILECHANGE();
	void TILEREVERSE();
	void ADD_BLOCKER_SET();
	void REMOVE_BLOCKER_SET();
	//---Battle
	void BATTLE();
	//---Shop
	void SHOP();
	//---Event Call
	void CALL();
	//---Camera
	void CAM1();
	void CAM2();
	//---Unit
	void MOVE();
	void REPOS();
	void CLEAR();
	void CLEAR_PARTY();
	void CLEE();
	void CLEN();
	void CLEAR_UNIT();
	void REMU();
	void REVEAL();
	void DEPART();
	void RETURN_TO_PARTY();
	void CHG_LAYER();
	void CHG_ANIMATION();
	void CHG_DIRECTION();
	void FACE_UNIT();
	void UNLOCK_ANIM();
	//---VFX
	void SHOW_VFX();
	//--Debug
	void PRINT();
	//---Logic
	void BEQ();
	void GOTO();
	void ENUT();
	void ENUF();
	void LABELS();
	void NULL_FUNC();
	void ENDA();

	//Line
	Json::Value current_line;

	//Units
	void loadUnit();
	void loadUnitLocation();

	//Helpers
	string GetUnitName(Json::Value line);
	UnitBlockHandler unit_block_handler;

	//Camera
	void findCameraSpeed(int& speed);

	//Labels
	int LABEL;
	void findLabel();

	//Values
	bool VALUE1;
	bool VALUE2;

	//Event linking
	string event_link;
	string previous_event;
	int previous_index;
	bool moveToPreviousEvent = false;

	//Text
	string text;
	int text_index;

	//VFX
	VFXAnimation* vfx;

	//Getters
	bool getFlag(string name, string type = "global");
	Unit* findUnit(string name);

	//Checkers

	//Logic
	bool line_read = false;
	bool line_done = false;
	int max_index = 0;
	int index = 0;

	//LOAD4 Variables
	Unit* UNIT;
	Unit* TARGET;
};