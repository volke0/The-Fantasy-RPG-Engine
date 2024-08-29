#include "save_select.h"
#include "input_handler.h"
#include "scene_manager.h"
#include "game.h"
#include "level.h"
#include "vars.h"

void SaveSelect::setup(Game* game)
{
	title = "Save Select";
	name = "save_select";
	type = "save_select";
	basicSetup(game);
}

void SaveSelect::draw()
{
	ClearBackground(Blue);
	DrawText(title.c_str(), 10, 10, 24, WHITE);
	DrawText("Press X to go to the first level of the game", 10, 44, 20, WHITE);
	DrawText("Press Z to go back to the Main Menu", 10, 68, 20, WHITE);
}

void SaveSelect::update()
{
	if (input->findKey("x"))
	{
		game->push_level();
	}
	if (input->findKey("z"))
	{
		scene_manager->pop();
	}
}