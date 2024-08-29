#include "ability_status_menu.h"
#include "menu_cursor.h"
#include "game.h"
#include "party.h"
#include "player_status_menu.h"
#include "magic_status_menu.h"
#include "unit.h"

void AbilityStatusMenu::setup(Game* game, PlayerStatusMenu* player_status_menu)
{
	SubStatusMenu::setup(game, player_status_menu);
	options = { "Magic", "Specials" };
	vis_x = 1.0f * TILE;
	vis_y = 52.f;
	max_vis_index = 15;
	vis_spacing = 30.f;
	option_color = LightBlue;
}

void AbilityStatusMenu::update()
{
	SubStatusMenu::update();
	if (cursor->go_back)
	{
		scene_manager->pop();
		cursor->go_back = false;
	}
	if (cursor->return_input)
	{
		sound_player.playSound("click");
		if (cursor->selected_option == "Magic")
		{
			shared_ptr<MagicStatusMenu> magic_menu = make_shared<MagicStatusMenu>();
			magic_menu->setup(game, player_status_menu, party->party[unit_num].get());
			PushMenu(magic_menu, true);
		}
		cursor->return_input = false;
	}
}

void AbilityStatusMenu::draw()
{
	DrawBackground();
	DrawCharacter();
	SubStatusMenu::draw();
	if (!pushedScene)
	{
		cursor->draw();
	}
}

void AbilityStatusMenu::DrawBackground()
{
	ClearBackground(BLACK);
	auto& background = game->GFX.UI["ability_status_menu"];
	Rectangle source_rect = { 0, 0, (float)(background.width), (float)(background.height) };
	Rectangle dest_rect = { 0.f, 0.f, (float)source_rect.width * SCALE, (float)source_rect.height * SCALE };
	DrawTexturePro(background, source_rect, dest_rect, Vector2{ 0, 0 }, 0.0f, WHITE);
}

void AbilityStatusMenu::DrawCharacter()
{
	float width = 96.f;
	float height = 75.f;
	float x = 5.f * TILE;
	float y = TILE / 4.f -  6.f;
	Rectangle source_rect = { 14, 0, width, height };
	Rectangle dest_rect = { x, y, width * 2, height * 2 };
	DrawTexturePro(player_status_menu->GetFace(unit_num), source_rect, dest_rect, Vector2{ 0, 0 }, 0.0f, WHITE);

	DrawStatus(dest_rect);
}

void AbilityStatusMenu::DrawStatus(Rectangle dest_rect)
{
	auto& unit = party->party[unit_num];
	//Stat text
	string name_text = unit->ingame_name;
	string class_name = "";
	if (unit->class_initialized)
	{
		class_name = unit->unit_class.ingame_name;
	}
	string lvl_text = "Lvl: " + to_string(unit->lvl);
	string hp_ratio = "HP:  " + to_string(unit->hp) + "/" + to_string(unit->max_hp);
	string mp_ratio = "MP:  " + to_string(unit->mp) + "/" + to_string(unit->max_mp);
	string atk_text = "Str: " + to_string(unit->str);
	string mag_text = "Mag: " + to_string(unit->mag);
	string def_text = "Def: " + to_string(unit->def);
	string res_text = "Res: " + to_string(unit->res);

	//Stat positions
	Vector2 name_pos = { dest_rect.x + (float)4 * TILE + 20.f, dest_rect.y + 10.f };
	Vector2 class_pos = { name_pos.x + 3.f * TILE + 24.f, name_pos.y };
	Vector2 lvl_pos = { name_pos.x, name_pos.y + 31.f };
	Vector2 hp_pos = { lvl_pos.x, lvl_pos.y + 25.f };
	Vector2 mp_pos = { hp_pos.x + (float)3 * TILE + 24.f, hp_pos.y };
	Vector2 atk_pos = { hp_pos.x, hp_pos.y + 25.f };
	Vector2 def_pos = { mp_pos.x, atk_pos.y};
	Vector2 mag_pos = { atk_pos.x, atk_pos.y + 25.f};
	Vector2 res_pos = { mp_pos.x, mag_pos.y};

	drawText(name_pos, name_text, 22.f, WHITE);
	drawText(class_pos, class_name, 22.f, WHITE);
	drawText(lvl_pos, lvl_text, 20.f, WHITE);
	drawText(hp_pos, hp_ratio, 20.f, WHITE);
	drawText(mp_pos, mp_ratio, 20.f, WHITE);
	drawText(atk_pos, atk_text, 20.f, WHITE);
	drawText(def_pos, def_text, 20.f, WHITE);
	drawText(mag_pos, mag_text, 20.f, WHITE);
	drawText(res_pos, res_text, 20.f, WHITE);
}
