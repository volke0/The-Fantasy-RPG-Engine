#pragma once
#include <raylib.h>
#include <vector>
#include <memory>
#include <unordered_map>
#include "handler.h"
#include "cursor_handler.h"
#include "c_unit_select_handler.h"

using namespace std;

class Game;
class Cursor;
class Input;

class CursorManager : public Handler
{
public:
	void basicSetup(Game* game);
	virtual void update();

	//Data
	Game* game;
};
