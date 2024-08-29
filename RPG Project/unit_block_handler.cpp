#include "unit_block_handler.h"
#include "game.h"
#include "level.h"
#include "event.h"
#include "npc.h"


void UnitBlockHandler::setup(Game* game, Event* current_event)
{
	this->game = game;
	this->current_event = current_event;
	level = current_event->GetLevel();
}

void UnitBlockHandler::SetCurrentEvent(Event* current_event)
{
	this->current_event = current_event;
	level = current_event->GetLevel();
}

void UnitBlockHandler::LoadUnits(Json::Value UnitBlockInfo)
{
	string file_name = UnitBlockInfo["file_name"].asString();
	string unit_block = UnitBlockInfo["unit_block"].asString();

	Json::Value UnitBlock = game->GetUnitBlock(file_name, unit_block);

	for (int i = 0; i < (int)UnitBlock.size(); i++)
	{
		Json::Value unit_line = UnitBlock[i];
		LoadUnit(unit_line);
		SetupUnitEquipment(unit_line);
		SetUnitLocation(unit_line);
	}
}

void UnitBlockHandler::LoadUnit(Json::Value unit_line)
{
	int UNIT_ID = unit_line["ID"].asInt();

	shared_ptr<NPC> temp_unit = make_shared<NPC>();
	temp_unit->setup(game, UNIT_ID);
	temp_unit->setLevel(level, current_event->GetMap());
	current_event->temp_units[temp_unit->name] = temp_unit;
	current_unit = current_event->temp_units[temp_unit->name].get();
	level->addUnitToList(temp_unit);
}

void UnitBlockHandler::SetupUnitEquipment(Json::Value unit_line)
{
	if (!current_unit) return;
	Json::Value equipment_script = unit_line["equipment"];
	current_unit->equip_system.SetAllEquipment(equipment_script);
}

void UnitBlockHandler::SetUnitLocation(Json::Value unit_line)
{
	if (!current_unit) return;
	Json::Value location = unit_line["Location"];
	Vector2 pos = { location["x"].asFloat() * TILE, location["y"].asFloat() * TILE };
	current_unit->movement_controller.setLocation(pos);
	//cout << current_unit->name << " STARTS AT X: " << pos.x / TILE << " Y: " << pos.y / TILE << endl;
}
