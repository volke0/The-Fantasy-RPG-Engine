#include "item_display_handler.h"
#include "btl_animation_manager.h"
#include "game.h"
#include "party_base.h"
#include "battle.h"
#include "unit.h"
#include "animation.h"
#include "char_anim_controller.h"
#include "text_anim.h"
#include "consumable.h"

void ItemDisplayHandler::setup(Game* game, BattleAnimationManager* btl_animation_manager)
{
	basicSetup(game, btl_animation_manager);
}

void ItemDisplayHandler::update()
{
	setBattleVars();
	updateItem();
}

void ItemDisplayHandler::draw()
{
	if (play_item && current_item->animation_name != "")
	{
		current_item->anim.play();
	}
}

void ItemDisplayHandler::setItemList(vector<shared_ptr<Consumable>> item_list, int current_item_index)
{
	this->item_list = item_list;
	this->current_item_index = current_item_index;
}

void ItemDisplayHandler::updateItem()
{
	setCurrentTarget();
	setCurrentItem();

	if (!btl_animation_manager->done)
	{
		if (!item_mode_on)
		{
			user->btl_con_controller.eraseCondition("idle");
			user->btl_logic_controller.setItemMode();
			if (user->party->CheckIfItemExists(current_item->ingame_name))
			{
				user->party->item_inventory.subFromInventory(current_item->ingame_name);
				current_target->bBeingDamaged = true;
				item_mode_on = true;
			}
			else
			{
				btl_animation_manager->EndBattle();
				item_mode_on = true;
			}
		}
		if (user->btl_logic_controller.play_item == true)
		{
			play_item = true;
		}
		else
		{
			play_item = false;
		}

		if (current_item->anim.done == true || (current_item->animation_name == "" && user->btl_logic_controller.play_item == true))
		{
			btl_animation_manager->play_text_anim = true;
		}

		if (btl_animation_manager->dmg_text[btl_animation_manager->target_index]->done == true)
		{
			btl_animation_manager->play_text_anim = false;
			if (applied_dmg == false)
			{
				battle->item_calculator.applyDamage(user, btl_animation_manager->targets, btl_animation_manager->target_index);
				applied_dmg = true;
			}
			if (item_index >= (int)item_list.size() - 1)
			{
				user->btl_logic_controller.setMagicDone();
			}
		}

		if (user->btl_logic_controller.item_done == true)
		{
			btl_animation_manager->done = true;
			btl_animation_manager->resetCurrentTarget();
		}
	}
}

void ItemDisplayHandler::setBattleVars()
{
	if (btl_animation_manager->targets.size() > 0)
	{
		btl_animation_manager->current_target = btl_animation_manager->targets[btl_animation_manager->target_index];
	}
}

void ItemDisplayHandler::setCurrentTarget()
{
	if (battle->item_calculator.item == nullptr) return;
	bool bChangeTarget = btl_animation_manager->checkIfTargetDead(battle->item_calculator.item->can_revive);
	btl_animation_manager->checkIfAllTargetsDead();
	current_target = btl_animation_manager->current_target;

	if (bChangeTarget)
	{
		if (!current_item) return;
		current_item->setTarget(current_target);
	}
}

void ItemDisplayHandler::setCurrentItem()
{
	if ((int)item_list.size() == 0) return;
	current_item = item_list[item_index];
	battle->current_item = current_item;
}
