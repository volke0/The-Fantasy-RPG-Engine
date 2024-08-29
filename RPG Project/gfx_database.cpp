#include "gfx_database.h"
#include "game.h"
#include "vars.h"

void GFXDatabase::setup()
{
	path = "./databases/graphics/graphics.json";
	Json::Value script = getScript();
	loadAllGFX(script);
}

void GFXDatabase::loadAllGFX(Json::Value script)
{
	for (int index = 0; index < (int)script.size(); index++)
	{
		Json::Value location = script[index];
		if (script[index]["type"] == "UI")
		{
			loadUIMap(location);
		}
		if (script[index]["type"] == "Tilesets")
		{
			loadAllTilesets(location);
		}
		if (script[index]["type"] == "backgrounds")
		{
			loadAllBackgrounds(location);
		}
		if (script[index]["type"] == "char")
		{
			loadAllCharSprites(location);
		}
		if (script[index]["type"] == "vfx")
		{
			loadAllVFX(location);
		}
		if (script[index]["type"] == "icon")
		{
			loadAllIcons(location);
		}
		if (script[index]["type"] == "Fonts")
		{
			loadAllFonts(location);
		}
	}
}

void GFXDatabase::loadUIMap(Json::Value script)
{
	string gfx_path = "./resources/graphics/" + script["folder"].asString();
	for (const auto& file : directory_iterator(gfx_path))
	{
		vector<string> ext = {".png", ".bmp", ".jpeg"};
		if (find(ext.begin(), ext.end(), file.path().extension().string()) != ext.end())
		{
			UI[file.path().stem().string()] = LoadTexture(file.path().string().c_str());
		}
	}
}

void GFXDatabase::loadAllTilesets(Json::Value script)
{
	string gfx_path = "./resources/graphics/" + script["folder"].asString();
	for (const auto& file : directory_iterator(gfx_path))
	{
		vector<string> ext = { ".png", ".bmp", ".jpeg" };
		if (find(ext.begin(), ext.end(), file.path().extension().string()) != ext.end())
		{
			/*Image temp_img = LoadImage(file.path().string().c_str());
			ImageResizeNN(&temp_img, temp_img.width * SCALE, temp_img.height * SCALE);*/
			tileset_locations[file.path().stem().string()] = file.path().string();
		}
	}
}

void GFXDatabase::loadAllBackgrounds(Json::Value script)
{
	string gfx_path = "./resources/graphics/" + script["folder"].asString();
	for (const auto& file : directory_iterator(gfx_path))
	{
		vector<string> ext = { ".png", ".bmp", ".jpeg" };
		if (find(ext.begin(), ext.end(), file.path().extension().string()) != ext.end())
		{
			backgrounds[file.path().stem().string()] = LoadTexture(file.path().string().c_str());
		}
	}
}

void GFXDatabase::loadAllCharSprites(Json::Value script)
{
	string gfx_path = "./resources/graphics/" + script["folder"].asString();
	for (const auto& file : directory_iterator(gfx_path))
	{
		vector<string> ext = { ".png", ".bmp", ".jpeg" };
		if (find(ext.begin(), ext.end(), file.path().extension().string()) != ext.end())
		{
			char_sprites[script["name"].asString()][file.path().stem().string()] = file.path().string();
		}
	}
}

void GFXDatabase::loadAllVFX(Json::Value script)
{
	string gfx_path = "./resources/graphics/" + script["folder"].asString();
	for (const auto& file : directory_iterator(gfx_path))
	{
		vector<string> ext = { ".png", ".bmp", ".jpeg" };
		if (find(ext.begin(), ext.end(), file.path().extension().string()) != ext.end())
		{
			vfx_sprites[script["name"].asString()][file.path().stem().string()] = file.path().string();
		}
	}
}

void GFXDatabase::loadAllIcons(Json::Value script)
{
	string gfx_path = "./resources/graphics/" + script["folder"].asString();
	for (const auto& file : directory_iterator(gfx_path))
	{
		vector<string> ext = { ".png", ".bmp", ".jpeg" };
		if (find(ext.begin(), ext.end(), file.path().extension().string()) != ext.end())
		{
			icon_sprites[script["name"].asString()][file.path().stem().string()] = file.path().string();
		}
	}
}

void GFXDatabase::loadAllFonts(Json::Value script)
{
	string gfx_path = "./resources/graphics/" + script["folder"].asString();
	for (const auto& file : directory_iterator(gfx_path))
	{
		vector<string> ext = { ".ttf"};
		if (find(ext.begin(), ext.end(), file.path().extension().string()) != ext.end())
		{
			if (file.path().stem().string() == font_name)
			{
				cout << "FOND THE FONT!!!!!!!!!!!!!!!!!!!" << endl;
				font = LoadFont(file.path().string().c_str());
			}
			else
			{
				cout << "Didn't find the font..." << endl;
			}
		}
	}
}



void GFXDatabase::LoadGFX(string type, string sheet_name, string db_name)
{
	if (type == "tileset")
	{
		if (tilesets.find(sheet_name) == tilesets.end())
		{
			tilesets[sheet_name] = LoadTexture(tileset_locations[sheet_name].c_str());
		}
	}
}

void GFXDatabase::UnloadGFX(string type, string image_name, string db_name)
{
	if (type == "tileset")
	{
		if (tilesets.size() > 0)
		{
			for (auto& [key, tileset] : tilesets)
			{
				/*cout << "UNLOADED TILESET TEXTURE: " << tileset.id << endl;*/
				UnloadTexture(tileset);
			}
			tilesets.clear();
		}
	}
}


bool GFXDatabase::SpriteExists(string sprite_name)
{
	if (sprites.find(sprite_name) == sprites.end())
	{
		return false;
	}
	return true;
}

bool GFXDatabase::CheckIfValidBuffIcon(string icon_name)
{
	if (icon_sprites["buffs"].find(icon_name) != icon_sprites["buffs"].end())
	{
		return true;
	}
	return false;
}

void GFXDatabase::LoadBuffSprite(string icon_name)
{
	if (buff_sprites.find(icon_name) == buff_sprites.end())
	{
		buff_sprites[icon_name] = LoadTexture(icon_sprites["buffs"][icon_name].c_str());
	}
}

void GFXDatabase::UnloadBuffSprite(string icon_name)
{
	if (game == nullptr) return;
	if (game->closingGame) return;

	UnloadTexture(buff_sprites[icon_name]);
	buff_sprites.erase(icon_name);
}


const unordered_map<string, string>& GFXDatabase::GetBuffIcons()
{
	const unordered_map<string, string>& buff_sprite_locations = icon_sprites["buffs"];
	return buff_sprite_locations;
}

Texture2D GFXDatabase::GetBuffIcon(string icon_name)
{
	LoadBuffSprite(icon_name);
	return buff_sprites[icon_name];
}
