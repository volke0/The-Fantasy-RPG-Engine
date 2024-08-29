#include "menu_cursor.h"
#include "vars.h"
#include "game.h"
#include "input_handler.h"
#include "unit.h"

MenuCursor::~MenuCursor()
{
	/*if (click.frameCount > 0)
	{
		UnloadSound(click);
	}*/
}

void MenuCursor::setup(Game* game)
{
	basicSetup(game);
	dest = { (float)0 * TILE, (float)0 * TILE, (float)(cursor.width) * 2, (float)(cursor.height) * 2 };
	click = game->sfx_database.SFX["click"];
	manager.setup(game, this);
}

void MenuCursor::update()
{
	if (on == true)
	{
		manager.update();
		sound_player.update();
	}
}

void MenuCursor::draw()
{
	if (on == true)
	{
		if (magic_mode == false)
		{
			x_factor = 0;
		}
		y_factor = (float)(vis_spacing * (vis_index));

		float x = dest.x + x_factor;
		float y = dest.y + y_factor;
		Rectangle new_dest = { x, y, dest.width, dest.height };
		DrawTexturePro(cursor, src, new_dest, Vector2{ 0, 0 }, 0.0f, WHITE);
	}
}

void MenuCursor::turnOn()
{
	if (on)
	{
		on = false;
	}
	else
	{
		on = true;
	}
}

void MenuCursor::changeVisSpacing(float vis_spacing)
{
	this->vis_spacing = vis_spacing;
}

bool MenuCursor::findKey(string key)
{
	return input->findKey(key);
}
