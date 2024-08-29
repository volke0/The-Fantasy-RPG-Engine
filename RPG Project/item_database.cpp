#include "item_database.h"

void ItemDatabase::setup()
{
	path = "./databases/items/";
	loadAllScripts();
}

void ItemDatabase::loadAllScripts()
{
	for (const auto& file : directory_iterator(path))
	{
		if (file.path().extension().string() == ".json")
		{
			ifstream temp_file(file.path().string());
			Json::Value item_script;
			reader.parse(temp_file, item_script);
			//cout << item_script["c_name"].asString() << endl;
			item_scripts[item_script["c_name"].asString()] = item_script;
		}
	}
}
