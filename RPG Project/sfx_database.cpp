#include "sfx_database.h"

void SFXDatabase::setup()
{
	path = "./databases/sfx/sfx_database.json";
	script = getScript();
	loadSFXFromPath();
}

void SFXDatabase::loadSFXFromPath()
{
	sfx_path = script["location"].asString();
	for (const auto& file : directory_iterator(sfx_path))
	{
		vector<string> ext = { ".wav" };
		if (find(ext.begin(), ext.end(), file.path().extension().string()) != ext.end())
		{
			SFX[file.path().stem().string()] = LoadSound(file.path().string().c_str());
		}
	}
}
