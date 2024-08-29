#include "unit_battle_logic_controller.h"
#include "game.h"
#include "unit.h"
#include "equipment_base.h"

void UnitBattleLogicController::setup(Game* game, Unit* unit)
{
	this->game = game;
	this->unit = unit;
}



void UnitBattleLogicController::update()
{
	if (unit->state_controller.findState("in_battle"))
	{
		updateLogic();
	}
}

void UnitBattleLogicController::updateLogic()
{
	if (unit->state_controller.findState("magic_mode"))
	{
		updateMagic();
	}
	if (unit->state_controller.findState("item_mode"))
	{
		updateItem();
	}
	if (unit->state_controller.findState("attack_mode"))
	{
		UpdateAttack();
	}
	if (unit->state_controller.findState("hurt_mode"))
	{
		UpdateMeleeHurt();
	}
}


void UnitBattleLogicController::updateMagic()
{
	if (mag_done == false)
	{
		if (unit->anim_controller.isAnimation("magic_attack")->call_magic == true)
		{
			play_mag = true;
		}
		else
		{
			play_mag = false;
		}

		if (unit->anim_controller.isAnimation("magic_attack")->done == true)
		{
			mag_done = true;
		}
	}
	if (mag_done == true)
	{
		endMagicMode();
	}
}

void UnitBattleLogicController::updateItem()
{
	if (!item_done)
	{
		if (unit->anim_controller.isAnimation("item_use")->call_magic == true)
		{
			play_item = true;
		}
		else
		{
			play_item = false;
		}

		if (unit->anim_controller.isAnimation("item_use")->done == true)
		{
			item_done = true;
		}
	}
	if (item_done == true)
	{
		endItemMode();
	}
}

void UnitBattleLogicController::UpdateAttack()
{
	if (!atk_done)
	{
		if (unit->GetEquipment(current_weapon)->anim.call_hit || unit->anim_controller.getCurrentAnimation()->call_hit)
		{
			play_atk = true;
		}
		else
		{
			play_atk = false;
		}

		if (unit->GetEquipment(current_weapon)->anim.done)
		{
			unit->anim_controller.getCurrentAnimation()->weapon_done = true;
		}
		if (unit->anim_controller.isAnimationDone("melee_attack") == true)
		{
			atk_done = true;
		}
	}
	if (atk_done == true)
	{
		EndAttackMode();
	}
}

void UnitBattleLogicController::UpdateMeleeHurt()
{
	if (!hurt_done)
	{
		if (unit->anim_controller.isAnimationDone("melee_hurt"))
		{
			EndHurtMode();
		}
	}
}


void UnitBattleLogicController::setMagicMode()
{
	unit->btl_con_controller.eraseCondition("mag_wait");
	unit->btl_con_controller.setCondition("using_magic");
	unit->state_controller.pushState("magic_mode");
	unit->anim_controller.DisableBattleWaitSwitch();
	mag_done = false;
}

void UnitBattleLogicController::setItemMode()
{
	unit->btl_con_controller.eraseCondition("item_wait");
	unit->btl_con_controller.setCondition("item_use");
	unit->state_controller.pushState("item_mode");
	unit->anim_controller.DisableBattleWaitSwitch();
	item_done = false;
}

void UnitBattleLogicController::SetAttackMode()
{
	unit->btl_con_controller.eraseCondition("atk_wait");
	unit->btl_con_controller.setCondition("melee_attack");
	unit->state_controller.pushState("attack_mode");
	unit->anim_controller.DisableBattleWaitSwitch();
	atk_done = false;
}

void UnitBattleLogicController::SetMeleeHurtMode()
{
	if (unit->btl_con_controller.findCondition("idle"))
	{
		unit->btl_con_controller.eraseCondition("idle");
		unit->bWasIdle = true;
	}
	unit->btl_con_controller.setCondition("melee_hurt");
	unit->state_controller.pushState("hurt_mode");
	hurt_done = false;
}

void UnitBattleLogicController::setMagicDone()
{
	unit->anim_controller.setMagicDone();
}


void UnitBattleLogicController::endMagicMode()
{
	unit->btl_con_controller.eraseCondition("mag_wait");
	unit->btl_con_controller.eraseCondition("using_magic");
	unit->btl_con_controller.setCondition("idle");
	unit->state_controller.eraseState("magic_mode");
	unit->anim_controller.EnableBattleWaitSwitch();
	unit->anim_controller.resetAllAnimations();
}

void UnitBattleLogicController::endItemMode()
{
	unit->btl_con_controller.eraseCondition("item_wait");
	unit->btl_con_controller.eraseCondition("item_use");
	unit->btl_con_controller.setCondition("idle");
	unit->state_controller.eraseState("item_mode");
	unit->anim_controller.EnableBattleWaitSwitch();
	unit->anim_controller.resetAllAnimations();
}

void UnitBattleLogicController::EndAttackMode()
{
	unit->btl_con_controller.eraseCondition("atk_wait");
	unit->btl_con_controller.eraseCondition("melee_attack");
	unit->btl_con_controller.setCondition("idle");
	unit->state_controller.eraseState("attack_mode");
	unit->anim_controller.EnableBattleWaitSwitch();
	unit->anim_controller.resetAllAnimations();
}

void UnitBattleLogicController::EndHurtMode()
{
	hurt_done = true;
	if (unit->bWasIdle)
	{
		unit->btl_con_controller.setCondition("idle");
		unit->bWasIdle = false;
	}
	unit->btl_con_controller.eraseCondition("melee_hurt");
	unit->state_controller.eraseState("hurt_mode");
}

void UnitBattleLogicController::SetTarget(Unit* target)
{
	this->target = target;
}

void UnitBattleLogicController::SetCurrentWeapon(string current_weapon)
{
	this->current_weapon = current_weapon;
}

