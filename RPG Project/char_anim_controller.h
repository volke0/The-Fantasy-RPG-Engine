#pragma once
#include <raylib.h>
#include <unordered_map>
#include <vector>
#include "char_anim.h"
#include "anim_mask.h"
#include "anim_controller.h"

using namespace std;

class Game;
class Unit;
class Animations;

class CharacterAnimationController : public AnimationController
{
public:
	virtual void setup(Game* game, Unit* unit, Json::Value controller_script);
	virtual void update();
	virtual void draw();

	//Getters
	virtual void getValues();
	Animations* getCurrentAnimation();
	bool isAnimationDone(string anim_name);
	Animations* isAnimation(string anim_name);

	//Setters
	void setMaskAnimation(string mask_name);
	void setTarget(Unit* target);
	void setMagicDone();
	void resetAllAnimations() override;
	void turnMaskOff();
	void deleteAllAnimations();

	//Logic
	bool start = true;

	//Setup
	void createAnimations();
	virtual void getDatabases() override;
	virtual void getScript();

	//Logic
	bool mask_on = false;

	//Mask
	MaskAnimation mask_anim;
};