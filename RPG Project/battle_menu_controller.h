#pragma once
#include <raylib.h>
#include <memory>
#include <iostream>
#include <json/json.h>
#include "vfx_animation.h"
#include "controller.h"
#include "battle_ui.h"
#include "player_menu.h"
#include "unit_select.h"
#include "exp_menu.h"
#include "magic_menu.h"
#include "item_menu.h"
#include "battle_menu_state_controller.h"
#include "timer.h"
#include "sound_player.h"

using namespace std;

class Game;
class Party;
class NPC;
class Cursor;
class BattleController;

class BattleMenuController : public Controller
{
public:
	~BattleMenuController();
	void setup(Game* game, Party* party, unordered_map<string, shared_ptr<NPC>> combatants, BattleController* battle_controller);
	void update() override;
	void draw();

	//Setters
	void turnOn();
	void playSound(string sfx);

	//Data
	unordered_map<string, shared_ptr<NPC>> combatants;
	Json::Value input_history;

	//Setters
	void turnUnitSelectOn();
	void turnMagicMenuOn();
	void turnItemMenuOn();
	void changeFocus(string focus);
	void changeUnit();
	void GotoPreviousUnit();
	void pushState(string key);
	void eraseState(string key);
	void clearStates();
	void refreshCombatants(unordered_map<string, shared_ptr<NPC>> combatants);

	//Getters
	bool checkFocus(string key);

	//Data
	Cursor* cursor;

	//Player
	Unit* current_unit;
	string current_unit_name;
	void getPartyMembers();
	vector<Unit*> party_members;
	vector<string> party_member_names;
	int player_index = 0;

	//Menus
	ExpMenu exp_menu;
	PlayerUnitMenu player_menu;
	UnitSelectMenu unit_select;
	MagicMenu magic_menu;
	ItemMenu item_menu;

	//Timer
	Timer timer;

	//Logic
	bool on = false;
	bool displayPlayerMenu;
	bool displayMagicMenu;
	bool displaySpecialMenu;
	bool displayItemMenu;
	bool displayUnitSelection;
	bool displayExpMenu;
	bool exiting = false;
private:
	//Setup
	void setupAllMenus();

	//Update
	void updateAllMenus();

	//Draw
	void drawAllMenus();

	//Setters
	void startEnemyTurn();

	//Audio
	SoundPlayer sound_player;

	//Data
	BattleController* battle_controller;
	BattleMenuStateController state_controller;
	string focus;

	//Menus
	BattleUI battle_ui;
};

