#include "text_char_anim.h"
#include "game.h"
#include "unit.h"

void TextCharAnimation::setup(Game* game, Unit* unit, Json::Value script, char text, Json::Value meta_data)
{
	this->meta_data = meta_data;
	this->text = text;
	basicSetup(game, script);
	this->unit = unit;
	initializeMetaData();
}


void TextCharAnimation::initializeMetaData()
{
	max_index = script.size() - 1;
	color.r = (unsigned char)meta_data["color"]["r"].asInt();
	color.g = (unsigned char)meta_data["color"]["g"].asInt();
	color.b = (unsigned char)meta_data["color"]["b"].asInt();
	position = meta_data["position"].asInt();
	fontSize = meta_data["fontSize"].asFloat();
	name = meta_data["name"].asString();
	//repeat = meta_data["repeat"].asBool();
	repeat = false;

	anim_rect = &battle_rect;
}

void TextCharAnimation::initializeTextMode()
{
	initializeScript();
	x.push_back(unit->battle_rect.x + dx[0] + ((float)(fontSize/1.7) * position));
	y.push_back(unit->battle_rect.y + dy[0]);
}


void TextCharAnimation::play()
{
	checkForNextIndex();
	initializeTextMode();
	movingLogic();
	playAlphaMode();
	dropshadow_color = Fade(dropshadow_color, alpha_ratio);
	generalLogic();
	checkIfMaxIndex();

	Vector2 drop_shadow = { anim_rect->x + dropshadow_offset, anim_rect->y + dropshadow_offset };
	Vector2 text_coords = { anim_rect->x, anim_rect->y };
	DrawTextEx(game->GFX.font, text.c_str(), drop_shadow, fontSize, 0.0f, dropshadow_color);
	DrawTextEx(game->GFX.font, text.c_str(), text_coords, fontSize, 0.0f, color);
}
