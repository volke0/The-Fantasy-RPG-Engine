#pragma once
#include <raylib.h>
#include "effect_base.h"

using namespace std;

class Game;
class Magic;
class Unit;

class MagicEffect : public EffectBase
{
public:
	void setup(Game* game, Magic* magic);
	virtual void GetHitandCritRates(Unit* user, Unit* target, float& hit_rate, float& crit_rate) override;
	virtual float GetDamage(Unit* user, Unit* target, bool crit, bool bMultiTarget = false) override;
	virtual bool CanYouApplyEffect(Unit* user, Unit* target) override;
	virtual bool CanYouApplyEffect(Unit* user, vector<Unit*> targets) override;
	virtual void ApplyEffect(int amount, Unit* user, Unit* target, bool hit = true, bool bMultiTarget = false) override;
	virtual void CheckAndApplyEffect(int amount, Unit* user, Unit* target) override;
private:
	//Setup
	void setupMagic(Magic* magic);

	//Getters
	float getMagicDamage(Unit* user, Unit* target, bool& crit);

	//Setters
	void ApplyHeal(int amount, Unit* target);
	void ApplyDamage(int amount, Unit* target, bool hit);

	//Data
	Magic* magic;
	string mag_type;
	string elem_type;
	int power = 0;
	int hit = 0;
	int crit = 0;
	string target_type;
	int cost;
	string cost_type;
	string heal_type = "hp";
	bool pct_hp_dmg = false;
	float pct_hp_amount = 0;
	bool can_revive = false;
};
