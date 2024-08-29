#include "char_anim.h"
#include "sprites.h"
#include "unit.h"
#include "weapon.h"
#include "game.h"
#include "party.h"
#include "vfx_animation.h"
#include "vars.h"

void CharacterAnimation::setup(Game* game, Unit* unit, Json::Value script, Sprites* sprite_sheet)
{
	basicSetup(game, script, sprite_sheet);
	this->unit = unit;
	initializeMetaData();
	initializeUnitMode();
}

void CharacterAnimation::initializeMetaData()
{
	max_index = script.size() - 1;
	auto& meta_data = script[0];
	name = meta_data["name"].asString();
	mode = meta_data["mode"].asString();
	if (meta_data.isMember("repeat"))
	{
		repeat = meta_data["repeat"].asBool();
	}
	if (mode == "battle")
	{
		anim_rect = &unit->battle_rect;
	}
	if (mode == "map")
	{
		map_rect = unit->map_rect;
		anim_rect = &map_rect;
	}
}

void CharacterAnimation::initializeUnitMode()
{
	initializeScript();
	initializeSprite();
	
	if ((int)x.size() > 0)
	{
		x.clear();
		x.shrink_to_fit();
	}
	if ((int)y.size() > 0)
	{
		y.clear();
		y.shrink_to_fit();
	}
	if (flip == false || anim_source_rect.width == 0.f)
	{
		anim_source_rect.width = width;
		anim_source_rect.height = height;
	}
	if (mode == "battle")
	{
		unit->battle_rect.width = width * (float)SCALE;
		unit->battle_rect.height = height * (float)SCALE;
		x.push_back(unit->bstart_x + dx[0]);
		y.push_back(unit->bstart_y + dy[0]);
		
	}
	else if (mode == "map")
	{
		map_rect.width = width * (float)SCALE;
		map_rect.height = height * (float)SCALE;
		float x_base = game->party.map_rect.x + dx[0] - (float)TILE;
		float y_base = game->party.map_rect.y + dy[0] - (1.5f * TILE) + (float)SCALE;
		x.push_back(x_base);
		y.push_back(y_base);
	}
}

void CharacterAnimation::checkIfMaxIndex()
{
	if (index >= max_index)
	{
		if (repeat == false && timer_done == true)
		{
			if (done_moving == true)
			{
				index = max_index;
				done = true;
			}
		}
		else if (repeat == true && timer_done == true && done_moving == true)
		{
			if (call_weapon == true)
			{
				index = max_index;
			}
			else
			{
				reset();
			}
		}
	}
}


void CharacterAnimation::play()
{
	checkForNextIndex();
	initializeUnitMode();
	movingLogic();
	playAlphaMode();
	generalLogic();
	checkIfMaxIndex();
	playWeaponAnimation();
	if (sheet_name != "null")
	{
		for (int i = 0; i < (int)frame_list.size(); i++)
		{
			Rectangle anime_rect = { 0.0f, 0.0f};
			if (i == 0)
			{
				anime_rect = *anim_rect;
			}
			else
			{
				anime_rect.x = x[0] + x[i];
				anime_rect.x = y[0] + y[i];
			}
			anime_rect.width = anim_rect->width;
			anime_rect.height = anim_rect->height;
			unit->anim_source_rect = frame_list[i];
			DrawTexturePro(sprite, frame_list[i], anime_rect, origin, rotation, color);
		}
	}
	playVFX();
}



