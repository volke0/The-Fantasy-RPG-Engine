#include "always_trigger_controller.h"
#include "level.h"

void AlwaysTriggerEventController::setup(Json::Value data, Level* level, EventController* event_controller)
{
	ConditionController::setup(data, level, event_controller);
	setupAllConditions();
}

void AlwaysTriggerEventController::setupAllConditions()
{
	for (int index = 0; index < (int)data.size(); index++)
	{
		AlwaysTriggerCondition condition;
		condition.setup(data[index], level, event_controller);
		always_conditions.push_back(condition);
	}
}

void AlwaysTriggerEventController::update()
{
	if (level->cameFromPortal == false)
	{
		updateAllConditions();
	}
}

void AlwaysTriggerEventController::draw()
{
	for (auto& condition : always_conditions)
	{
		condition.draw();
	}
}

void AlwaysTriggerEventController::updateAllConditions()
{
	for (auto& condition : always_conditions)
	{
		condition.update();
	}
}
