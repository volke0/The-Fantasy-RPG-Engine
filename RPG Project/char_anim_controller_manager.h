#pragma once
#include <raylib.h>
#include "controller.h"
#include "char_btl_anim_controller.h"
#include "char_map_anim_controller.h"

using namespace std;

class Game;
class Unit;
class NPC;
class VFXController;

class CharacterAnimationControllerManager : public Controller
{
public:
	void setup(Game* game, Unit* unit, Json::Value controller_script);
	virtual void update() override;
	virtual void draw();

	//Getters
	Animations* getCurrentAnimation();
	bool isAnimationDone(string anim_name);
	Animations* isAnimation(string anim_name);
	Json::Value getSpriteScript();
	unordered_map<string, string> getSprites();

	//Setters
	void turnBattleControllerOn();
	void turnBattleControllerOff();
	void EnableBattleWaitSwitch();
	void DisableBattleWaitSwitch();
	void changeAnimation(string anim_name);
	void resetAnimation(string anim_name);
	void resetCurrentAnimation();
	void FlipAnimation(bool flip);
	void setVFXController(VFXController* vfx_controller);
	void setMaskAnimation(string mask_name);
	void setTarget(Unit* target);
	void setMagicDone();
	void resetAllAnimations();
	void turnMaskOff();
	void deleteAllAnimations();
private:
	//Controllers
	CharacterBattleAnimationController battle_anim_controller;
	CharacterMapAnimationController map_anim_controller;

	//Data
	Unit* unit;
};