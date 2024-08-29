#include "anim_mask.h"
#include "unit.h"

void MaskAnimation::setup(Game* game, Unit* unit, Json::Value script)
{
	this->unit = unit;
	this->game = game;
	initializeMetaData();
	setupSpriteSheet();
	basicSetup(game, script, &mask_sprite_sheet);
	initializeMaskMode();
}

void MaskAnimation::setupSpriteSheet()
{
	mask_sprite_sheet.setup(unit->anim_controller.getSpriteScript(), unit->anim_controller.getSprites(), game, unit->name + name);
}

void MaskAnimation::initializeMetaData()
{
	max_index = script.size() - 1;
	auto& meta_data = script[0];
	name = meta_data["name"].asString();
	mode = meta_data["mode"].asString();
	palette_name = meta_data["palette"].asString();

	repeat = true;

	if (mode == "battle")
	{
		anim_rect = &unit->battle_rect;
	}
	if (mode == "map")
	{
		anim_rect = &unit->map_rect;
	}
}

void MaskAnimation::initializeMaskMode()
{
	initializeScript();
	if (index_script.isMember("follow_unit"))
	{
		follow_unit = index_script["follow_unit"].asBool();
	}
	mask_sprite_sheet.setPalette(palette_name);
	sheet_name = unit->anim_controller.getCurrentAnimation()->sheet_name;
	anim_source_rect = unit->anim_controller.getCurrentAnimation()->anim_source_rect;
	done_moving = true;

	if (follow_unit == true)
	{
		anim_rect = unit->anim_controller.getCurrentAnimation()->anim_rect;
	}
	sprite = mask_sprite_sheet.getFrame(sheet_name, anim_source_rect);
}



void MaskAnimation::play()
{
	checkForNextIndex();
	initializeMaskMode();
	playAlphaMode();
	generalLogic();
	checkIfMaxIndex();

	DrawTexturePro(sprite, anim_source_rect, *anim_rect, origin, rotation, color);
}


//Setters
void MaskAnimation::setAnimation(Animations* anim)
{
	this->animation = anim;
}
