#include "player_status_menu.h"
#include <format>
#include "game.h"
#include "party.h"
#include "menu_cursor.h"
#include "item_status_menu.h"
#include "player_select_menu.h"
#include "formation_status_menu.h"
#include "config_status_menu.h"
#include "input_handler.h"

PlayerStatusMenu::~PlayerStatusMenu()
{
	cursor.turnOn();
	unloadGraphics();
	party->state_controller.eraseState("status_menu");
}

void PlayerStatusMenu::setup(Game* game)
{
	basicSetup(game);
	title = "Player Status Menu";
	name = "player_status_menu";
	type = "status";
	setupParty(game);
	input->changeToMenuMode();
	setupGraphics();
	cursor.setup(game);
	cursor.turnOn();
	party->state_controller.pushState("status_menu");
}


void PlayerStatusMenu::setupGraphics()
{
	setupFaces();
}

void PlayerStatusMenu::setupFaces()
{
	int index = 0;
	for (auto& unit_pos : party->visible_party)
	{
		auto& unit = party->party[unit_pos];
		Texture2D face = LoadTexture(game->GFX.char_sprites[unit->graphics][unit->portrait].c_str());
		faces.push_back(face);
		face_map[unit_pos] = index;
		index++;
	}
}


void PlayerStatusMenu::update()
{
	SetCursor(cursor, options, TILE * 13.f + 24.f, 36.f, 48.f, 7);
	UpdateCursor(cursor);
	if (!scene_manager->busy)
	{
		if (cursor.go_back)
		{
			cursor.go_back = false;
			scene_manager->pop();
			input->changeToLevelMode();
		}
		if (cursor.return_input)
		{
			cursor.return_input = false;
			input_history.clear();
			input_history["option"] = cursor.selected_option;
			loadMenu();
		}
	}	
}

void PlayerStatusMenu::loadMenu()
{
	if (input_history["option"].asString() == "Items")
	{
		shared_ptr<ItemStatusMenu> item_menu = make_shared<ItemStatusMenu>();
		item_menu->setup(game, this);
		sound_player.playSound("click");
		PushMenu(item_menu);
	}
	if (input_history["option"].asString() == "Abilities")
	{
		shared_ptr<PlayerSelectMenu> player_select_menu = make_shared<PlayerSelectMenu>();
		player_select_menu->setup(game, this, "ability_status_menu");
		sound_player.playSound("click");
		PushMenu(player_select_menu, true);
	}
	if (input_history["option"].asString() == "Equip")
	{
		shared_ptr<PlayerSelectMenu> player_select_menu = make_shared<PlayerSelectMenu>();
		player_select_menu->setup(game, this, "equipment_status_menu");
		sound_player.playSound("click");
		PushMenu(player_select_menu, true);
	}
	if (input_history["option"].asString() == "Status")
	{
		shared_ptr<PlayerSelectMenu> player_select_menu = make_shared<PlayerSelectMenu>();
		player_select_menu->setup(game, this, "status_info_menu");
		sound_player.playSound("click");
		PushMenu(player_select_menu, true);
	}
	if (input_history["option"].asString() == "Formation")
	{
		shared_ptr<FormationStatusMenu> formation_menu = make_shared<FormationStatusMenu>();
		formation_menu->setup(game, this);
		sound_player.playSound("click");
		PushMenu(formation_menu, true);
	}
	if (input_history["option"].asString() == "Config")
	{
		shared_ptr<ConfigStatusMenu> config_menu = make_shared<ConfigStatusMenu>();
		config_menu->setup(game, this);
		sound_player.playSound("click");
		PushMenu(config_menu);
	}
	if (input_history["option"].asString() == "Save")
	{
		sound_player.playSound("cantclick");
		cout << "Not implemented yet" << endl;
		//--------INSERT SAVE MENU HERE--------
	}
}


void PlayerStatusMenu::draw()
{
	drawBackground();
	drawFaces();
	drawStatus();
	drawOptions();
	cursor.draw();
}

void PlayerStatusMenu::drawBackground()
{
	ClearBackground(BLACK);
	auto& background = game->GFX.UI["players_menu"];
	Rectangle source_rect = { 0, 0, (float)(background.width), (float)(background.height) };
	Rectangle dest_rect = { 0.f, 0.f, (float)source_rect.width * SCALE, (float)source_rect.height * SCALE };
	DrawTexturePro(background, source_rect, dest_rect, Vector2{ 0, 0 }, 0.0f, WHITE);
}

void PlayerStatusMenu::drawFaces()
{
	int num = 0;
	for (auto& unit_pos : party->visible_party)
	{
		auto& unit = party->party[unit_pos];
		float width = 96;
		float height = 32;
		float x = 48.0f;
		float y = (float)TILE/2 + TILE * ((num * 2)) + 26;
		Rectangle source_rect = { 0, 28, width, height };
		Rectangle dest_rect = { x, y, width * 2, height * 2 };
		Color color = WHITE;
		if (unit->dead == true)
		{
			color = GRAY;
		}
		DrawTexturePro(faces[num], source_rect, dest_rect, Vector2{ 0, 0 }, 0.0f, color);
		drawUnitStatus(unit.get(), dest_rect);
		num++;
	}
}

void PlayerStatusMenu::drawUnitStatus(Unit* unit, Rectangle dest_rect)
{
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

	//Stat positions
	Vector2 name_pos = { dest_rect.x + (float)4 * TILE + 20.f, dest_rect.y - 10.f};
	Vector2 class_pos = { name_pos.x + 3.f * TILE + 24.f, name_pos.y };
	Vector2 lvl_pos = { name_pos.x, name_pos.y + 31.f };
	Vector2 hp_pos = { lvl_pos.x, lvl_pos.y + 25.f };
	Vector2 mp_pos = { hp_pos.x + (float)3 * TILE + 24.f, hp_pos.y };
	
	drawText(name_pos, name_text, 22.f, WHITE);
	drawText(class_pos, class_name, 22.f, WHITE);
	drawText(lvl_pos, lvl_text, 20.f, WHITE);
	drawText(hp_pos, hp_ratio, 20.f, WHITE);
	drawText(mp_pos, mp_ratio, 20.f, WHITE);
}

void PlayerStatusMenu::drawStatus()
{
	float spacing = 32.f;
	double current_time = GetTime();
	int	hours = (int)current_time / (60 * 2);
	int min = ((int)current_time / (60 * 1)) % 60;
	int sec = (int)current_time % 60;

	string time_txt = "Time: ";	

	string hours_txt = to_string(hours);
	string mins_txt = format("{:02}", min);
	string secs_txt = format("{:02}", sec);

	string time_val = hours_txt + ":" + mins_txt + ":" + secs_txt;
	string step_txt = "Steps: ";
	string step_val = to_string(0);
	string silver_txt = "Ag: ";
	string silver_val = to_string(party->slv);

	Vector2 time_txt_pos = {(float)TILE * 12.f + 20.f, (float)TILE * 10.f + 12.f};
	Vector2 time_val_pos = { time_txt_pos.x + (float)MeasureText(time_txt.c_str(), 24), time_txt_pos.y};
	Vector2 step_txt_pos = { time_txt_pos.x, time_txt_pos.y + spacing };
	Vector2 step_val_pos = { step_txt_pos.x + (float)MeasureText(step_txt.c_str(), 24), step_txt_pos.y};
	Vector2 silver_txt_pos = { step_txt_pos.x, step_txt_pos.y + spacing };
	Vector2 silver_val_pos = { silver_txt_pos.x + (float)MeasureText(silver_txt.c_str(), 24), silver_txt_pos.y };

	Color light_blue = { 44, 238, 240 , 255};
	drawText(time_txt_pos, time_txt, 24.f, light_blue);
	drawText(time_val_pos, time_val, 24.f);
	drawText(step_txt_pos, step_txt, 24.f, light_blue);
	drawText(step_val_pos, step_val, 24.f);
	drawText(silver_txt_pos, silver_txt, 24.f, light_blue);
	drawText(silver_val_pos, silver_val, 24.f);
}

void PlayerStatusMenu::drawOptions()
{
	int num = 0;
	for (auto& option : options)
	{
		float x = vis_option_pos.x;
		float y = vis_option_pos.y + (vis_spacing * num);
		Vector2 pos{ x, y };
		drawText(pos, option, 24.f);
		num++;
	}
}


const Texture2D PlayerStatusMenu::GetFace(int unit_party_pos)
{
	return faces[face_map[unit_party_pos]];
}


void PlayerStatusMenu::unloadGraphics()
{
	unloadFaces();
}

void PlayerStatusMenu::unloadFaces()
{
	for (auto& face : faces)
	{
		UnloadTexture(face);
		cout << "UNLOADED FACE: "<< face.id << endl;
	}
	faces.clear();
	faces.shrink_to_fit();
	cout << "UNLOADED STATUS MENU" << endl;
}
