#pragma once
#include <raylib.h>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <json/json.h>
#include <string>
#include <cmath>

using namespace std;

class Game;
class Unit;

class EffectBase
{
public:
	void setup(Game* game);
	virtual void GetHitandCritRates(Unit* user, Unit* target, float& hit_rate, float& crit_rate);
	virtual float GetDamage(Unit* user, Unit* target, bool crit, bool bMultiTarget = false);
	virtual bool CanYouApplyEffect(Unit* user, Unit* target);
	virtual bool CanYouApplyEffect(Unit* user, vector<Unit*> targets);
	virtual void ApplyEffect(int amount, Unit* user, Unit* target, bool hit = true, bool bMultiTarget = false);
	virtual void CheckAndApplyEffect(int amount, Unit* user, Unit* target);

	//Data
	Game* game;
	int rng_frame = 0;
};