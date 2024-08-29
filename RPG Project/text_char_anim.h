#pragma once
#include <raylib.h>
#include <string>
#include <iostream>
#include "animation.h"
#include "vars.h"

using namespace std;

class Game;
class Unit;

class TextCharAnimation : public Animations
{
public:
	void setup(Game* game, Unit* unit, Json::Value script, char text, Json::Value meta_data);
	void play() override;

private:
	//Setup
	void initializeMetaData() override;
	void initializeTextMode();

	//Data
	Json::Value meta_data;
	string text;
	Color text_color;
	Color dropshadow_color = { 0, 0, 0, 255 };
	float dropshadow_offset = (float)1 + (TILE / 2 / 15);
	int position;
	float fontSize;

	Rectangle battle_rect = { 0, 0, 0, 0 };
};