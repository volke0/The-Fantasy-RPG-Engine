#include "status_menu.h"
#include "game.h"
#include "unit.h"
#include "party.h"
#include "menu_cursor.h"

StatusMenu::~StatusMenu()
{
	unloadGraphics();
}

void StatusMenu::setup(Game* game)
{
	SceneMenu::setup(game);
	title = "Status Menu";
	name = "status_menu";
	type = "status";
}

void StatusMenu::setupGraphics()
{
	
}

void StatusMenu::draw()
{
	ClearBackground(BLACK);
	auto& background = game->GFX.UI["players_menu"];
	Rectangle source_rect = { 0, 0, (float)(background.width), (float)(background.height) };
	Rectangle dest_rect = { 0.f, 0.f, (float)source_rect.width * SCALE, (float)source_rect.height * SCALE };
	DrawTexturePro(background, source_rect, dest_rect, Vector2{ 0, 0 }, 0.0f, WHITE);
	drawText(Vector2{ 10.f, 10.f }, title.c_str(), 24.f, WHITE, false);
}

void StatusMenu::update()
{
	if (input->findKey("z"))
	{
		scene_manager->pop();
		input->changeToLevelMode();
	}
}

void StatusMenu::unloadGraphics()
{
	
}
