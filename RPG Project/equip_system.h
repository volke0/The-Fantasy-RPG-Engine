#pragma once
#include <raylib.h>
#include "controller.h"

using namespace std;

class Game;
class Unit;
class Equipment;
class Weapon;

class EquipmentSystem : public Controller
{
public:
	void setup(Game* game, Unit* user);
	virtual void update() override;
	
	//Setters
	void SetAllEquipment(Json::Value equip_script);
	void Equip(string slot_name, Json::Value equipment_script, int inv_index = 0);
	void UnEquip(string slot_name);
	void ApplyAllEquipmentStats(Json::Value equipment_script);
	void addEquipmentToInventory(Equipment* equipment, int inv_index = 0);
	void RemoveEquipmentFromInventory();

	//Getters
	const shared_ptr<Equipment> GetEquipment(string slot_name);
	const shared_ptr<Equipment> GetEquipment(int inv_index);
	Json::Value GetAllEquipment();
	vector<string> GetEquipType(string slot_name);
	vector<string> GetEquipRanks(vector<string> equip_types);
	Equipment* GetWeapon(bool get2ndWeapon = false);
	unordered_map<string, int> GetEquipmentStats(Json::Value equipment_script);
	unordered_map<string, int> GetEquipmentStats(Equipment* equipment);
	int GetStat(unordered_map<string, int> equipment_stats, string stat_key);
	int CalculatePotentialNewStat(unordered_map<string, int> equipment_stats, string stat_key, string slot = "");
	bool HasWeapon();

	//Equipment
	Json::Value equipment;
	shared_ptr<Equipment> R_Hand;
	shared_ptr<Equipment> L_Hand;
	shared_ptr<Equipment> Head;
	shared_ptr<Equipment> Body;
	shared_ptr<Equipment> Feet;

	unordered_map<string, vector<string>> equip_types = { {"r_hand", {"weapon", "shield"}},
														  {"l_hand", {"weapon", "shield"}},
														  {"head", {"head_gear"}},
														  {"body", {"armor"}},
														  {"feet", {"foot_gear"}}, };

	//Data
	bool r_hand_weapon = false;
	bool l_hand_weapon = false;
	bool dual_wielding = false;

private:
	//Setup
	void SetupEquipment();
	
	//Getters
	shared_ptr<Equipment> CreateEquipment(string slot, Json::Value equipment_script);
	bool CheckIfInParty();

	//Setters
	void removeEquipmentFromSlot(string slot, int inventory_index);

	//Data
	Unit* user;
};
