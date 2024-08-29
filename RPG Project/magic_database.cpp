#include "magic_database.h"

void MagicDatabase::setup()
{
	path = "./databases/magic/scripts";
	loadAllScripts();
	loadOrderList();
	createMasterList();
}

Json::Value MagicDatabase::GetMagicScript(string stem_name)
{
	return magic_scripts[stem_name];
}

void MagicDatabase::loadOrderList()
{
	string ordered_list_path = "./databases/magic/magic.json";
	ifstream temp_file(ordered_list_path);
	Json::Value ordered_list;
	reader.parse(temp_file, ordered_list);

	for (int i = 0; i < (int)ordered_list.size(); i++)
	{
		order_database.push_back(ordered_list[i]);
	}
}

void MagicDatabase::createMasterList()
{
	vector<string> temp_option_list;
	int index = 0;
	int order_db_index = 0;
	int max_index = (int)order_database.size() - 1;
	while (true)
	{
		if (order_db_index > max_index)
		{
			break;
		}
		int pos = order_database[order_db_index]["pos"].asInt();
		if (index == pos)
		{
			temp_option_list.push_back(order_database[order_db_index]["name"].asString());
			index++;
			order_db_index++;
		}
		else
		{
			string str = "";
			temp_option_list.push_back(str);
			index++;
		}
	}
	master_list = temp_option_list;
}

void MagicDatabase::loadAllScripts()
{
	for (const auto& file : directory_iterator(path))
	{
		if (file.path().extension().string() == ".json")
		{
			ifstream temp_file(file.path().string());
			Json::Value magic_script;
			reader.parse(temp_file, magic_script);
			magic_scripts[file.path().stem().string()] = magic_script;
			//cout << file.path().stem().string() << endl;
		}
	}
}
