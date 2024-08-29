#include "magic_effect.h"
#include <cstdlib>
#include <cmath>
#include "unit.h"
#include "game.h"
#include "magic.h"
#include "weapon.h"

void MagicEffect::setup(Game* game, Magic* magic)
{
	EffectBase::setup(game);
	setupMagic(magic);
}

void MagicEffect::setupMagic(Magic* magic)
{
	this->magic = magic;
	mag_type = magic->mag_type;
	elem_type = magic->elem_type;
	power = magic->power;
	hit = magic->hit;
	crit = magic->crit;
	target_type = magic->target_type;
	cost = magic->cost;
	cost_type = magic->cost_type;
	heal_type = magic->heal_type;
	pct_hp_dmg = magic->pct_hp_dmg;
	pct_hp_amount = magic->pct_hp_amount;
	can_revive = magic->can_revive;
}



void MagicEffect::GetHitandCritRates(Unit* user, Unit* target, float& hit_rate, float& crit_rate)
{
	int hit = magic->hit;
	int crit = magic->crit;

	if (user->GetWeapon() != nullptr)
	{
		if (static_cast<Weapon*>(user->GetWeapon())->is_mag_weapon)
		{
			hit += user->base_hit + user->GetWeapon()->hit;
			crit += user->GetWeapon()->crit;
		}
	}
	if (user->GetWeapon(true) != nullptr)
	{
		if (static_cast<Weapon*>(user->GetWeapon(true))->is_mag_weapon)
		{
			hit += user->base_hit + user->weapon1_hit;
			crit += user->weapon1_crit;
		}
	}

	hit_rate = round((float)(hit - target->avoid));
	crit_rate = round((float)crit);
}

float MagicEffect::GetDamage(Unit* user, Unit* target, bool crit, bool bMultiTarget)
{
	float damage = getMagicDamage(user, target, crit);
	if (crit == true)
	{
		damage *= ((float)user->crit_dmg / 100.f);
	}
	if (bMultiTarget) damage /= 2;
	return damage;
}

bool MagicEffect::CanYouApplyEffect(Unit* user, Unit* target)
{
	if (user->mp >= magic->cost)
	{
		if (mag_type == "healing")
		{
			if (!target->dead && !can_revive)
			{
				if (target->hp < target->max_hp)
				{
					return true;
				}
				else
				{
					if (user->battle_mode)
					{
						return true;
					}
				}
			}
			else if (target->dead && can_revive)
			{
				return true;
			}
		}
		else if (mag_type == "dmg")
		{
			if (target != user)
			{
				if (!target->dead)
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool MagicEffect::CanYouApplyEffect(Unit* user, vector<Unit*> targets)
{
	int total_cost = 0;
	for (int index = 0; index < (int)targets.size(); index++)
	{
		total_cost += magic->cost;
	}
	if (user->mp >= total_cost)
	{
		for (auto& target : targets)
		{
			bool bCanApplyEffect = CanYouApplyEffect(user, target);
			//Return true if only one target can be healed and/or damaged (the damage application will only apply to living targets regardless)
			if (bCanApplyEffect) return true;
		}
	}
	return false;
}

void MagicEffect::ApplyEffect(int amount, Unit* user, Unit* target, bool hit, bool bMultiTarget)
{
	if (mag_type == "dmg")
	{
		ApplyDamage(amount, target, hit);
	}
	if (mag_type == "healing")
	{
		ApplyHeal(amount, target);
	}
	if (cost_type == "mp")
	{
		user->mp -= cost;
		if (user->mp < 0)
		{
			user->mp = 0;
		}
	}
}

void MagicEffect::CheckAndApplyEffect(int amount, Unit* user, Unit* target)
{
	if (CanYouApplyEffect(user, target))
	{
		ApplyEffect(amount, user, target);
	}
}

float MagicEffect::getMagicDamage(Unit* user, Unit* target, bool& crit)
{
	float damage = 0;
	float mag_power = 0;

	//Damage Modifiers
	float lvl_diff = round(1.0f + (float)(user->lvl / 99));
	float multipliers = 1.0f + 0.0f;  //Add other multipliers later

	if (pct_hp_dmg == true)
	{
		if (mag_type == "healing")
		{
			float hp_diff = (float)target->hp / (float)target->max_hp;
			mag_power = (float)pct_hp_amount * hp_diff;
		}
		else
		{
			mag_power = (float)target->hp * (float)pct_hp_amount;
		}
	}
	else
	{
		mag_power = (float)power;
	}

	if (mag_type == "healing")
	{
		damage = round(mag_power + (((pow((float)user->res, 2.0f)) * ((float)user->mag / 4.0f)) * lvl_diff));
		crit = false;
	}
	else
	{
		damage = round(mag_power * pow((float)user->mag, 2.0f));
	}

	if (mag_type != "healing")
	{
		damage = static_cast<NPC*>(target)->GetMagicDamage(damage, lvl_diff, multipliers);
	}
	return damage;
}


void MagicEffect::ApplyHeal(int amount, Unit* target)
{
	if (heal_type == "hp")
	{
		if (target->dead) return;
		target->hp += amount;
		if (target->hp > target->max_hp)
		{
			target->hp = target->max_hp;
		}
		target->predicted_hp = target->hp;
	}
	else if (heal_type == "mp")
	{
		if (target->dead) return;
		target->mp += amount;
		if (target->mp > target->max_mp)
		{
			target->mp = target->max_mp;
		}
		target->predicted_mp = target->mp;
	}
}

void MagicEffect::ApplyDamage(int amount, Unit* target, bool hit)
{
	if (hit)
	{
		target->hp -= amount;
		if (target->hp < 0)
		{
			target->hp = 0;
		}
	}
}
