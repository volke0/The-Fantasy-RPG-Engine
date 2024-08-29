#pragma once
#include <raylib.h>
#include <memory>
#include <vector>
#include "vfx_animation.h"
#include "battle_menu.h"

using namespace std;

class Game;
class Party;
class Unit;
class NPC;
class Cursor;
class BattleController;
class BattleMenuController;

class UnitSelectMenu : public BattleMenu
{
public:
	void setup(Game* game, Party* party, BattleController* battle_controller, BattleMenuController* menu_controller);
	void update();

	//Setters
	void turnOn();
	void refreshCombatants();
	void refreshEnemyList();
private:
	//Setters
	void sendInput();
	void cancelInput();
	void refreshAllyList();
	void setAppropiatePlayerReadyStatus();
	void playerReady();
	void AddTargetsToList(Json::Value& target_list, Json::Value& other_targets);
	void SwitchToMultiTarget();

	//Checkers
	void checkIfAttack(string current_name);
	void checkIfSpecial(string current_name);
	void checkIfMagic(string current_name);
	void checkIfItem(string current_name);

	//Options
	int index = 0;

	//Data
	Unit* target = nullptr;
	unordered_map<string, shared_ptr<NPC>> combatants = {};
	vector<Unit*> enemies = {};
	vector<string> enemy_names = {};
	vector<Unit*> allies = {};
	vector<string> ally_names = {};
	vector<string> current_selection = {};

	//Logic
	bool bMultiTarget = false;
};
