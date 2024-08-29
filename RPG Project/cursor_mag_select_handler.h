#pragma once
#include <raylib.h>
#include <vector>
#include <memory>
#include <unordered_map>
#include "basic_battle_cursor_handler.h"

using namespace std;

class Game;
class BattleCursor;

class CursorMagicSelectHandler : public BasicBattleCursorHandler
{
public:
	void setup(Game* game, BattleCursor* cursor);
	void update() override;

	//Setters
	void turnOn();
	void setCursor();

	//Menu
	int visual_index = 0;
	int memory_index = 0;
private:
	//Update
	void updateMagicSelectInput();
	void updateInput(bool& vertical_click, bool& horizontal_click, bool& is_odd);
	void fixCursorVisuals(bool& vertical_click, bool& horizontal_click, bool& is_odd);

	//Setters
	void moveDown();
	void moveUp();

	//Logic
	bool flip = false;
};