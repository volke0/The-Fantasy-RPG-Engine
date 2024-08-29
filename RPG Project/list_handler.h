#pragma once
#include <raylib.h>
#include "handler.h"

using namespace std;

class Game;
class Unit;

class ListHandler : public Handler
{
public:
	void basicSetup(Game* game, Unit* unit);

	Game* game;
	Unit* unit;
};