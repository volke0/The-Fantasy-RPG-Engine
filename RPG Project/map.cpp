#include "map.h"
#include <any>
#include "./lib/cpp_assert.hpp"
#include "unit.h"
#include "vars.h"
#include "gfx_database.h"
#include "npc.h"
#include "game.h"

void Maps::setup(string file, GFXDatabase* gfx)
{
	this->gfx = gfx;
	this->file = file;
	tson::Tileson parser;
	map = parser.parse(fs::path("./databases/maps/" + file + ".json"));
	setupMap();
}

void Maps::setupMap()
{
	setupObjects();
	getMaxLayerPosition();
	size = map->getSize();
	cout << "WORLD OFFSET SIZE: " << world_offset.size() << endl;
	//cout << "BLOCKER SIZE : " << blockers.size() << endl;
}

void Maps::setupObjects()
{
	if (map->getStatus() == tson::ParseStatus::OK)
	{
		for (auto& layer : map->getLayers())
		{
			if (layer.getType() == tson::LayerType::ObjectGroup)
			{
				SetBlockers(layer, true);
			}
			if (layer.getType() == tson::LayerType::Group)
			{
				auto& grouped_layers = layer.getLayers();
				for (auto& grouped_layer : grouped_layers)
				{
					auto tile_map = map->getTileMap();
					grouped_layer.assignTileMap(&tile_map);
					grouped_layer.createTileData(map->getSize(), false);
					if (grouped_layer.getType() == tson::LayerType::ObjectGroup)
					{
						SetBlockers(grouped_layer);
					}
					if (grouped_layer.getType() == tson::LayerType::TileLayer)
					{
						setupTileChangeLayer(grouped_layer);
					}
				}
			}
		}
	}
}

void Maps::SetBlockers(tson::Layer layer, bool accept)
{
	vector<Rectangle> blockers = findAllBlockers(layer);
	for (auto& object : layer.getObjects())
	{
		findAllLocations(object);
	}

	//Check to see if there is a layer property in the object layer
	string msg = "Cannot find the layer property in the Object Layer: " + layer.getName() + ". On the tiled map: " + file + ".tmx";
	log_assert((layer.getProp("layer") == nullptr), msg);

	int layer_pos = any_cast<int>(layer.getProp("layer")->getValue());
	layer_blockers[layer_pos][layer.getName()] = blockers;

	if (accept)
	{
		accepted_blocker_set.push_back(layer.getName());
	}
}

void Maps::setupTileChangeLayer(tson::Layer layer)
{
	//Check to see if there is a layer property in the tile change layer
	string msg = "Cannot find the layer property in the Tile Layer: " + layer.getName() + ". On the tiled map: " + file + ".tmx";
	log_assert((layer.getProp("layer") == nullptr), msg);

	int layer_pos = any_cast<int>(layer.getProp("layer")->getValue());
	tile_change_layers[layer_pos].push_back(layer);
}

void Maps::findAllLocations(tson::Object object)
{
	if (object.getName() == "player_start")
	{
		if (object.get<bool>("game_start") == true)
		{
			player_start_pos.x = (object.getPosition().x * (float)SCALE) + (float)world_offset[0];
			player_start_pos.y = (object.getPosition().y * (float)SCALE) + (float)world_offset[1];
		}
	}
}

vector<Rectangle> Maps::findAllBlockers(tson::Layer layer)
{
	vector<Rectangle> temp_blockers = {};
	for (auto& object : layer.getObjects())
	{
		if (object.getName() == "blocker")
		{
			Rectangle rect = { 0, 0, (float)object.getSize().x, (float)object.getSize().y };
			rect.x = (float)object.getPosition().x;
			rect.y = (float)object.getPosition().y;
			temp_blockers.push_back(rect);
			//cout << "X: " << rect.x/TILE << " Y: " << rect.y/TILE << " WIDTH: " << rect.width << " HEIGHT: " << rect.height << endl;
		}
	}
	return temp_blockers;
}

void Maps::getMaxLayerPosition()
{
	if (map->getSize().x > map->getSize().y)
	{
		max_layer_pos = map->getSize().x;
	}
	else
	{
		max_layer_pos = map->getSize().y;
	}
}


void Maps::draw(vector<NPC*> units)
{
	this->units = units;
	Rectangle tile_rect = { 0.f, 0.f, 0.f, 0.f };
	tile_rect.x = 0.0f;
	tile_rect.y = 0.0f;
	tile_rect.width = (float)map->getTileSize().x;
	tile_rect.height = (float)map->getTileSize().y;

	vector<tson::Layer> layers = map->getLayers();
	int total_layers = (int)layers.size() - 1;
	int max_index = getMaxLayerIndex(total_layers);

	int layer_index = 0;
	for (int i = 0; i <= max_index; i++)
	{
		drawLayers(i, total_layers, layers, tile_rect, layer_index);
		drawUnitSpriteLayer(layer_index);
		layer_index++;
	}
}

void Maps::drawLayers(int index, int total_layers, vector<tson::Layer>& layers, Rectangle tile_rect, int& layer_index)
{
	if (index <= total_layers)
	{
		auto& layer = layers[index];
		if (layer.getType() == tson::LayerType::TileLayer)
		{
			drawTileLayer(layer, tile_rect, layer_index);
		}
		drawTileChangeLayers(tile_rect, layer_index);
	}
}

void Maps::drawTileChangeLayers(Rectangle tile_rect, const int& layer_index)
{
	if (tile_change_layers.find(layer_index) != tile_change_layers.end())
	{
		auto& tile_change_layers_list = tile_change_layers[layer_index];
		for (auto& tile_change_layer : tile_change_layers_list)
		{
			auto tile_map = map->getTileMap();
			tile_change_layer.assignTileMap(&tile_map);
			tile_change_layer.createTileData(map->getSize(), false);
			if (find(tile_changes.begin(), tile_changes.end(), tile_change_layer.getName()) != tile_changes.end())
			{
				drawTileLayer(tile_change_layer, tile_rect, layer_index);
			}
		}
	}
}

void Maps::drawTileLayer(tson::Layer& layer, Rectangle tile_rect, const int& layer_index)
{
	float scale = (float)SCALE;
	for (auto& [pos, tile] : layer.getTileData())
	{
		if (tile != nullptr)
		{
			int firstId = tile->getTileset()->getFirstgid(); //First tile id of the tileset
			int columns = tile->getTileset()->getColumns();
			int margin = tile->getTileset()->getMargin();
			int space = tile->getTileset()->getSpacing();

			tson::Vector2f position = { (float)std::get<0>(pos) * map->getTileSize().x * scale,
										(float)std::get<1>(pos) * map->getTileSize().y * scale };
			int baseTilePosition = (tile->getId() - firstId);

			int tileModX = (baseTilePosition % columns);
			int currentRow = (baseTilePosition / columns);
			int offsetX = (tileModX * (map->getTileSize().x + space) + margin);
			int offsetY = (currentRow * (map->getTileSize().y + space) + margin);

			tile_rect.x = (float)offsetX;
			tile_rect.y = (float)offsetY;
			Rectangle tile_dest = { position.x + world_offset[0], position.y + world_offset[1], tile_rect.width * scale, tile_rect.height * scale };
			loadTileset(tile->getTileset()->getName());
			DrawTexturePro(gfx->tilesets[tile->getTileset()->getName()], tile_rect, tile_dest, Vector2 { 0.f, 0.f }, 0.f, WHITE);
		}
	}
}



void Maps::loadTileset(string tileset_name)
{
	gfx->LoadGFX("tileset", tileset_name);
}

void Maps::unloadTilesetGFX()
{
	gfx->UnloadGFX("tileset");
}

int Maps::getMaxLayerIndex(int total_layers)
{
	int max_index = total_layers;
	if (units.size() > 0)
	{
		for (auto& unit : units)
		{
			if (unit->layer_pos > max_index)
			{
				max_index = unit->layer_pos;
			}
		}
	}
	return max_index;
}

void Maps::drawUnitSpriteLayer(int layer_index)
{
	if (units.size() > 0)
	{
		for (int i = 0; i < (int)units.size(); i++)
		{
			auto& unit = units[i];
			if (unit->layer_pos == layer_index)
			{
				unit->draw();
			}
		}
	}
}

void Maps::setWorldOffset(vector<float>& offset)
{
	world_offset[0] += offset[0];
	world_offset[1] += offset[1];
	if (units.size() > 0)
	{
		for (auto& unit : units)
		{
			if (unit->name != "party")
			{
				unit->movement_controller.updateMapRect(offset);
			}
		}
	}
}

void Maps::setTileChange(string tile_name)
{
	if (find(tile_changes.begin(), tile_changes.end(), tile_name) == tile_changes.end())
	{
		tile_changes.push_back(tile_name);
	}
}

void Maps::undoTileChange(string tile_name)
{
	if (find(tile_changes.begin(), tile_changes.end(), tile_name) != tile_changes.end())
	{
		if ((int)tile_changes.size() > 0)
		{
			int index = 0;
			for (auto& name : tile_changes)
			{
				if (name == tile_name)
				{
					break;
				}
				index++;
			}
			tile_changes.erase(tile_changes.begin() + index);
			tile_changes.shrink_to_fit();
		}
	}
}

void Maps::AddAcceptedBlockerSet(string blocker_set_name)
{
	if (find(accepted_blocker_set.begin(), accepted_blocker_set.end(), blocker_set_name) == accepted_blocker_set.end())
	{
		accepted_blocker_set.push_back(blocker_set_name);
	}
}

void Maps::RemoveAcceptedBlockerSet(string blocker_set_name)
{
	if (find(accepted_blocker_set.begin(), accepted_blocker_set.end(), blocker_set_name) != accepted_blocker_set.end())
	{
		if ((int)accepted_blocker_set.size() > 0)
		{
			int index = 0;
			for (auto& name : accepted_blocker_set)
			{
				if (name == blocker_set_name)
				{
					break;
				}
				index++;
			}
			accepted_blocker_set.erase(accepted_blocker_set.begin() + index);
			accepted_blocker_set.shrink_to_fit();
		}
	}
}

void Maps::createBlockerSet(string name, vector<Rectangle> blocker_set, int layer_pos)
{
	layer_blockers[layer_pos][name] = blocker_set;
	accepted_blocker_set.push_back(name);
	needToUpdateBlockers = true;
}

void Maps::finishedUpdatingBlockers()
{
	needToUpdateBlockers = false;
}

bool Maps::isTileChangeActive(string tile_name)
{
	if (find(tile_changes.begin(), tile_changes.end(), tile_name) != tile_changes.end())
	{
		return true;
	}
	return false;
}

unordered_map<int, unordered_map<string, vector<Rectangle>>> Maps::getBlockers()
{
	return layer_blockers;
}

vector<string> Maps::GetAcceptedBlockerSets()
{
	return accepted_blocker_set;
}

bool Maps::NeedToUpdateBlockers()
{
	return needToUpdateBlockers;
}
