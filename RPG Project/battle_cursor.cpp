#include "battle_cursor.h"
#include "game.h"
#include "npc.h"
#include "party.h"
#include "input_handler.h"
#include "vars.h"

BattleCursor::~BattleCursor()
{
	/*if (click.frameCount > 0)
	{
		UnloadSound(click);
	}*/
}

void BattleCursor::setup(Game* game)
{
	basicSetup(game);
	dest = { (float)0 * TILE, (float)0 * TILE, (float)(cursor.width) * 2, (float)(cursor.height) * 2 };
	click = game->sfx_database.SFX["click"];
	manager.setup(game, this);
}


void BattleCursor::update()
{
	if (on == true)
	{
		manager.update();
		sound_player.update();
	}
}


void BattleCursor::draw()
{
	if (on == true)
	{
		if (unit_mode == false)
		{
			if (magic_mode == false)
			{
				x_factor = 0;
				y_factor = (float)(30 * (vis_index));
			}
			float x = dest.x + x_factor;
			float y = dest.y + y_factor;
			Rectangle new_dest = { x, y, dest.width, dest.height };
			DrawTexturePro(cursor, src, new_dest, Vector2{ 0, 0 }, 0.0f, WHITE);
		}
		if (unit_mode == true)
		{
			if (bMultiOn)
			{
				DrawMultiMode();
				return;
			}
			CalculateCursorUnitPos(index);
			string current_name = options[index];
			auto& current_unit = units[current_name];

			Rectangle new_dest = { dest.x , dest.y, dest.width, dest.height };
			DrawTexturePro(cursor, src, new_dest, Vector2{ 0, 0 }, 0.0f, WHITE);
		}
	}
}


void BattleCursor::setCursor(Vector2 pos, vector<string> options, bool horizontal, int max_vis_index)
{
	dest.x = pos.x - 36;
	dest.y = pos.y + 3;
	this->options = options;
	if (horizontal == true)
	{
		magic_mode = true;
	}
	else
	{
		magic_mode = false;
	}

	if ((int)options.size() - 1 > max_vis_index)
	{
		this->max_vis_index = max_vis_index;
	}
	else
	{
		this->max_vis_index = (int)options.size() - 1;
	}
	unit_mode = false;
	src.width = og_width;
}

void BattleCursor::setCursor(vector<string> unit_names, unordered_map<string, shared_ptr<NPC>> combatants)
{
	this->units = combatants;
	unit_mode = true;

	dest.x = 0.f;
	dest.y = 45.f;
	
	//index = 0;
	options.clear();
	options.shrink_to_fit();
	src.width = -1 * og_width;
	for (auto& name : unit_names)
	{
		options.push_back(name);
	}
	CalculateCursorUnitPos(index);
}

void BattleCursor::turnMagicCursorOn()
{
	manager.mag_select_handler.turnOn();
}

void BattleCursor::moveMagicVisualIndex(int num)
{
	manager.mag_select_handler.visual_index += num;
	manager.mag_select_handler.setCursor();
}

void BattleCursor::setMagicVisualIndex(int num)
{
	manager.mag_select_handler.visual_index = num;
}

void BattleCursor::SetMultiTargetMode(bool bMultiOn)
{
	this->bMultiOn = bMultiOn;
}

void BattleCursor::CalculateCursorUnitPos(int index)
{
	float width = 0.f;
	float height = 0.f;
	float middle = 0.f;

	auto& current_unit = units[options[index]];
	width = current_unit->battle_rect.width;
	height = current_unit->battle_rect.height;

	if (switch_to_enemies == true)
	{
		if (current_unit->type != "enemy") return;
		src.width = -1 * og_width;
		dest.x = current_unit->battle_rect.x + width - (TILE);
	}
	if (switch_to_allies == true)
	{
		if (current_unit->type != "playable") return;
		src.width = og_width;
		dest.y = 65.f;
		dest.x = current_unit->battle_rect.x + (width / 2.f) - (TILE + 16.f);
	}

	middle = current_unit->battle_rect.y + (height / 2.f);
	current_unit->anim_source_rect;
	dest.y = middle;
}

int BattleCursor::getMagicVisualIndex()
{
	return manager.mag_select_handler.visual_index;
}

void BattleCursor::DrawMultiMode()
{
	draw_index++;
	if (draw_index >= (int)options.size())
	{
		draw_index = 0;
	}

	CalculateCursorUnitPos(draw_index);
	string current_name = options[draw_index];
	auto& current_unit = units[current_name];

	Rectangle new_dest = { dest.x , dest.y, dest.width, dest.height };
	DrawTexturePro(cursor, src, new_dest, Vector2{ 0, 0 }, 0.0f, WHITE);
}


