#include "condition_controller.h"
#include "level.h"
#include "game.h"

void ConditionController::setup(Json::Value data, Level* level, EventController* event_controller)
{
	this->data = data;
	this->level = level;
	this->game = level->game;
	this->map = level->map.get();
	this->camera = level->camera;
	this->event_controller = event_controller;
}

void ConditionController::setupAllConditions()
{
}


void ConditionController::update()
{
}

void ConditionController::draw()
{
}



void ConditionController::updateAllConditions()
{
}

void ConditionController::updatePartyLocation()
{
}