#include "cursor.h"
#include "game.h"
#include "input_handler.h"

Cursor::~Cursor()
{
}

void Cursor::basicSetup(Game* game)
{
	this->game = game;
	this->input = game->scene_manager.input;
	this->type = "cursor";
	sound_player.setup(game);
	cursor = game->GFX.UI["cursor"];
	src = { 0, 0, (float)cursor.width, (float)cursor.height };
	og_width = src.width;
}

void Cursor::setCursor(Vector2 pos, vector<string> options, bool horizontal, int max_vis_index)
{
	dest.x = pos.x - 36;
	dest.y = pos.y + 3;
	this->options = options;
	src.width = og_width;
}

void Cursor::unloadSprite()
{
	UnloadTexture(cursor);
}

void Cursor::SetMultiTargetMode(bool bMultiOn)
{
}

bool Cursor::FindKey(string key_name)
{
	return input->findKey(key_name);
}
