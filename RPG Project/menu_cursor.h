#pragma once
#include <raylib.h>
#include "menu_cursor_manager.h"
#include "battle_cursor.h"

using namespace std;

class Game;
class Input;
class Unit;

class MenuCursor : public BattleCursor
{
public:
	~MenuCursor();
	virtual void setup(Game* game) override;
	virtual void update() override;
	virtual void draw() override;

	//Setters
	void turnOn();
	void changeVisSpacing(float vis_spacing);

	//Getters
	bool findKey(string key);
private:
	//Manager
	MenuCursorManager manager;

	//Data
	float vis_spacing = 48.f;
};
