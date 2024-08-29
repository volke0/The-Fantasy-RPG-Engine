#include "equip_status_menu.h"
#include "menu_cursor.h"
#include "game.h"
#include "unit.h"
#include "player_status_menu.h"
#include "equip_select_menu.h"
#include "equipment_base.h"

void EquipStatusMenu::setup(Game* game, PlayerStatusMenu* player_status_menu)
{
	SubStatusMenu::setup(game, player_status_menu);
	options = { "R. Hand", "L. Hand", "Head", "Body", "Feet"};
	vis_x = 1.0f * TILE;
	vis_y = 52.f;
	max_vis_index = 15;
	vis_spacing = 30.f;
	option_color = LightBlue;
	font_size = 20.f;
}

void EquipStatusMenu::update()
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
		cursor->return_input = false;
		auto& unit = party->party[unit_num];
		vector<string> equip_type = unit->equip_system.GetEquipType(equipment_values[cursor->index]);
		vector<string> ranks = unit->equip_system.GetEquipRanks(equip_type);

		shared_ptr<EquipSelectMenu> equip_list = make_shared<EquipSelectMenu>();
		equip_list->unit_num = unit_num;
		equip_list->setup(game, player_status_menu, equipment_values[cursor->index], equip_type, ranks);
		PushMenu(equip_list, true);
	}
}

void EquipStatusMenu::draw()
{
	DrawBackground();
	DrawCharacter();
	DrawEquipment();
	DrawStatus();
	SubStatusMenu::draw();
	if (!pushedScene)
	{
		cursor->draw();
	}
}

void EquipStatusMenu::DrawBackground()
{
	ClearBackground(BLACK);
	auto& background = game->GFX.UI["equipment_status_menu"];
	Rectangle source_rect = { 0, 0, (float)(background.width), (float)(background.height) };
	Rectangle dest_rect = { 0.f, 0.f, (float)source_rect.width * SCALE, (float)source_rect.height * SCALE };
	DrawTexturePro(background, source_rect, dest_rect, Vector2{ 0, 0 }, 0.0f, WHITE);
}

void EquipStatusMenu::DrawCharacter()
{
	float width = 96.f;
	float height = 75.f;
	float x = 7.f * TILE;
	float y = TILE - 7.f;
	Rectangle source_rect = { 14, 0, width, height };
	Rectangle dest_rect = { x, y, width * 2, height * 2 };
	DrawTexturePro(player_status_menu->GetFace(unit_num), source_rect, dest_rect, Vector2{ 0, 0 }, 0.0f, WHITE);
	DrawCharacterName(dest_rect);
}

void EquipStatusMenu::DrawCharacterName(Rectangle dest_rect)
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

	//Name positions
	Vector2 name_pos = { dest_rect.x + (float)4 * TILE + 12.f, dest_rect.y + 10.f };
	Vector2 class_pos = { name_pos.x + 2.f * TILE + 24.f, name_pos.y };
	Vector2 lvl_pos = { name_pos.x, name_pos.y + 42.f };
	Vector2 hp_pos = { lvl_pos.x, lvl_pos.y + 25.f };
	Vector2 mp_pos = { hp_pos.x + (float)3 * TILE + 24.f, hp_pos.y };

	drawText(name_pos, name_text, 22.f, WHITE);
	drawText(class_pos, class_name, 22.f, WHITE);
	drawText(lvl_pos, lvl_text, 20.f, WHITE);
	drawText(hp_pos, hp_ratio, 20.f, WHITE);
	drawText(mp_pos, mp_ratio, 20.f, WHITE);
}

void EquipStatusMenu::DrawEquipment()
{
	auto& unit = party->party[unit_num];
	int index = 0;
	for (auto& value : equipment_values)
	{
		auto& equipment = unit->equip_system.GetEquipment(value);

		Vector2 pos = { vis_option_pos.x + MeasureTextEx(game->GFX.font, options[0].c_str(), font_size, 0.f).x + 12.f, vis_option_pos.y + (index * vis_spacing) };
		string equip_name = "None";
		Color color = GRAY;
		if (equipment != nullptr)
		{
			equip_name = equipment->ingame_name;
			color = WHITE;
		}
		//cout << value << ": " << equip_name << endl;
		drawText(pos, equip_name, font_size, color);

		index++;
	}
}

void EquipStatusMenu::DrawStatus()
{
	auto& unit = party->party[unit_num];
	float spacing = 20.f;
	float font_size = 18.f;

	//Stat text
	vector<string> basic_stat_text = {"Str: ", "Mag: ", "Skl: ", "Spd: ", "Lck: ", "Def: ", "Res: ", "Con: "};
	vector<string> combat_stat_text = {"Attack: ", "Mag. Attack: ", "Atk. Speed: ", "Hit: ", "Avoid: ", "Block: ", "Mag. Defense: ", "Crit: ", "Crit Dmg: "};

	vector<int> basic_stats = { unit->str, unit->mag, unit->skl, unit->spd, unit->lck, unit->def, unit->res, unit->con };
	vector<int> combat_stats = { unit->atk, unit->mag_atk, unit->atk_spd, unit->hit, unit->avoid, unit->block, unit->mag_defense, unit->crit, unit->crit_dmg};

	float x = vis_option_pos.x;
	float y = 6.f * TILE - 39.f;
	for (int i = 0; i < (int)basic_stat_text.size(); i++)
	{
		string first_string = basic_stat_text[0];
		string text = basic_stat_text[i];
		string num = to_string(basic_stats[i]);

		Vector2 text_pos = {x, y};
		Vector2 num_pos = { x + MeasureTextEx(game->GFX.font, first_string.c_str(), 20.f, 0.f).x + 12.f, y};
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
