#pragma once
#include <raylib.h>
#include "sub_status_menu.h"

using namespace std;

class Game;
class Party;
class PlayerStatusMenu;
class MenuCursor;

class EquipStatusMenu : public SubStatusMenu
{
public:
	virtual void setup(Game* game, PlayerStatusMenu* player_status_menu) override;
	virtual void update() override;
	virtual void draw() override;
private:
	//Draw
	void DrawBackground();
	void DrawCharacter();
	void DrawCharacterName(Rectangle dest_rect);
	void DrawEquipment();
	void DrawStatus();

	//Data
	vector<string> equipment_values = { "r_hand", "l_hand", "head", "body", "feet" };
};
