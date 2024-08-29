#pragma once
#include <raylib.h>
#include <vector>
#include <memory>
#include <unordered_map>
#include "basic_cursor_handler.h"

using namespace std;

class Game;
class MenuCursor;
class Input;

class BasicMenuCursorHandler : public BasicCursorHandler
{
public:
	void basicSetup(Game* game, MenuCursor* cursor);
	virtual void update();

	//Data
	MenuCursor* cursor;
	Input* input;
};