#include "unit.h"
#include "game.h"
#include "npc.h"
#include "map.h"
#include "level.h"
#include "weapon.h"
#include "vars.h"


void Unit::setup(Game* game, int id)
{
	this->id = id;
	setupGame(game);
	setupUnit();
}

void Unit::setupUnit()
{
	map_mode = true;
	script = game->GetUnit(id);
	setupValues();
	setupLoot();
	setupControllers();
	setupClass();
	setupGraphics();
	setupEquipment();
	stat_handler.setup(game, this);
	magic_list_updater.setup(game, this);
	battle_ai.setup(game, this);
}

void Unit::setupControllers()
{
	movement_controller.setup(this);
	state_controller.setup(game, this);
	btl_con_controller.setup(game, this);
	btl_logic_controller.setup(game, this);
	buff_manager.setup(game, this);
}

void Unit::setupLoot()
{
	if (script.isMember("loot"))
	{
		auto& loot_script = script["loot"];
		loot["silver"] = loot_script["silver"].asInt();
		loot["exp"] = loot_script["exp"].asInt();
	}
}

void Unit::setupMagicOptions()
{
	Json::Value magic = script["magic"];
	for (Json::Value::const_iterator it = magic.begin(); it != magic.end(); it++)
	{
		magic_options.push_back(magic[it.key().asInt()].asString());
	}
}

void Unit::setupClass()
{
	if (class_link.size() != 0)
	{
		unit_class.setup(game, this, class_link);
		class_initialized = true;
	}
}

void Unit::setupValues()
{
	map_rect = { 0, 0, (float)TILE, (float)TILE };
	name = script["name"].asString();
	if (script.isMember("ingame_name"))
	{
		ingame_name = script["ingame_name"].asString();
	}
	if (script.isMember("type"))
	{
		type = script["type"].asString();
	}
	if (script.isMember("class"))
	{
		class_link = script["class"].asString();
	}
	if (script.isMember("magic"))
	{
		setupMagicOptions();
	}
	if (script.isMember("AI"))
	{
		battle_ai_script_name = script["AI"]["battle"].asString();
	}
	setupStats();
}

void Unit::setupEquipment()
{
	equip_system.setup(game, this);
	if (script.isMember("equipment"))
	{
		Json::Value equipment_script = script["equipment"];
		equip_system.SetAllEquipment(equipment_script);
	}
	if (script.isMember("equipment_rank"))
	{
		equipment_ranks = script["equipment_rank"];
	}
}


void Unit::setupGraphics()
{
	if (script.isMember("animations"))
	{
		string path = "./databases/animations/characters/" + script["animations"].asString() + ".json";
		ifstream temp_file(path);
		reader.parse(temp_file, anim_controller_script);
	}
	if (script.isMember("graphics"))
	{
		graphics = script["graphics"].asString();
	}
	if (script.isMember("portrait"))
	{
		portrait = script["portrait"].asString();
	}
	anim_controller.setup(game, this, anim_controller_script);
	setupColor();
}

void Unit::setupColor()
{
	if (script.isMember("color"))
	{
		if (script["color"] == "blue")
		{
			personal_color = BLUE;
		}
		else if (script["color"] == "red")
		{
			personal_color = RED;
		}
		else if (script["color"] == "pink")
		{
			personal_color = PINK;
		}
		else
		{
			personal_color = GREEN;
		}
	}
}

void Unit::setupStats()
{
	if (script.isMember("stats"))
	{
		stat_script = script["stats"];
		lvl = stat_script["lvl"].asInt();
		max_hp = stat_script["hp"].asInt();
		hp = max_hp;
		max_mp = stat_script["mp"].asInt();
		mp = max_mp;
		str = stat_script["str"].asInt();
		mag = stat_script["mag"].asInt();
		skl = stat_script["skl"].asInt();
		spd = stat_script["spd"].asInt();
		lck = stat_script["lck"].asInt();
		def = stat_script["def"].asInt();
		res = stat_script["res"].asInt();
		con = stat_script["con"].asInt();

		if (stat_script.isMember("exp"))
		{
			exp = stat_script["exp"].asInt();
		}
		if (stat_script.isMember("slv"))
		{
			slv = stat_script["slv"].asInt();
		}
		//Setup base stats
		base_stats["hp"] = stat_script["hp"];
		base_stats["max_hp"] = stat_script["hp"];
		base_stats["mp"] = stat_script["mp"];
		base_stats["max_mp"] = stat_script["max_mp"];
		base_stats["str"] = stat_script["str"];
		base_stats["mag"] = stat_script["mag"];
		base_stats["skl"] = stat_script["skl"];
		base_stats["spd"] = stat_script["spd"];
		base_stats["lck"] = stat_script["lck"];
		base_stats["def"] = stat_script["def"];
		base_stats["res"] = stat_script["res"];
		base_stats["con"] = stat_script["con"];
	}
}



//Setters
void Unit::setTarget(Unit* target)
{
	this->target = target;
	if (GetWeapon() != nullptr)
	{
		GetWeapon()->setTarget(target);
	}
	if (GetWeapon(true) != nullptr)
	{
		GetWeapon(true)->setTarget(target);
	}
	anim_controller.setTarget(target);
}

void Unit::setBattlePositions()
{
	cout << bstart_x << " ," << bstart_y << endl;
	battle_rect.x = bstart_x;
	battle_rect.y = bstart_y;
}

void Unit::setBattleMode()
{
	anim_controller.resetAllAnimations();
	state_controller.clearStates();
	state_controller.pushState("in_battle");
	state_controller.pushState("entrance");
	btl_con_controller.eraseCondition("idle");
	anim_controller.turnBattleControllerOff();
}

void Unit::setMapMode()
{
	state_controller.clearStates();
	state_controller.pushState("on_map");
}

void Unit::destroy()
{
	anim_controller.deleteAllAnimations();
}

const shared_ptr<Equipment> Unit::GetEquipment(string slot_name)
{
	return equip_system.GetEquipment(slot_name);
}

const shared_ptr<Equipment> Unit::GetEquipment(int inv_index)
{
	return equip_system.GetEquipment(inv_index);
}

Json::Value Unit::GetAllEquipment()
{
	return equip_system.GetAllEquipment();
}

vector<string> Unit::GetEquipType(string slot_name)
{
	return equip_system.GetEquipType(slot_name);
}

Equipment* Unit::GetWeapon(bool get2ndWeapon)
{
	return equip_system.GetWeapon(get2ndWeapon);
}

bool Unit::HasWeapon()
{
	return equip_system.HasWeapon();
}


//Location
void Unit::setLevel(Level* level, Maps* map)
{
	this->level = level;
	this->map = map;
	movement_controller.level = level;
	movement_controller.map = this->map;
	movement_controller.setCamera(level->camera);
	Vector2 vec_true_offset = { true_offset[0], true_offset[1] };
	movement_controller.setLocation(vec_true_offset);
}

//Draw
void Unit::draw()
{

}

//Update
void Unit::update()
{

}
