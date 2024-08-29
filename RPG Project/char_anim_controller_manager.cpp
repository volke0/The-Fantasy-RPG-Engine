#include "char_anim_controller_manager.h"
#include "unit.h"
#include "game.h"

void CharacterAnimationControllerManager::setup(Game* game, Unit* unit, Json::Value controller_script)
{
	this->game = game;
	this->unit = unit;
	battle_anim_controller.setup(game, unit, controller_script);
	map_anim_controller.setup(game, unit, controller_script);
}


void CharacterAnimationControllerManager::update()
{
	if (unit->map_mode)
	{
		map_anim_controller.update();
	}
	else if (unit->battle_mode)
	{
		battle_anim_controller.update();
	}
}

void CharacterAnimationControllerManager::draw()
{
	if (unit->map_mode)
	{
		map_anim_controller.draw();
	}
	else if (unit->battle_mode)
	{
		battle_anim_controller.draw();
	}
}


//Getters
Animations* CharacterAnimationControllerManager::getCurrentAnimation()
{
	if (unit->map_mode)
	{
		return map_anim_controller.getCurrentAnimation();
	}
	return battle_anim_controller.getCurrentAnimation();
}

bool CharacterAnimationControllerManager::isAnimationDone(string anim_name)
{
	if (unit->map_mode)
	{
		return map_anim_controller.isAnimationDone(anim_name);
	}
	return battle_anim_controller.isAnimationDone(anim_name);
}

Animations* CharacterAnimationControllerManager::isAnimation(string anim_name)
{
	if (unit->map_mode)
	{
		return map_anim_controller.isAnimation(anim_name);
	}
	return battle_anim_controller.isAnimation(anim_name);
}

Json::Value CharacterAnimationControllerManager::getSpriteScript()
{
	if (unit->map_mode)
	{
		return map_anim_controller.sprite_script;
	}
	return battle_anim_controller.sprite_script;
}

unordered_map<string, string> CharacterAnimationControllerManager::getSprites()
{
	if (unit->map_mode)
	{
		return map_anim_controller.sprites;
	}
	return battle_anim_controller.sprites;
}


//Setters
void CharacterAnimationControllerManager::turnBattleControllerOn()
{
	battle_anim_controller.start = true;
}

void CharacterAnimationControllerManager::turnBattleControllerOff()
{
	battle_anim_controller.start = false;
}

void CharacterAnimationControllerManager::EnableBattleWaitSwitch()
{
	battle_anim_controller.EnableWaitSwitches();
}

void CharacterAnimationControllerManager::DisableBattleWaitSwitch()
{
	battle_anim_controller.DisableWaitSwitches();
}

void CharacterAnimationControllerManager::changeAnimation(string anim_name)
{
	if (unit->map_mode)
	{
		map_anim_controller.changeAnimation(anim_name);
	}
	else if (unit->battle_mode)
	{
		battle_anim_controller.changeAnimation(anim_name);
	}
}

void CharacterAnimationControllerManager::resetAnimation(string anim_name)
{
	if (unit->map_mode)
	{
		map_anim_controller.resetAnimation(anim_name);
	}
	else if (unit->battle_mode)
	{
		battle_anim_controller.resetAnimation(anim_name);
	}
}

void CharacterAnimationControllerManager::resetCurrentAnimation()
{
	map_anim_controller.resetCurrentAnimation();
	battle_anim_controller.resetCurrentAnimation();
}

void CharacterAnimationControllerManager::FlipAnimation(bool flip)
{
	if (unit->map_mode)
	{
		map_anim_controller.FlipAnimation(flip);
	}
	else if (unit->battle_mode)
	{
		battle_anim_controller.FlipAnimation(flip);
	}
}

void CharacterAnimationControllerManager::setVFXController(VFXController* vfx_controller)
{
	map_anim_controller.setVFXController(vfx_controller);
	battle_anim_controller.setVFXController(vfx_controller);
}

void CharacterAnimationControllerManager::setMaskAnimation(string mask_name)
{
	if (unit->map_mode)
	{
		map_anim_controller.setMaskAnimation(mask_name);
	}
	else if (unit->battle_mode)
	{
		battle_anim_controller.setMaskAnimation(mask_name);
	}
}

void CharacterAnimationControllerManager::setTarget(Unit* target)
{
	if (unit->map_mode)
	{
		map_anim_controller.setTarget(target);
	}
	else if (unit->battle_mode)
	{
		battle_anim_controller.setTarget(target);
	}
}

void CharacterAnimationControllerManager::setMagicDone()
{
	if (unit->map_mode)
	{
		map_anim_controller.setMagicDone();
	}
	else if (unit->battle_mode)
	{
		battle_anim_controller.setMagicDone();
	}
}

void CharacterAnimationControllerManager::resetAllAnimations()
{
	if (unit->map_mode)
	{
		map_anim_controller.resetAllAnimations();
	}
	else if (unit->battle_mode)
	{
		battle_anim_controller.resetAllAnimations();
	}
}
	

void CharacterAnimationControllerManager::turnMaskOff()
{
	if (unit->map_mode)
	{
		map_anim_controller.turnMaskOff();
	}
	else if (unit->battle_mode)
	{
		battle_anim_controller.turnMaskOff();
	}
}

void CharacterAnimationControllerManager::deleteAllAnimations()
{
	if (unit->map_mode)
	{
		map_anim_controller.deleteAllAnimations();
	}
	else if (unit->battle_mode)
	{
		battle_anim_controller.deleteAllAnimations();
	}
}
