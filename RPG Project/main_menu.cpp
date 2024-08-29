#include "main_menu.h"
#include "scene_manager.h"
#include "input_handler.h"
#include "game.h"
#include "vars.h"

void MainMenu::setup(Game* game)
{
	title = "Main Menu";
	name = "main_menu";
	basicSetup(game);
}

void MainMenu::draw()
{
	ClearBackground(Blue);
	DrawText(title.c_str(), 10, 10, 24, WHITE);
	DrawText("Press X to go to the Save Select Screen", 10 , 44, 20, WHITE);
	DrawText("Press Z to go back to the Title Screen", 10, 68, 20, WHITE);
}

void MainMenu::update()
{
	if (input->findKey("x"))
	{
		push_scene(game->save_select);
	}
	if (input->findKey("z"))
	{
		scene_manager->pop();
	}
}
