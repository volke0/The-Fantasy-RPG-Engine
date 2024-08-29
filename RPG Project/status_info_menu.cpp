#include "status_info_menu.h"
#include "menu_cursor.h"
#include "game.h"
#include "unit.h"
#include "party.h"
#include "player_status_menu.h"

void StatusInfoMenu::setup(Game* game, PlayerStatusMenu* player_status_menu, int unit_num)
{
	SubStatusMenu::setup(game, player_status_menu);
	this->unit_num = unit_num;
}

void StatusInfoMenu::update()
{
	SubStatusMenu::update();
	if (cursor->go_back)
	{
		scene_manager->pop();
		sound_player.playSound("click2");
		cursor->go_back = false;
	}
	if (cursor->return_input)
	{
		cursor->return_input = false;
		sound_player.playSound("click");
	}
	if (cursor->findKey("w"))
	{
		SetUnitNum(true);
		sound_player.playSound("click");
	}
	else if (cursor->findKey("q"))
	{
		SetUnitNum(false);
		sound_player.playSound("click");
	}
}

void StatusInfoMenu::draw()
{
	DrawBackground();
	//DrawInfo();
	DrawCharacter();
	DrawStatus();
}

void StatusInfoMenu::DrawBackground()
{
	auto& background = game->GFX.UI["equipment_status_menu"];
	Rectangle source_rect = { 0, 0, (float)(background.width), (float)(background.height) };
	Rectangle dest_rect = { 0.f, 0.f, (float)source_rect.width * SCALE, (float)source_rect.height * SCALE };
	DrawTexturePro(background, source_rect, dest_rect, Vector2{ 0, 0 }, 0.0f, WHITE);
}

void StatusInfoMenu::DrawInfo()
{
	Vector2 pos = { 14.f * TILE, 1.f * TILE - 24.f };
	string info = "Status";
	drawText(pos, info, 26.f, LightBlue);
}

void StatusInfoMenu::DrawCharacter()
{
	float width = 96.f;
	float height = 75.f;
	float x = 1.f * TILE;
	float y = TILE - 7.f;
	Rectangle source_rect = { 14, 0, width, height };
	Rectangle dest_rect = { x, y, width * 2, height * 2 };
	DrawTexturePro(player_status_menu->GetFace(unit_num), source_rect, dest_rect, Vector2{ 0, 0 }, 0.0f, WHITE);
	DrawCharacterName(dest_rect);
}

void StatusInfoMenu::DrawCharacterName(Rectangle dest_rect)
{
	auto& unit = party->party[unit_num];
	//Name text
	string name_text = unit->ingame_name;
	string class_name = "";
	if (unit->class_initialized)
	{
		class_name = unit->unit_class.ingame_name;
	}
	string lvl_text = "Lvl: " + to_string(unit->lvl);
	string hp_ratio = "HP:  " + to_string(unit->hp) + "/" + to_string(unit->max_hp);
	string mp_ratio = "MP:  " + to_string(unit->mp) + "/" + to_string(unit->max_mp);
	string exp_text = "EXP: ";
	float base_exp = (float)unit->exp - (float)unit->base_exp;
	float required_exp = (float)unit->until_next_level - (float)unit->base_exp;
	string exp_ratio = to_string((int)base_exp) + "/" + to_string((int)required_exp);
	string total_exp = "Total EXP:  " + to_string(unit->exp);

	//Name positions
	Vector2 name_pos = { dest_rect.x + (float)4 * TILE + 12.f, dest_rect.y + 10.f };
	Vector2 class_pos = { name_pos.x + 3.f * TILE + 24.f, name_pos.y };
	Vector2 lvl_pos = { name_pos.x, name_pos.y + 42.f };
	Vector2 hp_pos = { lvl_pos.x, lvl_pos.y + 25.f };
	Vector2 mp_pos = { hp_pos.x + 3.f * TILE + 24.f, hp_pos.y };
	Vector2 exp_text_pos = { hp_pos.x,  hp_pos.y + (1.f * TILE - 3.f) };
	Vector2 exp_ratio_pos = { lvl_pos.x + (2.f * TILE + 43.f), hp_pos.y + (1.f * TILE + 10.f) };
	Vector2 total_exp_pos = { exp_text_pos.x, exp_ratio_pos.y + 25.f };

	//Max bar size
	Rectangle max_rect = { exp_text_pos.x + 40.f, exp_ratio_pos.y - 7.f, 250, 5 };
	float ratio = base_exp / required_exp;
	Rectangle exp_bar = { max_rect.x, max_rect.y, 250 * (ratio), 5 };
	DrawRectanglePro(max_rect, Vector2{ 0, 0 }, 0.0f, GRAY);
	DrawRectanglePro(exp_bar, Vector2{ 0, 0 }, 0.0f, BLUE);

	drawText(name_pos, name_text, 22.f, WHITE);
	drawText(class_pos, class_name, 22.f, WHITE);
	drawText(lvl_pos, lvl_text, 20.f, WHITE);
	drawText(hp_pos, hp_ratio, 20.f, WHITE);
	drawText(mp_pos, mp_ratio, 20.f, WHITE);
	drawText(exp_text_pos, exp_text, 16.f, WHITE);
	drawText(exp_ratio_pos, exp_ratio, 16.f, WHITE);
	drawText(total_exp_pos, total_exp, 18.f, WHITE);
}

void StatusInfoMenu::DrawStatus()
{
	auto& unit = party->party[unit_num];
	float spacing = 20.f;
	float font_size = 18.f;

	//Stat text
	vector<string> basic_stat_text = { "Str: ", "Mag: ", "Skl: ", "Spd: ", "Lck: ", "Def: ", "Res: ", "Con: " };
	vector<string> combat_stat_text = { "Attack: ", "Mag. Attack: ", "Atk. Speed: ", "Hit: ", "Avoid: ", "Block: ", "Mag. Defense: ", "Crit: ", "Crit Dmg: " };

	vector<int> basic_stats = { unit->str, unit->mag, unit->skl, unit->spd, unit->lck, unit->def, unit->res, unit->con };
	vector<int> combat_stats = { unit->atk, unit->mag_atk, unit->atk_spd, unit->hit, unit->avoid, unit->block, unit->mag_defense, unit->crit, unit->crit_dmg };

	float x = 1.0f * TILE;
	float y = 6.f * TILE - 39.f;
	for (int i = 0; i < (int)basic_stat_text.size(); i++)
	{
		string first_string = basic_stat_text[0];
		string text = basic_stat_text[i];
		string num = to_string(basic_stats[i]);

		Vector2 text_pos = { x, y };
		Vector2 num_pos = { x + MeasureTextEx(game->GFX.font, first_string.c_str(), 20.f, 0.f).x + 12.f, y };
		drawText(text_pos, text, font_size, LightBlue);
		drawText(num_pos, num, font_size, WHITE);
		y += spacing;
	}

	y += 17.f;
	for (int i = 0; i < (int)combat_stat_text.size(); i++)
	{
		string mag_atk_string = combat_stat_text[1];
		string text = combat_stat_text[i];
		string num = to_string(combat_stats[i]);
		if (combat_stat_text[i] == "Crit: " || combat_stat_text[i] == "Crit Dmg: ")
		{
			num = to_string(combat_stats[i]) + "%";
		}

		Vector2 text_pos = { x, y };
		Vector2 num_pos = { x + MeasureTextEx(game->GFX.font, mag_atk_string.c_str(), 20.f, 0.f).x + 12.f, y };
		drawText(text_pos, text, font_size, LightBlue);
		drawText(num_pos, num, font_size, WHITE);
		y += spacing;
	}
}
