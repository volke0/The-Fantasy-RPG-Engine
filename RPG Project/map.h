#pragma once
#include <raylib.h>
#include "./lib/tileson.hpp"
#include <memory>
#include <unordered_map>
#include <string>
#include <iostream>

using namespace std;

class NPC;
class Unit;
class Game;
class GFXDatabase;

class Maps
{
public:
	//Setup
	void setup(string file, GFXDatabase* gfx);

	//Basic Methods
	void draw(vector<NPC*> units);

	//Setters
	void setWorldOffset(vector<float>& offset);
	void unloadTilesetGFX();
	void setTileChange(string tile_name);
	void undoTileChange(string tile_name);
	void AddAcceptedBlockerSet(string blocker_set_name);
	void RemoveAcceptedBlockerSet(string blocker_set_name);
	void createBlockerSet(string name, vector<Rectangle> blocker_set, int layer_pos);
	void finishedUpdatingBlockers();

	//Getters
	bool isTileChangeActive(string tile_name);
	unordered_map<int, unordered_map<string, vector<Rectangle>>> getBlockers();
	vector<string> GetAcceptedBlockerSets();
	bool NeedToUpdateBlockers();

	//Offsets
	vector<float> world_offset = { 0.0f, 0.0f };
	Vector2 player_start_pos;
	tson::Vector2i size;
private:
	//Setup
	void setupMap();
	void setupObjects();
	void setupTileChangeLayer(tson::Layer layer);
	
	//Setters
	void SetBlockers(tson::Layer layer, bool accept = false);

	//Getters
	void findAllLocations(tson::Object object);
	vector<Rectangle> findAllBlockers(tson::Layer layer);
	void getMaxLayerPosition();
	void loadTileset(string tileset_name);
	int getMaxLayerIndex(int total_layers);

	//GFX
	GFXDatabase* gfx;

	//Map
	shared_ptr<tson::Map> map;
	int max_layer_pos;
	string file = "";

	//Blockers
	unordered_map<int, unordered_map<string, vector<Rectangle>>> layer_blockers;
	vector<string> accepted_blocker_set = {};
	bool needToUpdateBlockers = true;

	//Draw Methods
	void drawLayers(int index, int total_layers, vector<tson::Layer>& layers, Rectangle tile_rect, int& layer_index);
	void drawTileLayer(tson::Layer& layer, Rectangle tile_rect, const int& layer_index);
	void drawTileChangeLayers(Rectangle tile_rect, const int& layer_index);
	void drawUnitSpriteLayer(int layer_index);

	//Units
	vector<NPC*> units;

	//Tile Changes
	vector<string> tile_changes = {};
	unordered_map<int, vector<tson::Layer>> tile_change_layers = {};

	//Textures
	unordered_map<string, Texture2D> map_textures;
};