#include "battle_controller.h"
#include "npc.h"
#include "party.h"
#include "battle_scene.h"
#include "game.h"

void BattleController::setup(Game* game, Party* party, BattleScene* battle_scene, string background_name, unordered_map<string, shared_ptr<NPC>> combatants)
{
	this->game = game;
	this->party = party;
	this->background_name = background_name;
	this->combatants = combatants;
	this->battle_scene = battle_scene;
	cursor.setup(game);
	state_controller.setup(game, this);
	menu_controller.setup(game, party, combatants, this);
	battle_queue_manager.setup(game, this, this->combatants);
	vfx_controller.setup(game);
	handler.setup(game, this, &vfx_controller);
}


void BattleController::update()
{
	vfx_controller.update();
	state_controller.update();
	handler.update();
	battle_queue_manager.refreshCombatants(combatants);
	battle_queue_manager.update();
	menu_controller.refreshCombatants(combatants);
	menu_controller.update();
	cursor.update();
}


void BattleController::draw()
{
	drawBackground();
	drawCombatants();
	handler.draw();
	battle_queue_manager.draw();
	menu_controller.draw();
	cursor.draw();
	vfx_controller.draw();
}

void BattleController::drawBackground()
{
	auto& background = game->GFX.backgrounds[background_name];
	Rectangle source_rect = { 0, 0, (float)(background.width), (float)(background.height) };
	Rectangle dest_rect = { (float)0, (float)0, (float)(background.width) * SCALE, (float)(background.height) * SCALE };
	DrawTexturePro(background, source_rect, dest_rect, Vector2{ 0, 0 }, 0.0f, WHITE);
}

void BattleController::drawCombatants()
{
	for (auto& unit : party->party)
	{
		unit->draw();
	}
	for (auto& enemy : battle_scene->enemies)
	{
		enemy->draw();
	}
}
