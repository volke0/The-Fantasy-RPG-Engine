#include "battle_ui.h"
#include "game.h"
#include "party.h"
#include "unit.h"
#include "battle_controller.h"
#include "vars.h"

void BattleUI::setup(Game* game, Party* party, BattleController* battle_controller, BattleMenuController* menu_controller)
{
	basicSetup(game, party, battle_controller, menu_controller);
	this->combatants = menu_controller->combatants;
}



void BattleUI::update()
{
}



void BattleUI::draw()
{
	drawUI();
}

void BattleUI::drawUI()
{
	drawUIBox();
	drawUIText();
	drawPartyMembersUI();
}

void BattleUI::drawUIBox()
{
	auto& battle_box = game->GFX.UI["battle_box"];
	Rectangle source_rect = { 0, 0, (float)(battle_box.width), (float)(battle_box.height) };
	Rectangle dest_rect = { (float)0, (float)9 * TILE, (float)(battle_box.width) * SCALE, (float)(battle_box.height) * SCALE };
	DrawTexturePro(battle_box, source_rect, dest_rect, Vector2{ 0, 0 }, 0.0f, WHITE);
}

void BattleUI::drawUIText()
{
}

void BattleUI::drawPartyMembersUI()
{
	for (auto& unit : party->party)
	{
		if (unit != nullptr)
		{
			Vector2 pos = { (float)5 * TILE, (float)9 * TILE + 15 + (float)unit->party_pos * 30.0f };
			string name = unit->ingame_name;

			drawPartyMemberName(pos, name);
			drawPartyMemberStatus(pos, unit.get());
		}
	}
}

void BattleUI::drawPartyMemberName(Vector2 pos, string name)
{
	drawText(pos, name, partyNameFontSize);
}

void BattleUI::drawPartyMemberStatus(Vector2 pos, Unit* unit)
{
	//HP
	float name_size = 25 * SCALE + partyNameFontSize;
	Vector2 hp_pos = { pos.x + name_size, pos.y };
	string max_hp = to_string(unit->max_hp);
	string hp = to_string(unit->hp);
	string hp_ui = "HP: " + hp;
	drawText(hp_pos, hp_ui, partyNameFontSize);

	//MP
	Vector2 mp_pos = { hp_pos.x + (float)TILE * 3 + 18, pos.y };
	string max_mp = to_string(unit->max_mp);
	string mp = to_string(unit->mp);
	string mp_ui = "MP: " + mp;
	drawText(mp_pos, mp_ui, partyNameFontSize, SKYBLUE);

	//SP
	Vector2 sp_pos = { mp_pos.x + (float)TILE * 3 + 18, pos.y };
	string max_sp = to_string(unit->max_sp);
	string sp = to_string(unit->sp);
	string sp_ui = "SP: " + sp;
	drawText(sp_pos, sp_ui, partyNameFontSize, GREEN);
}
