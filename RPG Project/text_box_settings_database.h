#pragma once
#include <raylib.h>
#include "database.h"

using namespace std;
using std::filesystem::directory_iterator;


class TextBoxSettingsDatabase : public Database
{
public:
	//Setup
	void setup();

	//Getters
	Json::Value GetTextBoxSetting(string name);

private:
	//Setup
	void loadAllScripts();

	//Database
	unordered_map<string, Json::Value> text_box_settings;
};

