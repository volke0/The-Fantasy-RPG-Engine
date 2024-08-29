#pragma once
#include <raylib.h>
#include "sub_menu.h"

using namespace std;

class Game;
class Party;
class MenuCursor;
class ShopMenu;
struct ShopData;

class SubShopMenu : public SubMenu
{
public:
	virtual void setup(Game* game, ShopMenu* shop_menu, ShopData* shop_data, MenuCursor* menu_cursor);
	virtual void draw() override;
	virtual void update() override;
	virtual void SetupShop();
	virtual vector<string> SetupOptions(vector<string> input_options);

	//Draw
	virtual void DrawInfo();

	//Getters
	int getCurrentlyEquipped(string item_name);
	int getCurrentlyStocked(string item_name);

	//Data
	ShopMenu* shop_menu;
	ShopData* shop_data;
	Vector2 box_pos = {};
};
