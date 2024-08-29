#pragma once
#include <raylib.h>
#include "confirm_status_menu.h"

using namespace std;

class Game;
class Party;
class PlayerStatusMenu;
class MenuCursor;

class ItemConfirmStatusMenu : public ConfirmStatusMenu
{
public:
	virtual void setup(Game* game, PlayerStatusMenu* player_status_menu) override;
	virtual void update() override;
	virtual void draw() override;

	//Setters
	void setItem(int item_pos);

private:
	//Data
	int item_pos = 0;
};
