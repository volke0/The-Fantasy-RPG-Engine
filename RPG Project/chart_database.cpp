#include "chart_database.h"

void ChartDatabase::setup()
{
	path = "./databases/charts/";
	loadAllCharts();
}

void ChartDatabase::loadAllCharts()
{
	loadAllExpCharts();
	loadAllGrowthCharts();
}

void ChartDatabase::loadAllExpCharts()
{
	string exp_chart_path = path + "exp/";
	for (const auto& file : directory_iterator(exp_chart_path))
	{
		if (file.path().extension().string() == ".json")
		{
			ifstream temp_file(file.path().string());
			Json::Value exp_chart;
			reader.parse(temp_file, exp_chart);
			exp_charts[file.path().stem().string()] = exp_chart;
			//cout << file.path().stem().string() << endl;
		}
	}
}

void ChartDatabase::loadAllGrowthCharts()
{
	string growth_chart_path = path + "growth/";
	for (const auto& file : directory_iterator(growth_chart_path))
	{
		if (file.path().extension().string() == ".json")
		{
			ifstream temp_file(file.path().string());
			Json::Value growth_chart;
			reader.parse(temp_file, growth_chart);
			growth_charts[file.path().stem().string()] = growth_chart;
			//cout << growth_chart << endl;
		}
	}
}
