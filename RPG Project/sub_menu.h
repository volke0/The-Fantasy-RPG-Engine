#pragma once
#include <raylib.h>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include "scene_menu.h"

using namespace std;

class Game;
class Party;
class MenuCursor;

class SubMenu : public SceneMenu
{
public:
	virtual void setup(Game* game) override;
	virtual void draw() override;
	virtual void update() override;

	//Getters
	int GetUnitNumIndex(int unit_num);

	//Setters
	void SetUnitNum(bool bIncrementUp);

	//Options
	virtual void DrawOptions();
	vector<string> MakeVisibleOptions();
	vector<string> options;
	Color option_color = WHITE;

	//Data
	MenuCursor* cursor;
	int unit_num = 0;

	//Logic
	bool first_loaded = true;
	bool horizontal = false;
};
