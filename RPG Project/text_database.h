#pragma once
#include <raylib.h>
#include <string>
#include <json/json.h>
#include <unordered_map>
#include "database.h"

using namespace std;

class TextDatabase : public Database
{
public:
	//Setup
	void setup(Game* game) override;

	//Getters
	Json::Value getText(int index);

	//Debug
	void seeAllText();
private:
	//Setup
	void createDatabase(Json::Value script);
	//Database
	unordered_map<string, Json::Value> text;
};