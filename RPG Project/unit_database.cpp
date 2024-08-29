#include "unit_database.h"

void UnitDatabase::setup()
{
	path = "./databases/units/scripts";
	InitDatabase();
	//ShowUnitDatabase();
}

Json::Value UnitDatabase::GetUnit(int unit_id)
{
	return unit_scripts[unit_id];
}

string UnitDatabase::GetUnitName(int unit_id)
{
	return unit_scripts[unit_id]["name"].asString();
}

void UnitDatabase::InitDatabase()
{
	string database_path = "./databases/units/unit_database.json";
	unordered_map<string, Json::Value> temp_unit_list = loadAllScripts();

	ifstream database_file(database_path);
	Json::Value temp_database;
	reader.parse(database_file, temp_database);
	
	for (int i = 0; i < (int)temp_database.size(); i++)
	{
		int unit_id = temp_database[i]["id"].asInt();
		string unit_name = temp_database[i]["unit"].asString();
		unit_scripts[unit_id] = temp_unit_list[unit_name];
	}
}

unordered_map<string, Json::Value> UnitDatabase::loadAllScripts()
{
	unordered_map<string, Json::Value> temp_unit_scripts;
	for (const auto& file : directory_iterator(path))
	{
		if (file.path().extension().string() == ".json")
		{
			ifstream temp_file(file.path().string());
			Json::Value unit_script;
			reader.parse(temp_file, unit_script);
			temp_unit_scripts[file.path().stem().string()] = unit_script;
			//cout << file.path().stem().string() << endl;
		}
	}
	return temp_unit_scripts;
}

void UnitDatabase::ShowUnitDatabase()
{
	for (auto& Pair : unit_scripts)
	{
		cout << "ID: " << Pair.first << " UNIT: " << Pair.second << endl;
	}
}
