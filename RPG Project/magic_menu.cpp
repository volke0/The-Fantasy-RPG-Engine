#include "magic_menu.h"
#include <algorithm>
#include "game.h"
#include "battle_controller.h"
#include "battle_menu_controller.h"
#include "battle_cursor.h"
#include "magic.h"
#include "unit.h"

void MagicMenu::setup(Game* game, Party* party, BattleController* battle_controller, BattleMenuController* menu_controller)
{
	basicSetup(game, party, battle_controller, menu_controller);
	b_cursor = &battle_controller->cursor;
}


void MagicMenu::update()
{
	if (menu_controller->checkFocus("magic_menu"))
	{
		menu_index = b_cursor->index;
		current_unit->magic_index = menu_index;
		current_unit->magic_vis_index = menu_vis_index;
		current_unit->magic_cur_vis_index = b_cursor->getMagicVisualIndex();

		if (b_cursor->return_input == true)
		{
			selectMagic();
			b_cursor->return_input = false;
		}
		if (b_cursor->go_back == true)
		{
			b_cursor->go_back = false;
			b_cursor->index = 2;
			b_cursor->vis_index = 2;
			menu_controller->eraseState("magic_menu");
			menu_controller->input_history.clear();
			menu_controller->playSound("click2");
		}
		updateVisibleOptions();
	}
}

void MagicMenu::selectMagic()
{
	if (options[b_cursor->index] != "")
	{
		string name = options[b_cursor->index];
		auto temp_magic = game->magic_database.magic_scripts[name];
		string cost_type = temp_magic["cost_type"].asString();
		int cost = temp_magic["cost"].asInt();

		if (cost_type == "mp")
		{
			if (current_unit->mp >= cost)
			{
				menu_controller->input_history["magic"] = options[b_cursor->index];
				//cout << "MAGIC: " << menu_controller->input_history["magic"] << endl;
				moveToUnitSelect(temp_magic);
				menu_controller->playSound("click");
			}
			else
			{
				menu_controller->playSound("cantclick");
			}
		}
	}
	else
	{
		menu_controller->playSound("cantclick");
	}
}

void MagicMenu::moveToUnitSelect(Json::Value temp_magic)
{
	string magic_type = temp_magic["type"].asString();
	menu_controller->input_history["magic_type"] = magic_type;
	if (magic_type == "dmg")
	{
		b_cursor->switch_to_enemies = true;
		b_cursor->switch_to_allies = false;
		if (current_unit->type == "playable")
		{
			current_unit->anim_controller.isAnimation("magic_attack")->setVFX("magic_use");
		}
	}
	if (magic_type == "healing")
	{
		b_cursor->switch_to_enemies = false;
		b_cursor->switch_to_allies = true;
		if (current_unit->type == "playable")
		{
			current_unit->anim_controller.isAnimation("magic_attack")->setVFX("heal_use");
		}
	}
	b_cursor->unit_mode = true;
	menu_controller->turnUnitSelectOn();
}

void MagicMenu::updateVisibleOptions()
{
	if (b_cursor->move_up == true)
	{
		moveMenuUp();
	}
	else if (b_cursor->move_down == true)
	{
		moveMenuDown();
	}
	if (menu_vis_index < 0)
	{
		menu_vis_index = 0;
	}
	/*cout << "Cursor Index: " << b_cursor->index << endl;
	cout << "Cursor Visual Index: " << b_cursor->getMagicVisualIndex() << endl;
	cout << "Menu Index: " << menu_vis_index << endl;*/
}

void MagicMenu::moveMenuUp()
{
	if (b_cursor->index >= (int)b_cursor->options.size() - 2)
	{
		menu_vis_index = b_cursor->index - 4;

		if (menu_vis_index % 2 != 0)
		{
			menu_vis_index -= 1;
		}
	}
	else
	{
		if (b_cursor->index < b_cursor->options.size() - 6)
		{
			menu_vis_index -= 2;
		}
	}
}

void MagicMenu::moveMenuDown()
{
	if (b_cursor->index <= 1)
	{
		menu_vis_index = 0;
	}
	else if (b_cursor->index > 5)
	{
		if (b_cursor->getMagicVisualIndex() >= 4 && menu_vis_index != b_cursor->index - 4)
		{
			if (menu_vis_index != (int)options.size() - 6)
			{
				menu_vis_index += 2;
			}
		}
	}
}



void MagicMenu::draw()
{
	drawMagicOptionBox();
	drawMagicOptions();
}


void MagicMenu::setCurrentPlayer()
{
	int party_pos = menu_controller->player_index;
	current_unit = party->party[party_pos].get();
}

vector<string> MagicMenu::getMagicOptionList()
{
	current_unit->magic_list_updater.updatePlayerMagicList();
	return current_unit->magic_list;
}



void MagicMenu::turnOn()
{
	setCurrentPlayer();
	options = getMagicOptionList();
	og_pos = { (float)1 * TILE + 12, (float)9 * TILE + 12};
	b_cursor->setCursor(og_pos, options, true);
	menu_vis_index = current_unit->magic_vis_index;
	b_cursor->memory_index = current_unit->magic_index;
	b_cursor->setMagicVisualIndex(current_unit->magic_cur_vis_index);
	b_cursor->turnMagicCursorOn();
	visual_options = makeVisbleOptions();  //create intial visible options
}


//Draw
void MagicMenu::drawMagicOptionBox()
{
	auto& battle_box = game->GFX.UI["magic_box"];
	Rectangle source_rect = { 0, 0, (float)(battle_box.width), (float)(battle_box.height) };
	Rectangle dest_rect = { (float)0, (float)9 * TILE, (float)(battle_box.width) * SCALE, (float)(battle_box.height) * SCALE };
	DrawTexturePro(battle_box, source_rect, dest_rect, Vector2{ 0, 0 }, 0.0f, WHITE);
}

void MagicMenu::drawMagicOptions()
{
	float prev_y = 0;
	if (prev_menu_vis_index != menu_vis_index)
	{
		visual_options = makeVisbleOptions();
	}
	for (int i = 0; i < visual_options.size(); i++)
	{
		float x = 0;
		float y = 0;
		if (i % 2 == 0)
		{
			x = og_pos.x;
			y = og_pos.y + (float)i * 30.0f;
			prev_y = y;
		}
		else
		{
			x = og_pos.x + (float)TILE * 5;
			y = prev_y;
		}
		string name = game->magic_database.magic_scripts[visual_options[i]]["name"].asString();
		Vector2 pos = { x, y };
		drawText(pos, name, 25.0f);
	}
	drawMagicDetails(options);
}


vector<string> MagicMenu::makeVisbleOptions()
{
	prev_menu_vis_index = menu_vis_index;

	int max_option_index = (int)options.size() - 1;
	int vis_index_factor = max_option_index;
	vector<string> temp_options = {};

	if (max_option_index > 5)
	{
		vis_index_factor = 5;
	}
	int max_vis_index = menu_vis_index + vis_index_factor;
	if (max_vis_index > max_option_index)
	{
		max_vis_index = max_option_index;
	}

	for (int i = menu_vis_index; i <= max_vis_index; i++)
	{
		temp_options.push_back(options[i]);
	}
	return temp_options;
}

void MagicMenu::drawMagicDetails(vector<string> options)
{
	string name = options[menu_index];
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

	if (find(current_unit->magic_options.begin(), current_unit->magic_options.end(), name) != current_unit->magic_options.end())
	{
		string power = "Power:  " + game->magic_database.magic_scripts[name]["power"].asString();
		string mp_cost = "Cost:  " + game->magic_database.magic_scripts[name]["cost"].asString() + " MP";
		string type = "Type: " + type_map[game->magic_database.magic_scripts[name]["type"].asString()];
		string elem_type = "Elem. Type: ";
		string elem_val = elem_type_map[game->magic_database.magic_scripts[name]["elem_type"].asString()];
		string hit_val = "Hit:  " + game->magic_database.magic_scripts[name]["hit"].asString();
		string current_mp = "Current MP: " + to_string(current_unit->mp);

		Vector2 power_pos = { (float)10 * TILE + 12.0f, (float)9 * TILE + 12.0f };
		Vector2 hit_pos = { power_pos.x + (float)3 * TILE - (12.0f), power_pos.y };
		Vector2 type_pos = { power_pos.x, power_pos.y + 27.0f };
		Vector2 elem_type_pos = { hit_pos.x - (0.0f), type_pos.y + 3.0f };
		Vector2 elem_value_pos = { elem_type_pos.x + (float)TILE + 44.0f, type_pos.y + 2.0f};
		Vector2 mp_cost_pos = { type_pos.x, type_pos.y + 27.0f };
		Vector2 current_mp_pos = { mp_cost_pos.x + (float)3 * TILE - (12.0f), mp_cost_pos.y };

		drawText(power_pos, power, 20.0f);
		drawText(hit_pos, hit_val, 20.0f);
		drawText(type_pos, type, 20.0f);
		drawText(elem_type_pos, elem_type, 16.0f);
		drawText(elem_value_pos, elem_val, 18.0f);
		drawText(mp_cost_pos, mp_cost, 20.0f);
		drawText(current_mp_pos, current_mp, 20.0f);

		drawMagicDescription(name, options, mp_cost_pos);
	}
}

void MagicMenu::drawMagicDescription(string name, vector<string> options, Vector2 last_pos)
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
