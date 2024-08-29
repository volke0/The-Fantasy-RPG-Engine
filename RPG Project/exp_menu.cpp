#include "exp_menu.h"
#include "game.h"
#include "unit.h"
#include "party.h"
#include "battle_controller.h"
#include "vars.h"

ExpMenu::~ExpMenu()
{
	unloadPartyFaces();
}

void ExpMenu::setup(Game* game, Party* party, BattleController* battle_controller, BattleMenuController* menu_controller)
{
	text_drawer = &game->text_drawer;
	basicSetup(game, party, battle_controller, menu_controller);
	handler.setup(game, party, this);
	sfx_player.setup(game);
}

void ExpMenu::update()
{
	handler.update();
	sfx_player.update();
}

void ExpMenu::draw()
{
	auto& exp_box = game->GFX.UI["exp_box"];
	Rectangle source_rect = { 0, 0, (float)(exp_box.width), (float)(exp_box.height) };
	Rectangle dest_rect = { (float)2 * TILE, (float)1 * TILE, (float)(exp_box.width) * SCALE, (float)(exp_box.height) * SCALE };
	DrawTexturePro(exp_box, source_rect, dest_rect, Vector2{0, 0}, 0.0f, WHITE);
	drawExpFaces();
	drawSilverAmount();
	handler.draw();
}

void ExpMenu::drawExpFaces()
{
	int num = 0;
	loadPartyFaces();
	for (auto& unit : party->party)
	{
		float width = 96;
		float height = 32;
		float x = (float)2 * TILE + 6;
		float y = (float)1 * TILE * (1 + (num * 2)) + 26;
		Rectangle source_rect = { 0, 28, width, height };
		Rectangle dest_rect = { x, y, width * 2, height * 2 };
		DrawTexturePro(faces[num], source_rect, dest_rect, Vector2{ 0, 0 }, 0.0f, WHITE);
		drawExpBars(unit.get(), dest_rect, num);
		num++;
	}
}

void ExpMenu::drawExpBars(Unit* unit, Rectangle pos, int& num)
{
	//"LVL: " text
	Vector2 lvl_pos = { pos.x + pos.width + (float)12, pos.y + (float)16 + 13 };
	string lvl_num = to_string(unit->lvl);
	string lvl_text = "LVL:  " + lvl_num;
	text_drawer->drawText(lvl_pos, lvl_text, 24.0f);

	//"EXP: " text
	Vector2 text_pos = { pos.x + pos.width + (float)60 + TILE, pos.y + (float)16 + 13};
	text_drawer->drawText(text_pos, "EXP: ", 24.0f);

	//Max bar size
	Rectangle max_rect = { pos.x + pos.width + (float)132 + TILE, pos.y + (float)16 + 16, 200, 5 };
	DrawRectanglePro(max_rect, Vector2{ 0, 0 }, 0.0f, GRAY);

	//Current Exp
	float current_x = max_rect.x;
	float current_y = pos.y + (float)16 + 16;
	Rectangle current_rect = { current_x, current_y, 200 * exp_ratios[unit->name], 5 };
	DrawRectanglePro(current_rect, Vector2{ 0, 0 }, 0.0f, BLUE);

	//Current Exp TEXT FORM
	Vector2 exp_text_pos = { max_rect.x + 72, pos.y + 38};
	float base_exp = (float)unit->exp - (float)unit->base_exp;
	float required_exp = (float)unit->until_next_level - (float)unit->base_exp;
	string exp_text = to_string((int)base_exp) + "/" + to_string((int)required_exp);
	text_drawer->drawText(exp_text_pos, exp_text, 16.0f, true);
}

void ExpMenu::drawSilverAmount()
{
	string party_silver_total = "Total Ag: ";
	string current_amt = to_string(party->slv);
	string added_silver = " +" + to_string(battle_controller->total_silver);

	Vector2 silver_pos = { (float)2 * TILE + 12.f, (float)9 * TILE + 24.f};
	Vector2 current_amt_pos = { silver_pos.x + (float)MeasureText(party_silver_total.c_str(), 24) - 10.f, silver_pos.y };
	Vector2 added_silver_pos = { current_amt_pos.x + (float)MeasureText(current_amt.c_str(), 24) + 5.f, current_amt_pos.y };

	text_drawer->drawText(silver_pos, party_silver_total, 24.0f, true);
	text_drawer->drawText(current_amt_pos, current_amt, 24.0f, true, total_silver_color);
	text_drawer->drawText(added_silver_pos, added_silver, 24.0f);
}


void ExpMenu::loadPartyFaces()
{
	if (!loadedFacesOnce)
	{
		int index = 0;
		for (auto& unit : party->party)
		{
			faces.push_back(LoadTexture(game->GFX.char_sprites[unit->graphics][unit->portrait].c_str()));
			unit_order[unit->name] = index;
			index++;
		}
		loadedFacesOnce = true;
	}
}

void ExpMenu::unloadPartyFaces()
{
	if (!clearedFaces)
	{
		for (auto& face : faces)
		{
			UnloadTexture(face);
		}
		faces.clear();
		faces.shrink_to_fit();
		clearedFaces = true;
	}	
}




