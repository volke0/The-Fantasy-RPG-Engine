#pragma once
#include <raylib.h>
#include <memory>
#include <vector>
#include "vfx_animation.h"
#include "exp_menu_handler.h"
#include "sound_player.h"
#include "menu.h"

using namespace std;

class Game;
class Party;
class Unit;
class BattleController;
class BattleMenuController;

class ExpMenu : public Menu
{
public:
	~ExpMenu();
	void setup(Game* game, Party* party, BattleController* battle_controller, BattleMenuController* menu_controller);
	void update();
	void draw();

	//Getters
	void unloadPartyFaces();

	//Values
	unordered_map<string, float> exp_ratios;
	float exp_per_unit;
	unordered_map<string, unordered_map<string, int>> value_memory;

	//Sound
	SoundPlayer sfx_player;

	//Data
	vector<Texture2D> faces;
	unordered_map<string, int> unit_order;
	Color total_silver_color = GRAY;
private:
	//Draw
	void drawExpFaces();
	void drawExpBars(Unit* unit, Rectangle pos, int& num);
	void drawSilverAmount();

	//Getters
	void loadPartyFaces();

	//Update
	ExpMenuHandler handler;

	//Data
	Text* text_drawer;
	
	//Logic
	bool loadedFacesOnce = false;
	bool clearedFaces = false;
};
