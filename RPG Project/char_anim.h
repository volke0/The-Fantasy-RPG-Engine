#pragma once
#include <raylib.h>
#include <string>
#include <iostream>
#include "animation.h"

using namespace std;

class Game;
class Unit;
class Sprites;

class CharacterAnimation : public Animations
{
public:
	void setup(Game* game, Unit* unit, Json::Value script, Sprites* sprite_sheet);
	void play() override;
private:
	//Setup
	void initializeMetaData() override;
	void initializeUnitMode();

	//Play methods
	void checkIfMaxIndex() override;

	Rectangle map_rect;
};