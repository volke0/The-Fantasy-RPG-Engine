#pragma once
#include <raylib.h>
#include <vector>
#include <memory>
#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;

class Game;

class Text
{
public:
	void setup(Game* game);
	void drawText(Vector2 pos, string text, float fontSize, bool dropShadowOn = true, Color color = { 255, 255, 255, 255 });

private:
	//Data
	Game* game;
};
