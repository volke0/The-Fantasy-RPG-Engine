#pragma once
#include <raylib.h>
#include <vector>
#include <unordered_map>
#include <filesystem>
#include <fstream>
#include "database.h"

using namespace std;
using std::filesystem::directory_iterator;

class SFXDatabase : public Database
{
public:
	void setup();

	//SFX
	unordered_map<string, Sound> SFX;
private:
	//Data
	Json::Value script;
	string sfx_path;

	//Methods
	void loadSFXFromPath();
};