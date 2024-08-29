#include "dialogue_box.h"
#include "game.h"
#include "vars.h"

void DialogueBox::setup(Game* game)
{
	box = "dialogue_box";
	box_type = "dialogue";
	TextBox::setup(game);
	setupGFX(game->GFX.UI);
	parseSettings();
	//setText("Where am I?\tI can't seem to remember.[A]\tMaybe I should have a\tlook around.[A]");
}


void DialogueBox::update()
{
	TextBox::update();
}

void DialogueBox::draw()
{
	TextBox::draw();
}


