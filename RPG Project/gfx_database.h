#pragma once
#include <raylib.h>
#include <vector>
#include <unordered_map>
#include <filesystem>
#include "database.h"

using namespace std;
using std::filesystem::directory_iterator;

class GFXDatabase : public Database
{
public:
	void setup();

	//Graphics
	unordered_map<string, Texture2D> UI;
	unordered_map<string, Texture2D> tilesets;
	unordered_map<string, string> tileset_locations;
	unordered_map<string, unordered_map<string, string>> char_sprites;
	unordered_map<string, unordered_map<string, string>> vfx_sprites;
	unordered_map<string, unordered_map<string, string>> icon_sprites;
	unordered_map<string, Texture2D> backgrounds;
	unordered_map<string, Texture2D> sprites;
	Font font;

	//Setters
	void LoadGFX(string type, string sheet_name, string db_name = "");
	void LoadBuffSprite(string icon_name);
	void UnloadBuffSprite(string icon_name);
	void UnloadGFX(string type, string sheet_name = "", string db_name = "");

	//Getters
	bool SpriteExists(string sprite_name);
	bool CheckIfValidBuffIcon(string icon_name);
	const unordered_map<string, string>& GetBuffIcons();
	Texture2D GetBuffIcon(string icon_name);
private:
	//Setup
	void loadAllGFX(Json::Value script);
	void loadUIMap(Json::Value script);
	void loadAllTilesets(Json::Value script);
	void loadAllBackgrounds(Json::Value script);
	void loadAllCharSprites(Json::Value script);
	void loadAllVFX(Json::Value script);
	void loadAllIcons(Json::Value script);
	void loadAllFonts(Json::Value script);

	//Graphic locations
	vector<string> paths;

	//Font being used
	string font_name = "retro";

	//Data
	unordered_map<string, Texture2D> buff_sprites;
};
