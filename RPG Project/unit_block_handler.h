#pragma once
#include <raylib.h>
#include "handler.h"

using namespace std;

class Event;
class Game;
class Level;
class Unit;

class UnitBlockHandler : public Handler
{
public:
	void setup(Game* game, Event* current_event);
	
	//Setters
	void SetCurrentEvent(Event* current_event);
	void LoadUnits(Json::Value UnitBlockInfo);
private:
	//Setters
	void LoadUnit(Json::Value unit_line);
	void SetupUnitEquipment(Json::Value unit_line);
	void SetUnitLocation(Json::Value unit_line);

	//Data
	Game* game = nullptr;
	Event* current_event = nullptr;
	Level* level = nullptr;
	Unit* current_unit = nullptr;
};
