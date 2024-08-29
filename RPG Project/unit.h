#pragma once
#include <raylib.h>
#include <vector>
#include <iostream>
#include <string>
#include <unordered_map>
#include <json/json.h>
#include "entity.h"
#include "unit_class.h"
#include "movement_controller.h"
#include "unit_state_controller.h"
#include "char_anim_controller_manager.h"
#include "battle_condition_controller.h"
#include "magic_list_handler.h"
#include "battle_ai.h"
#include "unit_battle_logic_controller.h"
#include "stat_handler.h"
#include "equip_system.h"
#include "buff_manager.h"

using namespace std;

class Maps;
class Game;
class Level;
class Weapon;
class Equipment;
class PartyBase;

class Unit : public Entity
{
public:
	//Setup
	virtual void setup(Game* game, int id);
	void setupGraphics();
	void setupUnit();

	//Setters
	void setTarget(Unit* target);
	void setBattlePositions();
	void setBattleMode();
	void setMapMode();
	void destroy();

	//Script
	Json::Value script;
	Json::Reader reader;
	Json::Value anim_controller_script;

	//Data
	string ingame_name = "";
	string type = "";
	string class_link = "";

	//Unit
	Unit* target = nullptr;

	//Equipment
	string weapon_focus = "r_hand";
	vector<int> equipped_items = {};
	EquipmentSystem equip_system;
	const shared_ptr<Equipment> GetEquipment(string slot_name);
	const shared_ptr<Equipment> GetEquipment(int inv_index);
	Json::Value GetAllEquipment();
	vector<string> GetEquipType(string slot_name);
	Equipment* GetWeapon(bool get2ndWeapon = false);
	bool HasWeapon();

	//Ranks
	Json::Value equipment_ranks;

	//Class
	UnitClass unit_class;
	bool class_initialized = false;

	//Stats
	Json::Value stat_script;
	StatHandler stat_handler;
	Json::Value base_stats;
	int lvl = 0;
	int max_hp = 0;
	int hp = 0;
	int max_mp = 0;
	int mp = 0;
	int max_sp = 20;
	int sp = 0;
	int str = 0;
	int mag = 0;
	int skl = 0;
	int spd = 0;
	int lck = 0;
	int def = 0;
	int res = 0;
	int con = 0;
	int exp = 0;
	int tp = 0;

	//Enemy only stats
	int slv = 0;
	int exp_give = 0;
	unordered_map<string, int> loot;
	unordered_map<string, string> weapon_drops = {};

	//Bonus Stats
	int bonus_max_hp = 0;
	int bonus_max_mp = 0;
	int bonus_str = 0;
	int bonus_mag = 0;
	int bonus_skl = 0;
	int bonus_spd = 0;
	int bonus_lck = 0;
	int bonus_def = 0;
	int bonus_res = 0;
	int bonus_con = 0;

	int bonus_atk = 0;
	int bonus_mag_atk = 0;
	int bonus_atk_spd = 0;
	int bonus_hit = 0;
	int bonus_avoid = 0;
	int bonus_block = 0;
	int bonus_mag_defense = 0;
	int bonus_crit = 0;
	int bonus_crit_dmg = 0;

	//Combat Stats
	int atk = 0;
	int mag_atk = 0;
	int base_atk_spd = 0;
	int atk_spd = 0;
	int hit = 0;
	int base_hit = 0;
	int avoid = 0;
	int base_avoid = 0;
	int block = 0;
	int mag_defense = 0;
	int crit = 0;
	int base_crit = 0;
	int crit_dmg = 200;
	int base_crit_dmg = 200;

	//Weapon Stats
	int weight = 0;
	int weapon1_atk = 0;
	int weapon1_mag = 0;
	int weapon1_hit = 0;
	int weapon1_weight = 0;
	int weapon1_avoid = 0;
	int weapon1_crit = 0;

	int weapon2_atk = 0;
	int weapon2_mag = 0;
	int weapon2_hit = 0;
	int weapon2_weight = 0;
	int weapon2_avoid = 0;
	int weapon2_crit = 0;
	
	//Level/Exp Stats
	int base_exp = 0;
	int until_next_level = 0;

	//States and Conditions
	bool entrance = false;
	bool dead = false;

	//Predictive variables used only for AI at the moment
	int predicted_hp = INT_MAX;
	int predicted_mp = INT_MAX;

	//Combat Options
	vector<string> combat_options = { "Attack" };
	vector<string> magic_options = {};
	vector<string> magic_list = {};

	//Magic
	MagicListHandler magic_list_updater;
	int magic_index = 0;
	int magic_vis_index = 0;
	int magic_cur_vis_index = 0;

	//Logic
	bool followingPath = false;
	bool bWasIdle = false;

	//Battle
	UnitBattleLogicController btl_logic_controller;
	float bstart_x = 0;
	float bstart_y = 0;

	//Buffs
	BuffManager buff_manager;

	//AI
	string battle_ai_script_name = "";
	string map_ai_script_name = "";
	BattleAI battle_ai;

	//Rectangles
	Rectangle map_rect;
	Rectangle battle_rect;
	Vector2 map_origin;

	//Level
	Level* level = nullptr;

	//Party
	PartyBase* party = nullptr;
	int party_pos = 0;

	//Map
	Maps* map = nullptr;
	void setLevel(Level* level, Maps* map);

	//Modes
	bool map_mode = false;
	bool battle_mode = false;

	//Condition and States
	UnitStateController state_controller;
	BattleConditionController btl_con_controller;

	//Location
	int layer_pos = 0;
	vector<string> move_list = {};
	string last_move = "down";
	vector<int> direction = {0, 0};

	//Sprites
	string graphics = "";
	string portrait = "";

	//Animation
	CharacterAnimationControllerManager anim_controller;
	Rectangle anim_source_rect = {};

	//Offsets
	vector<float> true_offset = { 0.0f, 0.0f };

	//Movement
	MovementController movement_controller;
	bool currentlyMoving = false;
	bool running = false;

	//Draw
	virtual void draw();

	//Update
	virtual void update();

	//Data
	bool inParty = false;
	string combat_name = "";
	Color personal_color;
	int id = 0;
	bool bBeingDamaged = false;

private:
	//Setup
	void setupControllers();
	void setupClass();
	void setupColor();
	void setupValues();
	void setupLoot();
	void setupStats();
	void setupEquipment();
	void setupMagicOptions();
};