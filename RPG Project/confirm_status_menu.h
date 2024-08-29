#pragma once
#include <raylib.h>
#include "sub_status_menu.h"

using namespace std;

class Game;
class Party;
class PlayerStatusMenu;
class MenuCursor;

class ConfirmStatusMenu : public SubStatusMenu
{
public:
	virtual void setup(Game* game, PlayerStatusMenu* player_status_menu) override;
	virtual void update() override;
	virtual void draw() override;
	virtual void drawConfirmMessage();
	void drawOptions();
	vector<string> options = { "Yes", "No" };
	Vector2 box_pos = { 0.0f, 0.0f };
	string confirm_message;
	Vector2 confirm_pos;
	float fontSize = 23.f;
};