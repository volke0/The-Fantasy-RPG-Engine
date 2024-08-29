#include "equip_system.h"
#include "./lib/cpp_assert.hpp"
#include "unit.h"
#include "party.h"
#include "game.h"
#include "weapon.h"
#include "shield.h"
#include "foot_gear.h"
#include "head_gear.h"
#include "armor.h"

void EquipmentSystem::setup(Game* game, Unit* user)
{
	this->game = game;
	this->party = &game->party;
	this->user = user;
	SetupEquipment();
}

void EquipmentSystem::SetupEquipment()
{
	equipment["r_hand"] = "";
	equipment["l_hand"] = "";
	equipment["head"] = "";
	equipment["body"] = "";
	equipment["feet"] = "";

}


void EquipmentSystem::update()
{

}


void EquipmentSystem::SetAllEquipment(Json::Value equip_script)
{
	auto& equip_database = game->item_database.item_scripts;
	if (equip_script.isMember("r_hand"))
	{
		auto& equipment_script = equip_database[equip_script["r_hand"].asString()];
		UnEquip("r_hand");
		Equip("r_hand", equipment_script);
	}
	if (equip_script.isMember("l_hand"))
	{
		auto& equipment_script = equip_database[equip_script["l_hand"].asString()];
		UnEquip("l_hand");
		Equip("l_hand", equipment_script);
	}
	if (equip_script.isMember("head"))
	{
		auto& equipment_script = equip_database[equip_script["head"].asString()];
		UnEquip("head");
		Equip("head", equipment_script);
	}
	if (equip_script.isMember("body"))
	{
		auto& equipment_script = equip_database[equip_script["body"].asString()];
		UnEquip("body");
		Equip("body", equipment_script);
	}
	if (equip_script.isMember("feet"))
	{
		auto& equipment_script = equip_database[equip_script["feet"].asString()];
		UnEquip("feet");
		Equip("feet", equipment_script);
	}
}

void EquipmentSystem::Equip(string slot_name, Json::Value equipment_script, int inv_index)
{
	string equip_type = equipment_script["sub_type"].asString();
	shared_ptr<Equipment> equip;
	if (slot_name == "r_hand")
	{			
		R_Hand = CreateEquipment(slot_name, equipment_script);
		equip = R_Hand;
	}
	else if (slot_name == "l_hand")
	{
		L_Hand = CreateEquipment(slot_name, equipment_script);
		equip = L_Hand;
	}
	else if (slot_name == "head")
	{
		Head = CreateEquipment(slot_name, equipment_script);
		equip = Head;
	}
	else if (slot_name == "body")
	{
		Body = CreateEquipment(slot_name, equipment_script);
		equip = Body;
	}
	else if (slot_name == "feet")
	{
		Feet = CreateEquipment(slot_name, equipment_script);
		equip = Feet;
	}
	ApplyAllEquipmentStats(equipment_script);
	addEquipmentToInventory(equip.get(), inv_index);
}

shared_ptr<Equipment> EquipmentSystem::CreateEquipment(string slot, Json::Value equipment_script)
{
	string equip_type = equipment_script["sub_type"].asString();
	shared_ptr<Equipment> equip;
	if (equip_type == "weapon")
	{
		shared_ptr<Weapon> weapon = make_shared<Weapon>();
		weapon->setup(game, user, equipment_script);
		equip = weapon;
	}
	if (equip_type == "shield")
	{
		shared_ptr<Shield> shield = make_shared<Shield>();
		shield->setup(game, user, equipment_script);
		equip = shield;
	}
	if (equip_type == "head_gear")
	{
		shared_ptr<HeadGear> head_gear = make_shared<HeadGear>();
		head_gear->setup(game, user, equipment_script);
		equip = head_gear;
	}
	if (equip_type == "armor")
	{
		shared_ptr<Armor> armor = make_shared<Armor>();
		armor->setup(game, user, equipment_script);
		equip = armor;
	}
	if (equip_type == "foot_gear")
	{
		shared_ptr<FootGear> foot_gear = make_shared<FootGear>();
		foot_gear->setup(game, user, equipment_script);
		equip = foot_gear;
	}

	if (slot == "r_hand" && equip_type == "weapon")
	{
		r_hand_weapon = true;
	}
	else if (slot == "l_hand" && equip_type == "weapon")
	{
		l_hand_weapon = true;
	}

	if (equipment_script.isMember("dual_wield"))
	{
		dual_wielding = equipment_script["dual_wield"].asBool();
	}
	else
	{
		dual_wielding = false;
	}
	equipment[slot] = equipment_script["c_name"].asString();

	if (equip == nullptr)
	{
		string error_msg = "You loaded a non-existent item, either in the UnitBlock or Unit json script(s).";
		log_assert((equip == nullptr), error_msg);
	}
	
	equip->slot = slot;
	return equip;
}

void EquipmentSystem::UnEquip(string slot_name)
{
	auto& equipment = GetEquipment(slot_name);
	if (equipment != nullptr)
	{
		unordered_map<string, int> equipment_stats = GetEquipmentStats(equipment.get());

		user->bonus_str -= GetStat(equipment_stats, "str");
		user->bonus_mag -= GetStat(equipment_stats, "mag");
		user->bonus_skl -= GetStat(equipment_stats, "skl");
		user->bonus_spd -= GetStat(equipment_stats, "spd");
		user->bonus_lck -= GetStat(equipment_stats, "lck");
		user->bonus_def -= GetStat(equipment_stats, "def");
		user->bonus_res -= GetStat(equipment_stats, "res");
		user->bonus_con -= GetStat(equipment_stats, "con");

		user->bonus_atk -= GetStat(equipment_stats, "attack");
		user->bonus_mag_atk -= GetStat(equipment_stats, "mag_attack");
		user->bonus_atk_spd -= GetStat(equipment_stats, "atk_speed");
		user->bonus_hit -= GetStat(equipment_stats, "hit");
		user->bonus_avoid -= GetStat(equipment_stats, "avoid");
		user->bonus_block -= GetStat(equipment_stats, "block");
		user->bonus_mag_defense -= GetStat(equipment_stats, "mag_def");
		user->bonus_crit -= GetStat(equipment_stats, "crit");
		user->bonus_crit_dmg -= GetStat(equipment_stats, "crit_dmg");
		user->weight -= GetStat(equipment_stats, "weight");

		removeEquipmentFromSlot(slot_name, equipment->inv_index);
	}
	if (slot_name == "r_hand")
	{
		r_hand_weapon = false;
	}
	if (slot_name == "l_hand")
	{
		l_hand_weapon = false;
	}
}

void EquipmentSystem::ApplyAllEquipmentStats(Json::Value equipment_script)
{
	unordered_map<string, int> equipment_stats = GetEquipmentStats(equipment_script);

	user->bonus_str += GetStat(equipment_stats, "str");
	user->bonus_mag += GetStat(equipment_stats, "mag");
	user->bonus_skl += GetStat(equipment_stats, "skl");
	user->bonus_spd += GetStat(equipment_stats, "spd");
	user->bonus_lck += GetStat(equipment_stats, "lck");
	user->bonus_def += GetStat(equipment_stats, "def");
	user->bonus_res += GetStat(equipment_stats, "res");
	user->bonus_con += GetStat(equipment_stats, "con");

	user->bonus_atk += GetStat(equipment_stats, "attack");
	user->bonus_mag_atk += GetStat(equipment_stats, "mag_attack");
	user->bonus_atk_spd += GetStat(equipment_stats, "atk_speed");
	user->bonus_hit += GetStat(equipment_stats, "hit");
	user->bonus_avoid += GetStat(equipment_stats, "avoid");
	user->bonus_block += GetStat(equipment_stats, "block");
	user->bonus_mag_defense += GetStat(equipment_stats, "mag_def");
	user->bonus_crit += GetStat(equipment_stats, "crit");
	user->bonus_crit_dmg += GetStat(equipment_stats, "crit_dmg");
	user->weight += GetStat(equipment_stats, "weight");
}


vector<string> EquipmentSystem::GetEquipType(string slot_name)
{
	auto& equip_item = GetEquipment(slot_name);
	vector<string> equip_type = {};
	if (slot_name == "r_hand")
	{
		if (!l_hand_weapon || dual_wielding)
		{
			equip_type = equip_types[slot_name];
		}
		else
		{
			equip_type.push_back(equip_types[slot_name][1]);
		}
	}
	else if (slot_name == "l_hand")
	{
		if (!r_hand_weapon || dual_wielding)
		{
			equip_type = equip_types[slot_name];
		}
		else
		{
			equip_type.push_back(equip_types[slot_name][1]);
		}
	}
	else
	{
		equip_type = equip_types[slot_name];
	}

	return equip_type;
}

vector<string> EquipmentSystem::GetEquipRanks(vector<string> equip_types)
{
	vector<string> equip_ranks = {};

	if ((int)equip_types.size() > 0)
	{
		for (auto& equip_type : equip_types)
		{
			Json::Value temp_ranks = user->equipment_ranks[equip_type];
			for (int i = 0; i < (int)temp_ranks.size(); i++)
			{
				//cout << temp_ranks[i] << endl;
				equip_ranks.push_back(temp_ranks[i].asString());
			}
		}
	}
	return equip_ranks;
}

Equipment* EquipmentSystem::GetWeapon(bool get2ndWeapon)
{
	int count = 0;
	if (R_Hand != nullptr)
	{
		if (R_Hand->sub_type == "weapon")
		{
			count++;
			if (!get2ndWeapon)
			{
				return R_Hand.get();
			}
		}
	}
	if (L_Hand != nullptr)
	{
		if (L_Hand->sub_type == "weapon")
		{
			count++;
			if (get2ndWeapon && count >= 2)
			{
				return L_Hand.get();
			}
			else if (!get2ndWeapon)
			{
				return L_Hand.get();
			}
		}
	}
	return nullptr;
}

bool EquipmentSystem::HasWeapon()
{
	if (R_Hand != nullptr)
	{
		if (R_Hand->sub_type == "weapon")
		{
			return true;
		}
	}
	if (L_Hand != nullptr)
	{
		if (L_Hand->sub_type == "weapon")
		{
			return true;
		}
	}
	return false;
}

const shared_ptr<Equipment> EquipmentSystem::GetEquipment(string slot_name)
{
	if (slot_name == "r_hand")
	{
		if (R_Hand != nullptr)
		{
			return R_Hand;
		}
	}
	else if (slot_name == "l_hand")
	{
		if (L_Hand != nullptr)
		{
			return L_Hand;
		}
	}
	else if (slot_name == "head")
	{
		if (Head != nullptr)
		{
			return Head;
		}
	}
	else if (slot_name == "body")
	{
		if (Body != nullptr)
		{
			return Body;
		}
	}
	else if (slot_name == "feet")
	{
		if (Feet != nullptr)
		{
			return Feet;
		}
	}
	return nullptr;
}

const shared_ptr<Equipment> EquipmentSystem::GetEquipment(int inv_index)
{
	if (R_Hand != nullptr)
	{
		if (R_Hand->inv_index == inv_index)
		{
			return R_Hand;
		}
	}
	if (L_Hand != nullptr)
	{
		if (L_Hand->inv_index == inv_index)
		{
			return L_Hand;
		}
	}
	if (Head != nullptr)
	{
		if (Head->inv_index == inv_index)
		{
			return Head;
		}
	}
	if (Body != nullptr)
	{
		if (Body->inv_index == inv_index)
		{
			return Body;
		}
	}
	if (Feet != nullptr)
	{
		if (Feet->inv_index == inv_index)
		{
			return Feet;
		}
	}
	return nullptr;
}

Json::Value EquipmentSystem::GetAllEquipment()
{
	return equipment;
}


void EquipmentSystem::addEquipmentToInventory(Equipment* equipment, int inv_index)
{
	if (!CheckIfInParty()) return;
	
	if (!party->item_inventory.checkIfExists(equipment->ingame_name))
	{
		inv_index = (int)party->item_inventory.inventory.size();
		party->item_inventory.addToInventory(equipment->script);
		equipment->inv_index = inv_index;
	}
	else
	{
		equipment->inv_index = inv_index;
	}
	party->item_inventory.SetEquippedUnit(inv_index, user);
}

void EquipmentSystem::RemoveEquipmentFromInventory()
{
	if (!CheckIfInParty()) return;

	vector<string> slot_names = { "r_hand", "l_hand", "head", "body", "feet" };
	for (string slot_name : slot_names)
	{
		auto& equipment = GetEquipment(slot_name);
		if (!equipment) continue;

		party->item_inventory.subFromInventory(equipment->inv_index);
		removeEquipmentFromSlot(slot_name, equipment->inv_index);
	}
}

void EquipmentSystem::removeEquipmentFromSlot(string slot, int inventory_index)
{
	party->item_inventory.RemoveEquippedUnit(inventory_index);
	if (slot == "r_hand")
	{
		if (R_Hand->animation_name != "")
		{
			R_Hand->anim.unloadSprite();
		}
		R_Hand = nullptr;
	}
	else if (slot == "l_hand")
	{
		if (L_Hand->animation_name != "")
		{
			L_Hand->anim.unloadSprite();
		}
		L_Hand = nullptr;
	}
	else if (slot == "head")
	{
		if (Head->animation_name != "")
		{
			Head->anim.unloadSprite();
		}
		Head = nullptr;
	}
	else if (slot == "body")
	{
		if (Body->animation_name != "")
		{
			Body->anim.unloadSprite();
		}
		Body = nullptr;
	}
	else if (slot == "feet")
	{
		if (Feet->animation_name != "")
		{
			Feet->anim.unloadSprite();
		}
		Feet = nullptr;
	}
}

unordered_map<string, int> EquipmentSystem::GetEquipmentStats(Json::Value equipment_script)
{
	unordered_map<string, int> equipment_stats = {};
	if (equipment_script.isMember("stats"))
	{
		Json::Value stats = equipment_script["stats"];
		equipment_stats["attack"] = stats["attack"].asInt();
		equipment_stats["magic_attack"] = stats["magic"].asInt();
		equipment_stats["atk_speed"] = stats["atk_speed"].asInt();
		equipment_stats["hit"] = stats["hit"].asInt();
		equipment_stats["avoid"] = stats["avoid"].asInt();
		equipment_stats["block"] = stats["block"].asInt();
		equipment_stats["mag_def"] = stats["mag_def"].asInt();
		equipment_stats["crit"] = stats["crit"].asInt();
		equipment_stats["crit_dmg"] = stats["crit_dmg"].asInt();
		equipment_stats["weight"] = stats["weight"].asInt();

		if (equipment_script.isMember("bonus_stats"))
		{
			Json::Value bonus_stats = equipment_script["bonus_stats"];
			equipment_stats["str"] = bonus_stats["strength"].asInt();
			equipment_stats["mag"] = bonus_stats["magic"].asInt();
			equipment_stats["skl"] = bonus_stats["skill"].asInt();
			equipment_stats["spd"] = bonus_stats["speed"].asInt();
			equipment_stats["lck"] = bonus_stats["luck"].asInt();
			equipment_stats["def"] = bonus_stats["defense"].asInt();
			equipment_stats["res"] = bonus_stats["resistance"].asInt();
			equipment_stats["con"] = bonus_stats["condition"].asInt();
		}
		else
		{
			equipment_stats["str"] = stats["strength"].asInt();
			equipment_stats["mag"] = stats["magic"].asInt();
			equipment_stats["skl"] = stats["skill"].asInt();
			equipment_stats["spd"] = stats["speed"].asInt();
			equipment_stats["lck"] = stats["luck"].asInt();
			equipment_stats["def"] = stats["defense"].asInt();
			equipment_stats["res"] = stats["resistance"].asInt();
			equipment_stats["con"] = stats["condition"].asInt();
		}
	}
	
	return equipment_stats;
}

unordered_map<string, int> EquipmentSystem::GetEquipmentStats(Equipment* equipment)
{
	unordered_map<string, int> equipment_stats = {};
	if (equipment != nullptr)
	{
		equipment_stats["attack"] = equipment->attack;
		equipment_stats["magic_attack"] = equipment->mag_attack;
		equipment_stats["atk_speed"] = equipment->atk_speed;
		equipment_stats["hit"] = equipment->hit;
		equipment_stats["avoid"] = equipment->avoid;
		equipment_stats["block"] = equipment->block;
		equipment_stats["mag_def"] = equipment->mag_defense;
		equipment_stats["crit"] = equipment->crit;
		equipment_stats["crit_dmg"] = equipment->crit_dmg;
		equipment_stats["weight"] = equipment->weight;

		equipment_stats["str"] = equipment->str;
		equipment_stats["mag"] = equipment->mag;
		equipment_stats["skl"] = equipment->skl;
		equipment_stats["spd"] = equipment->spd;
		equipment_stats["lck"] = equipment->lck;
		equipment_stats["def"] = equipment->def;
		equipment_stats["res"] = equipment->res;
		equipment_stats["con"] = equipment->con;
	}
	else
	{
		equipment_stats["attack"] = 0;
		equipment_stats["magic_attack"] = 0;
		equipment_stats["atk_speed"] = 0;
		equipment_stats["hit"] = 0;
		equipment_stats["avoid"] = 0;
		equipment_stats["crit"] = 0;
		equipment_stats["crit_dmg"] = 0;
		equipment_stats["weight"] = 0;

		equipment_stats["str"] = 0;
		equipment_stats["mag"] = 0;
		equipment_stats["skl"] = 0;
		equipment_stats["spd"] = 0;
		equipment_stats["lck"] = 0;
		equipment_stats["def"] = 0;
		equipment_stats["res"] = 0;
		equipment_stats["con"] = 0;
	}
	
	return equipment_stats;
}

int EquipmentSystem::GetStat(unordered_map<string, int> equipment_stats, string stat_key)
{
	int equip_stat = equipment_stats[stat_key];
	return equip_stat;
}

int EquipmentSystem::CalculatePotentialNewStat(unordered_map<string, int> equipment_stats, string stat_key, string slot)
{
	int equip_stat = GetStat(equipment_stats, stat_key);
	if (slot != "")
	{
		auto& equipment = GetEquipment(slot);
		unordered_map<string, int> equipped_stats = GetEquipmentStats(equipment.get());
		int equipped_stat = equipped_stats[stat_key];
		if (stat_key == "str")
		{
			return user->str - equipped_stat + equip_stat;
		}
		if (stat_key == "mag")
		{
			return user->mag - equipped_stat + equip_stat;
		}
		if (stat_key == "skl")
		{
			return user->skl - equipped_stat + equip_stat;
		}
		if (stat_key == "spd")
		{
			return user->spd - equipped_stat + equip_stat;
		}
		if (stat_key == "lck")
		{
			return user->lck - equipped_stat + equip_stat;
		}
		if (stat_key == "def")
		{
			return user->def - equipped_stat + equip_stat;
		}
		if (stat_key == "res")
		{
			return user->res - equipped_stat + equip_stat;
		}
		if (stat_key == "con")
		{
			return user->con - equipped_stat + equip_stat;
		}
		if (stat_key == "attack")
		{
			int attack = user->str - equipped_stats["str"] + equipment_stats["str"];
			return user->bonus_atk - equipped_stat + equip_stat + attack;
		}
		if (stat_key == "mag_attack")
		{
			int mag_atk = user->mag - equipped_stats["mag"] + equipment_stats["mag"];
			return user->bonus_mag_atk - equipped_stat + equip_stat + mag_atk;
		}
		if (stat_key == "atk_speed")
		{
			int base_atk_spd = user->spd - equipped_stats["spd"] + equipment_stats["spd"];
			int con = (user->con - equipped_stats["con"] + equipment_stats["con"]);
			int burden = con - (user->weight - equipped_stats["weight"] + equipment_stats["weight"]);
			if (burden > 0)
			{
				burden = 0;
			}
			return base_atk_spd + user->bonus_atk_spd - equipped_stat + equip_stat + burden;
		}
		if (stat_key == "hit")
		{
			int skl = (user->skl - equipped_stats["skl"] + equipment_stats["skl"]);
			int lck = (user->lck - equipped_stats["lck"] + equipment_stats["lck"]);
			int base_hit = ( skl * 2) + (int)( lck / 2);
			return user->bonus_hit - equipped_stat + equip_stat + base_hit;
		}
		if (stat_key == "avoid")
		{
			int base_atk_spd = user->spd - equipped_stats["spd"] + equipment_stats["spd"];
			int burden = (user->con - equipped_stats["con"] + equipment_stats["con"]) - (user->weight - equipped_stats["weight"] + equipment_stats["weight"]);
			if (burden > 0)
			{
				burden = 0;
			}
			int atk_spd = base_atk_spd - equipped_stats["atk_speed"] + equipment_stats["atk_speed"] + burden;
			int base_avoid = atk_spd + user->lck - equipped_stats["lck"] + equipment_stats["lck"];
			return base_avoid + user->bonus_avoid - equipped_stat + equip_stat;
		}
		if (stat_key == "block")
		{
			int base_block = user->def - equipped_stats["def"] + equipment_stats["def"];
			return base_block + user->bonus_block - equipped_stat + equip_stat;
		}
		if (stat_key == "mag_def")
		{
			int base_mdef = user->res - equipped_stats["res"] + equipment_stats["res"];
			return base_mdef + user->bonus_mag_defense - equipped_stat + equip_stat;
		}
		if (stat_key == "crit")
		{
			int base_crit = (int)round(((float)(user->skl - equipped_stats["skl"] + equipment_stats["skl"]) / 2.0f));
			return base_crit + user->bonus_crit - equipped_stats["crit"] + equip_stat;
		}
		if (stat_key == "crit_dmg")
		{
			return user->base_crit_dmg + user->bonus_crit_dmg - equipped_stat + equip_stat;
		}
	}
	return equip_stat;
}



bool EquipmentSystem::CheckIfInParty()
{
	bool inParty = false;
	for (auto& unit : party->party)
	{
		if (user == unit.get())
		{
			inParty = true;
			break;
		}
	}
	return inParty;
}