#pragma once
#include <raylib.h>
#include <json/json.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "vfx_animation.h"
#include "effect_base.h"

using namespace std;

class Game;
class Unit;

class Item
{
public:
	//Setup
	void basicSetup(Game* game, Unit* unit, Json::Value script);
	void setUnit(Unit* unit);
	void setTarget(Unit* target);

	//Apply Effect
	virtual void GetHitAndCritRates(Unit* user, Unit* target, float& hit_rate, float& crit_rate);
	virtual float GetDamage(Unit* user, Unit* target, bool crit, bool bMultiTarget = false);
	virtual bool CheckIfEffectApplicable();
	virtual bool CheckIfEffectApplicable(vector<Unit*> targets);
	virtual bool CheckIfEffectApplicable(Unit* user, Unit* target);
	virtual void ApplyEffect();
	virtual void ApplyEffect(float amount, Unit* user, Unit* target, bool hit = true);
	virtual void CheckAndApplyEffect();
	virtual void CheckAndApplyEffect(float amount, Unit* user, Unit* target);

	//Effect
	EffectBase* effect;
	
	//Data
	Game* game = nullptr;
	Unit* target = nullptr;
	Unit* unit = nullptr;
	int inv_index = 0;

	//Data initialized from script
	Json::Value script;
	Json::Reader reader;
	string name = "";
	string type = "";
	string ingame_name = "";
	string animation_name = "";
	string heal_type = "hp";
	bool can_revive = false;
	bool can_heal = false;

	//Animation
	VFXAnimation anim;

	//Item
	void initializeItem();
	void basicInitialization();
	void setupGraphics();
};
