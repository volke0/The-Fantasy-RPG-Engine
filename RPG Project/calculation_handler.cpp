#include "calculation_handler.h"
#include <cstdlib>
#include <cmath>
#include "game.h"
#include "npc.h"
#include "magic.h"
#include "weapon.h"
#include "battle.h"

void CalculationHandler::basicSetup(Game* game, Battle* battle)
{
	this->game = game;
	this->battle = battle;
	this->type = "calculation";
}


void CalculationHandler::getHitCalculation(Unit* user, Unit* target)
{
	hit_pct = game->GetRNG(rng_frame);
	crit_pct = game->GetRNG(rng_frame);
	float hit_rate = round((float)(user->hit - target->avoid));
	float crit_rate = round((float)user->crit);
	pushHit(hit_rate);
	pushCrit(crit_rate);
}

void CalculationHandler::getAttackDamageCalculation(Unit* user, Unit* target, bool crit)
{
	//Damage formula for normal attacks
	float damage = static_cast<NPC*>(user)->GetAttackDamage(target);
	if (crit == true)
	{
		damage *= ((float)user->crit_dmg / 100.f);
	}
	battle->dmg.push_back(damage);
}


void CalculationHandler::applyDamage(Unit* user, vector<NPC*> targets, int target_index)
{
	auto& target = targets[target_index];
	if (battle->mode == "Attack")
	{
		applyAttackDamage(target, target_index);
	}
}


//Pushers
void CalculationHandler::pushHit(float hit_rate)
{
	if (hit_pct <= (int)hit_rate)
	{
		//cout << "HIT!" << endl;
		hit = true;
		battle->hit.push_back(true);
	}
	else
	{
		//cout << "MISS!" << endl;
		hit = false;
		battle->hit.push_back(false);
	}
	if (hit_rate > 100)
	{
		hit_rate = 100;
	}
	battle->hit_rates.push_back(hit_rate);
}

void CalculationHandler::pushCrit(float crit_rate)
{
	if (hit == true)
	{
		if (crit_pct <= (int)crit_rate)
		{
			//cout << "CRIT!" << endl;
			battle->crit.push_back(true);
		}
		else
		{
			battle->crit.push_back(false);
		}
	}
	else
	{
		battle->crit.push_back(false);
	}
	if (crit_rate > 100)
	{
		crit_rate = 100;
	}
	battle->crit_rates.push_back(crit_rate);
}


void CalculationHandler::applyAttackDamage(Unit* target, int target_index)
{
	if (battle->hit[target_index] == true)
	{
		target->hp -= (int)battle->dmg[target_index];
		if (target->hp < 0)
		{
			target->hp = 0;
		}
	}
}
