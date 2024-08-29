#pragma once
#include <raylib.h>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <filesystem>
#include <fstream>
#include <json/json.h>

class Game;
class SceneManager;

class Database
{
public:
	//Setup
	virtual void setup(Game* game);
	Json::Value getScript();

	//Path
	std::string path;

	//Reader
	Json::Reader reader;

	//Game
	Game* game;
	SceneManager* scene_manager;
};
