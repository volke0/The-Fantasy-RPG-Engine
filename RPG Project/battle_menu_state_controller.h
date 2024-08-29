#pragma once
#include <raylib.h>
#include "state_controller.h"

class Game;
class BattleMenuController;

using namespace std;

class BattleMenuStateController : public StateController
{
public:
	void setup(Game* game, BattleMenuController* menu_controller);
	void update() override;

	//Setters
	void eraseUnitSelectState();
private:
	BattleMenuController* menu_controller;

	//Modes
	void turnOn();
	void updateMenus();

	void updatePlayerMenu();
	void updateMagicMenu();
	void updateItemMenu();
	void updateUnitSelectMenu();
	void updateExpScreen();
};
