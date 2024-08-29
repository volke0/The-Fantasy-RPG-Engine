#include "magic_calculation_handler.h"
#include <cstdlib>
#include <cmath>
#include "battle.h"
#include "game.h"
#include "magic.h"
#include "weapon.h"

void MagicCalculationHandler::setup(Game* game, Battle* battle)
{
	basicSetup(game, battle);
}

void MagicCalculationHandler::setMagic(Magic* magic)
{
	this->magic = magic;
}

void MagicCalculationHandler::getMagHitCalculation(Unit* user, Unit* target)
{
	hit_pct = game->GetRNG(rng_frame);
	crit_pct = game->GetRNG(rng_frame);

	float hit_rate = 0.0f;
	float crit_rate = 0.0f;

	magic->GetHitAndCritRates(user, target, hit_rate, crit_rate);

	pushHit(hit_rate);
	pushCrit(crit_rate);
}

void MagicCalculationHandler::getMagAttackCalculation(Unit* user, Unit* target, bool crit, Magic* magic, bool bMultiTargeting)
{
	setMagic(magic);
	float damage = magic->GetDamage(user, target, crit, bMultiTargeting);
	battle->dmg.push_back(damage);
}

void MagicCalculationHandler::applyDamage(Unit* user, vector<NPC*> targets, int target_index)
{
	auto& target = targets[target_index];
	auto& dmg = battle->dmg[target_index];
	bool hit = battle->hit[target_index];

	battle->current_magic->ApplyEffect(dmg, user, target, hit);
}