#include "magic_status_menu.h"
#include "unit.h"
#include "game.h"
#include "party.h"
#include "menu_cursor.h"
#include "apply_magic_menu.h"

void MagicStatusMenu::setup(Game* game, PlayerStatusMenu* player_status_menu, Unit* unit)
{
	SubStatusMenu::setup(game, player_status_menu);
	this->unit = unit;
	getMagicOptions();
	vis_x = 1.0f * TILE;
	vis_y = 4.f * TILE;
	max_vis_index = 15;
	vis_spacing = 30.f;
	horizontal = true;
}

void MagicStatusMenu::update()
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
		cursor->return_input = false;
		string type = game->magic_database.magic_scripts[options[cursor->index]]["type"].asString();
		if (type == "healing")
		{
			sound_player.playSound("click");
			shared_ptr<ApplyMagicMenu> apply_magic = make_shared<ApplyMagicMenu>();
			apply_magic->setup(game, player_status_menu, options[cursor->index], unit);
			PushMenu(apply_magic);
		}
		else
		{
			sound_player.playSound("cantclick");
		}
	}
}

void MagicStatusMenu::draw()
{
	scene_manager->getLastScene()->draw();
	drawMagicInfoBox();
	drawMagicOptions();
	if (!scene_manager->busy)
	{
		cursor->draw();
	}
}

void MagicStatusMenu::drawMagicInfoBox()
{
	auto& background = game->GFX.UI["magic_info"];
	Rectangle source_rect = { 0, 0, (float)(background.width), (float)(background.height) };
	Rectangle dest_rect = { 9.f * TILE + 4.f, 3.f * TILE + 12.f, (float)source_rect.width * SCALE, (float)source_rect.height * SCALE };
	DrawTexturePro(background, source_rect, dest_rect, Vector2{ 0, 0 }, 0.0f, WHITE);
}

void MagicStatusMenu::drawMagicOptions()
{
	float prev_y = 0;
	vector<string> visual_options = getVisibleOptions();
	for (int i = 0; i < visual_options.size(); i++)
	{
		Color text_color = WHITE;
		float x = 0;
		float y = 0;
		if (i % 2 == 0)
		{
			x = vis_x;
			y = vis_y + (float)i * vis_spacing;
			prev_y = y;
		}
		else
		{
			x = vis_x + (float)TILE * 5;
			y = prev_y;
		}
		string name = game->magic_database.magic_scripts[visual_options[i]]["name"].asString();
		string type = game->magic_database.magic_scripts[visual_options[i]]["type"].asString();
		if (name == "")
		{
			name = "????";
			text_color = GRAY;
		}
		if (type != "healing")
		{
			text_color = GRAY;
		}
		Vector2 pos = { x, y };
		drawText(pos, name, 25.0f, text_color);
	}
	drawMagicDetails();
}

void MagicStatusMenu::drawMagicDetails()
{
	string name = options[memory_index];
	unordered_map<string, string> type_map = { {"dmg", "Dmg"}, {"healing", "Heal"}, {"cleanse", "Cls"}, {"buff", "Buff"}, {"debuff", "Debuff"} };
	unordered_map<string, string> target_map = { {"single", "Single"}, {"multi", "Multi"} };
	unordered_map<string, string> elem_type_map = {
													{"fire", "Fire"},
													{"water", "Water"},
													{"wind", "Wind"},
													{"thunder", "Thunder"},
													{"ice", "Ice"},
													{"earth", "Earth"},
													{"light", "Light"},
													{"dark", "Dark"},
													{"none", "None"},
	};

	if (find(unit->magic_options.begin(), unit->magic_options.end(), name) != unit->magic_options.end())
	{
		string power = "Power:  " + game->magic_database.magic_scripts[name]["power"].asString();
		string mp_cost = "Cost:  " + game->magic_database.magic_scripts[name]["cost"].asString() + " MP";
		string type = "Type: " + type_map[game->magic_database.magic_scripts[name]["type"].asString()];
		string elem_type = "Elem. Type: ";
		string elem_val = elem_type_map[game->magic_database.magic_scripts[name]["elem_type"].asString()];
		string hit_val = "Hit:  " + game->magic_database.magic_scripts[name]["hit"].asString();

		Vector2 power_pos = { 10.f * TILE - 10.0f, 4.f * TILE + 1.0f };
		Vector2 hit_pos = { power_pos.x + 3.f * TILE - (12.0f), power_pos.y };
		Vector2 type_pos = { power_pos.x, power_pos.y + 27.0f };
		Vector2 elem_type_pos = { hit_pos.x - (0.0f), type_pos.y + 3.0f };
		Vector2 elem_value_pos = { elem_type_pos.x + (float)TILE + 44.0f, type_pos.y + 2.0f };
		Vector2 mp_cost_pos = { type_pos.x, type_pos.y + 27.0f };

		drawText(power_pos, power, 20.0f);
		drawText(hit_pos, hit_val, 20.0f);
		drawText(type_pos, type, 20.0f);
		drawText(elem_type_pos, elem_type, 16.0f);
		drawText(elem_value_pos, elem_val, 18.0f);
		drawText(mp_cost_pos, mp_cost, 20.0f);

		drawMagicDescription(name, mp_cost_pos);
	}
}

void MagicStatusMenu::drawMagicDescription(string name, Vector2 last_pos)
{
	string base_descr = game->magic_database.magic_scripts[name]["descr"].asString();
	vector<string> description;
	boost::split(description, base_descr, boost::is_any_of("~"));
	int index = 0;

	for (string text : description)
	{
		Vector2 descr_pos = { last_pos.x, last_pos.y + 35.0f + (float)(20.0f * index) };
		drawText(descr_pos, text, 21.0f);
		index++;
	}
}


void MagicStatusMenu::getMagicOptions()
{
	unit->magic_list_updater.updatePlayerMagicList();
	options = unit->magic_list;
}

vector<string> MagicStatusMenu::getVisibleOptions()
{
	int max_option_index = (int)options.size() - 1;
	int vis_index_factor = max_option_index;
	vector<string> temp_options = {};
	if (max_option_index > max_vis_index)
	{
		vis_index_factor = max_vis_index;
	}
	int max_vis_index = vis_index + vis_index_factor;
	if (max_vis_index > max_option_index)
	{
		max_vis_index = max_option_index;
	}

	for (int i = vis_index; i <= max_vis_index; i++)
	{
		temp_options.push_back(options[i]);
	}
	return temp_options;
}
