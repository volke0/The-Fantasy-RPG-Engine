#include "palette_database.h"

void PaletteDatabase::setup()
{
	path = "./databases/palettes/";
	loadAllPaletteScripts();
}

void PaletteDatabase::loadAllPaletteScripts()
{
	for (const auto& file : filesystem::directory_iterator(path))
	{
		vector<string> ext = { ".json" };
		if (find(ext.begin(), ext.end(), file.path().extension().string()) != ext.end())
		{
			ifstream temp_file(file.path().string());
			Json::Value palette_script;
			reader.parse(temp_file, palette_script);
			palettes[file.path().stem().string()] = palette_script;
		}
	}
}
