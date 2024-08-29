#pragma once
#include <raylib.h>
#include <vector>
#include <memory>
#include <unordered_map>
#include "sound_player.h"
#include "battle_cursor_manager.h"
#include "cursor.h"

using namespace std;

class Game;
class Input;
class Unit;
class NPC;

class BattleCursor : public Cursor
{
public:
	~BattleCursor();
	virtual void setup(Game* game);
	virtual void update() override;
	virtual void draw();

	//Setters
	virtual void setCursor(Vector2 pos, vector<string> options, bool horizontal = false, int max_vis_index = 3) override;
	void setCursor(vector<string> unit_names, unordered_map<string, shared_ptr<NPC>> combatants);
	void turnMagicCursorOn();
	void moveMagicVisualIndex(int num);
	void setMagicVisualIndex(int num);
	virtual void SetMultiTargetMode(bool bMultiOn) override;
	void CalculateCursorUnitPos(int index);

	//Getters
	int getMagicVisualIndex();

	//Draw
	float x_factor = 0;
	float y_factor = 0;

	//Multi-Target
	void DrawMultiMode();
	int draw_index = 0;

	//Logic
	bool switch_to_enemies = false;
	bool switch_to_allies = false;
	bool unit_mode = false;
	bool magic_mode = false;
	bool bMultiOn = false;

private:
	//Update
	BattleCursorManager manager;
};