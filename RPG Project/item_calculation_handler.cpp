#include "item_calculation_handler.h"
#include "consumable.h"
#include "npc.h"
#include "battle.h"

void ItemCalculationHandler::setup(Game* game, Battle* battle)
{
	basicSetup(game, battle);
}

void ItemCalculationHandler::setItem(Consumable* item)
{
	this->item = item;
}

void ItemCalculationHandler::getItemHitCalculation(Unit* user, Unit* target)
{
	//Maybe add hit rates for damage consumables later
	float hit_rate = 0.0f;
	float crit_rate = 0.0f;

	item->GetHitAndCritRates(user, target, hit_rate, crit_rate);

	pushHit(hit_rate);
	pushCrit(crit_rate);
}

void ItemCalculationHandler::getItemAttackCalculation(Unit* user, Unit* target, bool crit, Consumable* item)
{
	setItem(item);
	this->user = user;
	//Implement whether consumable does magic or physical damage later + add more complex damage options
	float damage = item->GetDamage(user, target, crit);

	battle->dmg.push_back(damage);
}

void ItemCalculationHandler::applyDamage(Unit* user, vector<NPC*> targets, int target_index)
{
	auto& target = targets[target_index];
	float dmg = battle->dmg[target_index];

	if (battle->mode == "Item")
	{
		item->ApplyEffect(dmg, user, target);
	}
}
