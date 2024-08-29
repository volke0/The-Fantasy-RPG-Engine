#include "class_database.h"

void ClassDatabase::setup()
{
	path = "./databases/classes/";
	loadAllScripts();
}

void ClassDatabase::loadAllScripts()
{
	for (const auto& file : directory_iterator(path))
	{
		if (file.path().extension().string() == ".json")
		{
			ifstream temp_file(file.path().string());
			Json::Value class_script;
			reader.parse(temp_file, class_script);
			class_scripts[file.path().stem().string()] = class_script;
			//cout << file.path().stem().string() << endl;
		}
	}
}
