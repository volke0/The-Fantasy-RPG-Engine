#include "buff_database.h"

void BuffDatabase::setup()
{
	path = "./databases/buffs/";
	InitDatabase();
}

void BuffDatabase::InitDatabase()
{
	for (const auto& file : directory_iterator(path))
	{
		if (file.path().extension().string() == ".json")
		{
			ifstream temp_file(file.path().string());
			Json::Value buff_script;
			reader.parse(temp_file, buff_script);
			string buff_name = buff_script["c_name"].asString();
			buff_scripts[buff_name] = buff_script;
			//cout << buff_script << endl;
		}
	}
}



Json::Value BuffDatabase::GetBuffScript(string buff_name)
{
	return buff_scripts[buff_name];
}

