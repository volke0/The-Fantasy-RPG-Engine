#pragma once
#include <raylib.h>
#include <unordered_map>
#include "handler.h"
#include "buff_battle_manager.h"

using namespace std;

class Game;
class NPC;
class BattleController;
class VFXController;
class BattleScene;

class BattleControllerHandler : public Handler
{
public:
	void setup(Game* game, BattleController* battle_controller, VFXController* vfx_controller);
	void update() override;
	virtual void draw();

	//Setters
	void setTurnStart();
private:
	//Data
	Game* game;
	BattleController* battle_controller;
	VFXController* vfx_controller;
	BuffBattleManager buff_btl_manager;

	//Update
	void updateCombatants();
	void updateModes();

	//Setters
	void refreshUnitReadyStatus();
	void turnUnitAnimationsOn();
	void setVFXController(VFXController* vfx_controller);

	//Getters
	BattleScene* GetBattleScene();
	vector<NPC*> GetCombatUnits();

	//Modes
	void setBattleBegin();
	void setPlayerTurn();
	void BuffUpdateTurn();
	void setEnemyTurn();
	void startBattle();
	void battleWon();
	void battleLost();
	void endBattle();
	void updateBattle();

	//Checkers
	void resetBattleConditions();
	void checkIfDead();
	bool checkIfAllEnemiesDead();
	bool CheckIfAllPartyMembersAreDead();

	//Misc.
	unordered_map<string, shared_ptr<NPC>> makeEnemyMap();
	vector<NPC*> combat_units = {};

	//Logic
	bool bTurnedOnAnimations = false;
	bool bAllPartyMembersAreDead = false;
};
