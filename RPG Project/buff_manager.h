#pragma once
#include <raylib.h>
#include "entity.h"

using namespace std;

class Game;
class Unit;
class NPC;
class BuffBase;

class BuffManager : public Entity
{
public:
	void setup(Game* game, Unit* owner);

	virtual void update();
	virtual void draw();

	//Setters
	void ApplyBuff(string buff_name, NPC* source_unit);
	void RemoveBuff(string in_buff_name);
	void RemoveBuff();
	void CheckIfNeedToRemoveBuff(BuffBase* buff, int index);
	void RemoveAllBuffs();
	void ApplyEffects();
	void Reset();

	//Getters
	bool GetBuffSourceUnit(string buff_name, vector<NPC*>& unit_list);
	bool FindBuff(string buff_name, vector<BuffBase*>& in_buff_list);
	bool GetIfDoneApplyingEffects() const;
	bool GetPlayingBuffAnim() const;

	//Draw
	void DrawBuffAnimation();
private:
	//Draw
	void DrawBuffIconRow();

	//Setters
	void RefreshBuffList();
	void UnloadBuffIcon(string icon_name);

	//Getters
	Texture2D GetBuffIcon(string icon_name);

	//Data
	NPC* owner = nullptr;
	unordered_map<string, unordered_map<string, shared_ptr<BuffBase>>> buff_map = {};
	vector<BuffBase*> buff_list = {};
	int buff_count = 0;
	vector<vector<string>> buff_removal_list = {};
	int current_index = 0;
	bool bDoneApplyingEffects = false;
	bool bPlayingBuffAnim = false;

	//Icon Row
	vector<string> buff_icon_name_list = {};
	vector<int> buff_icon_indices_to_remove = {};
	Texture2D buff_icon;
};
