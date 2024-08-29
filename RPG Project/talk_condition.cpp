#include "talk_condition.h"
#include "level.h"
#include "game.h"
#include "npc.h"
#include "input_handler.h"
#include "party.h"

void TalkCondition::setup(Json::Value data, Level* level, EventController* event_controller)
{
	Condition::setup(data, level, event_controller);
	parseData();
}

void TalkCondition::parseData()
{
	Condition::parseData();
	unit_name = GetUnitName(data["Unit"]);
}


void TalkCondition::update()
{
	attemptToGetUnit();
	if (loaded_unit)
	{
		CheckIfTalkValid();
		if (event_link != "" && trigger_event)
		{
			RunEvent();
		}
	}
}

void TalkCondition::draw()
{
}

void TalkCondition::attemptToGetUnit()
{
	unit = getUnit();
}

void TalkCondition::CheckIfTalkValid()
{
	if (collision_handler->checkIfPartyFacesUnit(unit) && !party->running)
	{
		if (bHasConditionFlag)
		{
			if (!game->flags.getFlag(condition_flag, level->name)) return;
		}
		if (bHasGlobalConditionFlag)
		{
			if (!game->flags.getFlag(global_condition_flag)) return;
		}

		if (game->input.findKey("x"))
		{
			if (event_link != "")
			{
				trigger_event = true;
			}
		}
	}
}



NPC* TalkCondition::getUnit()
{
	NPC* temp_unit = nullptr;
	for (auto& unit : level->unit_list)
	{
		if (unit->name == unit_name)
		{
			temp_unit = unit;
			loaded_unit = true;
			return temp_unit;
		}
	}
	loaded_unit = false;
	return temp_unit;
}