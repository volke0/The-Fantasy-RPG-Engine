#include "char_map_anim_controller.h"
#include "unit.h"
#include "npc.h"
#include "game.h"

void CharacterMapAnimationController::setup(Game* game, Unit* unit, Json::Value controller_script)
{
	this->unit = unit;
	basicSetup(game, controller_script);
	getValues();
	getDatabases();
	getScript();
	setupSprites();
	createAnimations();
}

void CharacterMapAnimationController::getBasicValues()
{
	settings = controller_script["Settings"]["Map"];
	graphics_db_name = settings["graphics"].asString();
	sprite_db_name = settings["sprite_settings"].asString();
}

void CharacterMapAnimationController::getValues()
{
	getBasicValues();
	if (settings.isMember("front_stand"))
	{
		anim_names["front_stand"] = settings["front_stand"].asString();
	}
	if (settings.isMember("back_stand"))
	{
		anim_names["back_stand"] = settings["back_stand"].asString();
	}
	if (settings.isMember("right_stand"))
	{
		anim_names["right_stand"] = settings["right_stand"].asString();
	}
	if (settings.isMember("left_stand"))
	{
		anim_names["left_stand"] = settings["left_stand"].asString();
	}
	if (settings.isMember("front_walk"))
	{
		anim_names["front_walk"] = settings["front_walk"].asString();
	}
	if (settings.isMember("back_walk"))
	{
		anim_names["back_walk"] = settings["back_walk"].asString();
	}
	if (settings.isMember("right_walk"))
	{
		anim_names["right_walk"] = settings["right_walk"].asString();
	}
	if (settings.isMember("left_walk"))
	{
		anim_names["left_walk"] = settings["left_walk"].asString();
	}
	if (settings.isMember("emote"))
	{
		for (int i = 0; i < (int)settings["emote"].size(); i++)
		{
			string name = "emote_" + settings["emote"][i].asString();
			anim_names[name] = settings["emote"][i].asString();
		}
	}
}

void CharacterMapAnimationController::getScript()
{
	if (controller_script.isMember("Animations"))
	{
		animation_script = controller_script["Animations"]["Map"];
	}
}



void CharacterMapAnimationController::update()
{
	AnimationSwitch();
}

void CharacterMapAnimationController::AnimationSwitch()
{
	/*cout << unit->currentlyMoving << endl;*/
	if (!static_cast<NPC*>(unit)->anim_lock)
	{
		if (!unit->currentlyMoving)
		{
			if (unit->last_move == "down")
			{
				changeAnimation("front_stand");
			}
			if (unit->last_move == "up")
			{
				changeAnimation("back_stand");
			}
			if (unit->last_move == "right")
			{
				changeAnimation("right_stand");
			}
			if (unit->last_move == "left")
			{
				changeAnimation("left_stand");
			}

			//Reset all moving animations while not moving
			resetAnimation("front_walk");
			resetAnimation("back_walk");
			resetAnimation("right_walk");
			resetAnimation("left_walk");
		}
		else if (unit->currentlyMoving)
		{
			if (unit->last_move == "down")
			{
				changeAnimation("front_walk");
			}
			if (unit->last_move == "up")
			{
				changeAnimation("back_walk");
			}
			if (unit->last_move == "right")
			{
				changeAnimation("right_walk");
			}
			if (unit->last_move == "left")
			{
				changeAnimation("left_walk");
			}
		}
	}
}


void CharacterMapAnimationController::draw()
{
	if (animations.size() > 0)
	{
		CharacterAnimationController::draw();
	}
	else
	{
		DrawRectanglePro(unit->map_rect, { 0, 0 }, 0.0f, unit->personal_color);
	}	
}








