#include "ai_database.h"

void AIDatabase::setup()
{
	ai_scripts["battle"] = SetupAIScripts("battle");
	ai_scripts["map"] = SetupAIScripts("map");
}



Json::Value AIDatabase::GetAIScript(string ai_type, string ai_script_name)
{
	return ai_scripts[ai_type][ai_script_name];
}


unordered_map<string, Json::Value> AIDatabase::SetupAIScripts(string ai_type)
{
	string temp_path = "./databases/ai/" + ai_type;
	unordered_map<string, Json::Value> temp_scripts = {};
	for (const auto& file : directory_iterator(temp_path))
	{
		if (file.path().extension().string() == ".json")
		{
			ifstream temp_file(file.path().string());
			Json::Value unit_script;
			reader.parse(temp_file, unit_script);
			temp_scripts[file.path().stem().string()] = unit_script;
		}
	}
	return temp_scripts;
}
