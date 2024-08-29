#pragma once
#include <raylib.h>
#include "char_anim_controller.h"

using namespace std;

class Game;
class Unit;
class Animations;

class CharacterBattleAnimationController : public CharacterAnimationController
{
public:
	virtual void setup(Game* game, Unit* unit, Json::Value controller_script) override;
	virtual void update() override;
	virtual void draw() override;

	//Setters
	void DisableWaitSwitches();
	void EnableWaitSwitches();

private:
	//Update
	void AnimationSwitch();

	//Getters
	virtual void getValues() override;
	virtual void getScript() override;
	virtual void getBasicValues() override;

	//Logic
	bool bWaitSwitchOn = true;
};