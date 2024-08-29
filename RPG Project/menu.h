#pragma once
#include <raylib.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <json/json.h>
#include "vars.h"
#include "sound_player.h"

using namespace std;

class Game;
class Party;
class Unit;
class Cursor;
class BattleController;
class BattleMenuController;

class Menu
{
public:
	virtual void basicSetup(Game* game, Party* party, BattleController* battle_controller, BattleMenuController* menu_controller);
	virtual void update();
	virtual void draw();

	float dropshadow_offset = (float)1 + (TILE / 2 / 15);
	void drawText(Vector2 pos, string text, float fontSize, Color color = { 255, 255, 255, 255 });
	
	Game* game;
	Party* party;
	BattleController* battle_controller;
	Cursor* cursor;
	BattleMenuController* menu_controller;
	vector<string> options = {};
};
