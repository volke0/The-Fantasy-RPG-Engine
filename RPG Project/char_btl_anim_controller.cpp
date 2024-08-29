#include "char_btl_anim_controller.h"
#include "animation.h"
#include "unit.h"
#include "game.h"

void CharacterBattleAnimationController::setup(Game* game, Unit* unit, Json::Value controller_script)
{
	this->unit = unit;
	basicSetup(game, controller_script);
	getValues();
	getDatabases();
	getScript();
	setupSprites();
	createAnimations();
	current_animation = anim_names["battle_stand"];
}

void CharacterBattleAnimationController::getBasicValues()
{
	settings = controller_script["Settings"]["Battle"];
	graphics_db_name = settings["graphics"].asString();
	sprite_db_name = settings["sprite_settings"].asString();
}

void CharacterBattleAnimationController::getValues()
{
	getBasicValues();
	if (settings.isMember("battle_stand"))
	{
		anim_names["battle_stand"] = settings["battle_stand"].asString();
	}
	if (settings.isMember("melee_attack"))
	{
		anim_names["melee_attack"] = settings["melee_attack"].asString();
	}
	if (settings.isMember("magic_attack"))
	{
		anim_names["magic_attack"] = settings["magic_attack"].asString();
	}
	if (settings.isMember("magic_charge"))
	{
		anim_names["magic_charge"] = settings["magic_charge"].asString();
	}
	if (settings.isMember("low_hp_stand"))
	{
		anim_names["low_hp_stand"] = settings["low_hp_stand"].asString();
	}
	if (settings.isMember("battle_wait"))
	{
		anim_names["battle_wait"] = settings["battle_wait"].asString();
	}
	if (settings.isMember("melee_hurt"))
	{
		anim_names["melee_hurt"] = settings["melee_hurt"].asString();
	}
	if (settings.isMember("entrance"))
	{
		anim_names["entrance"] = settings["entrance"].asString();
	}
	if (settings.isMember("dead"))
	{
		anim_names["dead"] = settings["dead"].asString();
	}
	if (settings.isMember("item_use"))
	{
		anim_names["item_use"] = settings["item_use"].asString();
	}
	if (settings.isMember("item_wait"))
	{
		anim_names["item_wait"] = settings["item_wait"].asString();
	}
}

void CharacterBattleAnimationController::getScript()
{
	if (controller_script.isMember("Animations"))
	{
		animation_script = controller_script["Animations"]["Battle"];
	}
}



void CharacterBattleAnimationController::update()
{
	AnimationSwitch();
}

void CharacterBattleAnimationController::AnimationSwitch()
{
	if (unit->entrance == true)
	{
		changeAnimation("entrance");
	}
	if (unit->btl_con_controller.findCondition("melee_attack"))
	{
		changeAnimation("melee_attack");
	}
	if (unit->btl_con_controller.findCondition("using_magic"))
	{
		changeAnimation("magic_attack");
	}
	if (unit->btl_con_controller.findCondition("item_use"))
	{
		changeAnimation("item_use");
	}

	if (bWaitSwitchOn)
	{
		if (unit->btl_con_controller.findCondition("atk_wait"))
		{
			changeAnimation("battle_wait");
		}
		if (unit->btl_con_controller.findCondition("item_wait"))
		{
			changeAnimation("item_wait");
		}
		if (unit->btl_con_controller.findCondition("mag_wait"))
		{
			changeAnimation("magic_charge");
		}
	}
	
	if (unit->btl_con_controller.findCondition("idle"))
	{
		changeAnimation("battle_stand");
	}
	if (unit->btl_con_controller.findCondition("melee_hurt"))
	{
		changeAnimation("melee_hurt");
	}
	if (unit->dead == true)
	{
		changeAnimation("dead");
	}
}


void CharacterBattleAnimationController::draw()
{
	CharacterAnimationController::draw();
}

void CharacterBattleAnimationController::DisableWaitSwitches()
{
	bWaitSwitchOn = false;
}

void CharacterBattleAnimationController::EnableWaitSwitches()
{
	bWaitSwitchOn = true;
}


