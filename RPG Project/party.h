#pragma once
#include <raylib.h>
#include <memory>
#include <iostream>
#include <vector>
#include <json/json.h>
#include "party_base.h"

using namespace std;

class Unit;
class Maps;
class Input;
class Game;

class Party : public PartyBase
{
public:
	//Initialization Methods
	void setup(Game* game, Input* input);

	//Basic Methods
	virtual void draw() override;
	virtual void update() override;

	//Load
	void addUnitToParty(int unit_id);

	//Setters
	void AddSilver(int amount);
	void SubSilver(int amount);
	virtual void SetFormation() override;
	virtual void SetupCombatants(unordered_map<string, shared_ptr<NPC>>& combatants, BattleController* battle_controller) override;

	//Modes
	bool menu_mode = false;

	//Input
	Input* input;
private:
	//Update Methods
	void updateMenuMode();
};