#pragma once
#include <raylib.h>
#include "effect_base.h"

using namespace std;

class Game;
class Consumable;
class Unit;

class ItemEffect : public EffectBase
{
public:
	void setup(Game* game, Consumable* item);
	virtual void GetHitandCritRates(Unit* user, Unit* target, float& hit_rate, float& crit_rate) override;
	virtual float GetDamage(Unit* user, Unit* target, bool crit, bool bMultiTarget = false) override;
	virtual bool CanYouApplyEffect(Unit* user, Unit* target) override;
	virtual void ApplyEffect(int amount, Unit* user, Unit* target, bool hit = true, bool bMultiTarget = false) override;
	virtual void CheckAndApplyEffect(int amount, Unit* user, Unit* target) override;
private:
	//Data
	Consumable* item;
	string sub_type;
	string heal_type;
	bool can_revive = false;
};
