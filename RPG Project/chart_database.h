#pragma once
#include <raylib.h>
#include <string>
#include <unordered_map>
#include <filesystem>
#include <fstream>
#include <json/json.h>
#include "database.h"

using namespace std;
using std::filesystem::directory_iterator;

class ChartDatabase : public Database
{
public:
	void setup();
	//Charts
	unordered_map<string, Json::Value> exp_charts;
	unordered_map<string, Json::Value> growth_charts;
private:
	//Setup
	void loadAllCharts();
	void loadAllExpCharts();
	void loadAllGrowthCharts();
};