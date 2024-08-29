#pragma once
#include <raylib.h>
#include "sub_shop_menu.h"

using namespace std;

class Game;
class Party;
class MenuCursor;
class ShopMenu;
struct ShopData;

class ShopConfirmMenu : public SubShopMenu
{
public:
	void setup(Game* game, ShopMenu* shop_menu, MenuCursor* menu_cursor, string shop_type, float price);
	void update() override;
	void draw() override;
private:
	//Draw
	void DrawBox();
	void DrawStatusText();

	//Update
	void BuyItem();
	void SellItem();

	//Data
	Json::Value item;
	float price = 0.f;
	string shop_type = "";
};