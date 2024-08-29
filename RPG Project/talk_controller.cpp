#include "talk_controller.h"
#include "level.h"
#include "game.h"
#include "npc.h"

void TalkEventController::setup(Json::Value data, Level* level, EventController* event_controller)
{
	ConditionController::setup(data, level, event_controller);
	setupAllConditions();
}

void TalkEventController::setupAllConditions()
{
	for (int index = 0; index < (int)data.size(); index++)
	{
		TalkCondition condition;
		condition.setup(data[index], level, event_controller);
		talk_conditions.push_back(condition);
	}
}


void TalkEventController::update()
{
	if (level->cameFromPortal == false)
	{
		updateAllConditions();
	}
}

void TalkEventController::draw()
{
	for (auto& condition : talk_conditions)
	{
		condition.draw();
	}
}

void TalkEventController::updateAllConditions()
{
	for (auto& condition : talk_conditions)
	{
		condition.update();
	}
}
