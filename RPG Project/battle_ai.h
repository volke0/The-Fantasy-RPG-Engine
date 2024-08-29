#pragma once
#include <raylib.h>
#include "ai_base.h"

using namespace std;

class Game;
class Unit;
class BattleController;
class PartyBase;

class BattleAI : public AIBase
{
public:
	void setup(Game* game, Unit* user);

	//Setters
	void setupAI(BattleController* battle_controller, string ai_script_path, PartyBase* user_party);
	void calculateDecision();
private:
	//Setup
	virtual void ParseMetaData() override;
	virtual void GetSelectors() override;
	void GetTriggers();

	//Setters
	void setBattleController(BattleController* battle_controller);
	void setAppropiateReadyStatus();
	void setInput();
	void pushBattle();
	void SetCommand(Json::Value& command);
	void SetActionChain();

	//Getters
	bool isTargetDead(int target);
	bool isPartyWiped();
	int GetRng(int range = 100);
	string GetCommand(unordered_map<string, float> command_pcts);
	string GetRandomMagic();

	//Calculation
	void generateAlliesEnemies();
	void getTargetSelection();
	int getRandomTarget();

	//Behavior Methods
	virtual void RunBehavior() override;
	virtual void Evaluate() override;
	void CheckTriggers();
	void RunTriggerCommands();
	virtual void ExecuteCommands() override;

	//Evaluate
	void EvaluateAllies();
	void EvaluateEnemies();

	//Target Mode Methods
	virtual void SmartSelection(string& best_target);

	//Logic Functions
	virtual string HealingLogic(string best_heal_target, string heal_type);
	virtual void SetAIType(string ai_mode);


	//Party
	PartyBase* user_party = nullptr;


	//Data
	BattleController* battle_controller = nullptr;
	vector<Unit*> enemies = {};
	vector<string> enemy_names = {};
	vector<Unit*> allies = {};
	vector<string> ally_names = {};
	vector<Unit*> current_selection = {};
	unordered_map<string, float> mode_percents = {};
	unordered_map<string, float> magic_percents = {};
	unordered_map<string, float> item_percents = {};
	int user_unique_id = 0;
	unordered_map<string, string> best_enemies = {};
	unordered_map<string, string> best_allies = {};
	unordered_map < string, unordered_map<string, int>> target_stats = {};

	//Trigger Data
	unordered_map<string, vector<float>> hp_thresholds = {};
	unordered_map<string, Json::Value> threshold_commands = {};
	vector<string> trigger_commands = {};


	//AI Logic
	string user_name = "";
	unordered_map<string, int> user_type = { {"playable", 0}, {"enemy", 1} };
	int user_index = 0;
	vector<vector<string>> unit_types = { {"playable", "enemy"}, {"enemy", "playable"} };
	int target_index = 0;
	vector<string> target_names = {};
	int mode_index = 0;
	string mode = "";
	string magic_name = "";
	string item_name = "";
	vector<string> mode_list = { "Attack", "Special", "Magic", "Item" };
	Json::Value actions = {};


	//Logic
	bool bPlayDefaultAI = false;
	bool bMultiTarget = false;
	bool bRandomTarget = false;
	bool bSmartSelection = false;
	bool bHealingMode = false;
	bool bMagicMode = false;
	bool bMostDamageSelection = false;
	bool bEveryAllyMaxHP = false;
	bool bEveryAllyMaxMP = false;
	bool bGotoTriggerReact = false;
	bool bEnemiesOnly = false;
	bool bAlliesOnly = false;
};
