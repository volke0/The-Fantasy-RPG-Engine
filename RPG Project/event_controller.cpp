#include "event_controller.h"
#include "level.h"
#include "game.h"
#include "map.h"
#include "area_controller.h"
#include "talk_controller.h"
#include "map_obj_controller.h"
#include "always_trigger_controller.h"

void EventController::setup(Json::Value script, Level* level)
{
	this->main_script = script;
	this->level = level;
	this->name = level->name;
	this->map = level->map.get();
	this->game = level->game;
	AddPartyToAllEvents();
	makeAllEvents();
	setupAllControllers();
}

void EventController::makeAllEvents()
{
	setupEventScript();
}

void EventController::setupEventScript()
{
	for (Json::Value::const_iterator it = main_script["Events"].begin(); it != main_script["Events"].end(); it++)
	{
		Json::Value event_script = main_script["Events"][it.key().asInt()];
		for (auto const& name : event_script.getMemberNames())
		{
			setupEvent(name, event_script[name]);
		}
	}
}

void EventController::setupEvent(string name, Json::Value script)
{
	Event temp_event;
	temp_event.setup(name, script, this, level);
	if (game->flags.getFlag("LOADED_PARTY"))
	{
		temp_event.setParty(party);
	}
	events[name] = temp_event;
}

void EventController::setupAllControllers()
{
	if (main_script.isMember("AreaConditions"))
	{
		area_controller = make_shared<AreaEventController>();
		area_controller->setup(main_script["AreaConditions"], level, this);
	}
	if (main_script.isMember("TalkConditions"))
	{
		talk_controller = make_shared<TalkEventController>();
		talk_controller->setup(main_script["TalkConditions"], level, this);
	}
	if (main_script.isMember("MapObjects"))
	{
		map_obj_controller = make_shared<MapObjectController>();
		map_obj_controller->setup(main_script["MapObjects"], level, this);
	}
	if (main_script.isMember("AlwaysConditions"))
	{
		always_event_controller = make_shared<AlwaysTriggerEventController>();
		always_event_controller->setup(main_script["AlwaysConditions"], level, this);
	}
}


void EventController::draw()
{
	if (level->level_ready == true)
	{
		if (busy == false)
		{
			if (area_controller != nullptr)
			{
				area_controller->draw();
			}
			if (talk_controller != nullptr)
			{
				talk_controller->draw();
			}
			if (map_obj_controller != nullptr)
			{
				map_obj_controller->draw();
			}
			if (always_event_controller != nullptr)
			{
				always_event_controller->draw();
			}
		}
	}
}


void EventController::update()
{
	if (level->level_ready == true)
	{
		if (level->cameFromPortal == false)
		{
			if (current_event.empty() != true && events[current_event].done == false)
			{
				events[current_event].update();
			}
			else
			{
				runningEvent = false;
			}
		}
		updateAllControllers();
		checkIfBusy();
	}
}

void EventController::updateAllControllers()
{
	if (busy == false)
	{
		if (area_controller != nullptr)
		{
			area_controller->update();
		}
		if (talk_controller != nullptr)
		{
			talk_controller->update();
		}
		if (map_obj_controller != nullptr)
		{
			map_obj_controller->update();
		}
		if (always_event_controller != nullptr)
		{
			always_event_controller->update();
		}
	}
}

void EventController::checkIfBusy()
{
	if (current_event == "none")
	{
		busy = false;
	}
	else if (events[current_event].done == false && level->cameFromPortal == false)
	{
		busy = true;
	}
	else
	{
		busy = false;
	}
}


void EventController::AddPartyToAllEvents()
{
	if (game->flags.getFlag("LOADED_PARTY"))
	{
		party = &game->party;
	}
	if ((int)events.size() > 0)
	{
		for (auto& [key, eventy] : events)
		{
			eventy.setParty(party);
		}
	}
}

bool EventController::isEventDone(string name, string type)
{
	return level->flag_sys->getFlag(name + "Done", type);
}

void EventController::setLevel(Level* level)
{
	if ((int)events.size() > 0)
	{
		for (auto& [key, eventy] : events)
		{
			eventy.setLevel(level);
		}
	}
	this->level = level;
	this->map = level->map.get();
}

void EventController::ResetAllDoors()
{
	if (map_obj_controller)
	{
		map_obj_controller->ResetAllDoors();
	}
}

void EventController::runEvent(string event_name)
{
	resetEvent(event_name);
	current_event = event_name;
	runningEvent = true;
}

void EventController::EndEvent(string event_name)
{
	events[event_name].done = true;
	level->flag_sys->ENUT(event_name + "Done", level->name);
	runningEvent = false;
}

void EventController::runEvent(string event_name, string previous_event, int previous_index)
{
	resetEvent(event_name);
	current_event = event_name;
	events[current_event].setPreviousEvent(previous_event, previous_index);
	runningEvent = true;
}

void EventController::runEventNoReset(string event_name)
{
	current_event = event_name;
}

void EventController::resetEvent(string event_name)
{
	events[event_name].reset();
}

