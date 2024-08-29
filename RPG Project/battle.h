#pragma once
#include <raylib.h>
#include <string>
#include <memory>
#include <unordered_map>
#include <vector>
#include <json/json.h>
#include "calculation_handler.h"
#include "magic_calculation_handler.h"
#include "item_calculation_handler.h"
#include "magic.h"
#include "btl_animation_manager.h"

using namespace std;

class Game;
class NPC;
class BattleManager;
class PartyBase;

class Battle
{
public:
	~Battle();
	void setup(Game* game, BattleManager* battle_manager, Json::Value input, unordered_map<string, shared_ptr<NPC>> combatants, int position);
	void play();
	void draw();

	//Update
	void updateCombatants(unordered_map<string, shared_ptr<NPC>> combatants);

	//Battle Variables
	vector<float> hit_rates = {};
	vector<float> crit_rates = {};
	vector<bool> hit = {};
	vector<bool> crit = {};
	vector<float> dmg = {};
	vector<shared_ptr<Magic>> magic_list = {};
	vector<shared_ptr<class Consumable>> item_list = {};
	float heal = 0.f;

	//Handlers
	CalculationHandler btl_calculator;
	MagicCalculationHandler mag_calculator;
	ItemCalculationHandler item_calculator;

	//Data
	Unit* user;
	PartyBase* party;
	shared_ptr<Magic> current_magic = {};
	shared_ptr<class Consumable> current_item = {};
	vector<NPC*> other_targets = {};
	unordered_map<string, shared_ptr<NPC>> combatants = {};

	//Input
	Json::Value input;
	string mode = "";
	string user_name = "";
	Json::Value target_names;
	string magic_input = "";
	string magic_type = "";

	//Logic
	bool done = false;
	bool bMultiTarget = false;
private:
	//Battle Manager
	BattleManager* battle_manager = nullptr;

	//Preparations
	void prepareBattle();
	void prepareUnits();
	void CalculateDamage();

	//Handlers
	BattleAnimationManager btl_display_handler;

	//Data
	Game* game = nullptr;
	vector<NPC*> targets = {};
	vector<Unit*> unit_targets = {};

	//Debug
	int position;
	void showTargets();
	void showMagic();

	//Logic
	bool bCantUseMagic = false;
};