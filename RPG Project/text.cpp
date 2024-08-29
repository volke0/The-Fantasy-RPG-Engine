#include "text.h"
#include "game.h"
#include "vars.h"

void Text::setup(Game* game)
{
	this->game = game;
}

void Text::drawText(Vector2 pos, string text, float fontSize, bool dropShadowOn, Color color)
{
	float dropshadow_offset = (float)1 + (float)(TILE / 2 / 15);
	Vector2 drop_shadow = { pos.x + dropshadow_offset, pos.y + dropshadow_offset };
	if (dropShadowOn == true)
	{
		DrawTextEx(game->GFX.font, text.c_str(), drop_shadow, fontSize, 0.0f, BLACK);
	}
	DrawTextEx(game->GFX.font, text.c_str(), pos, fontSize, 0.0f, color);
}
