#pragma once
#include <raylib.h>
#include <iostream>
#include <vector>
#include "fade.h"
#include "timer.h"
#include "event_manager.h"

using namespace std;

class Scene;
class Game;
class Input;
class Level;

class SceneManager
{
public:
	void setup(Game* game);
	void setupEventManager();
	void push(shared_ptr<Scene> scene, bool isInstant = false, int speed = 16, float stall_time = 30.0f);
	void push_level(shared_ptr<Scene> scene, bool isInstant = false, int speed = 16, float stall_time = 30.0f);
	void pop(bool isInstant = false, int pop_amt = 1, int speed = 16, float stall_time = 30.0f);
	void draw();
	void update();

	//Setters
	void MakeBusy();
	void MakeReady();

	//Getters
	shared_ptr<Scene> getScene(int num);
	shared_ptr<Scene> getLastScene();
	shared_ptr<Scene> getCurrentScene();
	template <class T>
	inline T* CastToPreviousScene(int pos_from_last = 0)
	{
		return dynamic_cast<T*>(getScene(getLastScenePos() - pos_from_last).get());
	}
	int getLastScenePos();

	//Event Manager
	void setCurrentEventController(string controller, Level* level);
	void setAndRunEvent(string controller, string event_name);
	void runEvent(string event_name);

	//Data
	Game* game;
	shared_ptr<Scene> scene_to_push;
	Input* input;
	Fades fade;
	Timer timer;
	EventManager event_manager;

	//Logic
	bool busy = false;
private:
	//Scene
	vector<shared_ptr<Scene>> scenes;
	void updateScene();
	void drawScene();
	void pushScene();
	void pushingLevel();
	void popScene();

	//Fade
	void fadeOut();

	//Checkers
	void checkIfBusy();

	//Data
	float stall_time = 30.0f;
	int fade_speed = 16;
	int pop_amt = 1;
	string current_controller;

	//Logic
	bool pushing_scene = false;
	bool popping_scene = false;
	bool pushing_level = false;
	bool isInstant = false;
	bool bMakeBusy = false;
};
