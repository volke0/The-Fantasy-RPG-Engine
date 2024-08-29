#pragma once
#include <raylib.h>
#include "sub_menu.h"

using namespace std;

class Game;
class Party;
class PlayerStatusMenu;
class MenuCursor;

class SubStatusMenu : public SubMenu
{
public:
	virtual void setup(Game* game, PlayerStatusMenu* player_status_menu);
	virtual void draw() override;
	virtual void update() override;

	//Setup
	void setupPlayerStatusMenuVariables(PlayerStatusMenu* player_status_menu);

	//Data
	PlayerStatusMenu* player_status_menu;
};
