#pragma once
#include "equipment_base.h"

using namespace std;

class Game;
class Unit;

class FootGear : public Equipment
{
public:
	virtual void setup(Game* game, Unit* unit, Json::Value script) override;

	//Data
	string foot_gear_type = "";
private:
	void SetupFootGear();
};
