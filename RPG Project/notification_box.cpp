#include "notification_box.h"
#include "game.h"
#include "vars.h"

void NotificationBox::setup(Game* game)
{
	box = "dialogue_box";
	box_type = "notification";
	TextBox::setup(game);
	setupGFX(game->GFX.UI);
	parseSettings();
}

void NotificationBox::update()
{
	TextBox::update();
}

void NotificationBox::draw()
{
	TextBox::draw();
}
