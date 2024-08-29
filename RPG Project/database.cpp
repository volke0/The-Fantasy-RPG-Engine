#include "database.h"
#include "game.h"

void Database::setup(Game* game)
{
	this->game = game;
}

Json::Value Database::getScript()
{
	//Get the database script
	std::ifstream file(path);
	Json::Value database_script;
	reader.parse(file, database_script);
	return database_script;
}