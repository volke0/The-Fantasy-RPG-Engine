#pragma once
#include <raylib.h>
#include "unit.h"

using namespace std;

class Game;

class NPC : public Unit
{
public:
	virtual void setup(Game* game, int id) override;

	virtual void update() override;
	virtual void draw() override;

	//Update
	virtual void updateMapMode();
	virtual void updateBattleMode();

	//Draw
	virtual void drawMapMode();
	virtual void drawBattleMode();

	//Setters
	void setLayerPos(int pos_num);
	void setMapRect(Rectangle map_rect);
	void setDirection(string direction);
	void changeAnimation(string anim_name);
	void resetAnimation(string anim_name);
	void resetCurrentAnimation();
	void FlipAnimation(bool flip);

	//Getters
	float GetAttackDamage(Unit* target);
	float GetMagicDamage(float damage, float lvl_diff, float multipliers = 1.f);
	string getDirection();
	string getDirectionToFaceUnit();

	//Data
	bool anim_lock = false;
	bool bPartyHidden = false;
private:
};