#include "level_database.h"
#include "scene.h"
#include "game.h"
#include "scene_manager.h"
#include "level.h"

void LevelDatabase::setup(Game* game)
{
	this->game = game;
	this->scene_manager = &game->scene_manager;
	path = "./databases/levels/level_database.json";
	Json::Value database_script = getScript();
	createDatabase(database_script);
}

void LevelDatabase::createDatabase(Json::Value database_script)
{
	//Get the meta data
	for (Json::Value::const_iterator it = database_script.begin(); it != database_script.end(); ++it)
	{
		string base_name = "";
		Json::Value meta_data;
		Json::Value level_script = database_script[it.key().asInt()];
		makeAllLevels(level_script, meta_data, base_name);
	}
}

void LevelDatabase::makeAllLevels(Json::Value level_script, Json::Value meta_data, string base_name)
{
	//Make level
	meta_data = level_script;
	shared_ptr<Level> level = make_shared<Level>();
	level->setup(game, scene_manager, meta_data);
	levels[level_script["id"].asInt()] = level;
	cout << "LEVEL NAME: " << levels[level_script["id"].asInt()]->ID << endl;
	cout << "ID: " << level_script["id"].asInt() << endl;
}
