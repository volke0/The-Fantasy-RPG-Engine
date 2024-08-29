#include "equip_select_menu.h"
#include "game.h"
#include "menu_cursor.h"
#include "unit.h"
#include "weapon.h"
#include "equip_confirm_status_menu.h"

void EquipSelectMenu::setup(Game* game, PlayerStatusMenu* player_status_menu, string slot, vector<string> equip_type, vector<string> equip_ranks)
{
	SubStatusMenu::setup(game, player_status_menu);
	this->equip_type = equip_type;
	this->equip_ranks = equip_ranks;
	this->slot = slot;
	vis_x = 13.0f * TILE;
	vis_y = 5.f * TILE + 0.f;
	MakeOptions();
	max_vis_index = 11;
	vis_spacing = 30.f;
	font_size = 20.f;
	cursor->vis_index = 0;
	cursor->index = 0;
	lastSceneNum = scene_manager->getLastScenePos() + 1;
	SetCursor(*cursor, options, vis_x, vis_y, vis_spacing, max_vis_index);
}


void EquipSelectMenu::update()
{
	SubStatusMenu::update();
	if (cursor->go_back)
	{
		scene_manager->pop(true);
		sound_player.playSound("click2");
		cursor->go_back = false;
	}
	if (cursor->return_input)
	{
		if (cursor->selected_option != "None")
		{
			sound_player.playSound("click");
			auto& user = party->party[unit_num];
			int item_index = stoi(cursor->selected_option);
			auto& item = party->item_inventory.inventory[item_index];

			shared_ptr<EquipConfirmStatusMenu> equip_confirm = make_shared<EquipConfirmStatusMenu>();
			equip_confirm->setup(game, player_status_menu, user.get(), slot, item, item_index);
			PushMenu(equip_confirm, true);
		}
		else
		{
			sound_player.playSound("cantclick");
		}
		cursor->return_input = false;
	}
}


void EquipSelectMenu::draw()
{
	scene_manager->getScene(lastSceneNum)->draw();
	DrawBox();
	DrawOptions();
	if (!pushedScene)
	{
		cursor->draw();
	}
}

void EquipSelectMenu::DrawBox()
{
	auto& background = game->GFX.UI["equipment_list"];
	Rectangle source_rect = { 0, 0, (float)(background.width), (float)(background.height) };
	Rectangle dest_rect = { 12.f * TILE - 2.f, 5.f * TILE - 23.f, (float)source_rect.width * SCALE, (float)source_rect.height * SCALE };
	DrawTexturePro(background, source_rect, dest_rect, Vector2{ 0, 0 }, 0.0f, WHITE);
	DrawDescriptionBox(dest_rect);
}

void EquipSelectMenu::DrawDescriptionBox(Rectangle og_dest)
{
	if (find(options.begin(), options.end(), "None") == options.end())
	{
		auto& background = game->GFX.UI["equip_descrip"];
		Rectangle source_rect = { 0, 0, (float)(background.width), (float)(background.height) };
		Rectangle dest_rect = { og_dest.x - (5.f * TILE + 39.f), og_dest.y, (float)source_rect.width * SCALE, (float)source_rect.height * SCALE };
		DrawTexturePro(background, source_rect, dest_rect, Vector2{ 0, 0 }, 0.0f, WHITE);
	}
}

void EquipSelectMenu::DrawOptions()
{
	if (find(options.begin(), options.end(), "None") == options.end())
	{
		if (party->item_inventory.inventory.size() > 0)
		{
			auto inventory_display = MakeVisibleItemOptions();
			int index = 0;
			vector<string> option_types = equip_type;
			for (auto& item_num : inventory_display)
			{
				auto& item = party->item_inventory.inventory[item_num];
				float fontSize = 20.0f;
				Vector2 pos = { vis_option_pos.x , vis_option_pos.y + (index) * (vis_spacing) };
				Color color = WHITE;

				drawItem(item, item_num, option_types, pos, fontSize, color);
				if (item["item"]["stackable"].asBool() == true)
				{
					Vector2 amt_pos = { pos.x + (float)MeasureText(item["name"].asString().c_str(), (int)fontSize) + 5.0f, pos.y };
					string amt_text = "x" + item["amount"].asString();
					drawText(amt_pos, amt_text, fontSize, color);
				}
				index++;
			}
			if (memory_index < (int)options.size())
			{
				int item_index = stoi(options[memory_index]);
				auto& memory_item = party->item_inventory.inventory[item_index];
				drawEquippedStats(memory_item, item_index);
				drawItemDescription(memory_item);
			}
		}
	}
	else
	{
		drawText(vis_option_pos, options[0], 20.0f, GRAY);
	}
}

void EquipSelectMenu::drawItem(Json::Value& item, int item_num, vector<string>& option_types, Vector2& pos, float fontSize, Color& color)
{
	if (find(option_types.begin(), option_types.end(), item["item"]["sub_type"]) != option_types.end())
	{
		drawEquippedText(item, item_num, option_types, pos, fontSize, color);
		drawText(pos, item["name"].asString(), 20.0f, color);
	}
}

void EquipSelectMenu::drawEquippedText(Json::Value& item, int item_num, vector<string>& option_types, Vector2& pos, float fontSize, Color& color)
{
	if (item.isMember("equipped"))
	{
		if (item["equipped"].asBool())
		{
			Vector2 equip_pos = { pos.x + (float)MeasureText(item["name"].asString().c_str(), (int)fontSize) + 3.0f, pos.y + (fontSize - 18.f) };
			string equip_text = "E.";
			Unit* equipped_unit = party->item_inventory.GetEquippedUnit(item_num);
			if (equipped_unit == party->party[unit_num].get())
			{
				color = YELLOW;
			}
			else
			{
				color = GRAY;
			}
			drawText(equip_pos, equip_text, 18.f, color);
		}
	}
}

void EquipSelectMenu::drawEquippedStats(Json::Value& item, int item_num)
{
	auto& unit = party->party[unit_num];
	auto& equip_sys = unit->equip_system;
	
	vector<int> basic_unit_stats = { unit->str, unit->mag, unit->skl, unit->spd, unit->lck, unit->def, unit->res, unit->con };
	vector<int> unit_combat_stats = { unit->atk, unit->mag_atk, unit->atk_spd, unit->hit, unit->avoid, unit->block, unit->mag_defense, unit->crit, (int)unit->crit_dmg };

	unordered_map<string, int> equipment_stats = equip_sys.GetEquipmentStats(item["item"]);
	unordered_map<int, string> equipment_basic_stat_map = { {0, "str"}, {1, "mag"}, {2, "skl"}, {3, "spd"}, {4, "lck"}, {5, "def"}, {6, "res"}, {7, "con"} };
	unordered_map<int, string> equipment_combat_stat_map = { {0, "attack"}, {1, "mag_attack"}, {2, "atk_speed"}, {3, "hit"}, {4, "avoid"}, {5, "block"}, {6, "mag_def"}, {7, "crit"}, {8, "crit_dmg"}, {9, "weight"}};
													  
	float x = 101.25f;
	float x2 = 179.375f;
	float y = 6.f * TILE - 39.f;
	float x_spacing1 = 75.f;
	float x_spacing2 = 101.f;
	float spacing = 20.f;
	float font_size = 18.f;
	float scale = 2.f;
	int map_index = 0;
	Color base_stat_color = WHITE;
	auto& arrow = game->GFX.UI["text_arrow"];

	for (auto stat_num : basic_unit_stats)
	{
		string stat_key = equipment_basic_stat_map[map_index];
		int num = equip_sys.CalculatePotentialNewStat(equipment_stats, stat_key, slot);
		string num_text = to_string(num);
		Vector2 arrow_pos = { x + 37.5f, y + 2.f };
		Vector2 num_pos = { x + x_spacing1,  y };

		if (num > stat_num)
		{
			base_stat_color = YELLOW;
		}
		else if (num == stat_num)
		{
			base_stat_color = GRAY;
		}
		else
		{
			base_stat_color = RED;
		}
		DrawTextureEx(arrow, arrow_pos, 0.0f, scale, WHITE);
		drawText(num_pos, num_text, font_size, base_stat_color);
		y += spacing;
		map_index++;
	}

	y += 17.f;
	map_index = 0;
	for (auto stat_num : unit_combat_stats)
	{
		string stat_key = equipment_combat_stat_map[map_index];
		int num = equip_sys.CalculatePotentialNewStat(equipment_stats, stat_key, slot);
		Vector2 arrow_pos = { x2 + 65.f, y + 2.f };
		Vector2 num_pos = { x2 + x_spacing2,  y };
		string num_text = "";
		if (map_index == 7 || map_index == 8)
		{
			num_text = to_string(num) + "%";
		}
		else
		{
			num_text = to_string(num);
		}

		if (num > stat_num)
		{
			base_stat_color = YELLOW;
		}
		else if (num == stat_num)
		{
			base_stat_color = GRAY;
		}
		else
		{
			base_stat_color = RED;
		}
		DrawTextureEx(arrow, arrow_pos, 0.0f, scale, WHITE);
		drawText(num_pos, num_text, font_size, base_stat_color);
		y += spacing;
		map_index++;
	}
}

void EquipSelectMenu::drawItemDescription(Json::Value item)
{
	Vector2 base_descr_pos = { 7.f * TILE - 20.f, 5.f * TILE - 34.f };
	string base_descr = item["item"]["description"].asString();
	vector<string> description;
	boost::split(description, base_descr, boost::is_any_of("~"));
	int index = 0;

	for (string text : description)
	{
		Vector2 descr_pos = { base_descr_pos.x, base_descr_pos.y + 35.0f + (float)(24.0f * index) };
		drawText(descr_pos, text, 20.0f);
		index++;
	}
}



void EquipSelectMenu::MakeOptions()
{
	options.clear();
	options.shrink_to_fit();
	auto& user = party->party[unit_num];
	vector<string> option_types = equip_type;
	vector<string> rank_types = equip_ranks;
	int count = 0;
	for (int i = 0; i < (int)party->item_inventory.inventory.size(); i++)
	{
		auto& item = party->item_inventory.inventory[i]["item"];
		string item_sub_type = item["sub_type"].asString();
		string item_rank_type = GetEquipRank(item, item_sub_type);
		vector<string> special_locks = GetSpecialLocks(item);

		if (find(option_types.begin(), option_types.end(), item_sub_type) != option_types.end())
		{
			if (find(rank_types.begin(), rank_types.end(), item_rank_type) != rank_types.end())
			{
				if ((int)special_locks.size() > 0)
				{
					if (find(special_locks.begin(), special_locks.end(), user->name) != special_locks.end())
					{
						options.push_back(to_string(i));
					}
				}
				else
				{
					options.push_back(to_string(i));
					count++;
				}
			}
		}
	}
	if (count == 0)
	{
		options.push_back("None");
	}
}

string EquipSelectMenu::GetEquipRank(Json::Value item, string sub_type)
{
	string rank = "";
	if (sub_type == "weapon")
	{
		rank = item["weapon_type"].asString();
	}
	if (sub_type == "shield")
	{
		rank = item["shield_type"].asString();
	}
	if (sub_type == "head_gear")
	{
		rank = item["head_type"].asString();
	}
	if (sub_type == "armor")
	{
		rank = item["armor_type"].asString();
	}
	if (sub_type == "foot_gear")
	{
		rank = item["foot_type"].asString();
	}
	return rank;
}

vector<string> EquipSelectMenu::GetSpecialLocks(Json::Value item)
{
	vector<string> special_locks = {};
	if (item.isMember("special_lock"))
	{
		for (int i = 0; i < (int)item["special_lock"].size(); i++)
		{
			special_locks.push_back(item["special_lock"][i].asString());
		}
	}
	return special_locks;
}

vector<int> EquipSelectMenu::MakeVisibleItemOptions()
{
	vector<int> temp_options;
	int max_index = vis_index + max_vis_index + 1;

	if (max_index > (int)options.size())
	{
		max_index = (int)options.size();
	}
	for (int i = vis_index; i < max_index; i++)
	{
		int actual_index = stoi(options[i]);
		temp_options.push_back(actual_index);
	}
	return temp_options;
}
