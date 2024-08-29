#pragma once
#include <raylib.h>
#include "state_controller.h"

using namespace std;

class Game;
class Unit;

class UnitStateController : public StateController
{
public:
	void setup(Game* game, Unit* unit);
	void update() override;
private:
	Unit* unit;
};
