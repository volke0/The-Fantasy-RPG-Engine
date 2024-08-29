#include "color_database.h"

void ColorDatabase::setup()
{
	path = "./databases/colors/";
	loadAllColors();
}

void ColorDatabase::loadAllColors()
{
	for (const auto& file : filesystem::directory_iterator(path))
	{
		vector<string> ext = { ".json" };
		if (find(ext.begin(), ext.end(), file.path().extension().string()) != ext.end())
		{
			ifstream temp_file(file.path().string());
			Json::Value color_script;
			reader.parse(temp_file, color_script);
			Color color = { (unsigned char)color_script["r"].asInt(), (unsigned char)color_script["g"].asInt(), (unsigned char)color_script["b"].asInt(), 255 };
			colors[file.path().stem().string()] = color;
		}
	}
}
