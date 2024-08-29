#pragma once
#include <raylib.h>
#include <unordered_map>
#include <string>
#include <vector>
#include <unordered_set>
#include "timer.h"
#include "lvl_up_screen.h"
#include "handler.h"

using namespace std;

class Game;
class Unit;
class ExpMenu;
class Party;

class ExpMenuHandler : public Handler
{
public:
	void setup(Game* game, Party* party, ExpMenu* exp_menu);
	void update();
	void draw();
private:
	//Update Exp
	void prepareMemory();
	void prepareExpRatio();
	void updateExp();
	void updateExpRatio(Unit* unit);
	void addExp(Unit* unit);
	void checkIfExpDone();
	void addExpToUnit(Unit* unit, string exp_type);
	void skipExpAnimations();

	//Level
	void updateLevel();
	void drawLevel();
	void checkIfLevelUpDone();

	//Update Silver
	void updateSilver();

	//Update
	void checkIfDone();

	//Getter
	void getLvlUpScreenPosition();
	void generateLvLUpScreens();

	//Calculation
	void calculateExpPerUnit();

	//Data
	Game* game;
	ExpMenu* exp_menu;
	Party* party;

	//Level Up
	vector<shared_ptr<LvLUpScreen>> lvl_up_screens;
	int lvl_up_pos = 0;

	//Memory
	unordered_map<string, int> level_up_amount;

	//Timer
	Timer timer;

	//Logic
	bool start = true;
	bool need_to_lvl_up = false;
	bool lvl_up_screen_on = false;
	bool needs_generation = false;
	bool exp_done = false;
	bool lvl_done = false;
	bool slv_done = false;
	bool timer_done = false;
	bool skip = false;
	bool ready_for_lvl_up = false;
};