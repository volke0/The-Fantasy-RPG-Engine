#pragma once
#include "equipment_base.h"

using namespace std;

class Game;
class Unit;

class HeadGear : public Equipment
{
public:
	virtual void setup(Game* game, Unit* unit, Json::Value script) override;

	//Data
	string head_gear_type = "";
private:
	void SetupHeadGear();
};
