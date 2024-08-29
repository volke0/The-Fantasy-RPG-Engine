#include "item_effect.h"
#include "consumable.h"
#include "unit.h"

void ItemEffect::setup(Game* game, Consumable* item)
{
	EffectBase::setup(game);
	this->item = item;
	sub_type = item->consume_type;
	heal_type = item->heal_type;
	can_revive = item->can_revive;
}

void ItemEffect::GetHitandCritRates(Unit* user, Unit* target, float& hit_rate, float& crit_rate)
{
	//Maybe add hit rates for damage consumables later
	hit_rate = 100.f;
	crit_rate = 0.f;
}

float ItemEffect::GetDamage(Unit* user, Unit* target, bool crit, bool bMultiTarget)
{
	//Implement whether consumable does magic or physical damage later + add more complex damage options
	float amount = item->amount;
	return amount;
}

bool ItemEffect::CanYouApplyEffect(Unit* user, Unit* target)
{
	if (sub_type == "heal")
	{
		if (!target->dead && !can_revive)
		{
			if (heal_type == "hp")
			{
				if (target->hp < target->max_hp)
				{
					return true;
				}
			}
			if (heal_type == "mp")
			{
				if (target->mp < target->max_mp)
				{
					return true;
				}
			}
			if (target->battle_mode)
			{
				return true;
			}
		}
		else if (target->dead && can_revive)
		{
			return true;
		}
	}
	else if (sub_type == "dmg")
	{
		if (user != target)
		{
			if (!target->dead)
			{
				return true;
			}
		}
	}
	return false;
}

void ItemEffect::ApplyEffect(int amount, Unit* user, Unit* target, bool hit, bool bMultiTarget)
{
	if (sub_type == "heal")
	{
		if (!target->dead || can_revive)
		{
			if (heal_type == "hp")
			{
				target->hp += amount;
				if (target->hp >= target->max_hp)
				{
					target->hp = target->max_hp;
				}
				target->predicted_hp = INT_MAX;
			}
			if (heal_type == "mp")
			{
				target->mp += amount;
				if (target->mp >= target->max_mp)
				{
					target->mp = target->max_mp;
				}
				target->predicted_mp = INT_MAX;
			}
		}
		if (can_revive)
		{
			if (target->battle_mode)
			{
				target->btl_con_controller.setCondition("revived");
			}
		}
	}
	//TODO: Add damage type effects later...
}

void ItemEffect::CheckAndApplyEffect(int amount, Unit* user, Unit* target)
{
	if (CanYouApplyEffect(user, target))
	{
		ApplyEffect(amount, user, target);
	}
}
