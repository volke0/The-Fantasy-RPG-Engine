#pragma once
#include <raylib.h>
#include <vector>
#include <unordered_map>
#include <filesystem>
#include <json/json.h>
#include <fstream>
#include "database.h"

using namespace std;

class ColorDatabase : public Database
{
public:
	void setup();

	//Palette Scripts
	unordered_map<string, Color> colors;
private:
	void loadAllColors();
};
