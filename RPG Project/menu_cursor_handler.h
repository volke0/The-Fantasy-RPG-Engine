#pragma once
#include <raylib.h>
#include "basic_menu_cursor_handler.h"

using namespace std;

class Game;
class MenuCursor;
class Input;

class MenuCursorHandler : public BasicMenuCursorHandler
{
public:
	void setup(Game* game, MenuCursor* cursor);
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
