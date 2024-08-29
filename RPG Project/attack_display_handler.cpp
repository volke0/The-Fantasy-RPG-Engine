#include "attack_display_handler.h"
#include "game.h"
#include "battle.h"
#include "btl_animation_manager.h"
#include "unit.h"
#include "animation.h"
#include "text_anim.h"
#include "char_anim_controller.h"
#include "weapon.h"

void AttackDisplayHandler::setup(Game* game, BattleAnimationManager* btl_animation_manager)
{
	basicSetup(game, btl_animation_manager);
}


void AttackDisplayHandler::update()
{
	setBattleVars();
	displayAttack();
}

void AttackDisplayHandler::setBattleVars()
{
	btl_animation_manager->current_target = btl_animation_manager->targets[btl_animation_manager->target_index];
	btl_animation_manager->current_weapon = btl_animation_manager->current_weapon;
	current_weapon = btl_animation_manager->current_weapon;
}

void AttackDisplayHandler::SetCurrentTarget()
{
	btl_animation_manager->checkIfTargetDead();
	btl_animation_manager->checkIfAllTargetsDead();
	current_target = btl_animation_manager->current_target;

	//TODO: Add multi-unit targeting later...
	user->setTarget(btl_animation_manager->current_target);
	user->btl_logic_controller.SetTarget(current_target);
}

void AttackDisplayHandler::displayAttack()
{
	SetCurrentTarget();
	if (btl_animation_manager->done == false)
	{
		if (bAttackModeOn == false)
		{
			user->btl_con_controller.eraseCondition("idle");
			user->btl_logic_controller.SetAttackMode();
			bAttackModeOn = true;
			current_target->bBeingDamaged = true;
		}

		attackTargetAnimations();
		attackUserAnimations();
	}
}

void AttackDisplayHandler::attackTargetAnimations()
{
	if (user->btl_logic_controller.play_atk == true)
	{
		bool bHit = btl_animation_manager->battle->hit[btl_animation_manager->target_index];

		if (bHit) current_target->btl_logic_controller.SetMeleeHurtMode();
	}
}

void AttackDisplayHandler::attackUserAnimations()
{
	if (user->GetEquipment(current_weapon)->anim.done == true)
	{
		user->anim_controller.getCurrentAnimation()->weapon_done = true;
	}

	if (user->btl_logic_controller.atk_done)
	{
		btl_animation_manager->play_text_anim = true;
	}

	if (btl_animation_manager->dmg_text[btl_animation_manager->target_index]->done == true)
	{
		btl_animation_manager->play_text_anim = false;
		btl_animation_manager->done = true;
	}

	if (btl_animation_manager->done == true)
	{
		btl_animation_manager->resetCurrentTarget();
		btl_animation_manager->resetUser();
		battle->btl_calculator.applyDamage(user, btl_animation_manager->targets, btl_animation_manager->target_index);
	}
}

