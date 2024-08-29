#include "always_trigger_condition.h"
#include "game.h"
#include "level.h"

void AlwaysTriggerCondition::setup(Json::Value data, Level* level, EventController* event_controller)
{
	Condition::setup(data, level, event_controller);
	parseData();
}

void AlwaysTriggerCondition::parseData()
{
	Condition::parseData();
}

void AlwaysTriggerCondition::update()
{
	CheckIfValid();
	if (trigger_event)
	{
		RunEvent();
	}
}

void AlwaysTriggerCondition::draw()
{
}


void AlwaysTriggerCondition::CheckIfValid()
{
	if (bHasConditionFlag)
	{
		if (!game->flags.getFlag(condition_flag, level->name)) return;
	}
	if (bHasGlobalConditionFlag)
	{
		if (!game->flags.getFlag(global_condition_flag)) return;
	}

	if (event_link != "")
	{
		trigger_event = true;
	}
}
