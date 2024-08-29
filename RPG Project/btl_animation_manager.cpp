#include "btl_animation_manager.h"
#include "game.h"
#include "unit.h"
#include "battle.h"
#include "animation.h"
#include "char_anim_controller.h"
#include "text_anim.h"
#include "weapon.h"
#include "char_anim.h"
#include "consumable.h"
#include "magic.h"

void BattleAnimationManager::setup(Game* game, Battle* battle, Unit* user, vector<NPC*> targets)
{
	this->game = game;
	this->battle = battle;
	this->user = user;
	this->targets = targets;
	this->type = "animation";
	checkForWeapon();
	setupAnimations();
	atk_display.setup(game, this);
	mag_display.setup(game, this);
	item_display.setup(game, this);
}

void BattleAnimationManager::checkForWeapon()
{
	if (user->GetWeapon() != nullptr)
	{
		weapons.push_back(user->GetWeapon()->slot);
		if (user->GetWeapon(true) != nullptr)
		{
			weapons.push_back(user->GetWeapon(true)->slot);
		}
		current_weapon = weapons[weapon_index];
		user->weapon_focus = current_weapon;
		user->btl_logic_controller.SetCurrentWeapon(current_weapon);
	}
}

void BattleAnimationManager::setupAnimations()
{
	int index = 0;
	for (int i = 0; i < (int)battle->hit.size(); i++)
	{
		bool hit_value = battle->hit[i];

		shared_ptr<TextAnimation> temp_dmg_text = make_shared<TextAnimation>();
		auto& script = game->vfx_anim_database.vfx_anims["text_bounce"];
		Color text_color = { 255, 255, 255, 255 };

		if (hit_value == true)
		{
			if (battle->mode == "Magic" || battle->mode == "Item")
			{
				if (heal_list.find(i) != heal_list.end())
				{
					string heal_type = heal_list[i];

					if (heal_type == "hp") text_color = LightGreen;
					if (heal_type == "mp") text_color = LightBlue;
				}
			}
			int dmg_num = (int)battle->dmg[index];
			temp_dmg_text->setText(to_string(dmg_num), text_color);
			index++;
		}
		else
		{
			temp_dmg_text->setText("MISS");
		}
		temp_dmg_text->setTarget(targets[i]);
		temp_dmg_text->setup(game, script);
		dmg_text.push_back(temp_dmg_text);
	}
}

void BattleAnimationManager::MakeHealList(vector<shared_ptr<class Consumable>> item_list)
{
	int index = 0;
	for (auto& item : item_list)
	{
		if (item->can_heal)
		{
			heal_list[index] = item->heal_type;
		}
		index++;
	}
}

void BattleAnimationManager::MakeHealList(vector<shared_ptr<Magic>> magic_list)
{
	int index = 0;
	for (auto& magic : magic_list)
	{
		if (magic->can_heal)
		{
			heal_list[index] = magic->heal_type;
		}
		index++;
	}
}


void BattleAnimationManager::drawBattle()
{
	if (play_text_anim == true)
	{
		for (auto& dmg_anim : dmg_text)
		{
			dmg_anim->play();
		}
		//dmg_text[target_index]->play();
	}
	mag_display.draw();
	item_display.draw();
}


void BattleAnimationManager::updateBattle()
{
	if (battle->mode == "Attack")
	{
		atk_display.update();
	}
	if (battle->mode == "Magic")
	{
		mag_display.update();
	}
	if (battle->mode == "Item")
	{
		item_display.update();
	}
}



void BattleAnimationManager::setMagicList(vector<shared_ptr<Magic>> magic_list)
{
	MakeHealList(magic_list);
	mag_display.setMagicList(magic_list);
}

void BattleAnimationManager::setItemList(vector<shared_ptr<class Consumable>> item_list, int item_index)
{
	MakeHealList(item_list);
	item_display.setItemList(item_list, item_index);
}

void BattleAnimationManager::EndBattle()
{
	done = true;
	user->btl_logic_controller.endMagicMode();
	user->btl_logic_controller.endItemMode();
	user->btl_logic_controller.EndAttackMode();
}



bool BattleAnimationManager::checkIfTargetDead(bool ignoreChanging)
{
	if (current_target->dead == true && current_target->bBeingDamaged == false && !ignoreChanging)
	{
		//Make a copy of targets and then check the unit name in the target list and delete it if it is dead.  Also store the dead unit name in a list
		int index = 0;
		vector<string> dead_target_names = {};
		vector<NPC*> targets_copy = targets;
		for (auto& unit : targets)
		{
			if (unit->dead)
			{
				targets_copy.erase(targets_copy.begin() + index);
				dead_target_names.push_back(unit->combat_name);
			}
			index++;
		}

		//Now update the targets list
		targets_copy.shrink_to_fit();
		targets = targets_copy;

		//Reset index to 0
		index = 0;

		//If all of the valid targets are dead, now select from the other targets in the battle
		if ((int)targets.size() == 0)
		{
			//First clear all dead units from the battle->other_targets list
			vector<NPC*> other_targets_copy = battle->other_targets;
			for (auto& unit : battle->other_targets)
			{
				if (unit->dead == true)
				{
					other_targets_copy.erase(other_targets_copy.begin() + index);
				}
				index++;
			}
			other_targets_copy.shrink_to_fit();
			battle->other_targets = other_targets_copy;
			targets = battle->other_targets;
		}
		//To prevent accessing none from nothing in a vector
		if ((int)targets.size() == 0) return true;
		//Set the target
		target_index = 0;
		current_target = targets[target_index];

		//Temporary fix for the dmg_text being misplaced when switching targets. Will refactor later... (DOESN'T WORK!)
		auto& script = game->vfx_anim_database.vfx_anims["text_bounce"];
		dmg_text[target_index]->ChangeTarget(current_target);

		return true;
	}
	return false;
}

void BattleAnimationManager::checkIfAllTargetsDead()
{
	if (targets.size() == 0)
	{
		if (user->type == "playable")
		{
			wrap_up = true;
		}
		else
		{
			EndBattle();
		}
	}
	else
	{
		current_target = targets[target_index];
	}
}


void BattleAnimationManager::resetUser()
{
	user->anim_controller.resetAllAnimations();
}

void BattleAnimationManager::resetCurrentTarget()
{
	if (current_target != nullptr)
	{
		current_target->bBeingDamaged = false;
		current_target->anim_controller.resetAllAnimations();
	}
}