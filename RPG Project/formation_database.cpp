#include "formation_database.h"

void FormationDatabase::setup()
{
	path = "./databases/formations";
	loadAllScripts();
}

void FormationDatabase::loadAllScripts()
{
	for (const auto& file : directory_iterator(path))
	{
		if (file.path().extension().string() == ".json")
		{
			ifstream temp_file(file.path().string());
			Json::Value formation_script;
			reader.parse(temp_file, formation_script);
			formation_scripts[formation_script["Settings"]["name"].asString()] = formation_script;
			/*cout << formation_scripts[file.path().stem().string()] << endl;*/
		}
	}
}
