#include "unit_class.h"
#include "game.h"
#include "unit.h"

void UnitClass::setup(Game* game, Unit* unit, string class_name)
{
	this->game = game;
	this->unit = unit;
	this->class_name = class_name;
	script = game->class_database.class_scripts[class_name];
	setupCharts();
	setupStats();
}

void UnitClass::setupCharts()
{
	if (script.isMember("charts"))
	{
		auto& chart_list = script["charts"];
		setupExpChart(chart_list);
		setupGrowthChart(chart_list);
	}
}

void UnitClass::setupStats()
{
	if (script.isMember("max_stats"))
	{
		max_stats = script["max_stats"];
	}
	if (script.isMember("ingame_name"))
	{
		ingame_name = script["ingame_name"].asString();
	}
}

void UnitClass::setupExpChart(Json::Value chart_list)
{
	if (chart_list.isMember("exp_chart"))
	{
		exp_chart = game->chart_database.exp_charts[chart_list["exp_chart"].asString()];
		exp_chart_initialized = true;
	}
}

void UnitClass::setupGrowthChart(Json::Value chart_list)
{
	if (chart_list.isMember("growth_chart"))
	{
		growth_chart = game->chart_database.growth_charts[chart_list["growth_chart"].asString()];
	}
}
