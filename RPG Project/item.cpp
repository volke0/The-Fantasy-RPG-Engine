#include "item.h"
#include "game.h"
#include "vfx_animation.h"

void Item::basicSetup(Game* game, Unit* unit, Json::Value script)
{
	this->game = game;
	this->script = script;
	setUnit(unit);
}

void Item::setUnit(Unit* unit)
{
	this->unit = unit;
	anim.setUnit(unit);
}

void Item::setTarget(Unit* target)
{
	this->target = target;
	anim.setTarget(target);
}

void Item::GetHitAndCritRates(Unit* user, Unit* target, float& hit_rate, float& crit_rate)
{
	effect->GetHitandCritRates(user, target, hit_rate, crit_rate);
}

float Item::GetDamage(Unit* user, Unit* target, bool crit, bool bMultiTarget)
{
	return effect->GetDamage(user, target, crit, bMultiTarget);
}

bool Item::CheckIfEffectApplicable()
{
	return effect->CanYouApplyEffect(unit, target);
}

bool Item::CheckIfEffectApplicable(vector<Unit*> targets)
{
	return effect->CanYouApplyEffect(unit, targets);
}

bool Item::CheckIfEffectApplicable(Unit* user, Unit* target)
{
	return effect->CanYouApplyEffect(user, target);
}

void Item::ApplyEffect()
{
	float amt = effect->GetDamage(unit, target, false);
	effect->ApplyEffect((int)amt, unit, target);
}

void Item::ApplyEffect(float amount, Unit* user, Unit* target, bool hit)
{
	effect->ApplyEffect((int)amount, user, target);
}

void Item::CheckAndApplyEffect()
{
	float amt = effect->GetDamage(unit, target, false);
	effect->CheckAndApplyEffect((int)amt, unit, target);
}

void Item::CheckAndApplyEffect(float amount, Unit* user, Unit* target)
{
	effect->CheckAndApplyEffect((int)amount, user, target);
}


void Item::initializeItem()
{
	name = script["c_name"].asString();
	ingame_name = script["name"].asString();
	basicInitialization();
	setupGraphics();
}

void Item::basicInitialization()
{
	if (script.isMember("can_revive"))
	{
		can_revive = script["can_revive"].asBool();
	}
	if (type == "consumable")
	{
		can_heal = true;
	}
}

void Item::setupGraphics()
{
	if (script.isMember("animations"))
	{
		animation_name = script["animations"].asString();
	}

	if (animation_name != "")
	{
		anim.setup(game, game->vfx_anim_database.vfx_anims[animation_name]);
	}
}
