#include "unit_block_database.h"

void UnitBlockDatabase::setup()
{
	path = "./databases/unit_blocks";
	InitDatabase();
}

Json::Value UnitBlockDatabase::GetUnitBlock(string block_file_name, string unit_block_name)
{
	//cout << block_scripts[block_file_name][unit_block_name]["Units"] << endl;
	return block_scripts[block_file_name][unit_block_name]["Units"];
}

void UnitBlockDatabase::InitDatabase()
{
	CreateList();
}

void UnitBlockDatabase::CreateList()
{
	for (const auto& file : directory_iterator(path))
	{
		if (file.path().extension().string() == ".json")
		{
			ifstream temp_file(file.path().string());
			Json::Value block_script;
			reader.parse(temp_file, block_script);
			block_scripts[file.path().stem().string()] = block_script;
		}
	}
}
