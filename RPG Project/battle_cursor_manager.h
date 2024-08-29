#pragma once
#include <raylib.h>
#include <vector>
#include <memory>
#include <unordered_map>
#include "cursor_manager.h"
#include "cursor_handler.h"
#include "c_unit_select_handler.h"
#include "cursor_mag_select_handler.h"

using namespace std;

class Game;
class BattleCursor;
class Input;

class BattleCursorManager : public CursorManager
{
public:
	void setup(Game* game, BattleCursor* cursor);
	void update() override;

	//Handlers
	CursorMagicSelectHandler mag_select_handler;
private:
	//Setup
	void setupAllHandlers();

	//Handlers
	CursorHandler general_handler;
	CursorUnitSelectHandler unit_select_handler;

	//Data
	BattleCursor* cursor;
};
