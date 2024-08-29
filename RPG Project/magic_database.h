#pragma once
#include <raylib.h>
#include <string>
#include <unordered_map>
#include <filesystem>
#include <fstream>
#include <json/json.h>
#include "database.h"

using namespace std;
using std::filesystem::directory_iterator;

class MagicDatabase : public Database
{
public:
	//Setup
	void setup();

	//Getters
	Json::Value GetMagicScript(string stem_name);

	//Items
	vector<string> master_list;
	unordered_map<string, Json::Value> magic_scripts;
private:
	//Setup
	void loadOrderList();
	void createMasterList();
	void loadAllScripts();

	//Data
	vector<Json::Value> order_database;
};