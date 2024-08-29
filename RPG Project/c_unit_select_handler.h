#pragma once
#include <raylib.h>
#include <vector>
#include <memory>
#include <unordered_map>
#include "basic_battle_cursor_handler.h"

using namespace std;

class Game;
class BattleCursor;

class CursorUnitSelectHandler : public BasicBattleCursorHandler
{
public:
	void setup(Game* game, BattleCursor* cursor);
	void update() override;

	//Logic
	bool go_back_to_magic = false;
private:
	void updateUnitSelectInput();
};
