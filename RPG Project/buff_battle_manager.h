#pragma once
#include <raylib.h>
#include "entity.h"
#include "timer.h"

using namespace std;

class Game;
class NPC;


class BuffBattleManager : public Entity
{
public:
	void setup(Game* game);

	//Update
	void UpdateBuffs(vector<NPC*>& combat_units);

	//Draw
	void DrawBuffs(vector<NPC*>& combat_units);
	void DrawBuffIconRow(vector<NPC*>& combat_units);

	//Getters
	bool GetIfDone() const;

	//Setters
	void Reset(vector<NPC*>& combat_units);
	void RemoveAllBuffs(vector<NPC*>& combat_units);
private:
	//Data
	int current_index = 0;
	bool bDone = false;
	bool bTickThenTerminate = false;
	Timer timer;
};