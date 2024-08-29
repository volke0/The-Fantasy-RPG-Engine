#pragma once
#include <raylib.h>
#include "sub_status_menu.h"

class Game;
class Party;
class PlayerStatusMenu;
class MenuCursor;

class StatusInfoMenu : public SubStatusMenu
{
public:
	virtual void setup(Game* game, PlayerStatusMenu* player_status_menu, int unit_num);
	virtual void update() override;
	virtual void draw() override;
private:
	//Draw
	void DrawBackground();
	void DrawInfo();
	void DrawCharacter();
	void DrawCharacterName(Rectangle dest_rect);
	void DrawStatus();
};
