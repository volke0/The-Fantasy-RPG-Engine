#pragma once
#include <raylib.h>
#include "party_base.h"

using namespace std;

class Game;

class EnemyParty : public PartyBase
{
public:
	//Initialization Methods
	void setup(Game* game, Json::Value party_script);

	virtual void update() override;
	virtual void draw() override;

	//Setters
	virtual void SetFormation() override;
	void SetupCombatants(unordered_map<string, shared_ptr<NPC>>& combatants, vector<shared_ptr<NPC>>& enemies, BattleController* battle_controller);
private:
	//Setup
	void setupParty();
	void SetupInventory();

	//Data
	Json::Value party_script;
	string formation_name = "";
};