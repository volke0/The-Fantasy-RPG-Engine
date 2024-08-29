#pragma once
#include <raylib.h>
#include "confirm_status_menu.h"

using namespace std;

class Game;
class Party;
class PlayerStatusMenu;
class MenuCursor;
class Unit;

class EquipConfirmStatusMenu : public ConfirmStatusMenu
{
public:
	virtual void setup(Game* game, PlayerStatusMenu* player_status_menu, Unit* user, string slot, Json::Value item, int item_num);
	virtual void update() override;
	virtual void draw() override;
private:
	//Setup
	void FindMemberId();
	void SetupConfirmMessage();

	//Data
	Unit* user;
	string slot = "";
	string member_id = "";
	int member_inv_id = 0;
	Json::Value item;
	int item_num = 0;
	bool bEquippedUnit = false;
};
