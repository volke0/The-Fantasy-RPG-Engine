#pragma once
#include <raylib.h>
#include <vector>
#include <memory>
#include <unordered_map>
#include "basic_cursor_handler.h"

using namespace std;

class Game;
class BattleCursor;
class Input;

class BasicBattleCursorHandler : public BasicCursorHandler
{
public:
	void basicSetup(Game* game, BattleCursor* cursor);
	virtual void update();
	//Data
	BattleCursor* cursor;
};