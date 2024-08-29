#pragma once
#include <raylib.h>
#include "sub_status_menu.h"

using namespace std;

class Game;
class Party;
class PlayerStatusMenu;
class Unit;

class MagicStatusMenu : public SubStatusMenu
{
public:
	virtual void setup(Game* game, PlayerStatusMenu* player_status_menu, Unit* unit);
	virtual void update() override;
	virtual void draw() override;
private:
	//Draw
	void drawMagicInfoBox();
	void drawMagicOptions();
	void drawMagicDetails();
	void drawMagicDescription(string name, Vector2 last_pos);

	//Getters
	void getMagicOptions();
	vector<string> getVisibleOptions();

	//Data
	Unit* unit;
};

