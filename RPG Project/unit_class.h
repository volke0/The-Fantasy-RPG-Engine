#pragma once
#include <raylib.h>
#include <memory>
#include <unordered_map>
#include <json/json.h>
#include <iostream>
#include <string>

using namespace std;

class Game;
class Unit;

class UnitClass
{
public:
	void setup(Game* game, Unit* unit, string class_name);

	//Charts
	Json::Value exp_chart;
	Json::Value growth_chart;

	//Data
	string ingame_name = "";

	//Stats
	Json::Value max_stats;

	//Logic
	bool exp_chart_initialized = false;
private:
	//Setup
	void setupCharts();
	void setupStats();
	string class_name;
	Json::Value script;

	//Setup Charts
	void setupExpChart(Json::Value chart_list);
	void setupGrowthChart(Json::Value chart_list);

	//Data
	Game* game;
	Unit* unit;
};
