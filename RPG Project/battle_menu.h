#pragma once
#include <raylib.h>
#include <string>
#include <unordered_map>
#include "vars.h"
#include "sound_player.h"
#include "menu.h"

using namespace std;

class Game;
class Party;
class Unit;
class BattleCursor;
class BattleController;
class BattleMenuController;

class BattleMenu : public Menu
{
public:
	virtual void basicSetup(Game* game, Party* party, BattleController* battle_controller, BattleMenuController* menu_controller);

	//Data
	BattleCursor* cursor;
};