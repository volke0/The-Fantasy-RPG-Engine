#pragma once
#include "equipment_base.h"

using namespace std;

class Game;
class Unit;

class Armor : public Equipment
{
public:
	virtual void setup(Game* game, Unit* unit, Json::Value script) override;

	//Data
	string armor_type = "";
private:
	void SetupArmor();
};

