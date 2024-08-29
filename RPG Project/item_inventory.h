#pragma once
#include <raylib.h>
#include "inventory_base.h"

using namespace std;

class Game;
class PartyBase;
class Consumable;
class Unit;

class ItemInventory : public InventoryBase
{
public:
	void setup(Game* game, PartyBase* party);

	//Setters
	void SetEquippedUnit(int item_num, Unit* unit);
	void RemoveEquippedUnit(int item_num);

	//Getters
	Unit* GetEquippedUnit(int item_num);
private:
	//Data
	PartyBase* party;
};