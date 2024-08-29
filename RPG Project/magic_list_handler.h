#pragma once
#include <raylib.h>
#include "list_handler.h"

using namespace std;

class Game;
class Unit;

class MagicListHandler : public ListHandler
{
public:
	void setup(Game* game, Unit* unit);
	void updatePlayerMagicList();
private:
	//Debug
	void seeAllMagicOptions();
};