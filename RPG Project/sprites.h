#pragma once
#include <raylib.h>
#include <json/json.h>
#include <vector>
#include <unordered_map>
#include <iostream>

class Game;

using namespace std;

class Sprites
{
public:
	~Sprites();
	//Setup
	void setup(Json::Value sprite_script, unordered_map<string, string> sprites, Game* game, string graphics_db_name);

	//Getters
	Texture2D getFrame(string sheet_name, int frame, Rectangle& source_rect);
	Texture2D getFrame(string sheet_name, Rectangle source_rect);

	//Setters
	void setPalette(string palette_name);
	void turnOffPalette();
	void unloadSprite();

	//Data
	unordered_map<string, float> width_list;
	unordered_map<string, float> height_list;
	unordered_map<string, int> scale_list;
	unordered_map<string, int> max_colors;
	unordered_map<string, Color*> colors;
private:
	Json::Value sprite_script;

	//Setup
	void recordData();

	//Data
	Game* game;
	Image image;
	Image palette_image;
	string sheet_name;
	bool unloadedSprites = false;

	//Setters
	void CreateSprites();

	//Palette
	bool palette_on;
	string palette_name;
	Rectangle palette_rect;
	Color color_palette;
	Json::Value palette_script;
	void changeTexturePalette();

	//Debug

	//Sprites
	unordered_map<string, string> sprite_names;
	vector<string> sprites;
	Texture2D sprite;
	Texture2D temp_sprite;
	string graphics_db_name = "";

	//Others
	vector<float> getPosition(int frame, float width, float height, Texture2D sprite);
};