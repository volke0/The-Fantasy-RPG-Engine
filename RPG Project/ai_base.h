#pragma once
#include <raylib.h>
#include <iostream>
#include <vector>
#include <memory>
#include <unordered_map>
#include <string>
#include <json/json.h>

using namespace std;

class Game;
class Unit;

class AIBase
{
public:
	void basic_setup(Game* game, Unit* user);
	void setupAIScript(string ai_type, string ai_script_name);
	Game* game = nullptr;
	Unit* user = nullptr;
	Json::Value ai_script;
	Json::Reader script_reader;
	Json::Value input;
	int rng_frame = 0;

	//Script Parser
	virtual void ParseAIBehavior();

	//Behavior Methods
	virtual void RunBehavior();
	virtual void Evaluate();
	virtual void ParseMetaData();

	virtual void GetSelectors();
	virtual void ExecuteCommands();

	//Data
	Json::Value meta_data;
	Json::Value selectors;

	Json::Value selector_line;
};
