#pragma once
#include <raylib.h>
#include "database.h"

using namespace std;
using std::filesystem::directory_iterator;

class EnemyPartyDatabase : public Database
{
public:
	void setup();

	//Data
	unordered_map<string, Json::Value> parties;
private:
	//Setup
	void createDatabase(Json::Value database_script);

	//Data
	Json::Value database_script;
};

