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

class ClassDatabase : public Database
{
public:
	 void setup();
	 //Classes
	 unordered_map<string, Json::Value> class_scripts;
private:
	//Setup
	void loadAllScripts();
};
