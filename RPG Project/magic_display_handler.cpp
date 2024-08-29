#include "magic_display_handler.h"
#include "btl_animation_manager.h"
#include "game.h"
#include "battle.h"
#include "unit.h"
#include "animation.h"
#include "char_anim_controller.h"
#include "text_anim.h"
#include "magic.h"

void MagicDisplayHandler::setup(Game* game, BattleAnimationManager* btl_animation_manager)
{
	basicSetup(game, btl_animation_manager);
}


void MagicDisplayHandler::draw()
{
	if (play_mag == true)
	{
		current_magic->anim.play();
	}
}


void MagicDisplayHandler::update()
{
	setBattleVars();
	displayMagic();
}



void MagicDisplayHandler::setBattleVars()
{
	if (btl_animation_manager->targets.size() > 0)
	{
		btl_animation_manager->current_target = btl_animation_manager->targets[btl_animation_manager->target_index];
	}
}

void MagicDisplayHandler::displayMagic()
{
	setCurrentTarget();
	setCurrentMagic();
	if (btl_animation_manager->done == false)
	{
		if (magic_mode_on == false)
		{
			user->btl_con_controller.eraseCondition("idle");
			user->btl_logic_controller.setMagicMode();
			current_target->bBeingDamaged = true;
			magic_mode_on = true;
		}
		if (user->btl_logic_controller.play_mag == true)
		{
			play_mag = true;
		}
		else
		{
			play_mag = false;
		}

		if (current_magic->anim.done == true)
		{
			btl_animation_manager->play_text_anim = true;
		}

		if (btl_animation_manager->dmg_text[btl_animation_manager->target_index]->done == true)
		{
			btl_animation_manager->play_text_anim = false;
			if (applied_dmg == false)
			{
				for (int index = 0; index < (int)btl_animation_manager->targets.size(); index++)
				{
					battle->mag_calculator.applyDamage(user, btl_animation_manager->targets, index);
				}
				applied_dmg = true;
			}
			if (mag_index >= (int)magic_list.size() - 1)
			{
				user->btl_logic_controller.setMagicDone();
			}
		}

		if (user->btl_logic_controller.mag_done == true)
		{
			btl_animation_manager->done = true;
			btl_animation_manager->resetCurrentTarget();
		}
	}
}


void MagicDisplayHandler::setMagicList(vector<shared_ptr<Magic>> magic_list)
{
	this->magic_list = magic_list;
	current_magic = magic_list[mag_index];
}

void MagicDisplayHandler::setCurrentTarget()
{
	bool bChangeTarget = btl_animation_manager->checkIfTargetDead(battle->mag_calculator.magic->can_revive);

	btl_animation_manager->checkIfAllTargetsDead();
	current_target = btl_animation_manager->current_target;

	if (bChangeTarget)
	{
		if (!current_magic) return;
		if ((int)btl_animation_manager->targets.size() == 0) return;
		current_magic->setTarget(current_target);
	}
}

void MagicDisplayHandler::setCurrentMagic()
{
	if (current_magic->anim.done == true)
	{
		if (mag_index + 1 <= (int)magic_list.size() - 1)
		{
			mag_index++;
			applied_dmg = false;
			play_mag = true;
		}
	}
	current_magic = magic_list[mag_index];
	battle->current_magic = current_magic;
}

