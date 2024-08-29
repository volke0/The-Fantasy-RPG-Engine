#pragma once
#include <raylib.h>
#include <vector>
#include <memory>
#include <unordered_map>
#include "basic_battle_cursor_handler.h"

using namespace std;

class Game;
class BattleCursor;
class Input;

class CursorHandler : public BasicBattleCursorHandler
{
public:
	void setup(Game* game, BattleCursor* cursor);
	void update() override;

private:
	//Update
	void updateInput();
	void updateVisibleIndex();
	void refreshMaxIndex();
	void sendInput();

	//Debug
	void seeAllOptions();
};
