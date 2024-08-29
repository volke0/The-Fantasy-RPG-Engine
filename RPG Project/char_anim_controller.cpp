#include "char_anim_controller.h"
#include <iostream>
#include <cstdio>
#include "game.h"
#include "item.h"
#include "weapon.h"
#include "animation.h"
#include "unit.h"

void CharacterAnimationController::setup(Game* game, Unit* unit, Json::Value controller_script)
{
	this->unit = unit;
	basicSetup(game, controller_script);
	getValues();
	getDatabases();
	setupSprites();
	createAnimations();
}

void CharacterAnimationController::getDatabases()
{
	if (this->graphics_db_name.size() > 0)
	{
		sprites = this->game->GFX.char_sprites[this->graphics_db_name];
	}
	if (sprite_db_name.size() > 0)
	{
		string path = "./databases/sprite_settings/" + sprite_db_name + ".json";
		ifstream file(path);
		reader.parse(file, sprite_script);
	}
}

void CharacterAnimationController::getScript()
{
	if (controller_script.isMember("Animations"))
	{
		animation_script = controller_script["Animations"][0];
	}
}

void CharacterAnimationController::createAnimations()
{
	for (Json::Value::const_iterator it = animation_script.begin(); it != animation_script.end(); it++)
	{
		Json::Value temp_script = animation_script[it.key().asString()];
		shared_ptr<CharacterAnimation> char_anim = make_shared<CharacterAnimation>();
		char_anim->setup(game, unit, temp_script, &sprite_sheet);
		animations[it.key().asString()] = char_anim;
	}
}

void CharacterAnimationController::getValues()
{
	getBasicValues();
	
}



void CharacterAnimationController::update()
{
	
}

void CharacterAnimationController::draw()
{
	if (start == true)
	{
		if (current_animation != "")
		{
			animations[current_animation]->play();
		}
		if (mask_on == true)
		{
			mask_anim.play();
		}
	}
}


//Getters
Animations* CharacterAnimationController::getCurrentAnimation()
{
	return animations[current_animation].get();
}

bool CharacterAnimationController::isAnimationDone(string anim_name)
{
	if (anim_names.size() > 0)
	{
		string name = anim_names[anim_name];
		return animations[name]->done;
	}
	return true;
}

Animations* CharacterAnimationController::isAnimation(string anim_name)
{
	for (auto& [cname, anim] : animations)
	{
		string name = anim_names[anim_name];
		if (cname == name)
		{
			return animations[name].get();
		}
	}
	cout << "You called a non-existant animation!" << endl;
	return nullptr;
}


//Setters
void CharacterAnimationController::setMaskAnimation(string mask_name)
{
	Json::Value mask = game->mask_database.mask_anims[mask_name]["Animation"];
	mask_anim.setup(game, unit, mask);
	mask_on = true;
}

void CharacterAnimationController::setTarget(Unit* target)
{
	for (auto& [name, animation] : animations)
	{
		animation->setTarget(target);
	}
}

void CharacterAnimationController::setMagicDone()
{
	getCurrentAnimation()->mag_done = true;
}

void CharacterAnimationController::resetAllAnimations()
{
	for (auto& [name, animation] : animations)
	{
		animation->reset();
	}
	if (unit->HasWeapon())
	{
		if (unit->GetWeapon() != nullptr)
		{
			unit->GetWeapon()->anim.reset();
		}
		if (unit->GetWeapon(true) != nullptr)
		{
			unit->GetWeapon(true)->anim.reset();
		}
	}
}

void CharacterAnimationController::turnMaskOff()
{
	mask_on = false;
}

void CharacterAnimationController::deleteAllAnimations()
{
	for (auto& [name, animation] : animations)
	{
		animation->unloadSprite();
	}
	if (unit->HasWeapon())
	{
		if (unit->GetWeapon() != nullptr)
		{
			unit->GetWeapon()->anim.unloadSprite();
		}
		if (unit->GetWeapon(true) != nullptr)
		{
			unit->GetWeapon(true)->anim.unloadSprite();
		}
	}
}


