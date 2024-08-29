#include "vfx_anim_database.h"
#include "vfx_animation.h"
#include "game.h"

void VFXAnimationDatabase::setup(Game* game)
{
	this->game = game;
	loadAllVFXAnimations();
}

void VFXAnimationDatabase::loadAllVFXAnimations()
{
	string vfx_anim_path = "./databases/animations/vfx/";
	for (const auto& file : filesystem::directory_iterator(vfx_anim_path))
	{
		vector<string> ext = { ".json" };
		if (find(ext.begin(), ext.end(), file.path().extension().string()) != ext.end())
		{
			ifstream temp_file(file.path().string());
			Json::Value temp_script;
			reader.parse(temp_file, temp_script);

			string name = temp_script["Settings"][0]["name"].asString();
			vfx_anims[name] = temp_script;
		}
	}
}
