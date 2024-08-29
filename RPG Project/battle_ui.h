#pragma once
#include <raylib.h>
#include <memory>
#include "menu.h"

using namespace std;

class Game;
class Party;
class Unit;
class NPC;
class CursorHandler;
class BattleMenuController;

class BattleUI : public Menu
{
public:
	void setup(Game* game, Party* party, BattleController* battle_controller, BattleMenuController* menu_controller);
	void update() override;
	void draw() override;
private:
	unordered_map<string, shared_ptr<NPC>> combatants;

	//Text
	float partyNameFontSize = 25.0f;

	//Draw
	void drawUI();
	void drawUIBox();
	void drawUIText();
	void drawPartyMembersUI();
	void drawPartyMemberName(Vector2 pos, string name);
	void drawPartyMemberStatus(Vector2 pos, Unit* unit);
};
