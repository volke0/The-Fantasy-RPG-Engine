#include "shop_database.h"

void ShopDatabase::setup()
{
	path = "./databases/shops/";
	loadAllScripts();
}

void ShopDatabase::loadAllScripts()
{
	for (const auto& file : directory_iterator(path))
	{
		if (file.path().extension().string() == ".json")
		{
			ifstream temp_file(file.path().string());
			Json::Value shop_script;
			reader.parse(temp_file, shop_script);
			shop_scripts[shop_script["name_id"].asString()] = shop_script;
		}
	}
}
