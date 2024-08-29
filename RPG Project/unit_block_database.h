#pragma once
#include "raylib.h"
#include "database.h"

using namespace std;
using std::filesystem::directory_iterator;

class UnitBlockDatabase : public Database
{
public:
	void setup();

	//Getters
	Json::Value GetUnitBlock(string block_file_name, string unit_block_name);

private:
	//Setup
	void InitDatabase();
	void CreateList();

	//Block list
	unordered_map<string, Json::Value> block_scripts;
};