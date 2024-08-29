#include "scene_manager.h"
#include "scene.h"
#include "event_controller.h"
#include "party.h"
#include "level.h"
#include "map.h"
#include "game.h"

void SceneManager::setup(Game* game)
{
	this->game = game;
	this->input = &game->input;
}

void SceneManager::setupEventManager()
{
	this->event_manager.setup(game);
}

void SceneManager::push(shared_ptr<Scene> scene, bool isInstant, int speed, float stall_time)
{
	if (fade.done == true)
	{
		if (isInstant == false)
		{
			fade.in(speed);
		}
		scene_to_push = scene;
		pushing_scene = true;
		fade_speed = speed;
		this->stall_time = stall_time;
		this->isInstant = isInstant;
	}
}

void SceneManager::push_level(shared_ptr<Scene> scene, bool isInstant, int speed, float stall_time)
{
	if (fade.done == true)
	{
		if (isInstant == false)
		{
			fade.in(speed);
		}
		scene_to_push = scene;
		pushing_scene = true;
		pushing_level = true;
		fade_speed = speed;
		this->stall_time = stall_time;
		this->isInstant = isInstant;
	}
}

void SceneManager::pop(bool isInstant, int pop_amt, int speed, float stall_time)
{
	if (fade.done == true)
	{
		if (isInstant == false)
		{
			fade.in(speed);
		}
		fade_speed = speed;
		this->stall_time = stall_time;
		this->isInstant = isInstant;
		this->pop_amt = pop_amt;
		scene_to_push.reset();
		popping_scene = true;
	}
}

void SceneManager::draw()
{
	drawScene();
	//event_manager.draw();
	fade.draw();
}

void SceneManager::update()
{
	input->update();
	if (timer.wait == false)
	{
		updateScene();
		event_manager.update();
		fade.update();
		if (fade.fadein_done == true)
		{
			pushScene();
			pushingLevel();
			popScene();
		}
	}
	timer.tick();
	checkIfBusy();
}

void SceneManager::MakeBusy()
{
	bMakeBusy = true;
}

void SceneManager::MakeReady()
{
	bMakeBusy = false;
}

shared_ptr<Scene> SceneManager::getScene(int num)
{
	return scenes[num];
}

shared_ptr<Scene> SceneManager::getLastScene()
{
	int last_index = getLastScenePos();
	return getScene(last_index);
}

shared_ptr<Scene> SceneManager::getCurrentScene()
{
	return getScene((int)scenes.size() - 1);
}

int SceneManager::getLastScenePos()
{
	return (int)scenes.size() - 2;
}

void SceneManager::setCurrentEventController(string name, Level* level)
{
	current_controller = name;
	event_manager.SetController(current_controller, level);
}

void SceneManager::setAndRunEvent(string controller, string event_name)
{
	current_controller = controller;
	event_manager.runEvent(current_controller, event_name);
}

void SceneManager::runEvent(string event_name)
{
	event_manager.runEvent(current_controller, event_name);
}

void SceneManager::updateScene()
{
	if (scenes.size() > 0)
	{
		scenes.back()->update();
	}
}

void SceneManager::drawScene()
{
	if (scenes.size() > 0)
	{
		scenes.back()->draw();
	}
}

void SceneManager::pushScene()
{
	if (pushing_scene == true)
	{
		if (pushing_level == false)
		{
			if (scene_to_push)
			{
				if (scene_to_push->type == "battle")
				{
					game->party.setBattleMode();
				}
				else if (scene_to_push->type == "level")
				{
					game->party.setMapMode();
				}
				scenes.push_back(scene_to_push);
				pushing_scene = false;
				fadeOut();
			}
		}
	}
}

void SceneManager::pushingLevel()
{
	if (pushing_scene == true)
	{
		if (pushing_level == true)
		{
			if (scenes.back()->type == "level")
			{
				static_cast<Level*>(scenes.back().get())->map->unloadTilesetGFX();
			}
			scenes.pop_back();
			scenes.push_back(scene_to_push);
			pushing_level = false;
			pushing_scene = false;
		}
	}
}

void SceneManager::popScene()
{
	if (popping_scene == true)
	{
		for (int i = 0; i < pop_amt; i++)
		{
			scenes.back().reset();
			scenes.pop_back();
			scenes.shrink_to_fit();
		}
		popping_scene = false;

		if (scenes.back()->type == "level")
		{
			game->party.setMapMode();
		}
		if (isInstant == false)
		{
			fade.out(fade_speed);
			timer.STAL(stall_time);
		}
	}
}

void SceneManager::fadeOut()
{
	if (scene_to_push->type != "level" && isInstant == false)
	{
		fade.out(fade_speed);
		timer.STAL(stall_time);
	}
}

void SceneManager::checkIfBusy()
{
	if (fade.done == true && timer.wait == false && !bMakeBusy)
	{
		if (!event_manager.checkIfBusy())
		{
			busy = false;
			return;
		}
		if ((int)scenes.size() > 0)
		{
			if (scenes.back()->type == "menu" || scenes.back()->type == "shop")
			{
				busy = false;
				return;
			}
		}
		busy = true;
	}
	else
	{
		busy = true;
	}
}
