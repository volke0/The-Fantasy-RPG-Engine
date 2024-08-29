#pragma once
#include <raylib.h>
#include <string>
#include <iostream>
#include <unordered_map>
#include <fstream>
#include "database.h"

class Game;
class Level;

using namespace std;

class LevelDatabase : public Database
{
public:
	//Setup
	void setup(Game* game) override;

	//Database
	unordered_map<int, shared_ptr<Level>> levels;
private:
	//Setup
	void createDatabase(Json::Value database_script);
	void makeAllLevels(Json::Value level_script, Json::Value meta_data, string base_name);
};