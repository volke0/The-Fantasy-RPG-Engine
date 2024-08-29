#pragma once
#include <raylib.h>
#include "display_handler.h"

using namespace std;

class Game;
class BattleAnimationManager;
class Consumable;

class ItemDisplayHandler : public DisplayHandler
{
public:
	void setup(Game* game, BattleAnimationManager* btl_animation_manager);
	virtual void update() override;
	void draw();

	//Setters
	void setItemList(vector<shared_ptr<Consumable>> item_list, int current_item_index);
private:
	//Update
	void updateItem();

	//Setters
	void setBattleVars();
	void setCurrentTarget();
	void setCurrentItem();

	//Magic
	vector<shared_ptr<Consumable>> item_list;
	int item_index = 0;
	int current_item_index = 0;
	shared_ptr<Consumable> current_item;

	//Logic
	bool play_item = false;
	bool applied_dmg = false;
	bool item_mode_on = false;
};