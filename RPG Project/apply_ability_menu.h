#pragma once
#include <raylib.h>
#include "sub_status_menu.h"

using namespace std;

class Game;
class Party;
class PlayerStatusMenu;
class MenuCursor;

class ApplyAbilityMenu : public SubStatusMenu
{
public:
	virtual void setup(Game* game, PlayerStatusMenu* player_status_menu) override;
	virtual void update() override;
	virtual void draw() override;

	//Data
	int player_menu_pos = 0;
private:
	//Setup
	void setupBasicMenuStuff();
	vector<string> createPlayerSelectOptionList();
};
