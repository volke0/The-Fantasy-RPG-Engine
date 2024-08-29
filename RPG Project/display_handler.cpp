#include "display_handler.h"
#include "btl_animation_manager.h"

void DisplayHandler::basicSetup(Game* game, BattleAnimationManager* btl_animation_manager)
{
	this->game = game;
	this->btl_animation_manager = btl_animation_manager;
	this->battle = btl_animation_manager->battle;
	this->user = btl_animation_manager->user;
}

void DisplayHandler::update()
{
}
