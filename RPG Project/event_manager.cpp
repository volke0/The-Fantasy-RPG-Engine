#include "event_manager.h"
#include "game.h"
#include "level_database.h"
#include "event_controller.h"
#include "level.h"

void EventManager::setup(Game* game)
{
	this->game = game;
	this->level_database = &game->level_database;
}

void EventManager::InitController(Level* level, bool bUseCurrentController)
{
	string name = level->name;
	if (controllers.find(name) == controllers.end())
	{
		ifstream file("./databases/events/" + level->events + ".json");
		Json::Value script;
		reader.parse(file, script);
		shared_ptr<EventController> event_controller = make_shared<EventController>();
		event_controller->setup(script, level);
		controllers[name] = event_controller;
	}
	else
	{
		setLevel(level, bUseCurrentController);
	}
	//cout << "TOTAL EVENT CONTROLLERS: " << controllers.size() << endl;
}

void EventManager::SetController(string controller_name, Level* level)
{
	current_controller = controller_name;
	InitController(level, true);
}

void EventManager::setLevel(Level* level, bool bUseCurrentController)
{
	string name = level->name;
	if (bUseCurrentController)
	{
		name = current_controller;
	}
	controllers[name]->setLevel(level);
}


void EventManager::update()
{
	if (current_controller.size() > 0)
	{
		controllers[current_controller]->update();
	}
}

void EventManager::draw()
{
	if (current_controller.size() > 0)
	{
		controllers[current_controller]->draw();
	}
}

void EventManager::runEvent(string controller, string event_name)
{
	current_controller = controller;
	controllers[current_controller]->runEvent(event_name);
}

void EventManager::EndEvent(string controller, string event_name)
{
	current_controller = controller;
	controllers[current_controller]->EndEvent(event_name);
}

bool EventManager::checkIfBusy()
{
	if (current_controller.size() > 0)
	{
		if (controllers[current_controller]->busy == true)
		{
			return true;
		}
	}
	return false;
}


