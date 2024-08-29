#pragma once
#include <raylib.h>
#include "scene.h"

using namespace std;

class Game;

class Title : public Scene
{
public:
	void setup(Game* game) override;
	void draw() override;
	void update() override;
private:
};
