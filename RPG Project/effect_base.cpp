#include "effect_base.h"

void EffectBase::setup(Game* game)
{
	this->game = game;
}

void EffectBase::GetHitandCritRates(Unit* user, Unit* target, float& hit_rate, float& crit_rate)
{
}

float EffectBase::GetDamage(Unit* user, Unit* target, bool crit, bool bMultiTarget)
{
	return 0.0f;
}

bool EffectBase::CanYouApplyEffect(Unit* user, Unit* target)
{
	return false;
}

bool EffectBase::CanYouApplyEffect(Unit* user, vector<Unit*> targets)
{
	return false;
}

void EffectBase::ApplyEffect(int amount, Unit* user, Unit* target, bool hit, bool bMultiTarget)
{
}

void EffectBase::CheckAndApplyEffect(int amount, Unit* user, Unit* target)
{

}
