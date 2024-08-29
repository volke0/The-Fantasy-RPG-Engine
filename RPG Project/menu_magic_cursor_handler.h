#pragma once
#include <raylib.h>
#include "basic_menu_cursor_handler.h"

using namespace std;

class Game;
class MenuCursor;
class Input;

class MenuMagicCursorHandler : BasicMenuCursorHandler
{
public:
	void setup(Game* game, MenuCursor* cursor);
	void update() override;
private:
	//Update
	void updateInput(bool& vertical_click, bool& horizontal_click, bool& is_odd);
	void updateVisibleIndex(bool& vertical_click, bool& horizontal_click, bool& is_odd);
	void refreshMaxIndex();
	void sendInput();

	//Setters
	void moveDown();
	void moveUp();

	//Logic
	bool flip = false;
};
