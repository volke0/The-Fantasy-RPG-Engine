#include "lvl_up_screen.h"
#include "game.h"
#include "party.h"
#include "battle_controller.h"
#include "battle_menu_controller.h"

void LvLUpScreen::setup(Game* game, Party* party, Unit* unit, int num_of_lvl, BattleController* battle_controller, BattleMenuController* menu_controller)
{
	this->unit = unit;
	this->num_of_lvl = num_of_lvl;
	basicSetup(game, party, battle_controller, menu_controller);
	text_drawer = &game->text_drawer;
	handler.setup(game, this);
}

void LvLUpScreen::update()
{
	handler.update();
	timer.tick();
}


void LvLUpScreen::draw()
{
	drawLvLBox();
	drawLvlHeader();
	drawUnitPortrait();
	drawName();
	drawStatus();
}

void LvLUpScreen::drawLvlHeader()
{
	auto& lvl_header = game->GFX.UI["lvl_up_header"];
	Rectangle source_rect = { 0, 0, (float)(lvl_header.width), (float)(lvl_header.height) };
	Rectangle header_dest_rect = { (float)1 * TILE, (float)3 * TILE + 12, (float)source_rect.width * SCALE, (float)source_rect.height * SCALE };

	DrawTexturePro(lvl_header, source_rect, header_dest_rect, Vector2{ 0, 0 }, 0.0f, WHITE);
}

void LvLUpScreen::drawLvLBox()
{
	auto& lvl_box = game->GFX.UI["lvl_up_box_2"];
	Rectangle source_rect = { 0, 0, (float)(lvl_box.width), (float)(lvl_box.height) };
	Rectangle box_dest_rect = { (float)1 * TILE, (float)5 * TILE, (float)source_rect.width * SCALE, (float)source_rect.height * SCALE };

	DrawTexturePro(lvl_box, source_rect, box_dest_rect, Vector2{ 0, 0 }, 0.0f, WHITE);
}

void LvLUpScreen::drawUnitPortrait()
{
	int num = menu_controller->exp_menu.unit_order[unit->name];
	auto& face = menu_controller->exp_menu.faces[num];

	Rectangle source_rect = { 0, 0, (float)(face.width), (float)(face.height) };
	Rectangle dest_rect = { (float)9 * TILE, (float)5 * TILE, (float)source_rect.width * 4, (float)source_rect.height * 4 };

	DrawTexturePro(face, source_rect, dest_rect, Vector2{ 0, 0 }, 0.0f, WHITE);
}

void LvLUpScreen::drawName()
{
	float name_pos = (float)(unit->ingame_name.size() * 1) * 27.0f;
	Vector2 pos = { (float)2 * TILE + 30 + round(name_pos/2), (float)3 * TILE + 24 };
	text_drawer->drawText(pos, unit->ingame_name, 27.0f);
}

void LvLUpScreen::drawStatus()
{
	int count = 0;
	int index_1 = 0;
	int index_2 = 0;
	for (auto& stat_name_list : stat_names)
	{
		auto& key = stat_name_list[0];
		auto& name = stat_name_list[1];
		float x = (float)48 + 24;
		float y = (float)(5 * TILE + 32);
		float spacing_y = 40;
		if (count > 5)
		{
			x = (float)5 * TILE + 30;
			y = y + (float)(spacing_y * index_2);
			index_2++;
		}
		else
		{
			y = y + (float)(spacing_y * index_1);
			index_1++;
		}
		Vector2 pos = { x, y };
		Vector2 stat_pos = { pos.x + 50.0f, pos.y };
		text_drawer->drawText(pos, name, 21.0f);
		drawNormalStats(key, stat_pos);
		drawGrownStats(key, stat_pos);
		count++;
	}
}

void LvLUpScreen::drawNormalStats(string key, Vector2 stat_pos)
{
	string stat = to_string(unit_stats[key]);
	if (grown_stats.size() > 0)
	{
		if (grown_stats[key] > 0)
		{
			text_drawer->drawText(stat_pos, stat, 20.0f, true, GREEN);
		}
		else
		{
			text_drawer->drawText(stat_pos, stat, 20.0f, true, SKYBLUE);
		}
	}
	else
	{
		text_drawer->drawText(stat_pos, stat, 20.0f, true, SKYBLUE);
	}
}

void LvLUpScreen::drawGrownStats(string key, Vector2 stat_pos)
{
	if (show_grown_stats == true)
	{
		if (grown_stats[key] != 0)
		{
			Vector2 pos = { stat_pos.x + 50.0f, stat_pos.y };
			string stat = "+" + to_string(grown_stats[key]);
			text_drawer->drawText(pos, stat, 20.0f, true, GREEN);
		}
	}
}
