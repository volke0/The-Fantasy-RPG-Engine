#include "anim_controller.h"
#include "sprites.h"
#include "animation.h"
#include "vfx_controller.h"
#include "game.h"

void AnimationController::basicSetup(Game* game, Json::Value controller_script)
{
	this->game = game;
	this->controller_script = controller_script;
}

void AnimationController::changeAnimation(string anim_name)
{
	/*if (anim_names[anim_name] != current_animation)
	{
		string name = anim_names[anim_name];
		if (animations[name]->done == true)
		{
			animations[name]->reset();
		}
	}*/
	this->current_animation = anim_names[anim_name];
}

void AnimationController::resetAnimation(string anim_name)
{
	for (auto& [read_name, custom_name] : anim_names)
	{
		if (anim_name == read_name)
		{
			if (custom_name != "")
			{
				animations[anim_names[anim_name]]->reset();
			}
		}
	}
}

void AnimationController::resetCurrentAnimation()
{
	if (current_animation != "")
	{
		animations[current_animation]->reset();
	}
}

void AnimationController::FlipAnimation(bool flip)
{
	if (flip)
	{
		animations[current_animation]->FlipAnimation();
	}
	else if (!flip)
	{
		animations[current_animation]->UnFlipAnimation();
	}
}

void AnimationController::resetAllAnimations()
{
	for (auto& [name, animation] : animations)
	{
		animation->reset();
	}
}

void AnimationController::setVFXController(VFXController* vfx_controller)
{
	for (auto& [key, anim] : animations)
	{
		anim->setVFXController(vfx_controller);
	}
}

void AnimationController::getBasicValues()
{
	settings = controller_script["Settings"][0];
	graphics_db_name = settings["graphics"].asString();
	sprite_db_name = settings["sprite_settings"].asString();
}

void AnimationController::getValues()
{
	
}

void AnimationController::getDatabases()
{
	
}

void AnimationController::setupSprites()
{
	sprite_sheet.setup(sprite_script, sprites, game, graphics_db_name);
}
