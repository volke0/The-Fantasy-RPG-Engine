#pragma once
#include <raylib.h>
#include "database.h"

using namespace std;
using std::filesystem::directory_iterator;

class ItemDatabase : public Database
{
public:
	//Setup
	void setup();

	//Items
	unordered_map<string, Json::Value> item_scripts;
private:
	//Setup
	void loadAllScripts();
};
