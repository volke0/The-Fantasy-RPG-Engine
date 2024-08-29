#pragma once
#include <raylib.h>
#include <vector>
#include <memory>
#include <unordered_map>
#include "menu_cursor_handler.h"
#include "menu_magic_cursor_handler.h"
#include "cursor_manager.h"



using namespace std;

class Game;
class MenuCursor;
class Input;

class MenuCursorManager : public CursorManager
{
public:
	void setup(Game* game, MenuCursor* cursor);
	virtual void update() override;
private:
	//Setup
	void setupAllHandlers();

	//Handlers
	MenuCursorHandler general_handler;
	MenuMagicCursorHandler magic_handler;

	//Data
	MenuCursor* cursor;
};
