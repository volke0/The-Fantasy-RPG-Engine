#pragma once
#include <raylib.h>
#include "sub_shop_menu.h"

using namespace std;

class Game;
class Party;
class MenuCursor;
class ShopMenu;
struct ShopData;

class ShopBuyMenu : public SubShopMenu
{
public:
	virtual void setup(Game* game, ShopMenu* shop_menu, ShopData* shop_data, MenuCursor* menu_cursor) override;
	void draw() override;
	void update() override;
private:
	//Setup
	virtual void SetupShop() override;
	virtual vector<string> SetupOptions(vector<string> input_options) override;

	//Draw
	virtual void drawBackground() override;
	virtual void DrawOptions() override;
	virtual void DrawInfo() override;
	void DrawItemInfo();
	void drawItemStats(Json::Value item);

};
