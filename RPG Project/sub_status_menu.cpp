#include "sub_status_menu.h"
#include "player_status_menu.h"

void SubStatusMenu::setup(Game* game, PlayerStatusMenu* player_status_menu)
{
	basicSetup(game);
	setupParty(game);
	setupPlayerStatusMenuVariables(player_status_menu);
	horizontal = false;
	options = { "" };
}

void SubStatusMenu::draw()
{
	SubMenu::draw();
}

void SubStatusMenu::update()
{
	SubMenu::update();
}

void SubStatusMenu::setupPlayerStatusMenuVariables(PlayerStatusMenu* player_status_menu)
{
	this->player_status_menu = player_status_menu;
	this->cursor = &player_status_menu->cursor;
}
