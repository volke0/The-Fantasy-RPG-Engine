#include "game_over_scene.h"
#include "battle_controller_handler.h"
#include "scene_manager.h"

void GameOverScene::setup(Game* game, BattleControllerHandler* battle_handler)
{
	basicSetup(game);
	this->battle_handler = battle_handler;
}



void GameOverScene::update()
{
	//---IMPLEMENT RETURN TO LAST SAVE POINT HERE---//
}


void GameOverScene::draw()
{
	scene_manager->getLastScene()->draw();
	drawText(Vector2{ 300.f, 300.f }, "GAMEOVER", 70.f, Color{ 255, 0, 0, 255 });
}
