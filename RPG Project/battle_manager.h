#pragma once
#include <raylib.h>
#include <memory>
#include <queue>
#include <json/json.h>
#include "controller.h"
#include "timer.h"

using namespace std;

class Game;
class PartyBase;
class BattleController;
class NPC;
class Battle;

class BattleManager : public Controller
{
public:
	void setup(Game* game, BattleController* battle_controller, unordered_map<string, shared_ptr<NPC>> combatants);
	void update() override;
	void draw();
	void refreshCombatants(unordered_map<string, shared_ptr<NPC>> combatants);

	//Setters
	void pushBattle(Game* game, Json::Value input);
	void startBattles();
	void clearAllBattles();
	void RemovePreviousBattle();

	//Getters
	bool isDone();

	//Logic
	bool pause = false;
	bool wrap_up = false;

private:
	//Data
	BattleController* battle_controller;
	Timer timer;

	//Units
	unordered_map<string, shared_ptr<NPC>> combatants;

	//Queue
	vector<shared_ptr<Battle>> battle_queue;
	unordered_map<int, string> priority_queue;

	//Setters
	void turnOff();
	void checkIfBattleDone();
	void reArrangeBattleQueue();
	void createPriorityQueue();

	//Logic
	bool on = false;
	bool done = false;
};
