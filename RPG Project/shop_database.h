#pragma once
#include <raylib.h>
#include "database.h"

using namespace std;
using std::filesystem::directory_iterator;

class ShopDatabase : public Database
{
public:
	//Setup
	void setup();

	//Shop
	unordered_map<string, Json::Value> shop_scripts;
private:
	//Setup
	void loadAllScripts();
};
