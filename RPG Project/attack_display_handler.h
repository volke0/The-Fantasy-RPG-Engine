#pragma once
#include <raylib.h>
#include "display_handler.h"

using namespace std;

class Game;
class BattleAnimationManager;

class AttackDisplayHandler : public DisplayHandler
{
public:
	void setup(Game* game, BattleAnimationManager* btl_animation_manager);
	void update() override;
private:
	void displayAttack();
	void attackTargetAnimations();
	void attackUserAnimations();

	//Setters
	void setBattleVars();
	void SetCurrentTarget();

	//Logic
	bool wasIdle = false;
	bool bPlayAttack = false;
	bool bAttackModeOn = false;
};