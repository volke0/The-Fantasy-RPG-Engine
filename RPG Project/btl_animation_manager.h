#pragma once
#include <raylib.h>
#include <vector>
#include <json/json.h>
#include <memory>
#include "attack_display_handler.h"
#include "magic_display_handler.h"
#include "item_display_handler.h"
#include "handler.h"

using namespace std;

class Game;
class Unit;
class NPC;
class Battle;
class TextAnimation;
class Weapon;
class Magic;
class Item;

class BattleAnimationManager : public Handler
{
public:
	void setup(Game* game, Battle* battle, Unit* user, vector<NPC*> targets);
	void updateBattle();
	void drawBattle();

	//Data
	Game* game = nullptr;
	Battle* battle = nullptr;
	Unit* user = nullptr;
	vector<string> weapons = {};
	vector<NPC*> targets = {};

	//Setters
	void setMagicList(vector<shared_ptr<Magic>> magic_list);
	void setItemList(vector<shared_ptr<class Consumable>> item_list, int item_index);
	void EndBattle();

	//Getters
	bool checkIfTargetDead(bool ignoreChanging = false);
	void checkIfAllTargetsDead();

	//Reset
	void resetUser();
	void resetCurrentTarget();

	//Animation Stuff
	int target_index = 0;
	Unit* current_target = nullptr;
	int weapon_index = 0;
	string current_weapon = "";
	vector<shared_ptr<TextAnimation>> dmg_text = {};

	//Logic
	bool done = false;
	bool wrap_up = false;
	bool play_text_anim = false;
private:
	//Setup
	void checkForWeapon();
	void setupAnimations();

	//Update
	AttackDisplayHandler atk_display;
	MagicDisplayHandler mag_display;
	ItemDisplayHandler item_display;

	//Setters
	void MakeHealList(vector<shared_ptr<class Consumable>> item_list);
	void MakeHealList(vector<shared_ptr<Magic>> magic_list);

	//Data
	unordered_map<int, string> heal_list = {};
};