#pragma once
#include <raylib.h>
#include "equipment_base.h"

using namespace std;

class Game;
class Unit;

class Weapon : public Equipment
{
public:
	virtual void setup(Game* game, Unit* unit, Json::Value script) override;

	string weapon_type;
	bool is_mag_weapon;

	//Logic
	bool equipped = false;

private:
	//setup
	void setupWeapon();
};
