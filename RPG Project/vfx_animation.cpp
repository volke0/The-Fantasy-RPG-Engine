#include "vfx_animation.h"
#include "unit.h"
#include "game.h"

void VFXAnimation::setup(Game* game, Json::Value script)
{
	this->game = game;
	this->base_script = script;
	initializeSettings();
	basicSetup(game, this->script, &vfx_sprite_sheet);
	initializeMetaData();
	initializeVFXMode();
}

void VFXAnimation::initializeSettings()
{
	getValues();
	getDatabases();
	setupSpriteSettings();
}

void VFXAnimation::getValues()
{
	settings = base_script["Settings"][0];
	graphics_db_name = settings["graphics"].asString();
	sprite_db_name = settings["sprite_settings"].asString();
	name = settings["name"].asString();
}

void VFXAnimation::getDatabases()
{
	if (graphics_db_name.size() > 0)
	{
		sprites = game->GFX.vfx_sprites[graphics_db_name];
	}
	if (sprite_db_name.size() > 0)
	{
		string path = "./databases/sprite_settings/" + sprite_db_name + ".json";
		ifstream file(path);
		reader.parse(file, sprite_script);
	}
	if (base_script.isMember("Animation"))
	{
		script = base_script["Animation"];
	}
}

void VFXAnimation::setupSpriteSettings()
{
	vfx_sprite_sheet.setup(sprite_script, sprites, game, graphics_db_name);
}



void VFXAnimation::initializeMetaData()
{
	max_index = script.size() - 1;
	auto& meta_data = script[0];
	name = meta_data["name"].asString();
	mode = meta_data["mode"].asString();
	unit_name = "";
	if (meta_data.isMember("unit"))
	{
		unit_name = meta_data["unit"].asString();
		if (unit_name != "user")
		{
			unit = game->unit_list[unit_name];
		}
	}
	if (meta_data.isMember("repeat"))
	{
		repeat = meta_data["repeat"].asBool();
	}
	
	if (meta_data.isMember("wait"))
	{
		wait = meta_data["wait"].asInt();
	}


	if (mode == "battle")
	{
		anim_rect = &battle_rect;
	}
	if (mode == "map")
	{
		anim_rect = &map_rect;
	}
}



void VFXAnimation::initializeVFXScript()
{
	if (index_script.isMember("blend_mode"))
	{ 
		blend_on = true;
		blend_mode = index_script["blend_mode"].asInt();
	}
	if (index_script.isMember("blend_end"))
	{
		blend_on = false;
	}
}



void VFXAnimation::initializeVFXMode()
{
	initializeScript();
	initializeVFXScript();
	initializeSprite();

	if (flip == false)
	{
		anim_source_rect.width = width;
		anim_source_rect.height = height;
	}

	initializeXY();
	
	if (mode == "battle")
	{
		battle_rect.width = width * (float)SCALE;
		battle_rect.height = height * (float)SCALE;

	}
	else if (mode == "map")
	{
		map_rect.width = width * (float)SCALE;
		map_rect.height = height * (float)SCALE;
	}
}

void VFXAnimation::initializeXY()
{
	x.clear();
	y.clear();
	bool bUnit = false;
	bool bTarget = false;
	for (int i = 0; i < frame_size; i++)
	{
		if (on_character == true && unit)
		{
			x.push_back(unit->battle_rect.x + dx[i]);
			y.push_back(unit->battle_rect.y + dy[i]);
			bUnit = true;
		}
		else if (on_target == true && target)
		{
			x.push_back(target->battle_rect.x + dx[i]);
			y.push_back(target->battle_rect.y + dy[i]);
			bTarget = true;
		}
		else if (!on_character && !on_target)
		{
			x.push_back(dx[i]);
			y.push_back(dy[i]);
		}
		else if (!bUnit && !bTarget)
		{
			//TODO: Add auto centering or some sort of image alignment later...
			x.push_back(dx[i]);
			y.push_back(dy[i]);
		}
	}
}


void VFXAnimation::checkIfMaxIndex()
{
	if (index >= max_index)
	{
		if (repeat == false && timer_done == true && done_moving == true)
		{
			animationDone();
		}
		else if (repeat == true && timer_done == true && done_moving == true)
		{
			reset();
		}
	}
}




void VFXAnimation::play()
{
	checkForNextIndex();
	initializeVFXMode();
	movingLogic();
	playAlphaMode();
	generalLogic();
	checkIfMaxIndex();

	playNormalMode();
}



void VFXAnimation::playNormalMode()
{
	if (sheet_name != "null")
	{
		for (int i = 0; i < (int)frame_list.size(); i++)
		{
			Rectangle anime_rect = { x[i], y[i], anim_rect->width, anim_rect->height };

			if (blend_on == true)
			{
				BeginBlendMode(blend_mode);
			}
			DrawTexturePro(sprite, frame_list[i], anime_rect, origin, rotation, color);
			EndBlendMode();
		}
		
	}
	playVFX();
}


