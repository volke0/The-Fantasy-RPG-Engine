#pragma once
#include <raylib.h>
#include "database.h"

using namespace std;
using std::filesystem::directory_iterator;

class FormationDatabase : public Database
{
public:
	//Setup
	void setup();

	//Data
	unordered_map<string, Json::Value> formation_scripts;
private:
	//Setup
	void loadAllScripts();
};