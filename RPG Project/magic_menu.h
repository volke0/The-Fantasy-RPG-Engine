#pragma once
#include <memory>
#include "menu.h"
#include "vars.h"
#include <json/json.h>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

using namespace std;

class Game;
class Party;
class Unit;
class BattleCursor;
class BattleController;
class BattleMenuController;

class MagicMenu : public Menu
{
public:
	void setup(Game* game, Party* party, BattleController* battle_controller, BattleMenuController* menu_controller);

	void update();
	void draw();

	//Setters
	void turnOn();
private:
	//Update
	void updateVisibleOptions();
	void selectMagic();
	void moveToUnitSelect(Json::Value temp_magic);

	//Draw
	void drawMagicOptionBox();
	void drawMagicOptions();

	//Setters
	void setCurrentPlayer();
	void moveMenuUp();
	void moveMenuDown();
	
	//Getters
	vector<string> getMagicOptionList();

	//Cursor
	Vector2 og_pos = {};
	int menu_vis_index = 0;
	int prev_menu_vis_index = 0;
	int menu_index = 0;

	//Menu
	vector<string> makeVisbleOptions();
	void drawMagicDetails(vector<string> options);
	void drawMagicDescription(string name, vector<string> options, Vector2 last_pos);

	//Data
	vector<string> visual_options = {};
	Unit* current_unit = nullptr;
	BattleCursor* b_cursor = nullptr;
};