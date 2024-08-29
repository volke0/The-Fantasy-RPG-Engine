#pragma once
#include <raylib.h>
#include <memory>
#include "vfx_animation.h"
#include "battle_menu.h"
#include "vars.h"

using namespace std;

class Game;
class Party;
class Unit;
class NPC;
class BattleCursor;
class BattleController;
class BattleMenuController;

class PlayerUnitMenu : public BattleMenu
{
public:
	void setup(Game* game, Party* party, BattleController* battle_controller, BattleMenuController* menu_controller);
	void update();
	void draw();

	//Setters
	void setCurrentPlayer();
	void turnPlayerMenuOn();
	void deleteAnimations();

	//Logic
	bool party_done = true;
	unordered_map<string, vector<string>> decisions;
private:
	Unit* current_unit;
	unordered_map<string, shared_ptr<NPC>> combatants;

	//Animations
	shared_ptr<VFXAnimation> char_highlighter;

	//Cursor
	Vector2 og_pos = { (float)2 * TILE, (float)9 * TILE + 19 };

	//Logic
	int party_pos = -1;
	bool switching_char = false;

	//Options
	vector<string> total_options = { "Attack", "Special", "Magic", "Item", "Defend"};
	vector<bool> selectable_options = {};
	void FindSelectableOptions();
	string option = "";

	//Update
	void checkForAttack();
	void checkForMagic();
	void checkForItem();

	//Draw
	void drawPlayerCombatBox();
	void drawPlayerOptions();
};
