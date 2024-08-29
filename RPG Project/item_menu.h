#pragma once

#include <raylib.h>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include "menu.h"

using namespace std;

class Game;
class Party;
class Unit;
class BattleCursor;
class BattleController;
class BattleMenuController;

class ItemMenu : public Menu
{
public:
	void setup(Game* game, Party* party, BattleController* battle_controller, BattleMenuController* menu_controller);
	virtual void update() override;
	virtual void draw() override;

	//Setters
	void turnOn();
	void makeAllItemsVisible();
	void makeConsumablesVisibleOnly();
private:
	//Draw
	void drawItemMenuBox();
	void drawItems();
	void drawItemDescription(Json::Value item);

	//Update
	void updateVisibleOptions();

	//Setters
	void makeOptions();
	vector<Json::Value> makeVisibleOptions();

	//Cursor
	Vector2 og_pos = { 2.0f * TILE , 9.5f * TILE + 7.0f };

	//Menu
	int vis_index = 0;
	int memory_index = 0;
	int memory_vis_index = 0;
	int memory_cursor_vis_index = 0;

	//Data
	BattleCursor* b_cursor;
	Unit* current_unit;
	int menu_index = 0;

	//Logic
	bool showUnusableItems = false;
};