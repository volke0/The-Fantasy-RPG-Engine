#pragma once
#include <raylib.h>
#include "sub_shop_menu.h"

using namespace std;

class Game;
class Party;
class MenuCursor;
class ShopMenu;
struct ShopData;

class SellShopMenu : public SubShopMenu
{
public:
	virtual void setup(Game* game, ShopMenu* shop_menu, ShopData* shop_data, MenuCursor* menu_cursor) override;
	void update() override;
	void draw() override;

	//Setup
	virtual void SetupShop() override;
private:
	//Setup
	virtual vector<string> SetupOptions(vector<Json::Value> input_items);

	//Draw
	virtual void drawBackground() override;
	virtual void DrawOptions() override;
	virtual void DrawInfo() override;
	virtual void DrawItem();

	//Data
	float sell_percent = 0.f;
	float sell_price = 0.f;
};
