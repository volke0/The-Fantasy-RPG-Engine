#include "title.h"
#include "game.h"
#include "vars.h"

void Title::setup(Game* game)
{
	title = "Title";
	name = "title";
	basicSetup(game);
}

void Title::draw()
{
	ClearBackground(Blue);
	DrawText(title.c_str(), 10, 10, 24, WHITE);
	DrawText("Press X to go to the Main Menu", 10, 44, 20, WHITE);
	//auto& texture = game->GFX.UI["dialogue_box"];
	//DrawTexturePro(texture, Rectangle{ 0, 0, (float)(texture.width), (float)(texture.height)}, Rectangle{ (float)0, (float)0, (float)(texture.width)*SCALE, (float)(texture.height)*SCALE}, Vector2{ (float)-33.5*SCALE, 0 }, 0.0f, WHITE);
}

void Title::update()
{
	if (input->findKey("x"))
	{
		push_scene(game->main_menu);
	}
}
