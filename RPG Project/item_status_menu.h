#pragma once
#include <raylib.h>
#include "sub_status_menu.h"

using namespace std;

class Game;
class Party;
class PlayerStatusMenu;
class MenuCursor;

class ItemStatusMenu : public SubStatusMenu
{
public:
	virtual void setup(Game* game, PlayerStatusMenu* player_status_menu) override;
	virtual void update() override;
	virtual void draw() override;

	//Setup
	void makeOptions();

	//Setters
	void setShowUnusableItems(bool showUnusableItems = false);
private:

	//Draw
	void drawBackground() override;
	void drawItems();
	void drawItemDescription(Json::Value item);
	vector<Json::Value> makeVisibleOptions();

	//Logic
	bool showUnusableItems = false;
};
