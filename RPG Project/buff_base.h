#pragma once
#include <raylib.h>
#include <json/json.h>
#include "entity.h"

using namespace std;

class Game;
class Unit;
class NPC;
class TextAnimation;

struct BuffMagnitude
{
	BuffMagnitude() {};
	
	string output_type = "";
	float base = 0.f;
	float scaling = 0.f;
	string scaling_type = "magic";
	bool bIsDoT = false;
	bool bIsHoT = false;
	string stat_modifying = "atk";
	string scaler = "";
private:
	float amount = 0.f;
	NPC* applier = nullptr;
	NPC* target = nullptr;
public:
	//Setters
	void SetApplierAndTarget(NPC* applier, NPC* target);
	float GetAmount();
};


class BuffBase : public Entity
{
public:
	//Setup
	void setup(Game* game, Json::Value buff_script, Unit* user, Unit* target);

	virtual void update();
	virtual void draw();

	//Setters
	void SetPosition(int position);
	void AddToStacks();
	void ApplyEffect();
	void ResetAppliedEffectBool();

	//Getters
	NPC* GetSourceUnit();
	int GetPosition() const;
	string GetName() const;
	bool GetIfAppliedEffect() const;
	bool NeedsTermination() const;
	bool PlayingAnimation() const;
	string GetIconName() const;
private:
	//Setup
	void ParseBuffScript(Json::Value& buff_script);

	//Update
	void CheckTurnCounter();
	void ApplyStatModifiers();

	//Draw

	//Setters
	void IncrementTurnCounter();

	//Data
	Json::Value buff_script;
	string c_name = "";
	string ingame_name = "";
	string icon_name = "";
	string description = "";
	string buff_type = "";
	string effect_type = "";
	NPC* applier = nullptr;
	NPC* target = nullptr;
	int turn = 0;
	int old_turn = -1;
	int duration = 0;
	int stacks = 0;
	int max_stacks = 0;
	int position = 0;
	BuffMagnitude Magnitude;
	shared_ptr<TextAnimation> text_anim = nullptr;

	//Logic
	bool bStatModifying = false;
	bool bIsDoT = false;
	bool bIsHealOverTime = false;
	bool bAppliedStatChanges = false;
	bool bPlayingAnim = false;
	bool bEndEffect = false;
	bool bAppliedEffect = false;
	bool bHasValidIcon = false;
	bool done = false;
};