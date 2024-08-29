#pragma once
#include <raylib.h>
#include <memory>
#include <vector>
#include <map>
#include "timer.h"
#include "vars.h"
#include "lvl_up_screen_handler.h"
#include "menu.h"


using namespace std;

class Game;
class Party;
class Unit;
class BattleController;
class BattleMenuController;
class Text;

class LvLUpScreen : public Menu
{
public:
	void setup(Game* game, Party* party, Unit* unit, int num_of_lvl, BattleController* battle_controller, BattleMenuController* menu_controller);
	void update();
	void draw();

	//Timer
	Timer timer;

	//Handler
	LvlUpScreenHandler handler;

	//Data
	Unit* unit;
	int num_of_lvl = 0;

	//Stats
	unordered_map<string, int> unit_stats;
	unordered_map<string, int> grown_stats;
	vector<vector<string>> stat_names = {
										{"max_hp", "HP"},
										{"max_mp", "MP"},
										{"str", "STR"},
										{"mag", "MAG"},
										{"skl", "SKL"},
										{"spd", "SPD"},
										{"lck", "LCK"},
										{"def", "DEF"},
										{"res", "RES"},
	};

	//Logic
	bool start = true;
	bool show_grown_stats = false;
	bool grow_stats = false;
	bool grow_stats_done = false;
	bool done_updating = false;
	bool done = false;
private:
	//Draw
	void drawLvlHeader();
	void drawLvLBox();
	void drawUnitPortrait();
	void drawName();
	void drawStatus();
	void drawNormalStats(string key, Vector2 stat_pos);
	void drawGrownStats(string key, Vector2 stat_pos);

	//Data
	Text* text_drawer;
};
