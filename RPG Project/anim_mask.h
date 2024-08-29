#pragma once
#include <raylib.h>
#include <string>
#include <iostream>
#include "animation.h"
#include "sprites.h"

using namespace std;

class Unit;
class Game;


class MaskAnimation : public Animations
{
public:
	void setup(Game* game, Unit* unit, Json::Value script);

	void play();

	//Setters
	void setAnimation(Animations* anim);

private:
	//Setup
	void initializeMetaData() override;
	void initializeMaskMode();
	void setupSpriteSheet();

	//Animation
	Animations* animation;

	//Sprites
	Sprites mask_sprite_sheet;

	//Palette
	string palette_name;

	//Logic
	bool follow_unit = true;
};