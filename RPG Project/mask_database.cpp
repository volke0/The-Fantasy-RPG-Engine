#include "mask_database.h"

void MaskAnimationDatabase::setup()
{
	path = "./databases/animations/masks/";
	loadAllMaskAnimations();
}

void MaskAnimationDatabase::loadAllMaskAnimations()
{
	for (const auto& file : filesystem::directory_iterator(path))
	{
		vector<string> ext = { ".json" };
		if (find(ext.begin(), ext.end(), file.path().extension().string()) != ext.end())
		{
			ifstream temp_file(file.path().string());
			Json::Value temp_script;
			reader.parse(temp_file, temp_script);

			string name = temp_script["Settings"][0]["name"].asString();
			mask_anims[name] = temp_script;
		}
	}
}
