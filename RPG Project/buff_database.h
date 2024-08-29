#pragma once
#include <raylib.h>
#include "database.h"

using namespace std;
using std::filesystem::directory_iterator;

class BuffDatabase : public Database
{
public:
	void setup();

	//Getters
	Json::Value GetBuffScript(string buff_name);
private:
	//Setup
	void InitDatabase();
	//Data
	unordered_map<string, Json::Value> buff_scripts = {};
};

