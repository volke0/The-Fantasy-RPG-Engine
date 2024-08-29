#pragma once
#include <raylib.h>
#include <string>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>
#include <cmath>
#include <json/json.h>
#include "scene_manager.h"
#include "sound_player.h"
#include "vars.h"

using namespace std;

class Game;
class Input;
class Cam;
class SceneManager;

class Scene
{
public:
	//Setup
	virtual void setup(Game* game);
	void basicSetup(Game* game);

	//Basic Methods
	virtual void draw();
	virtual void update();
	void push_scene(shared_ptr<Scene> scene, bool isInstant = false, int speed = 16, float stall_time = 30.0f);

	float dropshadow_offset = 1.0f + ((float)TILE / 2.0f / 15.0f);
	void drawText(Vector2 pos, string text, float fontSize, Color color = { 255, 255, 255, 255 }, bool useGameFont = true);

	//Getters
	template <class T>
	inline T* CastToPreviousScene(int pos_from_last = 0)
	{
		return scene_manager->CastToPreviousScene<T>(pos_from_last);
	}
	Scene* GetScene(int pos_from_last);

	//Sound
	SoundPlayer sound_player;

	//Data
	Game* game;
	SceneManager* scene_manager;
	Input* input;
	Cam* camera;

	//Title
	string title;
	string name;
	string type;
};
