#pragma once
#include <raylib.h>
#include "state_controller.h"

class Game;
class BattleController;

using namespace std;

class BattleStateController : public StateController
{
public:
	void setup(Game* game, BattleController* battle_controller);
	void update() override;
private:
	BattleController* battle_controller;

	//Modes
	void beginBattle();
	void turnStart();
	void playerTurn();
	void enemyTurn();
	void startBattle();
	void checkForGameOver();
	void checkForBattleWin();
	void checkForBattleEnd();

	//Debug
	void viewStates();
};
