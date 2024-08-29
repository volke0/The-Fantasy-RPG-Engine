#include "config_status_menu.h"
#include "game.h"
#include "menu_cursor.h"

void ConfigStatusMenu::setup(Game* game, PlayerStatusMenu* player_status_menu)
{
	SubStatusMenu::setup(game, player_status_menu);
	options = { "Fullscreen" };
	vis_x = 1.0f * TILE;
	vis_y = 1.f * TILE;
	max_vis_index = 15;
	vis_spacing = 30.f;
}


void ConfigStatusMenu::update()
{
	SubStatusMenu::update();
	if (cursor->go_back)
	{
		scene_manager->pop();
		cursor->go_back = false;
	}
	if (cursor->return_input)
	{
		if (cursor->selected_option == "Fullscreen")
		{
			/*if (SCALE == 3)
			{
				SCALE = 4;
			}
			else if (SCALE == 4)
			{
				SCALE = 3;
			}
			TILE = TILE_SIZE * SCALE;
			int newWidth = 17 * TILE;
			int newHeight = 13 * TILE;
			SetWindowSize(newWidth, newHeight);*/
		}
		cursor->return_input = false;
	}
}


void ConfigStatusMenu::draw()
{
	DrawBackground();
	DrawOptions();
	cursor->draw();
}

void ConfigStatusMenu::DrawBackground()
{
	ClearBackground(BLACK);
	auto& background = game->GFX.UI["config_menu"];
	Rectangle source_rect = { 0, 0, (float)(background.width), (float)(background.height) };
	Rectangle dest_rect = { 0.f, 0.f, (float)source_rect.width * SCALE, (float)source_rect.height * SCALE };
	DrawTexturePro(background, source_rect, dest_rect, Vector2{ 0, 0 }, 0.0f, WHITE);
}

void ConfigStatusMenu::DrawOptions()
{
	int num = 0;
	vector<string> vis_options = MakeVisibleOptions();
	for (auto& option : vis_options)
	{
		Vector2 pos = { vis_option_pos.x, vis_option_pos.y + ((float)num * vis_spacing) };
		string text = option;
		drawText(pos, text, font_size, option_color);
		num++;
	}
}
