#pragma once
#include <raylib.h>
#include "menu_cursor.h"
#include "status_menu.h"

using namespace std;

class Game;
class Party;
class ItemStatusMenu;

class PlayerStatusMenu : public StatusMenu
{
public:
	~PlayerStatusMenu();
	virtual void setup(Game* game) override;
	virtual void draw() override;
	virtual void update() override;

	//Draw
	virtual void drawFaces();
	virtual void drawBackground() override;

	//Getters
	const Texture2D GetFace(int unit_party_pos);

	//Cursor
	MenuCursor cursor;

	//Data
	Json::Value input_history;
private:
	//Setup
	virtual void setupGraphics() override;
	void setupFaces();

	//Draw
	void drawUnitStatus(Unit* unit, Rectangle dest_rect);
	void drawStatus();
	void drawOptions();

	//Update
	void loadMenu();

	//Data
	vector<string> options = {"Items", "Abilities", "Equip", "Status", "Formation", "Config", "Save"};

	//Disassembly
	virtual void unloadGraphics() override;
	void unloadFaces();
};
