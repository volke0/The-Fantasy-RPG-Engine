#pragma once
#include <raylib.h>
#include <vector>
#include <memory>
#include <unordered_map>
#include "handler.h"

using namespace std;

class Game;
class Cursor;
class Input;

class BasicCursorHandler : public Handler
{
public:
	void basicSetup(Game* game);
	virtual void update();

	//Data
	Game* game;
	Input* input;
};
