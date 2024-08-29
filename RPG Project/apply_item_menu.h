#pragma once
#include <raylib.h>
#include "apply_ability_menu.h"

using namespace std;

class Game;
class Party;
class PlayerStatusMenu;
class MenuCursor;
class Consumable;
class Unit;

class ApplyItemMenu : public ApplyAbilityMenu
{
public:
	virtual void setup(Game* game, PlayerStatusMenu* player_status_menu, int item_pos);
	virtual void update() override;
	virtual void draw() override;
private:
	//Setters
	void applyItemEffect(Unit* unit, Consumable* item);
	void popToItemSelectMenu();

	//Data
	int item_pos = 0;
};
