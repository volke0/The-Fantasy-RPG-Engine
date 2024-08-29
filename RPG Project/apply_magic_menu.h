#pragma once
#include <raylib.h>
#include "apply_ability_menu.h"

using namespace std;

class Game;
class Party;
class PlayerStatusMenu;
class MenuCursor;
class Magic;
class Unit;

class ApplyMagicMenu : public ApplyAbilityMenu
{
public:
	virtual void setup(Game* game, PlayerStatusMenu* player_status_menu, string magic_option, Unit* user);
	virtual void update() override;
	virtual void draw() override;
private:
	//Data
	string magic_option;
	Unit* user;
};
