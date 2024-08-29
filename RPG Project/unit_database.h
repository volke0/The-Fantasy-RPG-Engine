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

class UnitDatabase : public Database
{
public:
	void setup();

	//Getters
	Json::Value GetUnit(int unit_id);
	string GetUnitName(int unit_id);

	//Units
	unordered_map<int, Json::Value> unit_scripts = {};
private:
	//Setup
	void InitDatabase();
	unordered_map<string, Json::Value> loadAllScripts();

	//Debug
	void ShowUnitDatabase();
};
