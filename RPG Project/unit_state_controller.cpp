#include "unit_state_controller.h"
#include "unit.h"

void UnitStateController::setup(Game* game, Unit* unit)
{
	basicSetup(game);
	this->unit = unit;
}


void UnitStateController::update()
{
	if (findState("in_battle"))
	{
		unit->battle_mode = true;
		unit->map_mode = false;
	}
	if (findState("on_map"))
	{
		unit->battle_mode = false;
		unit->map_mode = true;
	}
	if (findState("entrance"))
	{
		unit->entrance = true;
	}
	else
	{
		unit->entrance = false;
	}
	if (findState("dead"))
	{
		unit->dead = true;
	}
	else
	{
		unit->dead = false;
	}

	/*cout << "Unit: " << unit->name << " States: ";
	for (auto& state : states)
	{
		cout << state << ", ";
	}
	cout << endl;*/
}
