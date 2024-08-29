#pragma once
#include <raylib.h>
#include "apply_ability_menu.h"

using namespace std;

class PlayerSelectMenu : public ApplyAbilityMenu
{
public:
	virtual void setup(Game* game, PlayerStatusMenu* player_status_menu, string menu_to_push);
	virtual void update() override;
	virtual void draw() override;
private:
	//Setters
	void loadScene();
	//Data
	string menu_to_push;
};