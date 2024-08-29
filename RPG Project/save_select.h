#pragma once
#include <raylib.h>
#include "scene_menu.h"

using namespace std;

class Game;

class SaveSelect : public SceneMenu
{
public:
	void setup(Game* game) override;
	void draw() override;
	void update() override;
private:
};