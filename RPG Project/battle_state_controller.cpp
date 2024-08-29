#include "battle_state_controller.h"
#include "battle_controller.h"
#include "game.h"

void BattleStateController::setup(Game* game, BattleController* battle_controller)
{
	basicSetup(game);
	this->battle_controller = battle_controller;
}


void BattleStateController::update()
{
	checkForGameOver();
	checkForBattleWin();
	checkForBattleEnd();
	beginBattle();
	turnStart();
	playerTurn();
	enemyTurn();
	startBattle();
	//viewStates();
}

void BattleStateController::beginBattle()
{
	if (findState("begin"))
	{
		battle_controller->battle_start = true;
	}
	else
	{
		battle_controller->battle_start = false;
	}
}

void BattleStateController::turnStart()
{
	if (findState("turn_start"))
	{
		battle_controller->turn_start = true;
	}
	else
	{
		battle_controller->turn_start = false;
	}
}

void BattleStateController::playerTurn()
{

}

void BattleStateController::enemyTurn()
{
	if (findState("enemy_turn"))
	{
		battle_controller->enemy_turn = true;
	}
	else
	{
		battle_controller->enemy_turn = false;
	}
}

void BattleStateController::startBattle()
{
	if (findState("battle"))
	{
		battle_controller->battle_on = true;
	}
	else
	{
		battle_controller->battle_on = false;
	}
}

void BattleStateController::checkForGameOver()
{
	if (findState("game_over"))
	{
		if (battle_controller->no_gameover == false)
		{
			battle_controller->game_over = true;
		}
		else
		{
			battle_controller->game_over = false;
		}
	}
	else
	{
		battle_controller->game_over = false;
	}
}

void BattleStateController::checkForBattleWin()
{
	if (findState("battle_won"))
	{
		battle_controller->battle_won = true;
	}
	else
	{
		battle_controller->battle_won = false;
	}
}

void BattleStateController::checkForBattleEnd()
{
	if (findState("battle_end"))
	{
		battle_controller->battle_end = true;
	}
	else
	{
		battle_controller->battle_end = false;
	}
}

void BattleStateController::viewStates()
{
	cout << "States: ";
	for (auto& state : states)
	{
		cout << state << ", ";
	}
	cout << endl;
}
