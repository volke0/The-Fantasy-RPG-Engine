#include "state_controller.h"
#include "unit.h"

void StateController::basicSetup(Game* game)
{
	this->game = game;
}



void StateController::update()
{
	
}

void StateController::pushState(string state)
{
	if (findState(state) != true)
	{
		states.push_back(state);
	}
}

void StateController::eraseState(string state)
{
	if (findState(state))
	{
		states.erase(find(states.begin(), states.end(), state));
	}
}

void StateController::clearStates()
{
	states.clear();
	states.shrink_to_fit();
}

bool StateController::findState(string key)
{
	return find(states.begin(), states.end(), key) != states.end();
}

bool StateController::findLastState(string key)
{
	auto& temp_string = states.back();
	if (temp_string == key)
	{
		return true;
	}
	return false;
}
