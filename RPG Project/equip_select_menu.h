#pragma once
#include <raylib.h>
#include "sub_status_menu.h"

class Game;
class Party;
class PlayerStatusMenu;
class MenuCursor;

class EquipSelectMenu : public SubStatusMenu
{
public:
	virtual void setup(Game* game, PlayerStatusMenu* player_status_menu, string slot, vector<string> equip_type, vector<string> equip_ranks);
	virtual void update() override;
	virtual void draw() override;
private:
	//Setup
	void MakeOptions();
	vector<int> MakeVisibleItemOptions();
	vector<string> equip_type = {};
	vector<string> equip_ranks = {};
	string slot = "";
	int lastSceneNum = 0;

	//Getters
	string GetEquipRank(Json::Value item, string sub_type);
	vector<string> GetSpecialLocks(Json::Value item);

	//Draw
	void DrawBox();
	void DrawDescriptionBox(Rectangle og_dest);
	void DrawOptions();
	void drawItem(Json::Value& item, int item_num, vector<string>& option_types, Vector2& pos, float fontSize, Color& color);
	void drawEquippedText(Json::Value& item, int item_num, vector<string>& option_types, Vector2& pos, float fontSize, Color& color);
	void drawEquippedStats(Json::Value& item, int item_num);
	void drawItemDescription(Json::Value item);
};
