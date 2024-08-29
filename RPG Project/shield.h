#pragma once
#include <raylib.h>
#include "equipment_base.h"

using namespace std;

class Game;
class Unit;

class Shield : public Equipment
{
public:
	virtual void setup(Game* game, Unit* unit, Json::Value script) override;
	
	//Data
	string shield_type = "";
private:
	void SetupShield();
};
