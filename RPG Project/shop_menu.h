#pragma once
#include <raylib.h>
#include "scene_menu.h"
#include "menu_cursor.h"

using namespace std;

class Game;
class Level;

struct ShopData
{
	ShopData() {};
	ShopData(vector<string> items_selling, string shop_name, string greeting, string buy_msg, string sell_msg)
		: items_selling(items_selling), shop_name(shop_name), greeting(greeting), buy_msg(buy_msg), sell_msg(sell_msg) {};
	
	vector<string> items_selling = {};
	string shop_name = "";
	string greeting = "";
	string buy_msg = "";
	string sell_msg = "";
};

class ShopMenu : public SceneMenu
{
public:
	~ShopMenu();
	virtual void setup(Game* game, ShopData shop_data, Level* level);
	virtual void update() override;
	virtual void draw() override;

	//Cursor
	MenuCursor cursor;

	//Data
	Json::Value input_history;
private:
	//Update
	void LoadOption();

	//Draw
	virtual void drawBackground() override;
	void drawOptions();

	//Data
	ShopData shop_data;
	vector<string> options = { "Buy", "Sell", "Quit" };
	Level* level;
};
