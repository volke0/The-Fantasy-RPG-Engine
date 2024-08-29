#pragma once
#include <raylib.h>
#include "item_inventory.h"
#include "party_formation.h"
#include "npc.h"

using namespace std;

class Unit;
class Maps;
class Input;
class Game;

class PartyBase : public NPC
{
public:
	virtual void setup(Game* game);

	//Basic Methods
	virtual void draw() override;
	virtual void update() override;

	//Getters
	Json::Value& getItem(int pos);
	bool CheckIfItemExists(string name);
	NPC* GetUnitFromParty(int unit_id);

	//Setters
	virtual void setBattleMode();
	virtual void setMapMode();
	virtual void addToInventory(string item_name, int amount = 1);
	virtual void removeFromInventory(int inv_index, int amount = 1);
	virtual void removeFromInventory(string item_name, int amount = 1);
	virtual void SetFormation();
	virtual void SetupCombatants(unordered_map<string, shared_ptr<NPC>>& combatants, BattleController* battle_controller);
	void RemoveFromParty(int unit_id);
	void RevealUnitInParty(int unit_id);
	void DepartFromParty(int unit_id, bool bDISA = false);
	void ReturnToParty(int unit_id);
	void SetVisibleParty();
	
	//Data
	vector<shared_ptr<NPC>> party = {};
	vector<shared_ptr<NPC>> reserve = {};
	vector<int> visible_party = {};
	ItemInventory item_inventory;
	PartyFormation formation;
	int leader_index = 0;
	bool bNeedToChangeLeader = false;
	bool bNoLeader = false;

private:
	//Setup
	void setupMapMode();

	//Update
	void updateMapMode();
	void UpdatePartyMembers();
	void ChangeLeader();

	//Draw Methods
	void drawMapMode();
};
