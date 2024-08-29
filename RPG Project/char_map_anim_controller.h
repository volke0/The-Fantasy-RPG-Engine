#pragma once
#include <raylib.h>
#include "char_anim_controller.h"

class Game;
class Unit;
class Animations;

class CharacterMapAnimationController : public CharacterAnimationController
{
public:
	virtual void setup(Game* game, Unit* unit, Json::Value controller_script) override;

	virtual void update() override;
	virtual void draw() override;
private:
	//Update
	void AnimationSwitch();

	//Getters
	virtual void getValues() override;
	virtual void getScript() override;
	virtual void getBasicValues() override;
};