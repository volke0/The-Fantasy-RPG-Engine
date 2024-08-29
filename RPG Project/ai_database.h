#pragma once
#include <raylib.h>
#include "database.h"


using namespace std;
using std::filesystem::directory_iterator;

class AIDatabase : public Database
{
public:
	void setup();

	//Getters
	Json::Value GetAIScript(string ai_type, string ai_script_name);
private:
	//Getters
	unordered_map<string, Json::Value> SetupAIScripts(string ai_type);

	unordered_map<string, unordered_map<string, Json::Value>> ai_scripts = {};
};