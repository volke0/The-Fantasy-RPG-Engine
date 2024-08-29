#pragma once
#include <raylib.h>
#include <vector>
#include <iostream>
#include <string>
#include <unordered_map>
#include <json/json.h>
#include "scene.h"
#include "battle_controller.h"
#include "vars.h"

using namespace std;

class Party;
class EnemyParty;
class NPC;
class VFXAnimation;
class Level;

class BattleScene : public Scene
{
public:
	~BattleScene();
	void setup(Game* game, Party* party, Level* level, string enemy_party_id, Json::Value params);
	void update() override;
	void draw() override;

	//Setters
	void setDebugModeOn();
	void setDebugModeOff();
	void endBattle();

	unordered_map<string, shared_ptr<NPC>> combatants;
	vector<shared_ptr<NPC>> enemies;

	//Party
	Party* party;
	shared_ptr<EnemyParty> enemy_party;

	//Logic
	bool ready = false;
private:
	//Controller
	BattleController battle_controller;

	//Normal
	void updateNormalMode();

	//Setters
	void setMusic();
	void beginBattle();
	void setCombatantAnimation(string combatant_name, string anim_name);
	void setAllCombatants();
	void setAllCombatantsDebug();
	void setAllCombatantsNormal();
	void setupPlayerParty();
	void setupEnemyParty();

	//Text
	float dropshadow_offset = (float)1 + (TILE / 2 / 15);
	void drawText(Vector2 pos, string text, float fontSize, Color color = {255, 255, 255, 255});
	float partyNameFontSize = 25.0f;

	//Data
	Level* level;
	string enemy_party_id = "";
	Json::Value params;

	//Debug
	bool debug_mode = false;
	shared_ptr<VFXAnimation> vfx_anim;
	shared_ptr<VFXAnimation> b_text_anim;
	void setText(string text, NPC* target);
	void drawDebugMode();
	void debugInfo();
	void DebugCommands();
};