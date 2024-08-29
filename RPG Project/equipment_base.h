#pragma once
#include <raylib.h>
#include "item.h"

using namespace std;

class Game;
class Unit;

class Equipment : public Item
{
public:
	virtual void setup(Game* game, Unit* unit, Json::Value script);

	//Stats
	string sub_type = "";
	int str = 0;
	int mag = 0;
	int skl = 0;
	int spd = 0;
	int lck = 0;
	int def = 0;
	int res = 0;
	int con = 0;

	int attack = 0;
	int mag_attack = 0;
	int atk_speed = 0;
	int hit = 0;
	int avoid = 0;
	int block = 0;
	int mag_defense = 0;
	int crit = 0;
	int crit_dmg = 0;
	int weight = 0;

	//Data
	string slot = "";
	Json::Value special_lock;

private:
	//Setup
	void setupStats();
};
