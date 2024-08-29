#include "shop_menu.h"
#include "level.h"
#include "game.h"
#include "shop_buy_menu.h"
#include "sell_shop_menu.h"

ShopMenu::~ShopMenu()
{
	party->state_controller.eraseState("shop_menu");
	level->bShopDone = true;
}

void ShopMenu::setup(Game* game, ShopData shop_data, Level* level)
{
	SceneMenu::setup(game);
	this->shop_data = shop_data;
	this->level = level;
	level->bShopDone = false;
	title = "Shop Menu";
	name = "shop_menu";
	type = "shop";
	cursor.setup(game);
	cursor.turnOn();
	party->state_controller.pushState("shop_menu");
}

void ShopMenu::update()
{
	SetCursor(cursor, options, TILE * 13.f + 24.f, 36.f, 48.f, 3);
	UpdateCursor(cursor);
	if (!scene_manager->busy)
	{
		if (cursor.go_back)
		{
			cursor.go_back = false;
		}
		if (cursor.return_input)
		{
			cursor.return_input = false;
			input_history.clear();
			input_history["option"] = cursor.selected_option;
			LoadOption();
		}
	}
}

void ShopMenu::LoadOption()
{
	if (input_history["option"].asString() == "Buy")
	{
		sound_player.playSound("click");
		shared_ptr<ShopBuyMenu> shop_buy_menu = make_shared<ShopBuyMenu>();
		shop_buy_menu->setup(game, this, &shop_data, &cursor);
		PushMenu(shop_buy_menu);
	}
	if (input_history["option"].asString() == "Sell")
	{
		sound_player.playSound("click");
		shared_ptr<SellShopMenu> shop_sell_menu = make_shared<SellShopMenu>();
		shop_sell_menu->setup(game, this, &shop_data, &cursor);
		PushMenu(shop_sell_menu);
		cout << input_history["option"].asString() << endl;
	}
	if (input_history["option"].asString() == "Quit")
	{
		sound_player.playSound("click2");
		cursor.turnOn();
		scene_manager->pop();
		input->changeToLevelMode();
	}
}


void ShopMenu::draw()
{
	drawBackground();
	drawOptions();
	if (!pushedScene)
	{
		cursor.draw();
	}
}

void ShopMenu::drawBackground()
{
	ClearBackground(BLUE);
}

void ShopMenu::drawOptions()
{
	int num = 0;
	for (auto& option : options)
	{
		float x = vis_option_pos.x;
		float y = vis_option_pos.y + (vis_spacing * num);
		Vector2 pos{ x, y };
		drawText(pos, option, 24.f);
		num++;
	}
}