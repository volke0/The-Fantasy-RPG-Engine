#include "battle_condition_controller.h"
#include "game.h"
#include "unit.h"

void BattleConditionController::setup(Game* game, Unit* unit)
{
	basicSetup(game, unit);
}



void BattleConditionController::update()
{
	if (unit->battle_mode == true)
	{
		checkIfDead();
		//viewAllConditions();
	}
}


void BattleConditionController::resetToIdle()
{
	eraseAllConditions();
	setCondition("idle");
}

void BattleConditionController::checkIfDead()
{
	if (unit->dead == true)
	{
		if (!findCondition("revived"))
		{
			eraseAllConditions();
		}
	}
}

void BattleConditionController::viewAllConditions()
{
	cout << "Unit: " << unit->name;
	for (auto& condition : conditions)
	{
		cout << " " << condition << ", ";
	}
	cout << endl;
}
