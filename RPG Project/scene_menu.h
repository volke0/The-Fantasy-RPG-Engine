#pragma once
#include <raylib.h>
#include "scene.h"

using namespace std;

class Game;
class MenuCursor;
class Party;

class SceneMenu : public Scene
{
public:
	virtual void setup(Game* game) override;
	void draw() override;
	void update() override;

	//Setup
	void setupParty(Game* game);

	//Getters
	void GetPreviousCursorPosition(MenuCursor& cursor);

	//Setters
	void SetCursor(MenuCursor& cursor, vector<string> options, float vis_x, float vis_y, float vis_spacing, int max_vis_index, bool horizontal = false);
	void RememberCursorPosition(MenuCursor& cursor);
	void PushMenu(shared_ptr<Scene> scene, bool isInstant = false, int speed = 16, float stall_time = 30.0f);
	void PopMenu(MenuCursor& cursor, bool isInstant = false, int pop_amt = 1, int speed = 16, float stall_time = 30.0f);
	void SetVisIndexToTheMax(MenuCursor& cursor);

	//Update
	void UpdateCursor(MenuCursor& cursor);
	void updateVisibleOptions(MenuCursor& cursor);

	//Draw 
	virtual void drawBackground();
	Vector2 vis_option_pos = {};
	int max_vis_index = 15;
	float vis_x = 0.0f;
	float vis_y = 0.0f;
	float vis_spacing = 48.f;
	int memory_index = 0;
	int memory_vis_index = 0;
	int memory_cursor_vis_index = 0;
	int vis_index = 0;
	float font_size = 24.f;

	//Logic
	bool pushedScene = false;

	//Data
	Party* party;
};
