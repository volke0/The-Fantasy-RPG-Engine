#pragma once
#include <raylib.h>
#include "sub_status_menu.h"

using namespace std;

class Game;
class Party;
class PlayerStatusMenu;
class MenuCursor;

class AbilityStatusMenu : public SubStatusMenu
{
public:
	virtual void setup(Game* game, PlayerStatusMenu* player_status_menu) override;
	virtual void update() override;
	virtual void draw() override;

private:
	//Draw
	void DrawBackground();
	void DrawCharacter();
	void DrawStatus(Rectangle dest_rect);
};
