#include "unit_condition_controller.h"
#include "game.h"
#include "unit.h"

void UnitConditionController::basicSetup(Game* game, Unit* unit)
{
	this->game = game;
	this->unit = unit;
}


bool UnitConditionController::findCondition(string condition)
{
	return find(conditions.begin(), conditions.end(), condition) != conditions.end();
}


void UnitConditionController::setCondition(string condition)
{
	if (findCondition(condition) != true)
	{
		conditions.push_back(condition);
	}
}

void UnitConditionController::eraseCondition(string condition)
{
	if (findCondition(condition))
	{
		conditions.erase(find(conditions.begin(), conditions.end(), condition));
	}
}

void UnitConditionController::eraseAllConditions()
{
	conditions.clear();
	conditions.shrink_to_fit();
}


