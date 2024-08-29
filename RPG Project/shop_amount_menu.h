#pragma once
#include <raylib.h>
#include "sub_shop_menu.h"

using namespace std;

class Game;
class Party;
class MenuCursor;
class ShopMenu;
struct ShopData;

struct ShopAmountParams
{
	ShopAmountParams() {};
	ShopAmountParams(int index, float vis_spacing, Json::Value selection, string shop_type, float sell_price = 0.f) : index(index), vis_spacing(vis_spacing), selection(selection), shop_type(shop_type), sell_price(sell_price) {};
	int index = 0;
	float vis_spacing = 0;
	Json::Value selection;
	string shop_type = "";
	float sell_price = 0.f;
};

class ShopAmountMenu : public SubShopMenu
{
public:
	virtual void setup(Game* game, ShopMenu* shop_menu, ShopData* shop_data, MenuCursor* menu_cursor, shared_ptr<ShopAmountParams> params);
	void update() override;
	void draw() override;

	//Update
	void AnimTimer();

	//Draw
	void DrawStatusText();
	void DrawGhostCursor();
private:
	//Setup
	void SetupGhostCursor();

	//Data
	int quantity = 1;
	int max_quantity = 99;
	const int min_quantity = 1;
	int anim_timer = 0;
	bool bShowAmountHelp = true;
	bool bFlicker = true;
	float price = 0;
	Rectangle ghost_cursor_pos = {};
	int ghost_index = 0;
	shared_ptr<ShopAmountParams> params;
};
