#pragma once
#include <raylib.h>
#include <unordered_map>
#include <string>
#include "controller.h"
#include "battle_cursor.h"
#include "battle_state_controller.h"
#include "battle_menu_controller.h"
#include "battle_manager.h"
#include "battle_controller_handler.h"
#include "vfx_controller.h"
#include "buff_battle_manager.h"

using namespace std;

class Game;
class NPC;
class Party;
class BattleScene;

class BattleController : public Controller
{
public:
	void setup(Game* game, Party* party, BattleScene* battle_scene, string background_name, unordered_map<string, shared_ptr<NPC>> combatants);
	void update() override;
	void draw();

	//Scene
	BattleScene* battle_scene;

	//VFX Controller
	VFXController vfx_controller;

	//State Controller
	BattleStateController state_controller;

	//Menu
	BattleMenuController menu_controller;

	//Manager
	BattleManager battle_queue_manager;
	BuffBattleManager buff_btl_manager;

	//Handler
	BattleControllerHandler handler;

	//Cursor
	BattleCursor cursor;

	//Logic
	bool turn_start = false;
	bool enemy_turn = false;
	bool player_menu_on = false;
	bool battle_on = false;
	bool battle_won = false;
	bool battle_end = false;
	bool scripted_battle = false;
	bool game_over = false;
	bool battle_start = false;
	bool no_gameover = false;
	int turn = 0;

	//Data
	unordered_map<string, shared_ptr<NPC>> combatants;
	unordered_map<string, shared_ptr<NPC>> enemy_combatants;
	unordered_map<string, string> loot;
	int total_exp = 0;
	int total_silver = 0;
private:
	//Text
	string background_name;

	//Draw
	void drawBackground();
	void drawCombatants();
};